module;

#include <common.hxx>
#include "d3dx9.h"
#pragma comment(lib, "d3dx9.lib")

export module settings;

import common;
import comvars;

export class CSettings
{
private:
    struct CSetting
    {
        int32_t value = 0;
        std::string prefName;
        std::string iniSec;
        std::string iniName;
        std::string strEnum;
        int32_t iniDefValInt = 0;
        std::function<void(int32_t value)> callback;
        int32_t idStart;
        int32_t idEnd;

        auto GetValue() { return value; }
        auto SetValue(auto v) { value = v; WriteToIni(); if (callback) callback(value); }
        auto ReadFromIni(auto& iniReader) { return iniReader.ReadInteger(iniSec, iniName, iniDefValInt); }
        auto ReadFromIni() { CIniReader iniReader(""); return ReadFromIni(iniReader); }
        void WriteToIni(auto& iniWriter) { iniWriter.WriteInteger(iniSec, iniName, value); }
        void WriteToIni() { CIniReader iniWriter(""); iniWriter.WriteInteger(iniSec, iniName, value); }
    };

    struct MenuPrefs
    {
        uint32_t prefID;
        char* name;
    };

    static inline std::vector<MenuPrefs> aMenuPrefs;
    static inline auto firstCustomID = 0;
private:
    static inline int32_t* mPrefs;
    static inline std::map<uint32_t, CSetting> mFusionPrefs;

