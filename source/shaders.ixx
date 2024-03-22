module;

#include <common.hxx>
#include <concepts>

export module shaders;

import common;
import comvars;
import settings;
import natives;
import shadows;

template<typename T, typename ... U>
concept IsAnyOf = (std::same_as<T, U> || ...);

template<typename T>
using remove_cvref_t = std::remove_cvref_t<std::remove_pointer_t<std::decay_t<T>>>;

export template <typename T> requires IsAnyOf<remove_cvref_t<T>, IDirect3DPixelShader9, IDirect3DVertexShader9>
int GetFusionShaderID(T pShader)
{
    if (pShader)
    {
        UINT len = 0;
        pShader->GetFunction(nullptr, &len);
        std::vector<uint8_t> pbFunc(len + len % 4);
        pShader->GetFunction(pbFunc.data(), &len);
        static auto sFusionShader = to_bytes("FusionShader");
        auto s = pattern_str(sFusionShader);
        while (s.back() == ' ' || s.back() == '0') s.pop_back();
        auto pattern = hook::range_pattern((uintptr_t)pbFunc.data(), (uintptr_t)pbFunc.data() + pbFunc.size(), s);
        if (!pattern.empty()) {
            auto id = *pattern.get_first<int>(sFusionShader.size() - 1);
            if constexpr (IsAnyOf<remove_cvref_t<T>, IDirect3DVertexShader9>)
                id -= 2000;
            return id;
        }
    }
    return -1;
}

