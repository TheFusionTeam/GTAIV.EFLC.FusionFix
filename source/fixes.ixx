module;

#include <common.hxx>

export module fixes;

import common;
import comvars;
import settings;
import natives;

class Fixes
{
public:
    static inline uint32_t nTimeToPassBeforeCenteringCameraOnFoot = 0;
    static inline bool bWaitBeforeCenteringCameraOnFootUsingPad = false;

    static inline bool* bIsPhoneShowing = nullptr;
    static inline injector::hook_back<int32_t(__cdecl*)()> hbsub_B2CE30;
    static int32_t sub_B2CE30() 
    {
        if ((bIsPhoneShowing && *bIsPhoneShowing))
            return 1;

        return hbsub_B2CE30.fun();
    }

    static inline injector::hook_back<void(__fastcall*)(int32_t, int32_t)> hbsub_B07600;
    static void __fastcall sub_B07600(int32_t _this, int32_t) 
    {
        hbsub_B07600.fun(_this, 0);

        static auto nCustomFOV = FusionFixSettings.GetRef(PREF_CUSTOMFOV);
        *(float*)(_this + 0x60) += nCustomFOV->get() * 5.0f;
    }

    Fixes()
    {
        FusionFix::onInitEventAsync() += []()
        {
            CIniReader iniReader("");

            int32_t nAimingZoomFix = iniReader.ReadInteger("MAIN", "AimingZoomFix", 1);
            bool bRecoilFix = iniReader.ReadInteger("MAIN", "RecoilFix", 1) != 0;

            //[MISC]
            bool bDefaultCameraAngleInTLAD = iniReader.ReadInteger("MISC", "DefaultCameraAngleInTLAD", 0) != 0;
            bool bPedDeathAnimFixFromTBOGT = iniReader.ReadInteger("MISC", "PedDeathAnimFixFromTBOGT", 1) != 0;

            //fix for zoom flag in tbogt
            if (nAimingZoomFix)
            {
                auto pattern = find_pattern("8A C1 32 05 ? ? ? ? 24 01 32 C1", "8A D0 32 15");
                static auto& byte_F47AB1 = **pattern.get_first<uint8_t*>(4);
                if (nAimingZoomFix > 1)
                    injector::MakeNOP(pattern.get_first(-2), 2, true);
                else if (nAimingZoomFix < 0)
                    injector::WriteMemory<uint8_t>(pattern.get_first(-2), 0xEB, true);

                pattern = find_pattern("80 8E ? ? ? ? ? EB 43");
                struct AimZoomHook1
                {
                    void operator()(injector::reg_pack& regs)
                    {
                        *(uint8_t*)(regs.esi + 0x200) |= 1;
                        byte_F47AB1 = 1;
                    }
                }; 
                if (!pattern.empty())
                    injector::MakeInline<AimZoomHook1>(pattern.get_first(0), pattern.get_first(7));
                else {
                    pattern = find_pattern("08 9E ? ? ? ? E9");
                    injector::MakeInline<AimZoomHook1>(pattern.get_first(0), pattern.get_first(6));
                }

                pattern = hook::pattern("76 09 80 A6 ? ? ? ? ? EB 26");
                struct AimZoomHook2
                {
                    void operator()(injector::reg_pack& regs)
                    {
                        *(uint8_t*)(regs.esi + 0x200) &= 0xFE;
                        byte_F47AB1 = 0;
                    }
                };
                if (!pattern.empty())
                    injector::MakeInline<AimZoomHook2>(pattern.get_first(2), pattern.get_first(9));
                else {
                    pattern = find_pattern("80 A6 ? ? ? ? ? EB 25");
                    injector::MakeInline<AimZoomHook2>(pattern.get_first(0), pattern.get_first(7));
                }

                //let's default to 0 as well
                pattern = hook::pattern("C6 05 ? ? ? ? ? 74 12 83 3D");
                if (!pattern.empty())
                    injector::WriteMemory<uint8_t>(pattern.get_first(6), 0, true);
                else {
                    pattern = hook::pattern("88 1D ? ? ? ? 74 10");
                    injector::WriteMemory<uint8_t>(pattern.get_first(1), 0x25, true); //mov ah
                }
            }

            if (bRecoilFix)
            {
                static float fRecMult = 0.65f;
                auto pattern = find_pattern("F3 0F 10 44 24 ? F3 0F 59 05 ? ? ? ? EB 1E E8 ? ? ? ? 84 C0", "F3 0F 10 44 24 ? F3 0F 59 05 ? ? ? ? EB ? E8");
                injector::WriteMemory(pattern.get_first(10), &fRecMult, true);
            }

            if (bDefaultCameraAngleInTLAD)
            {
                static uint32_t episode_id = 0;
                auto pattern = find_pattern<2>("83 3D ? ? ? ? ? 8B 01 0F 44 C2 89 01 B0 01 C2 08 00", "83 3D ? ? ? ? ? 75 06 C7 00 ? ? ? ? B0 01 C2 08 00");
                injector::WriteMemory(pattern.count(2).get(0).get<void>(2), &episode_id, true);
            }

            if (bPedDeathAnimFixFromTBOGT)
            {
                auto pattern = hook::pattern("8B D9 75 2E");
                if (!pattern.empty())
                    injector::MakeNOP(pattern.get_first(2), 2, true);
                else
                {
                    pattern = hook::pattern("BB ? ? ? ? 75 29 80 7F 28 00");
                    injector::MakeNOP(pattern.get_first(5), 2, true);
                }
            }

            {
                static constexpr float xmm_0 = FLT_MAX / 2.0f;
                unsigned char bytes[4];
                auto n = (uint32_t)&xmm_0;
                bytes[0] = (n >> 24) & 0xFF;
                bytes[1] = (n >> 16) & 0xFF;
                bytes[2] = (n >> 8) & 0xFF;
                bytes[3] = (n >> 0) & 0xFF;

                auto pattern = find_pattern("F3 0F 10 05 ? ? ? ? F3 0F 58 47 ? F3 0F 11 47 ? 8B D1 89 54 24 10", "F3 0F 10 05 ? ? ? ? F3 0F 58 46 ? 89 8C 24");
                static raw_mem CoverCB(pattern.get_first(4), { bytes[3], bytes[2], bytes[1], bytes[0] });
                FusionFixSettings.SetCallback("PREF_COVERCENTERING", [](int32_t value) {
                    if (value)
                        CoverCB.Restore();
                    else
                        CoverCB.Write();
                });
                if (!FusionFixSettings("PREF_COVERCENTERING"))
                    CoverCB.Write();
            }

            // reverse lights fix
            {
                auto pattern = find_pattern("8B 40 64 FF D0 F3 0F 10 40 ? 8D 44 24 40 50", "8B 42 64 8B CE FF D0 F3 0F 10 40");
                injector::WriteMemory<uint8_t>(pattern.get_first(2), 0x60, true);
            }

            // animation fix for phone interaction on bikes
            {
                auto pattern = hook::pattern("83 3D ? ? ? ? 01 0F 8C 18 01 00 00");
                if (!pattern.empty())
                    injector::MakeNOP(pattern.get(0).get<int>(0), 13, true);
                else {
                    pattern = hook::pattern("80 BE 18 02 00 00 00 0F 85 36 01 00 00 80 BE");
                    injector::MakeNOP(pattern.get(0).get<int>(0x21), 6, true);
                }
            }

            //fix for lods appearing inside normal models, unless the graphics menu was opened once (draw distances aren't set properly?)
            {
                auto pattern = find_pattern("E8 ? ? ? ? 8D 4C 24 10 F3 0F 11 05 ? ? ? ? E8 ? ? ? ? 8B F0 E8 ? ? ? ? DF 2D", "E8 ? ? ? ? 8D 44 24 10 83 C4 04 50");
                auto sub_477300 = injector::GetBranchDestination(pattern.get_first(0));
                pattern = find_pattern("E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 83 C4 2C C3", "E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 8B 35 ? ? ? ? E8 ? ? ? ? 25 FF FF 00 00");
                injector::MakeCALL(pattern.get_first(0), sub_477300, true);
            }

            // Make LOD lights appear at the appropriate time like on the console version (consoles: 7 PM, pc: 10 PM)
            {
                auto pattern = find_pattern("8D 42 13", "8D 51 13");
                injector::WriteMemory<uint8_t>(pattern.get_first(2), 0x10, true);
                pattern = find_pattern("8D 42 14 3B C8", "83 C1 14 3B C1");
                injector::WriteMemory<uint8_t>(pattern.get_first(2), 0x10, true);
                injector::WriteMemory<uint8_t>(pattern.get_first(8), 0x07, true);
                // Removing episode id check that resulted in flickering LOD lights at certain camera angles in TBOGT
                pattern = hook::pattern("83 3D ? ? ? ? ? 0F 85 ? ? ? ? F3 0F 10 05 ? ? ? ? F3 0F 10 8C 24");
                if (!pattern.empty())
                    injector::WriteMemory<uint16_t>(pattern.get_first(7), 0xE990, true); // jnz -> jmp
                else {
                    pattern = hook::pattern("83 3D ? ? ? ? ? 75 68 F3 0F 10 05");
                    injector::WriteMemory<uint8_t>(pattern.get_first(7), 0xEB, true); // jnz -> jmp
                }
            }

            {
                auto pattern = find_pattern("F3 0F 11 4C 24 ? 85 DB 74 1B", "F3 0F 11 44 24 ? 74 1B F6 86");
                static auto reg = *pattern.get_first<uint8_t>(5);
                static auto nTimeToWaitBeforeCenteringCameraOnFootKB = FusionFixSettings.GetRef(PREF_KBCAMCENTERDELAY);
                static auto nTimeToWaitBeforeCenteringCameraOnFootPad = FusionFixSettings.GetRef(PREF_PADCAMCENTERDELAY);
                struct OnFootCamCenteringHook 
                {
                    void operator()(injector::reg_pack& regs) 
                    {
                        float f = 0.0f;
                        _asm { movss f, xmm1 }

                        if (reg == 0x48)
                            _asm { movss f, xmm0 }

                        float& posX = *(float*)(regs.esp + reg);
                        bool pad = Natives::IsUsingController();
                        int32_t x = 0;
                        int32_t y = 0;

                        if (pad) 
                        {
                            Natives::GetPadState(0, 2, &x);
                            Natives::GetPadState(0, 3, &y);
                        }
                        else
                            Natives::GetMouseInput(&x, &y);

                        if (x || y)
                            nTimeToPassBeforeCenteringCameraOnFoot = *CTimer__m_snTimeInMilliseconds + ((pad ? nTimeToWaitBeforeCenteringCameraOnFootPad->get() : nTimeToWaitBeforeCenteringCameraOnFootKB->get()) * 1000);

                        if (pad && !nTimeToWaitBeforeCenteringCameraOnFootPad->get())
                            nTimeToPassBeforeCenteringCameraOnFoot = 0;

                        if (nTimeToPassBeforeCenteringCameraOnFoot < *CTimer__m_snTimeInMilliseconds)
                            posX = f;
                        else
                            posX = 0.0f;
                    }
                }; 
                
                if (reg != 0x48)
                    injector::MakeInline<OnFootCamCenteringHook>(pattern.get_first(0), pattern.get_first(6));
                else
                {
                    injector::MakeInline<OnFootCamCenteringHook>(pattern.get_first(-2), pattern.get_first(6));
                    injector::WriteMemory<uint16_t>(pattern.get_first(3), 0xDB85, true);
                }
            }

            // Disable drive-by while using cellphone
            {
                auto pattern = find_pattern("E8 ? ? ? ? 85 C0 0F 85 ? ? ? ? 84 DB 74 5A 85 F6 0F 84", "E8 ? ? ? ? 85 C0 0F 85 ? ? ? ? 84 DB 74 61");
                bIsPhoneShowing = *find_pattern("C6 05 ? ? ? ? ? E8 ? ? ? ? 6A 00 E8 ? ? ? ? 8B 80", "88 1D ? ? ? ? 88 1D ? ? ? ? E8 ? ? ? ? 6A 00").get_first<bool*>(2);
                hbsub_B2CE30.fun = injector::MakeCALL(pattern.get_first(0), sub_B2CE30, true).get();
            }

            // Custom FOV
            {
                auto pattern = hook::pattern("E8 ? ? ? ? F6 87 ? ? ? ? ? 5B");
                if (!pattern.empty())
                    hbsub_B07600.fun = injector::MakeCALL(pattern.get_first(0), sub_B07600, true).get();
            }
        };
    }
} Fixes;