    std::optional<std::string> GetPrefNameByID(auto prefID) {
        auto it = std::find_if(std::begin(aMenuPrefs), std::end(aMenuPrefs), [&prefID](auto& it) {
            return it.prefID == prefID;
        });
        if (it != std::end(aMenuPrefs))
            return std::string(it->name);
        return std::nullopt;
    }
    std::optional<int32_t> GetPrefIDByName(auto prefName) {
        auto it = std::find_if(std::begin(aMenuPrefs), std::end(aMenuPrefs), [&prefName](auto& it) {
            return std::string_view(it.name) == prefName;
        });
        if (it != std::end(aMenuPrefs))
            return it->prefID;
        return std::nullopt;
    }
public:
    CSettings()
    {
        MenuPrefs* originalPrefs = nullptr;
        MenuPrefs** ppOriginalPrefs = nullptr;

        auto pattern = hook::pattern("8B 04 FD ? ? ? ? 5F 5E C3");
        if (pattern.size() == 4)
        {
            ppOriginalPrefs = pattern.count(4).get(3).get<MenuPrefs*>(3);
            originalPrefs = *ppOriginalPrefs;
        }
        else
        {
            pattern = hook::pattern("8B 04 F5 ? ? ? ? 5E C3 8B 04 F5 ? ? ? ? 5E C3 8B 04 F5 ? ? ? ? 5E C3 8B 04 F5");
            ppOriginalPrefs = pattern.get_first<MenuPrefs*>(3);
            originalPrefs = *ppOriginalPrefs;
        }
        
        auto pOriginalPrefsNum = find_pattern("81 FF ? ? ? ? 7C DF", "81 FE ? ? ? ? 7C E0 33 F6").get_first<uint32_t>(2);

        for (auto i = 0; originalPrefs[i].prefID < *pOriginalPrefsNum; i++)
        {
            aMenuPrefs.emplace_back(originalPrefs[i].prefID, originalPrefs[i].name);
        }

        aMenuPrefs.reserve(aMenuPrefs.size() * 2);
        firstCustomID = aMenuPrefs.back().prefID + 1;

        injector::WriteMemory(ppOriginalPrefs, &aMenuPrefs[0].prefID, true);
        injector::WriteMemory(find_pattern("FF 34 FD ? ? ? ? 56 E8 ? ? ? ? 83 C4 08 85 C0 0F 84 ? ? ? ? 47 81 FF", "8B 04 F5 ? ? ? ? 50 57 E8 ? ? ? ? 83 C4 08 85 C0 74 7C").get_first(3), &aMenuPrefs[0].name, true);

        pattern = hook::pattern("89 1C ? ? ? ? ? E8");
        mPrefs = *pattern.get_first<int32_t*>(3);

        CIniReader iniReader("");

        static CSetting arr[] = {
            { 0, "PREF_SKIP_INTRO",       "MAIN",       "SkipIntro",                    "",                           1, nullptr, 0, 1 },
            { 0, "PREF_SKIP_MENU",        "MAIN",       "SkipMenu",                     "",                           1, nullptr, 0, 1 },
            { 0, "PREF_BORDERLESS",       "MAIN",       "BorderlessWindowed",           "",                           1, nullptr, 0, 1 },
            { 0, "PREF_FPS_LIMIT_PRESET", "FRAMELIMIT", "FpsLimitPreset",               "MENU_DISPLAY_FRAMELIMIT",    0, nullptr, FpsCaps.eOFF, std::distance(std::begin(FpsCaps.data), std::end(FpsCaps.data)) - 1 },
            { 0, "PREF_FXAA",             "MISC",       "FXAA",                         "",                           1, nullptr, 0, 1 },
            { 0, "PREF_CONSOLE_GAMMA",    "MISC",       "ConsoleGamma",                 "",                           0, nullptr, 0, 1 },
            { 0, "PREF_TIMECYC",          "MISC",       "ScreenFilter",                 "MENU_DISPLAY_TIMECYC",       5, nullptr, TimecycText.eMO_DEF, std::distance(std::begin(TimecycText.data), std::end(TimecycText.data)) - 1 },
            { 0, "PREF_CUTSCENE_DOF",     "MISC",       "DepthOfField",                 "",                           0, nullptr, 0, 1 },
            { 0, "PREF_CONSOLE_SHADOWS",  "SHADOWS",    "ConsoleShadows",               "",                           1, nullptr, 0, 1 },
            { 0, "PREF_SHADOW_FILTER",    "SHADOWS",    "ShadowFilter",                 "MENU_DISPLAY_SHADOWFILTER",  0, nullptr, ShadowFilterText.eSharp, std::distance(std::begin(ShadowFilterText.data), std::end(ShadowFilterText.data)) - 1 },
            { 0, "PREF_TREE_LIGHTING",    "MISC",       "TreeLighting",                 "MENU_DISPLAY_TREE_LIGHTING", 5, nullptr, TreeFxText.ePC, std::distance(std::begin(TreeFxText.data), std::end(TreeFxText.data)) - 1 },
            { 0, "PREF_TCYC_DOF",         "MISC",       "DistantBlur",                  "MENU_DISPLAY_DOF",           5, nullptr, DofText.eOff, std::distance(std::begin(DofText.data), std::end(DofText.data)) - 1 },
            { 0, "PREF_MOTIONBLUR",       "MAIN",       "MotionBlur",                   ""                          , 0, nullptr, 0, 1 },
            { 0, "PREF_LEDILLUMINATION",  "MISC",       "LightSyncRGB",                 ""                          , 0, nullptr, 0, 1 },
            { 0, "PREF_DEFINITION",       "MAIN",       "Definition",                   "MENU_DISPLAY_DEFINITION"   , 1, nullptr, DefinitionText.eClassic, std::distance(std::begin(DefinitionText.data), std::end(DefinitionText.data)) - 1 },
            { 0, "PREF_BLOOM",            "MAIN",       "Bloom",                        ""                          , 0, nullptr, 0, 1 },
            { 0, "PREF_FPSCOUNTER",       "FRAMELIMIT", "DisplayFpsCounter",            ""                          , 0, nullptr, 0, 1 },
            { 0, "PREF_ALWAYSRUN",        "MISC",       "AlwaysRun",                    ""                          , 0, nullptr, 0, 1 },
        };

        auto i = firstCustomID;
        for (auto& it : arr)
        {
            mFusionPrefs[i] = it;
            mFusionPrefs[i].value = std::clamp(it.ReadFromIni(iniReader), it.idStart, it.idEnd);
            aMenuPrefs.emplace_back(i, mFusionPrefs[i].prefName.data());
            i++;
        }

        injector::WriteMemory(pOriginalPrefsNum, aMenuPrefs.size(), true);

        // MENU_DISPLAY enums hook
        CSettings::MenuPrefs* originalEnums = nullptr;
        static std::vector<MenuPrefs> aMenuEnums;
        pattern = hook::pattern("8B 04 FD ? ? ? ? 5F 5E C3");
        if (pattern.size() == 4)
        {
            ppOriginalPrefs = pattern.count(4).get(1).get<MenuPrefs*>(3);
            originalEnums = *ppOriginalPrefs;
        }
        else
        {
            pattern = hook::pattern("8B 04 F5 ? ? ? ? 5E C3 8B 04 F5");
            ppOriginalPrefs = pattern.count(4).get(2).get<MenuPrefs*>(3);
            originalEnums = *ppOriginalPrefs;
            
        }

        auto pOriginalEnumsNum = find_pattern("83 FF 3C 7C E2", "83 FE 3C 7C E3 33 F6").get_first<uint8_t>(2);
        auto pOriginalEnumsNum2 = find_pattern("83 FE 3C 7C E6", "83 FE 3C 7C E3 33 C0").get_first<uint8_t>(2);

        for (auto i = 0; originalEnums[i].prefID < *pOriginalEnumsNum; i++)
        {
            aMenuEnums.emplace_back(originalEnums[i].prefID, originalEnums[i].name);
        }
        aMenuEnums.reserve(aMenuEnums.size() * 2);
        auto firstEnumCustomID = aMenuEnums.back().prefID + 1;

        for (auto& it : arr)
        {
            if (!it.strEnum.empty()) {
                aMenuEnums.emplace_back(firstEnumCustomID, it.strEnum.data());
                firstEnumCustomID += 1;
            }
        }

        injector::WriteMemory(ppOriginalPrefs, &aMenuEnums[0].prefID, true);
        injector::WriteMemory(find_pattern("FF 34 FD ? ? ? ? 56 E8 ? ? ? ? 83 C4 08 85 C0 0F 84 ? ? ? ? 47 83 FF 3C", "8B 14 F5 ? ? ? ? 52").get_first(3), &aMenuEnums[0].name, true);
        pattern = find_pattern("33 C0 5E C3 8B 04 F5 ? ? ? ? 5F 5E C3 8B 04 F5", "33 C0 5E C3 8B 04 F5 ? ? ? ? 5E C3 8B 04 F5");
        injector::WriteMemory(pattern.get_first(7), &aMenuEnums[0].prefID, true);
        pattern = find_pattern("FF 34 F5 ? ? ? ? 57 E8 ? ? ? ? 83 C4 08 85 C0 74 0B 46 83 FE 3C", "8B 0C F5 ? ? ? ? 51 57 E8 ? ? ? ? 83 C4 08 85 C0 74 15");
        injector::WriteMemory(pattern.get_first(3), &aMenuEnums[0].name, true);

        injector::WriteMemory<uint8_t>(pOriginalEnumsNum, aMenuEnums.size(), true);
        injector::WriteMemory<uint8_t>(pOriginalEnumsNum2, aMenuEnums.size(), true);

        {
            static std::vector<MenuPrefs> aMenuPrefs2(aMenuPrefs.size());

            hook::pattern patterns[] = {
                hook::pattern("89 14 8D ? ? ? ? 66 8B 0D"), hook::pattern("89 3C AD ? ? ? ? 0F B7 3A 83 C6 01"),
                hook::pattern("89 04 8D ? ? ? ? 8B 74 24 10"), hook::pattern("8B 04 85 ? ? ? ? 85 C0 7F 05")
            };

            for (auto& pattern : patterns)
                if (!pattern.empty())
                    injector::WriteMemory(pattern.get_first(3), &aMenuPrefs2[0].prefID, true);
        }
    }
public:
    int32_t Get(int32_t prefID)
    {
        if (prefID >= firstCustomID)
            return mFusionPrefs[prefID].GetValue();
        else {
            DWORD tmp;
            injector::UnprotectMemory(&mPrefs[prefID], sizeof(int32_t), tmp);
            return mPrefs[prefID];
        }
    }
    auto Set(int32_t prefID, int32_t value) {
        if (prefID >= firstCustomID) {
            mFusionPrefs[prefID].SetValue(value);
        }
        else {
            DWORD tmp;
            injector::UnprotectMemory(&mPrefs[prefID], sizeof(int32_t), tmp);
            mPrefs[prefID] = value;
        }
    }
    int32_t Get(std::string_view name)
    {
        auto prefID = GetPrefIDByName(name);
        if (prefID) { return Get(*prefID); }
        return 0;
    }
    auto Set(std::string_view name, int32_t value) {
        auto prefID = GetPrefIDByName(name);
        if (prefID) return Set(*prefID, value);
    }
    auto isSame(int32_t id, std::string_view name) {
        auto prefID = GetPrefIDByName(name);
        if (prefID && *prefID == id)
            return true;
        return false;
    }
    std::optional<std::reference_wrapper<int32_t>> GetRef(std::string_view name)
    {
        auto prefID = GetPrefIDByName(name);
        if (prefID) {
            if (prefID >= firstCustomID)
                return std::ref(mFusionPrefs[*prefID].value);
            else {
                DWORD tmp;
                injector::UnprotectMemory(&mPrefs[*prefID], sizeof(int32_t), tmp);
                return std::ref(mPrefs[*prefID]);
            }
        }
        return std::nullopt;
    }
    void SetCallback(std::string_view name, std::function<void(int32_t)>&& cb)
    {
        auto prefID = GetPrefIDByName(name);
        if (prefID) mFusionPrefs[*prefID].callback = cb;
    }
    void ForEachPref(std::function<void(int32_t id, int32_t idStart, int32_t idEnd)>&& cb)
    {
        for (auto& it : mFusionPrefs)
        {
            cb(it.first, it.second.idStart, it.second.idEnd);
        }
    }
    auto operator()(int32_t i) { return Get(i); }
    auto operator()(std::string_view name) { return Get(name); }

public:
    struct
    {
        enum eFpsCaps { eTOGGLE, eHOLD, eOFF, eCustom, e30 = 30, e40 = 40, e50 = 50, e60 = 60, e75 = 75, e100 = 100, e120 = 120, e144 = 144, e165 = 165, e240 = 240, e360 = 360 };
        std::vector<int32_t> data = { eTOGGLE, eHOLD, eOFF, eCustom, e30, e40, e50, e60, e75, e100, e120, e144, e165, e240, e360 };
    } FpsCaps;