class Shaders
{
public:
    Shaders()
    {
        static UINT oldCascadesWidth = 0;
        static UINT oldCascadesHeight = 0;
        static IDirect3DTexture9* pHDRTexQuarter = nullptr;
        static bool bFixAutoExposure = false;
        static auto bFixCascadedShadowMapResolution = false;

        FusionFix::onInitEvent() += []()
        {
            CIniReader iniReader("");
            bFixAutoExposure = iniReader.ReadInteger("MISC", "FixAutoExposure", 1) != 0;

            // Redirect path to one unified folder
            auto pattern = hook::pattern("8B 04 8D ? ? ? ? A3 ? ? ? ? 8B 44 24 04");
            if (!pattern.empty())
            {
                static auto off_1045520 = *pattern.get_first<const char**>(3);
                struct ShaderPathHook
                {
                    void operator()(injector::reg_pack& regs)
                    {
                        regs.ecx = 0;
                        *(const char**)&regs.eax = *off_1045520;
                    }
                }; injector::MakeInline<ShaderPathHook>(pattern.get_first(0), pattern.get_first(7));
            }
            else
            {
                pattern = hook::pattern("8B 14 85 ? ? ? ? A3 ? ? ? ? 8B 44 24 04");
                static auto off_1045520 = *pattern.get_first<const char**>(3);
                struct ShaderPathHook
                {
                    void operator()(injector::reg_pack& regs)
                    {
                        regs.eax = 0;
                        *(const char**)&regs.edx = *off_1045520;
                    }
                }; injector::MakeInline<ShaderPathHook>(pattern.get_first(0), pattern.get_first(7));
            }
        };

        FusionFix::onGameInitEvent() += []()
        {
            FusionFix::D3D9::onBeginScene() += [](LPDIRECT3DDEVICE9 pDevice)
            {
                // Setup variables for shaders
                static auto dw11A2948 = *find_pattern("C7 05 ? ? ? ? ? ? ? ? 0F 85 ? ? ? ? 6A 00", "D8 05 ? ? ? ? D9 1D ? ? ? ? 83 05").get_first<float*>(2);
                static auto dw103E49C = *hook::get_pattern<void**>("A3 ? ? ? ? C7 80", 1);
                auto bLoadscreenActive = (bLoadscreenShown && *bLoadscreenShown) || bLoadingShown;

                if (*dw103E49C && !bLoadscreenActive)
                {
                    static Cam cam = 0;
                    Natives::GetRootCam(&cam);
                    if (cam)
                    {
                        static float farclip;
                        static float nearclip;
                
                        Natives::GetCamFarClip(cam, &farclip);
                        Natives::GetCamNearClip(cam, &nearclip);
                
                        static float arr[4];
                        arr[0] = nearclip;
                        arr[1] = farclip;
                        arr[2] = nearclip;
                        arr[3] = farclip;
                        pDevice->SetVertexShaderConstantF(227, &arr[0], 1);
                    }
                
                    // DynamicShadowForTrees Wind Sway, More Shadows
                    {
                        static float arr2[4];
                        arr2[0] = Natives::IsInteriorScene() ? 0.0f : *dw11A2948;
                        arr2[1] = static_cast<float>(bMoreShadows);
                        arr2[2] = 0.0f;
                        arr2[3] = 0.0f;
                        pDevice->SetVertexShaderConstantF(233, &arr2[0], 1);
                    }

                    // Current Settings
                    {
                        static float arr5[4];

                        switch (FusionFixSettings.Get("PREF_WATER_QUALITY"))
                        {
                        case 0:
                            arr5[0] = 2.0f;
                            break;
                        case 1:
                            arr5[0] = 1.0f;
                            break;
                        case 2:
                            arr5[0] = 0.5f;
                            break;
                        case 3:
                        default:
                            arr5[0] = 0.25f;
                            break;
                        }
                        
                        arr5[1] = static_cast<float>(FusionFixSettings.Get("PREF_SHADOW_QUALITY"));
                        arr5[2] = 1.0f / (30.0f * Natives::Timestep());
                        arr5[3] = bFixCascadedShadowMapResolution ? 2.0f : 1.0f;
                        pDevice->SetPixelShaderConstantF(221, &arr5[0], 1);
                    }

                    // FXAA, DOF, Gamma
                    {
                        static auto cutscene_dof = FusionFixSettings.GetRef("PREF_TCYC_DOF");
                        static auto gamma = FusionFixSettings.GetRef("PREF_CONSOLE_GAMMA");
                        static auto mblur = FusionFixSettings.GetRef("PREF_MOTIONBLUR");
                        static float arr3[4];
                        arr3[0] = (bFixAutoExposure ? 1.0f : 0.0f);
                        arr3[1] = static_cast<float>(cutscene_dof->get() >= FusionFixSettings.DofText.eCutscenesOnly);
                        arr3[2] = static_cast<float>(gamma->get());
                        arr3[3] = static_cast<float>(mblur->get());
                        pDevice->SetPixelShaderConstantF(222, &arr3[0], 1);
                    }

                    // Cascaded Shadow Map Res, Time of Day, Tree Translucency
                    {
                        uint32_t hour = -1;
                        uint32_t minute = -1;
                        Natives::GetTimeOfDay(&hour, &minute);
                        static auto tree_lighting = FusionFixSettings.GetRef("PREF_TREE_LIGHTING");
                        static auto shadowFilter = FusionFixSettings.GetRef("PREF_SHADOW_FILTER");
                        static auto definition = FusionFixSettings.GetRef("PREF_DEFINITION");
                        static float arr4[4];
                        arr4[0] = static_cast<float>(tree_lighting->get() - FusionFixSettings.TreeFxText.ePC);
                        arr4[1] = static_cast<float>(FusionFixSettings.Get("PREF_PCSS"));
                        arr4[2] = static_cast<float>(definition->get() - FusionFixSettings.DefinitionText.eClassic);
                        arr4[3] = (((hour == 6 && minute >= 45) || (hour > 6)) && ((hour == 19 && minute < 15) || (hour < 19))) ? 0.0f : 1.0f;
                        pDevice->SetPixelShaderConstantF(223, &arr4[0], 1);
                    }
                }
            };
        };

        FusionFix::onInitEvent() += []()
        {
            CIniReader iniReader("");
            bFixCascadedShadowMapResolution = iniReader.ReadInteger("SHADOWS", "FixCascadedShadowMapResolution", 0) != 0;

            static auto bFixRainDrops = iniReader.ReadInteger("MISC", "FixRainDrops", 1) != 0;
            static auto nRainDropsBlur = iniReader.ReadInteger("MISC", "RainDropsBlur", 2);
            if (nRainDropsBlur < 1) {
                nRainDropsBlur = 1;
            }
            if (nRainDropsBlur > 4) {
                nRainDropsBlur = 4;
            }
            if (nRainDropsBlur != 1 && nRainDropsBlur != 2 && nRainDropsBlur != 4) {
                nRainDropsBlur = 2;
            }

            //SetRenderState D3DRS_ADAPTIVETESS_X
            auto pattern = hook::pattern("74 ? 68 4E 56 44 42 68");
            injector::WriteMemory<uint8_t>(pattern.get_first(0), 0xEB, true);

            //FusionFix::D3D9::onSetVertexShaderConstantF() += [](LPDIRECT3DDEVICE9& pDevice, UINT& StartRegister, float*& pConstantData, UINT& Vector4fCount)
            //{
            //
            //};

            FusionFix::D3D9::onSetPixelShaderConstantF() += [](LPDIRECT3DDEVICE9& pDevice, UINT& StartRegister, float*& pConstantData, UINT& Vector4fCount)
            {               
                if (bFixCascadedShadowMapResolution) {
                    if (StartRegister == 53 && Vector4fCount == 1 &&
                        oldCascadesWidth != 0 && oldCascadesHeight != 0 &&
                        pConstantData[0] == 1.0f / (float)oldCascadesWidth && pConstantData[1] == 1.0f / (float)oldCascadesHeight)
                    {
                        // set pixel size to pixel shader
                        pConstantData[0] = 1.0f / float(oldCascadesWidth * 2);
                        pConstantData[1] = 1.0f / float(oldCascadesHeight * 2);
                        pConstantData[2] = 1.0f / float(oldCascadesHeight * 2);
                    };
                }
            };

            FusionFix::D3D9::onBeforeCreateTexture() += [](LPDIRECT3DDEVICE9& pDevice, UINT& Width, UINT& Height, UINT& Levels, DWORD& Usage, D3DFORMAT& Format, D3DPOOL& Pool, IDirect3DTexture9**& ppTexture, HANDLE*& pSharedHandle)
            {
                if (bFixCascadedShadowMapResolution)
                {
                    if (Format == D3DFORMAT(D3DFMT_R16F) && Height >= 256 && Width == Height * 4 && Levels == 1)
                    {
                        oldCascadesWidth = Width;
                        oldCascadesHeight = Height;
            
                        Width *= 2;
                        Height *= 2;
                    }
                }
            };
            
            FusionFix::D3D9::onAfterCreateTexture() += [](LPDIRECT3DDEVICE9& pDevice, UINT& Width, UINT& Height, UINT& Levels, DWORD& Usage, D3DFORMAT& Format, D3DPOOL& Pool, IDirect3DTexture9**& ppTexture, HANDLE*& pSharedHandle)
            {
                if (bFixRainDrops && Format == D3DFMT_A16B16G16R16F && Width == (gRect.right - gRect.left) / nRainDropsBlur &&
                    Height == (gRect.bottom - gRect.top) / nRainDropsBlur && ppTexture != nullptr && *ppTexture != nullptr) {
                    pHDRTexQuarter = *ppTexture;
                }
            };
            
            FusionFix::D3D9::onSetTexture() += [](LPDIRECT3DDEVICE9& pDevice, DWORD& Stage, IDirect3DBaseTexture9*& pTexture)
            {
                if (bFixRainDrops && Stage == 1 && pTexture == nullptr && pHDRTexQuarter) {
                    pTexture = pHDRTexQuarter;
                }
            };

            FusionFix::D3D9::onBeforeCreateDevice() += [](LPDIRECT3D9& pDirect3D9, UINT& Adapter, D3DDEVTYPE& DeviceType, HWND& hFocusWindow, DWORD& BehaviorFlags, D3DPRESENT_PARAMETERS*& pPresentationParameters, IDirect3DDevice9**& ppReturnedDeviceInterface)
            {
                pHDRTexQuarter = nullptr;
            };

            FusionFix::D3D9::onBeforeReset() += [](LPDIRECT3DDEVICE9& pDevice, D3DPRESENT_PARAMETERS*& pPresentationParameters)
            {
                pHDRTexQuarter = nullptr;
            };

            FusionFix::onBeforeReset() += []()
            {
                pHDRTexQuarter = nullptr;
            };
        };
    };
} Shaders;