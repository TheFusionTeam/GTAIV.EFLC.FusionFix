module;

#include <common.hxx>

export module consoleshadows;

import common;
import settings;
import comvars;

void* fnAE3DE0 = nullptr;
void* fnAE3310 = nullptr;
bool bHeadlightShadows = false;
bool bVehicleNightShadows = false;
int __cdecl sub_AE3DE0(int a1, int a2)
{
    if (bVehicleNightShadows || bHeadlightShadows)
        injector::cstd<void(int, int, int, int, int)>::call(fnAE3310, a1, 0, 0, 0, a2);
    return injector::cstd<int(int, int)>::call(fnAE3DE0, a1, a2);
}

SafetyHookInline sh_grcSetRendersState{};
uint32_t* dword_17F58E4;
void __stdcall grcSetRenderStateHook()
{
    if (dword_17F58E4 && *dword_17F58E4)
    {
        return sh_grcSetRendersState.unsafe_stdcall();
    }
}

namespace CShadows
{
    injector::hook_back<void(__cdecl*)(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)> hbStoreStaticShadow;

    void __cdecl StoreStaticShadowPlayerDriving(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15)
    {
        if (!bHeadlightShadows)
        {
            a3 &= ~3;
            a3 &= ~4;
        }

        return hbStoreStaticShadow.fun(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
    }

    void __cdecl StoreStaticShadowNPC(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15)
    {
        a3 &= ~3;
        a3 &= ~4;

        return hbStoreStaticShadow.fun(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
    }
}
class ConsoleShadows
{
public:
    ConsoleShadows()
    {
        FusionFix::onInitEventAsync() += []()
        {
            CIniReader iniReader("");
            bVehicleNightShadows = iniReader.ReadInteger("NIGHTSHADOWS", "VehicleNightShadows", 0) != 0;

            // Render dynamic shadows casted by vehicles from point lights.
            {
                auto pattern = hook::pattern("E8 ? ? ? ? 83 C4 08 57 56");
                fnAE3DE0 = injector::GetBranchDestination(pattern.get_first()).get();
                injector::MakeCALL(pattern.get_first(), sub_AE3DE0, true);
                pattern = find_pattern("55 8B EC 83 E4 F0 83 EC 28 80 3D ? ? ? ? ? 56 57 74 27", "55 8B EC 83 E4 F0 83 EC 24 53 56 57 33 FF 80 3D");
                fnAE3310 = pattern.get_first();
            }

            // Fix crash caused by the patches below.
            {
                auto pattern = find_pattern("89 35 ? ? ? ? E8 ? ? ? ? 5E C2 04 00", "89 1D ? ? ? ? 8B 07 8B 50 10 8B CF 89 1D");
                dword_17F58E4 = *pattern.get_first<uint32_t*>(2);
                pattern = find_pattern("51 53 55 8B 2D ? ? ? ? 56 0F B7 5D 1C 33 F6 85 DB 7E 3A 57 EB 09", "53 56 57 8B 3D ? ? ? ? 0F B7 5F 1C 33 F6 85 DB 7E 41 55 EB 0A");
                sh_grcSetRendersState = safetyhook::create_inline(pattern.get_first(0), grcSetRenderStateHook);
            }

            // Enable player/ped shadows while in vehicles
            //if (bVehicleNightShadows)
            //{
            //    auto pattern = hook::pattern("75 14 F6 86 ? ? ? ? ? 74 0B 80 7C 24 ? ? 0F 84 ? ? ? ? C6 44 24");
            //    if (!pattern.empty())
            //    {
            //        injector::WriteMemory<uint8_t>(pattern.get_first(0), 0xEB, true);
            //        pattern = hook::pattern("75 12 8B 86 ? ? ? ? C1 E8 0B 25 ? ? ? ? 89 44 24 0C 85 D2");
            //        injector::WriteMemory<uint8_t>(pattern.get_first(0), 0xEB, true);
            //    }
            //    else
            //    {
            //        pattern = hook::pattern("75 17 F6 86 ? ? ? ? ? 74 0E 80 7C 24 ? ? 0F 84");
            //        injector::WriteMemory<uint8_t>(pattern.get_first(0), 0xEB, true);
            //        pattern = hook::pattern("75 0F 8B 86 ? ? ? ? C1 E8 0B 24 01 88 44 24 0E");
            //        injector::WriteMemory<uint8_t>(pattern.get_first(0), 0xEB, true);
            //    }
            //
            //}

            // Headlight shadows
            {
                auto pattern = hook::pattern("68 04 05 00 00 6A 02 6A 00");
                if (!pattern.count(2).empty())
                {
                    CShadows::hbStoreStaticShadow.fun = injector::MakeCALL(pattern.count(2).get(0).get<void*>(9), CShadows::StoreStaticShadowPlayerDriving).get();
                    CShadows::hbStoreStaticShadow.fun = injector::MakeCALL(pattern.count(2).get(1).get<void*>(9), CShadows::StoreStaticShadowPlayerDriving).get();
                }
            
                pattern = hook::pattern("68 04 01 00 00 6A 02 6A 00");
                if (!pattern.count(2).empty())
                {
                    CShadows::hbStoreStaticShadow.fun = injector::MakeCALL(pattern.count(2).get(0).get<void*>(9), CShadows::StoreStaticShadowNPC).get();
                    CShadows::hbStoreStaticShadow.fun = injector::MakeCALL(pattern.count(2).get(1).get<void*>(9), CShadows::StoreStaticShadowNPC).get();
                }
            
                FusionFixSettings.SetCallback("PREF_HEADLIGHTSHADOWS", [](int32_t value)
                {
                    bHeadlightShadows = value;
                });
                bHeadlightShadows = FusionFixSettings("PREF_HEADLIGHTSHADOWS");

                struct test
                {
                    void operator()(injector::reg_pack& regs)
                    {
                        auto FindPlayerCar = (int (*)())0x93F1C0;
                        auto getLocalPlayerPed = (int (*)())0x93F050;

                        if (bHeadlightShadows)
                        {
                            auto car = FindPlayerCar();

                            if (regs.esi && (regs.esi == car || (regs.esi == getLocalPlayerPed() && car && *(BYTE*)(car + 0xFA0))))
                            {
                                *(uintptr_t*)(regs.esp - 4) = 0xAE3867;
                                return;
                            }
                        }
                
                        if ((bVehicleNightShadows && !bHeadlightShadows) && (regs.eax == 3 || regs.eax == 4))
                        {
                            *(uintptr_t*)(regs.esp - 4) = 0xAE376B;
                            return;
                        }
                
                        if ((*(BYTE*)(regs.esi + 620) & 4) != 0)
                        {
                            
                        }
                        else
                        {
                            *(uintptr_t*)(regs.esp - 4) = 0xAE374F;
                        }
                    }
                }; injector::MakeInline<test>(0xAE3736, 0xAE3744);
            }
        };
    }
} ConsoleShadows;