    struct
    {
        enum eTimecycText { eLow, eMedium, eHigh, eVeryHigh, eHighest, eMO_DEF, eOFF, eIV, eTLAD, eTBOGT };
        std::vector<const char*> data = { "Low", "Medium", "High", "Very High", "Highest", "MO_DEF", "OFF", "IV", "TLAD", "TBOGT" };
    } TimecycText;

    struct
    {
        enum eShadowFilterText {
            eRadio, eSequential, eShuffle, eSharp, eSoft, eSofter, eSoftest, PCSS
        };
        std::vector<const char*> data = { "Radio", "Sequential", "Shuffle", "Sharp", "Soft", "Softer", "Softest", "PCSS" };
    } ShadowFilterText;

    struct
    {
        enum eDofText {
            eAuto, e43, e54, e159, e169, eOff, eLow, eMedium, eHigh, eVeryHigh
        };
        std::vector<const char*> data = { "Auto", "4:3", "5:4", "15:9", "16:9", "Off", "Low", "Medium", "High", "Very High" };
    } DofText;

    struct
    {
        enum eTreeFxText {
            eAuto, e43, e54, e159, e169, e1610, ePC, ePCPlus, eConsole
        };
        std::vector<const char*> data = { "Auto", "4:3", "5:4", "15:9", "16:9", "16:10", "PC", "PC+", "Console" };
    } TreeFxText;

    struct
    {
        enum eDefinitionText { eLow, eMedium, eHigh, eVeryHigh, eClassic, eImproved, eExtra };
        std::vector<const char*> data = { "Low", "Medium", "High", "Very High", "Classic", "Improved", "Extra" };
    } DefinitionText;

} FusionFixSettings;

class Settings
{
public:
    Settings()
    {
        FusionFix::onInitEvent() += []()
        {
            // runtime settings
            auto pattern = hook::pattern("89 1C ? ? ? ? ? E8 ? ? ? ? A1");
            static auto reg = *pattern.get_first<uint8_t>(2);
            struct IniWriter
            {
                void operator()(injector::reg_pack& regs)
                {
                    auto id = regs.edx;
                    auto value = regs.ebx;

                    if (reg == 0x8D) {
                        id = regs.ecx;
                        value = regs.ebx;
                    }

                    auto old = FusionFixSettings(id);

                    FusionFixSettings.ForEachPref([&](int32_t prefID, int32_t idStart, int32_t idEnd) {
                        if (prefID == id) {
                            if (value <= idStart) {
                                if (old > idStart)
                                    value = idStart;
                                else
                                    value = idEnd;
                            }
                        }
                    });

                    FusionFixSettings.Set(id, value);
                }
            }; injector::MakeInline<IniWriter>(pattern.get_first(0), pattern.get_first(7));

            pattern = find_pattern("8B 1C 95 ? ? ? ? 89 54 24 14", "8B 1C 8D ? ? ? ? 89 4C 24 18");
            static auto reg2 = *pattern.get_first<uint8_t>(2);
            struct MenuTogglesHook1
            {
                void operator()(injector::reg_pack& regs)
                {
                    if (reg2 == 0x8D) {
                        regs.ebx = FusionFixSettings.Get(regs.ecx);
                        return;
                    }
                    regs.ebx = FusionFixSettings.Get(regs.edx);
                }
            }; injector::MakeInline<MenuTogglesHook1>(pattern.get_first(0), pattern.get_first(7));

            pattern = find_pattern("8B 14 85 ? ? ? ? 66 83 F9 31", "8B 0C 8D ? ? ? ? 75 12");
            static auto reg3 = *pattern.get_first<uint8_t>(2);
            struct MenuTogglesHook2
            {
                void operator()(injector::reg_pack& regs)
                {
                    if (reg3 == 0x8D) {
                        regs.ecx = FusionFixSettings.Get(regs.ecx);
                        return;
                    }
                    regs.edx = FusionFixSettings.Get(regs.eax);
                }
            }; injector::MakeInline<MenuTogglesHook2>(pattern.get_first(0), pattern.get_first(7));

            // show game in display menu
            pattern = find_pattern("75 1F FF 35 ? ? ? ? E8 ? ? ? ? 8B 4C 24 18", "75 10 57 E8 ? ? ? ? 83 C4 04 83 F8 03");
            injector::MakeNOP(pattern.get_first(), 2);

            pattern = hook::pattern("83 F8 03 0F 44 CE");
            if (!pattern.empty())
            {
                struct MenuHook
                {
                    void operator()(injector::reg_pack& regs)
                    {
                        regs.ecx = 1;
                    }
                }; injector::MakeInline<MenuHook>(pattern.get_first(0), pattern.get_first(6));
            }
            else
            {
                pattern = hook::pattern("75 02 B3 01 57 E8");
                if (!pattern.empty())
                    injector::MakeNOP(pattern.get_first(), 2);
            }

            pattern = hook::pattern("7E 4E 8A 1D ? ? ? ? 8B 35");
            if (!pattern.empty())
                injector::WriteMemory<uint8_t>(pattern.get_first(0), 0xEB, true);
            else
            {
                pattern = hook::pattern("83 F8 03 7F 06");
                if (!pattern.empty())
                    injector::MakeNOP(pattern.get_first(3), 2);
            }

            //menu scrolling
            pattern = find_pattern("83 F8 10 7E 37 6A 00 E8 ? ? ? ? 83 C4 04 8D 70 F8 E8 ? ? ? ? D9 5C 24 30", "83 F8 10 7E 2A 6A 00 E8 ? ? ? ? 83 E8 08 89 44 24 14");
            injector::WriteMemory<uint8_t>(pattern.get_first(2), 0x10 * 2, true);
            pattern = hook::pattern("8D 70 F8 E8 ? ? ? ? D9 5C 24 30");
            if (!pattern.empty())
                injector::WriteMemory<uint8_t>(pattern.get_first(2), 0xF0, true);
            else
            {
                pattern = hook::pattern("83 E8 08 89 44 24 14");
                if (!pattern.empty())
                    injector::WriteMemory<uint8_t>(pattern.get_first(2), 0x10, true);
            }
            pattern = find_pattern("83 FE 10 7F 08", "83 FF 10 7F 0C");
            injector::WriteMemory<uint8_t>(pattern.get_first(2), 0x10 * 2, true);
            pattern = find_pattern("83 F8 10 7E 37 6A 00 E8 ? ? ? ? 83 C4 04 8D 70 F8", "83 F8 ? 7E ? 6A 00 E8 ? ? ? ? 83 E8 08 89 44 24 24");
            injector::WriteMemory<uint8_t>(pattern.get_first(2), 0x10 * 2, true);
            pattern = hook::pattern("8D 70 F8 E8 ? ? ? ? D9 5C 24 38");
            if (!pattern.empty())
                injector::WriteMemory<uint8_t>(pattern.get_first(2), 0xF0, true);
            else
            {
                pattern = hook::pattern("83 E8 08 89 44 24 24");
                if (!pattern.empty())
                    injector::WriteMemory<uint8_t>(pattern.get_first(2), 0x10, true);
            }
            pattern = find_pattern("8D 46 F0 66 0F 6E C0", "83 C7 F0 89 7C");
            injector::WriteMemory<uint8_t>(pattern.get_first(2), 0xE0, true);
        };

        // FPS Counter
        auto pattern = find_pattern("A1 ? ? ? ? 83 F8 08 74 17", "A1 ? ? ? ? 83 F8 08 74 0C");
        if (!pattern.empty())
        {
            static auto& menuTab = **pattern.get_first<int32_t*>(1);
            static ID3DXFont* pFPSFont = nullptr;

            FusionFix::D3D9::onBeforeCreateDevice() += [](LPDIRECT3D9& pDirect3D9, UINT& Adapter, D3DDEVTYPE& DeviceType, HWND& hFocusWindow, DWORD& BehaviorFlags, D3DPRESENT_PARAMETERS*& pPresentationParameters, IDirect3DDevice9**& ppReturnedDeviceInterface)
            {
                if (pFPSFont)
                    pFPSFont->Release();
                pFPSFont = nullptr;
            };

            FusionFix::onBeforeReset() += []()
            {
                if (pFPSFont)
                    pFPSFont->Release();
                pFPSFont = nullptr;
            };

            FusionFix::D3D9::onEndScene() += [](LPDIRECT3DDEVICE9 pDevice)
            {
                if (!bMainEndScene)
                    return;
                else
                    bMainEndScene = false;

                static auto fpsc = FusionFixSettings.GetRef("PREF_FPSCOUNTER");
                if (menuTab == 8 || menuTab == 49 || fpsc->get())
                {
                    static std::list<int> m_times;

                    LARGE_INTEGER frequency;
                    LARGE_INTEGER time;
                    QueryPerformanceFrequency(&frequency);
                    QueryPerformanceCounter(&time);

                    if (m_times.size() == 50)
                        m_times.pop_front();
                    m_times.push_back(static_cast<int>(time.QuadPart));

                    uint32_t fps = 0;
                    if (m_times.size() >= 2)
                        fps = static_cast<uint32_t>(0.5f + (static_cast<double>(m_times.size() - 1) * static_cast<double>(frequency.QuadPart)) / static_cast<double>(m_times.back() - m_times.front()));

                    if (!pFPSFont)
                    {
                        D3DDEVICE_CREATION_PARAMETERS cparams;
                        RECT rect;
                        pDevice->GetCreationParameters(&cparams);
                        GetClientRect(cparams.hFocusWindow, &rect);

                        D3DXFONT_DESC fps_font;
                        ZeroMemory(&fps_font, sizeof(D3DXFONT_DESC));
                        fps_font.Height = rect.bottom / 20;
                        fps_font.Width = 0;
                        fps_font.Weight = 400;
                        fps_font.MipLevels = 0;
                        fps_font.Italic = 0;
                        fps_font.CharSet = DEFAULT_CHARSET;
                        fps_font.OutputPrecision = OUT_DEFAULT_PRECIS;
                        fps_font.Quality = ANTIALIASED_QUALITY;
                        fps_font.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
                        wchar_t FaceName[] = L"Arial";
                        memcpy(&fps_font.FaceName, &FaceName, sizeof(FaceName));

                        if (D3DXCreateFontIndirect(pDevice, &fps_font, &pFPSFont) != D3D_OK)
                            return;
                    }
                    else
                    {
                        auto DrawTextOutline = [](ID3DXFont* pFont, FLOAT X, FLOAT Y, D3DXCOLOR dColor, CONST PCHAR cString, ...)
                        {
                            const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
                            CHAR cBuffer[101] = "";

                            va_list oArgs;
                            va_start(oArgs, cString);
                            _vsnprintf((cBuffer + strlen(cBuffer)), (sizeof(cBuffer) - strlen(cBuffer)), cString, oArgs);
                            va_end(oArgs);

                            RECT Rect[5] =
                            {
                                { X - 1, Y, X + 500.0f, Y + 50.0f },
                                { X, Y - 1, X + 500.0f, Y + 50.0f },
                                { X + 1, Y, X + 500.0f, Y + 50.0f },
                                { X, Y + 1, X + 500.0f, Y + 50.0f },
                                { X, Y, X + 500.0f, Y + 50.0f },
                            };

                            if (dColor != BLACK)
                            {
                                for (auto i = 0; i < 4; i++)
                                    pFont->DrawTextA(NULL, cBuffer, -1, &Rect[i], DT_NOCLIP, BLACK);
                            }

                            pFont->DrawTextA(NULL, cBuffer, -1, &Rect[4], DT_NOCLIP, dColor);
                        };

                        static char str_format_fps[] = "%02d";
                        static const D3DXCOLOR TBOGT(D3DCOLOR_XRGB(0xD7, 0x11, 0x6E));
                        static const D3DXCOLOR TLAD(D3DCOLOR_XRGB(0x6F, 0x0D, 0x0F));
                        static const D3DXCOLOR IV(D3DCOLOR_XRGB(0xF0, 0xA0, 0x00));
                        DrawTextOutline(pFPSFont, 10, 10, (*_dwCurrentEpisode == 2) ? TBOGT : ((*_dwCurrentEpisode == 1) ? TLAD : IV), str_format_fps, fps);
                    }
                }
            };

            FusionFix::onShutdownEvent() += []()
            {
                if (pFPSFont)
                    pFPSFont->Release();
                pFPSFont = nullptr;
            };
        }
    }
} Settings;