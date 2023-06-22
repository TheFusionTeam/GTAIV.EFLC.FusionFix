#include <misc.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") // needed for timeBeginPeriod()/timeEndPeriod()
#include <filesystem>
#include <map>
#include <d3d9.h>

enum MenuItems
{
    PREF_NULL,
    PREF_WAYPOINT_ID,
    PREF_AUTOSAVE,
    PREF_AUTOSAVE_SLOT_UNUSED,
    PREF_VIBRATION,
    PREF_CONTROL_METHOD,
    PREF_INVERT_MOUSE,
    PREF_INVERT_LOOK,
    PREF_AUTO_AIM,
    PREF_CONTROLLER_SENSITIVITY,
    PREF_SNIPER_CONTROL,
    PREF_CONTROL_CONFIG,
    PREF_BRIGHTNESS,
    PREF_CONTRAST,
    PREF_SATURATION,
    PREF_LOD,
    PREF_SUBTITLES,
    PREF_DISPLAY_HUD,
    PREF_RETICULE,
    PREF_DISPLAY_GPS,
    PREF_HANDBRAKE_CAM,
    PREF_RADAR_MODE,
    PREF_MAP_LEGEND,
    PREF_MUSIC_VOLUME,
    PREF_MUSIC_VOLUME_IN_MP,
    PREF_SFX_VOLUME,
    PREF_FRAME_LIMITER,
    PREF_AUDIO_OUTPUT,
    PREF_BASS,
    PREF_GPS_SPEECH,
    PREF_KEYBOARD_LANGUAGE,
    PREF_SYSTEM_LANGUAGE,
    PREF_CURRENT_LANGUAGE,
    PREF_PREVIOUS_LANGUAGE,
    PREF_REDEFINE_SCREEN,
    PREF_NETWORK_GAME_NAME,
    PREF_NETWORK_GAME_TYPE,
    PREF_NETWORK_GAME_PARAM_1,
    PREF_NETWORK_GAME_PARAM_2,
    PREF_NETWORK_GAME_PARAM_3,
    PREF_NETWORK_GAME_PARAM_4,
    PREF_NETWORK_GAME_PARAM_5,
    PREF_NETWORK_GAME_PARAM_6,
    PREF_GAMEMODE,
    PREF_GAMETYPE,
    PREF_SCORES,
    PREF_RACETYPE,
    PREF_RACENAME,
    PREF_GAMECLASS,
    PREF_GENRE,
    PREF_HEAD_FEMALE,
    PREF_TORSO_FEMALE,
    PREF_LEGS_FEMALE,
    PREF_GLASSES_FEMALE,
    PREF_HATS_FEMALE,
    PREF_UNK1,
    PREF_UNK2,
    PREF_HEAD_MALE,
    PREF_TORSO_MALE,
    PREF_LEGS_MALE,
    PREF_GLASSES_MALE,
    PREF_HATS_MALE,
    PREF_HEAD_MALE_UNDRESSED,
    PREF_HAIR_MALE,
    PREF_UNK3,
    PREF_UNK4,
    PREF_UNK5,
    PREF_UNK6,
    PREF_HAIR_FEMALE,
    PREF_SUSE_FEMALE,
    PREF_SUSE_MALE,
    PREF_HAND_FEMALE,
    PREF_HAND_MALE,
    PREF_RADIO_STATION,
    PREF_HDR,
    PREF_SPEAKER_OUTPUT,
    PREF_FLICKER_FILTER,
    PREF_DISPLAY_BLIPS,
    PREF_VOICE_OUTPUT,
    PREF_GAMETYPE_COMP,
    PREF_SCORES_COMP,
    PREF_GAMETYPE_TEAM,
    PREF_SCORES_TEAM,
    PREF_GAMETYPE_COOP,
    PREF_SCORES_COOP,
    PREF_GAMETYPE_RACE,
    PREF_GAMETYPE_GAMER_RANK,
    PREF_GAMETYPE_TRUESKILL,
    PREF_GAMEMODE_TRUESKILL,
    PREF_EPISODIC_GAMEMODE_0,
    PREF_EPISODIC_GAMEMODE_1,
    PREF_EPISODIC_GAMEMODE_2,
    PREF_EPISODIC_GAMEMODE_3,
    PREF_EPISODIC_GAMEMODE_4,
    PREF_EPISODIC_GAMEMODE_5,
    PREF_EPISODIC_GAMEMODE_6,
    PREF_EPISODIC_GAMEMODE_7,
    PREF_EPISODIC_GAMEMODE_8,
    PREF_EPISODIC_GAMEMODE_9,
    PREF_EPISODIC_SCORES_0,
    PREF_EPISODIC_SCORES_1,
    PREF_EPISODIC_SCORES_2,
    PREF_EPISODIC_SCORES_3,
    PREF_EPISODIC_SCORES_4,
    PREF_EPISODIC_SCORES_5,
    PREF_EPISODIC_SCORES_6,
    PREF_EPISODIC_SCORES_7,
    PREF_EPISODIC_SCORES_8,
    PREF_EPISODIC_SCORES_9,
    PREF_EPISODIC_GAMETYPE_0,
    PREF_EPISODIC_GAMETYPE_1,
    PREF_EPISODIC_GAMETYPE_2,
    PREF_EPISODIC_GAMETYPE_3,
    PREF_EPISODIC_GAMETYPE_4,
    PREF_EPISODIC_GAMETYPE_5,
    PREF_EPISODIC_GAMETYPE_6,
    PREF_EPISODIC_GAMETYPE_7,
    PREF_EPISODIC_GAMETYPE_8,
    PREF_EPISODIC_GAMETYPE_9,
    PREF_EPISODIC_GAMETYPE_RACE_0,
    PREF_EPISODIC_GAMETYPE_RACE_1,
    PREF_EPISODIC_GAMETYPE_RACE_2,
    PREF_EPISODIC_GAMETYPE_RACE_3,
    PREF_EPISODIC_GAMETYPE_RACE_4,
    PREF_EPISODIC_GAMETYPE_RACE_5,
    PREF_EPISODIC_RACENAME_RACE_0,
    PREF_EPISODIC_RACENAME_RACE_1,
    PREF_EPISODIC_RACENAME_RACE_2,
    PREF_EPISODIC_RACENAME_RACE_3,
    PREF_EPISODIC_RACENAME_RACE_4,
    PREF_EPISODIC_RACENAME_RACE_5,
    PREF_EPISODIC_RACECLASS_RACE_0,
    PREF_EPISODIC_RACECLASS_RACE_1,
    PREF_EPISODIC_RACECLASS_RACE_2,
    PREF_EPISODIC_RACECLASS_RACE_3,
    PREF_EPISODIC_RACECLASS_RACE_4,
    PREF_EPISODIC_RACECLASS_RACE_5,
    PREF_EPISODIC_NOISE_FILTER,
};

enum eSettings
{
    bSkipIntro = PREF_EPISODIC_GAMEMODE_0,
    bSkipMenu = PREF_EPISODIC_GAMEMODE_1,
    bBorderlessWindowed = PREF_EPISODIC_GAMEMODE_2,
    nFpsLimitPreset = PREF_EPISODIC_GAMEMODE_3,
    bFXAA = PREF_EPISODIC_GAMEMODE_4,
    bConsoleGamma = PREF_EPISODIC_GAMEMODE_5,
};

class CSettings
{
private:
    struct CSetting
    {
        eSettings assocEnum;
        std::string_view iniSec;
        std::string_view iniName;
        int32_t iniDefValInt;

        int32_t operator()() { return mPrefs[assocEnum]; }
        void ReadFromIni(auto& iniReader) { mPrefs[assocEnum] = iniReader.ReadInteger(iniSec, iniName, iniDefValInt); }
        void ReadFromIni() { CIniReader iniReader(""); ReadFromIni(iniReader); }
        void WriteToIni(auto& iniWriter, auto value) { iniWriter.WriteInteger(iniSec, iniName, value); }
        void WriteToIni(auto value) { CIniReader iniWriter(""); iniWriter.WriteInteger(iniSec, iniName, value); }
    };
public:
    static inline int32_t* mPrefs;
    static inline std::map<eSettings, CSetting> mSettings;

public:
    CSettings()
    {
        auto pattern = hook::pattern("89 1C 95 ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 83 C4 04");
        mPrefs = *pattern.get_first<int32_t*>(3);

        CIniReader iniReader("");

        CSetting arr[] = {
            { bSkipIntro,                     "MAIN",       "SkipIntro",                     1 },
            { bSkipMenu,                      "MAIN",       "SkipMenu",                      1 },
            { bBorderlessWindowed,            "MAIN",       "BorderlessWindowed",            1 },
            { nFpsLimitPreset,                "FRAMELIMIT", "FpsLimitPreset",                1 },
            { bFXAA,                          "MISC",       "FXAA",                          1 },
            { bConsoleGamma,                  "MISC",       "ConsoleGamma",                  0 },
        };

        for (auto& it : arr)
        {
            it.ReadFromIni(iniReader);
            mSettings.emplace(it.assocEnum, it);
        }
    }
public:
    auto operator()(eSettings i) { return mSettings[i](); }
    auto Get(eSettings i) { return mSettings[i]; }
    bool Exists(eSettings key) { return mSettings.count(key) > 0; }
} FusionFixSettings;

bool(*CCutscenes__hasCutsceneFinished)();
bool(*CCamera__isWidescreenBordersActive)();

int32_t nFrameLimitType;
float fFpsLimit;
float fCutsceneFpsLimit;
float fScriptCutsceneFpsLimit;
float fScriptCutsceneFovLimit;
std::vector<int32_t> fpsCaps = { 0, 1, 30, 40, 50, 60, 75, 100, 120, 144, 165, 240 };

class FrameLimiter
{
public:
    enum FPSLimitMode { FPS_NONE, FPS_REALTIME, FPS_ACCURATE };
    FPSLimitMode mFPSLimitMode = FPS_NONE;
private:
    double TIME_Frequency = 0.0;
    double TIME_Ticks = 0.0;
    double TIME_Frametime = 0.0;
    float  fFPSLimit = 0.0f;
public:
    void Init(FPSLimitMode mode, float fps_limit)
    {
        mFPSLimitMode = mode;
        fFPSLimit = fps_limit;

        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        static constexpr auto TICKS_PER_FRAME = 1;
        auto TICKS_PER_SECOND = (TICKS_PER_FRAME * fFPSLimit);
        if (mFPSLimitMode == FPS_ACCURATE)
        {
            TIME_Frametime = 1000.0 / (double)fFPSLimit;
            TIME_Frequency = (double)frequency.QuadPart / 1000.0; // ticks are milliseconds
        }
        else // FPS_REALTIME
        {
            TIME_Frequency = (double)frequency.QuadPart / (double)TICKS_PER_SECOND; // ticks are 1/n frames (n = fFPSLimit)
        }
        Ticks();
    }
    DWORD Sync_RT()
    {
        DWORD lastTicks, currentTicks;
        LARGE_INTEGER counter;

        QueryPerformanceCounter(&counter);
        lastTicks = (DWORD)TIME_Ticks;
        TIME_Ticks = (double)counter.QuadPart / TIME_Frequency;
        currentTicks = (DWORD)TIME_Ticks;

        return (currentTicks > lastTicks) ? currentTicks - lastTicks : 0;
    }
    DWORD Sync_SLP()
    {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        double millis_current = (double)counter.QuadPart / TIME_Frequency;
        double millis_delta = millis_current - TIME_Ticks;
        if (TIME_Frametime <= millis_delta)
        {
            TIME_Ticks = millis_current;
            return 1;
        }
        else if (TIME_Frametime - millis_delta > 2.0) // > 2ms
            Sleep(1); // Sleep for ~1ms
        else
            Sleep(0); // yield thread's time-slice (does not actually sleep)

        return 0;
    }
    void Sync()
    {
        if (mFPSLimitMode == FPS_REALTIME)
            while (!Sync_RT());
        else if (mFPSLimitMode == FPS_ACCURATE)
            while (!Sync_SLP());
    }
private:
    void Ticks()
    {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        TIME_Ticks = (double)counter.QuadPart / TIME_Frequency;
    }
};

FrameLimiter FpsLimiter;
FrameLimiter CutsceneFpsLimiter;
FrameLimiter ScriptCutsceneFpsLimiter;
void __cdecl sub_855640()
{
    if (FusionFixSettings(nFpsLimitPreset) >= 1) {
        if (fFpsLimit > 0.0f || FusionFixSettings(nFpsLimitPreset) > 1)
            FpsLimiter.Sync();
    }

    if (CCamera__isWidescreenBordersActive())
    {
        if (CCutscenes__hasCutsceneFinished())
            if (fCutsceneFpsLimit)
                CutsceneFpsLimiter.Sync();
            else if (fScriptCutsceneFpsLimit)
                ScriptCutsceneFpsLimiter.Sync();
    }
}

injector::hook_back<void(__cdecl*)(int)> hbsub_7870A0;
void __cdecl sub_7870A0(int a1)
{
    static bool bOnce = false;
    if (!bOnce)
    {
        if (a1 == 0)
        {
            bool bNoLoad = (GetAsyncKeyState(VK_SHIFT) & 0xF000) != 0;
            if (!bNoLoad)
                a1 = 6;

            bOnce = true;
        }
    }
    return hbsub_7870A0.fun(a1);
}

BOOL WINAPI MoveWindow_Hook(HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint)
{
    HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info = {};
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    int32_t DesktopResW = info.rcMonitor.right - info.rcMonitor.left;
    int32_t DesktopResH = info.rcMonitor.bottom - info.rcMonitor.top;

    RECT rect = { X, Y, nWidth, nHeight };
    rect.left = (LONG)(((float)DesktopResW / 2.0f) - ((float)rect.right / 2.0f));
    rect.top = (LONG)(((float)DesktopResH / 2.0f) - ((float)rect.bottom / 2.0f));
    return MoveWindow(hWnd, rect.left, rect.top, rect.right, rect.bottom, bRepaint);
}

HWND WINAPI CreateWindowExA_Hook(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
    auto hwnd = CreateWindowExA(dwExStyle, lpClassName, lpWindowName, 0, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
    lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    SetWindowLong(hwnd, GWL_STYLE, lStyle);
    MoveWindow_Hook(hwnd, 0, 0, nWidth - X, nHeight - Y, TRUE);
    return hwnd;
}

BOOL WINAPI AdjustWindowRect_Hook(LPRECT lpRect, DWORD dwStyle, BOOL bMenu)
{
    dwStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    return AdjustWindowRect(lpRect, dwStyle, bMenu);
}

uint32_t dword_F43AD8;
uint32_t dword_1826D48;
uint32_t dword_1826D34;
uint32_t dword_1826D4C;
uint32_t dword_1826D6C;
double __cdecl MouseFix(void*, int32_t axis)
{
    auto address = *(int32_t*)(dword_F43AD8 + 0xC);
    if (*(BYTE*)(address + 0x1C4) & 8)
    {
        *(int32_t*)(dword_1826D48) = 0;
        *(int32_t*)(dword_1826D34) = 0;
        *(int32_t*)(dword_1826D4C) = 0;
        *(int32_t*)(dword_1826D6C) = 0;
    }
    if (axis == 0)
    {
        return (double)(*(int32_t*)(dword_1826D48)) * 0.0078125;
    }
    else if (axis == 1)
    {
        return (double)(*(int32_t*)(dword_1826D34)) * 0.0078125;
    }

    return 0.0;
}

template<class T>
T GetModulePath(HMODULE hModule)
{
    static constexpr auto INITIAL_BUFFER_SIZE = MAX_PATH;
    static constexpr auto MAX_ITERATIONS = 7;
    T ret;
    auto bufferSize = INITIAL_BUFFER_SIZE;
    for (size_t iterations = 0; iterations < MAX_ITERATIONS; ++iterations)
    {
        ret.resize(bufferSize);
        size_t charsReturned = 0;
        if constexpr (std::is_same_v<T, std::string>)
            charsReturned = GetModuleFileNameA(hModule, &ret[0], bufferSize);
        else
            charsReturned = GetModuleFileNameW(hModule, &ret[0], bufferSize);
        if (charsReturned < ret.length())
        {
            ret.resize(charsReturned);
            return ret;
        }
        else
        {
            bufferSize *= 2;
        }
    }
    return T();
}

template<class T>
T GetExeModulePath()
{
    T r = GetModulePath<T>(NULL);
    if constexpr (std::is_same_v<T, std::string>)
        r = r.substr(0, r.find_last_of("/\\") + 1);
    else
        r = r.substr(0, r.find_last_of(L"/\\") + 1);
    return r;
}

template <typename T, typename V>
bool iequals(const T& s1, const V& s2)
{
    T str1(s1); T str2(s2);
    std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
    std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
    return (str1 == str2);
}

void ReadIni()
{
    CIniReader iniReader("");
    for (auto& it : FusionFixSettings.mSettings)
    {
        it.second.ReadFromIni(iniReader);
    }
}

void (*sub_8C00D0)();
void sub_8C00D0_hook()
{
    sub_8C00D0();
    ReadIni();
}

int (*sub_8B7830)();
int sub_8B7830_hook()
{
    auto res = sub_8B7830();
    ReadIni();
    return res;
}

void(__cdecl* sub_59E1C0)();
void __cdecl sub_59E1C0_hook()
{
    sub_59E1C0();
    ReadIni();
}

void (*sub_5A8FE0)();
void sub_5A8FE0_hook()
{
    sub_5A8FE0();
    ReadIni();
}

void Init()
{
    CIniReader iniReader("");
    //[MAIN]
    bool bSkipIntro = iniReader.ReadInteger("MAIN", "SkipIntro", 1) != 0;
    bool bSkipMenu = iniReader.ReadInteger("MAIN", "SkipMenu", 1) != 0;
    bool bBorderlessWindowed = iniReader.ReadInteger("MAIN", "BorderlessWindowed", 1) != 0;
    bool bRecoilFix = iniReader.ReadInteger("MAIN", "RecoilFix", 1) != 0;
    bool bDefinitionFix = iniReader.ReadInteger("MAIN", "DefinitionFix", 1) != 0;
    bool bEmissiveShaderFix = iniReader.ReadInteger("MAIN", "EmissiveShaderFix", 1) != 0;
    bool bHandbrakeCamFix = iniReader.ReadInteger("MAIN", "HandbrakeCamFix", 0) != 0;
    int32_t nAimingZoomFix = iniReader.ReadInteger("MAIN", "AimingZoomFix", 1);
    bool bFlickeringShadowsFix = iniReader.ReadInteger("MAIN", "FlickeringShadowsFix", 1) != 0;

    //[FRAMELIMIT]
    nFrameLimitType = iniReader.ReadInteger("FRAMELIMIT", "FrameLimitType", 2);
    fFpsLimit = static_cast<float>(iniReader.ReadInteger("FRAMELIMIT", "FpsLimit", 0));
    fCutsceneFpsLimit = static_cast<float>(iniReader.ReadInteger("FRAMELIMIT", "CutsceneFpsLimit", 0));
    fScriptCutsceneFpsLimit = static_cast<float>(iniReader.ReadInteger("FRAMELIMIT", "ScriptCutsceneFpsLimit", 0));
    fScriptCutsceneFovLimit = static_cast<float>(iniReader.ReadInteger("FRAMELIMIT", "ScriptCutsceneFovLimit", 0));

    //[MISC]
    bool bDefaultCameraAngleInTLAD = iniReader.ReadInteger("MISC", "DefaultCameraAngleInTLAD", 0) != 0;
    bool bPedDeathAnimFixFromTBOGT = iniReader.ReadInteger("MISC", "PedDeathAnimFixFromTBOGT", 1) != 0;
    bool bDisableCameraCenteringInCover = iniReader.ReadInteger("MISC", "DisableCameraCenteringInCover", 1) != 0;
    bool bMouseFix = iniReader.ReadInteger("MISC", "MouseFix", 1) != 0;

    //[BudgetedIV]
    uint32_t nVehicleBudget = iniReader.ReadInteger("BudgetedIV", "VehicleBudget", 0);
    uint32_t nPedBudget = iniReader.ReadInteger("BudgetedIV", "PedBudget", 0);

    static float& fTimeStep = **hook::get_pattern<float*>("F3 0F 10 05 ? ? ? ? F3 0F 59 05 ? ? ? ? 8B 43 20 53", 4);

    // reverse lights fix
    {
        auto pattern = hook::pattern("8B 40 64 FF D0 F3 0F 10 40 ? 8D 44 24 40 50");
        injector::WriteMemory<uint8_t>(pattern.get_first(2), 0x60, true);
    }

    // animation fix for phone interaction on bikes
    {
        auto pattern = hook::pattern("83 3D ? ? ? ? 01 0F 8C 18 01 00 00");
        injector::MakeNOP(pattern.get(0).get<int>(0), 13, true);
    }

    //fix for lods appearing inside normal models, unless the graphics menu was opened once (draw distances aren't set properly?)
    {
        auto pattern = hook::pattern("E8 ? ? ? ? 8D 4C 24 10 F3 0F 11 05 ? ? ? ? E8 ? ? ? ? 8B F0 E8 ? ? ? ? DF 2D");
        auto sub_477300 = injector::GetBranchDestination(pattern.get_first(0));
        pattern = hook::pattern("E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 83 C4 2C C3");
        injector::MakeCALL(pattern.get_first(0), sub_477300, true);
    }

    if (bSkipIntro)
    {
        auto pattern = hook::pattern("8B 0D ? ? ? ? 8B 44 24 18 83 C4 0C 69 C9 ? ? ? ? 89 81");
        static auto dword_15A6F0C = *pattern.count(2).get(1).get<int32_t*>(2);
        struct Loadsc
        {
            void operator()(injector::reg_pack& regs)
            {
                *(int32_t*)&regs.ecx = *dword_15A6F0C;
                *(int32_t*)&regs.eax = *(int32_t*)(regs.esp + 0x18);
                static int once = 0;
                if (once < 8 && *(int32_t*)&regs.eax < 8000)
                {
                    regs.eax = 0;
                    once++;
                }
            }
        }; injector::MakeInline<Loadsc>(pattern.count(2).get(1).get<void*>(0), pattern.count(2).get(1).get<void*>(10));
    }

    if (bSkipMenu)
    {
        auto pattern = hook::pattern("E8 ? ? ? ? 83 C4 04 8B 8C 24 ? ? ? ? 5F 5E 5D 5B 33 CC E8 ? ? ? ? 81 C4 ? ? ? ? C3");
        hbsub_7870A0.fun = injector::MakeCALL(pattern.count(5).get(1).get<void*>(0), sub_7870A0).get();
    }

    if (bBorderlessWindowed)
    {
        //grcWindow__m_dwWidth = *hook::get_pattern<uint32_t*>("8B 0D ? ? ? ? F3 0F 10 8C B7 ? ? ? ? F3 0F 59 C2 84 C0 0F 45 0D ? ? ? ? F3 0F 5C C8 66 0F 6E C1 0F 5B C0 F3 0F 59 C8 0F 57 C0 0F 2F C1", 2);
        //grcWindow__m_dwHeight = *hook::get_pattern<uint32_t*>("8B 0D ? ? ? ? F3 0F 10 94 B7 ? ? ? ? F3 0F 59 C1 84 C0 0F 45 0D ? ? ? ? F3 0F 5C D0 66 0F 6E C1 0F 5B C0 F3 0F 59 D0 0F 57 C0 0F 2F C2", 2);
        auto pattern = hook::pattern("FF 15 ? ? ? ? 8B F0 6A 01");
        injector::MakeNOP(pattern.get_first(0), 6, true);
        injector::MakeCALL(pattern.get_first(0), CreateWindowExA_Hook, true);
        pattern = hook::pattern("FF 15 ? ? ? ? 8B 44 24 34");
        injector::MakeNOP(pattern.get_first(0), 6, true);
        injector::MakeCALL(pattern.get_first(0), MoveWindow_Hook, true);
        pattern = hook::pattern("FF 15 ? ? ? ? 8B 54 24 24 8B 74 24 20");
        injector::MakeNOP(pattern.get_first(0), 6, true);
        injector::MakeCALL(pattern.get_first(0), AdjustWindowRect_Hook, true);
        pattern = hook::pattern("FF 15 ? ? ? ? 8B 44 24 10 85 C0");
        injector::MakeNOP(pattern.get_first(0), 6, true);
        injector::MakeCALL(pattern.get_first(0), AdjustWindowRect_Hook, true);
        pattern = hook::pattern("FF 15 ? ? ? ? 8B 74 24 1C 8B 44 24 24");
        injector::MakeNOP(pattern.get_first(0), 6, true);
        injector::MakeCALL(pattern.get_first(0), AdjustWindowRect_Hook, true);
    }

    //fix for zoom flag in tbogt
    if (nAimingZoomFix)
    {
        auto pattern = hook::pattern("8A C1 32 05 ? ? ? ? 24 01 32 C1");
        static auto& byte_F47AB1 = **pattern.get_first<uint8_t*>(4);
        if (nAimingZoomFix > 1)
            injector::MakeNOP(pattern.get_first(-2), 2, true);
        else if (nAimingZoomFix < 0)
            injector::WriteMemory<uint8_t>(pattern.get_first(-2), 0xEB, true);

        pattern = hook::pattern("80 8E ? ? ? ? ? EB 43");
        struct AimZoomHook1
        {
            void operator()(injector::reg_pack& regs)
            {
                *(uint8_t*)(regs.esi + 0x200) |= 1;
                byte_F47AB1 = 1;
            }
        }; injector::MakeInline<AimZoomHook1>(pattern.get_first(0), pattern.get_first(7));

        pattern = hook::pattern("76 09 80 A6 ? ? ? ? ? EB 26");
        struct AimZoomHook2
        {
            void operator()(injector::reg_pack& regs)
            {
                *(uint8_t*)(regs.esi + 0x200) &= 0xFE;
                byte_F47AB1 = 0;
            }
        }; injector::MakeInline<AimZoomHook2>(pattern.get_first(2), pattern.get_first(9));

        //let's default to 0 as well
        pattern = hook::pattern("C6 05 ? ? ? ? ? 74 12 83 3D");
        injector::WriteMemory<uint8_t>(pattern.get_first(6), 0, true);
    }

    if (bFlickeringShadowsFix)
    {
        auto pattern = hook::pattern("C3 68 ? ? ? ? 6A 02 6A 00 E8 ? ? ? ? 83 C4 40 8B E5 5D C3");
        injector::WriteMemory(pattern.count(2).get(1).get<void*>(2), 0x100, true);
    }

    if (bRecoilFix)
    {
        static float fRecMult = 0.65f;
        auto pattern = hook::pattern("F3 0F 10 44 24 ? F3 0F 59 05 ? ? ? ? EB 1E");
        injector::WriteMemory(pattern.get_first(10), &fRecMult, true);
    }

    if (bDefinitionFix)
    {
        //disable forced definition-off in cutscenes
        auto pattern = hook::pattern("E8 ? ? ? ? 0F B6 0D ? ? ? ? 33 D2 84 C0 0F 45 CA 8A C1 C3");
        injector::MakeNOP(pattern.count(2).get(1).get<void*>(12), 7, true);
    }

    if (bEmissiveShaderFix)
    {
        // workaround for gta_emissivestrong.fxc lights on patch 6+,
        //"A0 01 00 00 02 00 00 08" replaced in shader files with "A1 01 00 00 02 00 00 08" (5 occurrences)
        auto pattern = hook::pattern("C1 E7 04 C1 E0 04 8B F7 8D 80 ? ? ? ? 89 4C 24 10 89 44 24 0C");
        struct ShaderTest
        {
            void operator()(injector::reg_pack& regs)
            {
                if (
                    *(uint32_t*)(regs.ebx + 0x00) == 0x39D1B717 &&
                    *(uint32_t*)(regs.ebx + 0x10) == 0x41000000 && //traffic lights and lamps
                    *(uint32_t*)(regs.ebx - 0x10) == 0x3B03126F &&
                    *(uint32_t*)(regs.ebx + 0x20) == 0x3E59999A &&
                    *(uint32_t*)(regs.ebx + 0x24) == 0x3F372474 &&
                    *(uint32_t*)(regs.ebx + 0x28) == 0x3D93A92A
                )
                {
                    auto f_ptr = (uint32_t*)(regs.ebx - 0x158);
                    if (f_ptr)
                    {
                        //auto f_00 = f_ptr[3];
                        //auto f_01 = f_ptr[7];
                        auto f_02 = f_ptr[18];
                        auto f_04 = f_ptr[10];
                        auto f_05 = f_ptr[13];
                        //auto f_06 = f_ptr[12];
                        //auto f_07 = f_ptr[17];
                        auto f_08 = f_ptr[19];
                        //auto f_09 = f_ptr[22];
                        //auto f_10 = f_ptr[23];
                        //auto f_11 = f_ptr[24];

                        if ((f_05 != 0x7f800001 && f_05 != 0xcdcdcdcd && f_04 == 0x00000000 && f_08 != 0xba8bfc22 && f_02 != 0x3f800000)
                                || (f_02 == 0x3f800000 && f_04 == 0x42480000 && f_05 == 0x00000000 && f_08 == 0x00000000))
                        {
                            *(float*)(regs.ebx + 0x00) = -*(float*)(regs.ebx + 0x00);
                        }
                    }
                }

                regs.edi = regs.edi << 4;
                regs.eax = regs.eax << 4;
            }
        }; injector::MakeInline<ShaderTest>(pattern.count(2).get(1).get<void>(0), pattern.count(2).get(1).get<void>(6));
    }

    if (bHandbrakeCamFix)
    {
        static auto unk_117E700 = hook::get_pattern("68 ? ? ? ? B9 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? F6 D8 1A C0", 1);
        static auto GET_POSITION_OF_ANALOGUE_STICKS = hook::get_pattern("6A 00 E8 ? ? ? ? 83 C4 04 80 B8 ? ? ? ? ? 74 78", 0);
        auto pattern = hook::pattern("F3 0F 10 44 24 ? F3 0F 11 04 24 8D 44 24 34 50 8D 44 24 28 50 53 8B CF");
        struct HandbrakeCam
        {
            void operator()(injector::reg_pack& regs)
            {
                int pLeftX = 0;
                int pLeftY = 0;
                int pRightX = 0;
                int pRightY = 0;
                injector::cstd<void(int, int*, int*, int*, int*)>::call(GET_POSITION_OF_ANALOGUE_STICKS, 0, &pLeftX, &pLeftY, &pRightX, &pRightY);
                if (pRightX == 0 && pRightY == 0)
                    *(float*)(regs.esp + 0x2C) *= (1.0f / 30.0f) / fTimeStep;
                //if (!*(uint8_t*)(*(uint32_t*)unk_117E700 + 0x328C))
                //    *(float*)(regs.esp + 0x2C) *= (1.0f / 30.0f) / fTimeStep;
                float f = *(float*)(regs.esp + 0x2C);
                _asm movss xmm0, dword ptr[f]
            }
        }; injector::MakeInline<HandbrakeCam>(pattern.get_first(0), pattern.get_first(6));
    }

    //camera position in TLAD
    if (bDefaultCameraAngleInTLAD)
    {
        static uint32_t episode_id = 0;
        auto pattern = hook::pattern("83 3D ? ? ? ? ? 8B 01 0F 44 C2 89 01 B0 01 C2 08 00");
        injector::WriteMemory(pattern.count(2).get(0).get<void>(2), &episode_id, true);
    }

    if (bPedDeathAnimFixFromTBOGT)
    {
        auto pattern = hook::pattern("8B D9 75 2E");
        injector::MakeNOP(pattern.get_first(2), 2, true);
    }

    if (bDisableCameraCenteringInCover)
    {
        static constexpr float xmm_0 = FLT_MAX / 2.0f;
        auto pattern = hook::pattern("F3 0F 10 05 ? ? ? ? F3 0F 58 47 ? F3 0F 11 47 ? 8B D1 89 54 24 10");
        injector::WriteMemory(pattern.get_first(4), &xmm_0, true);
    }

    if (bMouseFix)
    {
        auto pattern = hook::pattern("B9 ? ? ? ? E8 ? ? ? ? 85 C0 0F 84 ? ? ? ? F3 0F 10 9C 24 ? ? ? ? F3 0F 10 A4 24 ? ? ? ? F3 0F 10 AC 24");
        dword_F43AD8 = *pattern.get_first<uint32_t>(1);
        pattern = hook::pattern("74 3D C7 05");
        dword_1826D48 = *pattern.get_first<int32_t>(4);
        dword_1826D34 = *pattern.get_first<int32_t>(14);
        dword_1826D4C = *pattern.get_first<int32_t>(24);
        dword_1826D6C = *pattern.get_first<int32_t>(34);
        pattern = hook::pattern("51 8B 54 24 0C C7 04 24 ? ? ? ? 85 D2 75 0D 39 15 ? ? ? ? 75 17 D9 04 24 59 C3");
        injector::MakeJMP(pattern.get_first(0), MouseFix, true);

        //ped cam behaves the same way as aiming cam
        static constexpr float f255 = 255.0f;
        pattern = hook::pattern("F3 0F 59 05 ? ? ? ? F3 0F 2C C0 83 C4 0C");
        injector::WriteMemory(pattern.get_first(4), &f255, true);

        //sniper scope is slow
        static constexpr float f01 = 0.1f;
        pattern = hook::pattern("F3 0F 59 15 ? ? ? ? F3 0F 58 97");
        injector::WriteMemory(pattern.get_first(4), &f01, true);
        injector::WriteMemory(pattern.get_first(28), &f01, true);

        //first person vehicle view is slow
        pattern = hook::pattern("F3 0F 59 15 ? ? ? ? F3 0F 58 96");
        injector::WriteMemory(pattern.get_first(4), &f01, true);
        injector::WriteMemory(pattern.get_first(28), &f01, true);
    }

    //if (fFpsLimit || fCutsceneFpsLimit || fScriptCutsceneFpsLimit)
    {
        auto mode = (nFrameLimitType == 2) ? FrameLimiter::FPSLimitMode::FPS_ACCURATE : FrameLimiter::FPSLimitMode::FPS_REALTIME;
        if (mode == FrameLimiter::FPSLimitMode::FPS_ACCURATE)
            timeBeginPeriod(1);

        auto preset = FusionFixSettings(nFpsLimitPreset);
        if (preset > 1 && preset < fpsCaps.size())
            FpsLimiter.Init(mode, (float)fpsCaps[preset]);
        else
            FpsLimiter.Init(mode, fFpsLimit);
        CutsceneFpsLimiter.Init(mode, fCutsceneFpsLimit);
        ScriptCutsceneFpsLimiter.Init(mode, fScriptCutsceneFpsLimit);

        auto pattern = hook::pattern("E8 ? ? ? ? 84 C0 75 89");
        CCutscenes__hasCutsceneFinished = (bool(*)()) injector::GetBranchDestination(pattern.get_first(0)).get();
        pattern = hook::pattern("E8 ? ? ? ? 84 C0 75 44 38 05 ? ? ? ? 74 26");
        CCamera__isWidescreenBordersActive = (bool(*)()) injector::GetBranchDestination(pattern.get_first(0)).get();

        pattern = hook::pattern("8B 35 ? ? ? ? 8B 0D ? ? ? ? 8B 15 ? ? ? ? A1");
        injector::WriteMemory(pattern.get_first(0), 0x901CC483, true); //nop + add esp,1C
        injector::MakeJMP(pattern.get_first(4), sub_855640, true); // + jmp
    }

    if (fScriptCutsceneFovLimit)
    {
        auto pattern = hook::pattern("8B 4E 0C 89 48 60 5E C3");
        struct SetFOVHook
        {
            void operator()(injector::reg_pack& regs)
            {
                float f = *(float*)(regs.esi + 0x0C);
                *(float*)(regs.eax + 0x60) = f > fScriptCutsceneFovLimit ? f : fScriptCutsceneFovLimit;
            }
        }; injector::MakeInline<SetFOVHook>(pattern.get_first(0), pattern.get_first(6));
    }

    if (nVehicleBudget)
    {
        auto pattern = hook::pattern("F7 2D ? ? ? ? 8B CA C1 E9 1F 03 CA B8 ? ? ? ? F7 2D ? ? ? ? 8B C2 C1 E8 1F 03 C2 89 0D ? ? ? ? A3 ? ? ? ? 83 C4 10 C3");
        injector::WriteMemory(*pattern.get_first<void*>(2), nVehicleBudget, true);
    }

    if (nPedBudget)
    {
        auto pattern = hook::pattern("F7 2D ? ? ? ? 8B C2 C1 E8 1F 03 C2 89 0D ? ? ? ? A3 ? ? ? ? 83 C4 10 C3");
        injector::WriteMemory(*pattern.get_first<void*>(2), nPedBudget, true);
    }

    {
        //IMG Loader
        auto pattern = hook::pattern("E8 ? ? ? ? 6A 00 E8 ? ? ? ? 83 C4 14 6A 00");
        static auto CImgManager__addImgFile = (void(__cdecl*)(const char*, char, int)) injector::GetBranchDestination(pattern.get_first(0)).get();
        static auto sub_A95980 = (void(__cdecl*)(char)) injector::GetBranchDestination(pattern.get_first(7)).get();

        pattern = hook::pattern("83 3D ? ? ? ? ? 75 0F 6A 02");
        static auto _dwCurrentEpisode = *pattern.get_first<int*>(2);

        pattern = hook::pattern("89 44 24 44 8B 44 24 4C 53");
        struct ImgListHook
        {
            void operator()(injector::reg_pack& regs)
            {
                *(uint32_t*)(regs.esp + 0x44) = regs.eax;
                regs.eax = *(uint32_t*)(regs.esp + 0x4C);

                auto exePath = std::filesystem::path(GetExeModulePath<std::wstring>());
                auto updatePath = exePath.remove_filename() / L"update";

                if (std::filesystem::exists(updatePath))
                {
                    for (const auto& file : std::filesystem::recursive_directory_iterator(updatePath, std::filesystem::directory_options::skip_permission_denied))
                    {
                        static auto gamePath = std::filesystem::path(GetExeModulePath<std::wstring>()).remove_filename();
                        auto filePath = std::filesystem::path(file.path());
                        auto relativePath = std::filesystem::relative(filePath, gamePath);

                        if (!std::filesystem::is_directory(file) && file.is_regular_file() && iequals(filePath.extension().wstring(), L".img"))
                        {
                            static std::vector<std::filesystem::path> episodicPaths = {
                                std::filesystem::path("/IV/").make_preferred(),
                                std::filesystem::path("/TLAD/").make_preferred(),
                                std::filesystem::path("/TBoGT/").make_preferred(),
                            };

                            auto is_subpath = [](const std::filesystem::path& path, const std::filesystem::path& base) -> bool {
                                std::wstring str1(path.wstring()); std::wstring str2(base.wstring());
                                std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
                                std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
                                return str1.contains(str2);
                            };
                            
                            auto imgPath = std::filesystem::relative(filePath, exePath).native();
                            std::replace(std::begin(imgPath), std::end(imgPath), L'\\', L'/');
                            auto pos = imgPath.find(std::filesystem::path("/").native());

                            if (pos != imgPath.npos)
                            {
                                imgPath.replace(pos, 1, std::filesystem::path(":/").native());
                                if (std::any_of(std::begin(episodicPaths), std::end(episodicPaths), [&](auto& it) { return is_subpath(relativePath, it); }))
                                {
                                    if (*_dwCurrentEpisode < episodicPaths.size() && is_subpath(relativePath, episodicPaths[*_dwCurrentEpisode]))
                                    {
                                        sub_A95980(255);
                                        CImgManager__addImgFile(std::filesystem::path(imgPath).string().c_str(), 1, -1);
                                        sub_A95980(0);
                                    }
                                }
                                else
                                {
                                    sub_A95980(255);
                                    CImgManager__addImgFile(std::filesystem::path(imgPath).string().c_str(), 1, -1);
                                    sub_A95980(0);
                                }
                            }
                        }
                    }
                }
            }
        }; injector::MakeInline<ImgListHook>(pattern.get_first(0), pattern.get_first(8));
    }

    // runtime settings
    {
        ReadIni();

        auto pattern = hook::pattern("89 1C 95 ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 83 C4 04");
        struct IniWriter
        {
            void operator()(injector::reg_pack& regs)
            {
                auto s = (eSettings)regs.edx;
                if (FusionFixSettings.Exists(s))
                {
                    switch (s)
                    {
                    case nFpsLimitPreset:
                    {
                        auto preset = regs.ebx;
                        auto mode = (nFrameLimitType == 2) ? FrameLimiter::FPSLimitMode::FPS_ACCURATE : FrameLimiter::FPSLimitMode::FPS_REALTIME;
                        if (preset > 1 && preset < fpsCaps.size())
                            FpsLimiter.Init(mode, (float)fpsCaps[preset]);
                        else
                            FpsLimiter.Init(mode, fFpsLimit);
                    }
                    [[fallthrough]];
                    default:
                        FusionFixSettings.Get(s).WriteToIni(regs.ebx);
                        break;
                    }
                }

                FusionFixSettings.mPrefs[s] = regs.ebx;
            }
        }; injector::MakeInline<IniWriter>(pattern.get_first(0), pattern.get_first(7));

        // Additional ini reader
        pattern = hook::pattern("E8 ? ? ? ? B1 01 E8");
        sub_8C00D0 = (void(*)())injector::GetBranchDestination(pattern.get_first(0)).as_int();
        injector::MakeCALL(pattern.get_first(0), sub_8C00D0_hook, true);
        pattern = hook::pattern("E8 ? ? ? ? E8 ? ? ? ? 8B 0D ? ? ? ? 89 41 14");
        injector::MakeCALL(pattern.get_first(0), sub_8C00D0_hook, true);
        pattern = hook::pattern("68 ? ? ? ? 6A 00 8D 4C 24 10 E8 ? ? ? ? A1");
        injector::WriteMemory(pattern.get_first(1), sub_8C00D0_hook, true);
        pattern = hook::pattern("E9 ? ? ? ? CC A1 ? ? ? ? 85 C0");
        injector::MakeJMP(pattern.get_first(0), sub_8C00D0_hook, true);

        pattern = hook::pattern("E8 ? ? ? ? 80 3D ? ? ? ? ? A3 ? ? ? ? A3");
        sub_8B7830 = (int(*)())injector::GetBranchDestination(pattern.get_first(0)).as_int();
        injector::MakeCALL(pattern.get_first(0), sub_8B7830_hook, true);

        pattern = hook::pattern("E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 6A 32");
        sub_59E1C0 = (void(__cdecl*)())injector::GetBranchDestination(pattern.get_first(0)).as_int();
        injector::MakeCALL(pattern.get_first(0), sub_59E1C0_hook, true);

        pattern = hook::pattern("E8 ? ? ? ? C6 05 ? ? ? ? ? C6 05 ? ? ? ? ? E8 ? ? ? ? B9");
        sub_5A8FE0 = (void(__cdecl*)())injector::GetBranchDestination(pattern.get_first(0)).as_int();
        injector::MakeCALL(pattern.get_first(0), sub_5A8FE0_hook, true);

        // show game in display menu
        pattern = hook::pattern("75 1F FF 35 ? ? ? ? E8 ? ? ? ? 8B 4C 24 18");
        injector::MakeNOP(pattern.get_first(), 2);

        pattern = hook::pattern("83 F8 03 0F 44 CE");
        struct MenuHook
        {
            void operator()(injector::reg_pack& regs)
            {
                regs.ecx = 1;
            }
        }; injector::MakeInline<MenuHook>(pattern.get_first(0), pattern.get_first(6));

        pattern = hook::pattern("7E 4E 8A 1D");
        injector::WriteMemory<uint8_t>(pattern.get_first(0), 0xEB, true);
    }

    {
        static std::map<IDirect3DPixelShader9*, std::tuple<IDirect3DPixelShader9*, IDirect3DPixelShader9*, IDirect3DPixelShader9*, IDirect3DPixelShader9*>> shadermap;
        auto pattern = hook::pattern("53 57 51 FF 90");
        struct CreatePixelShaderHook
        {
            void operator()(injector::reg_pack& regs)
            {
                auto pDevice = (IDirect3DDevice9*)regs.ecx;
                DWORD* pFunction = (DWORD*)regs.edi;
                IDirect3DPixelShader9** ppShader = (IDirect3DPixelShader9**)regs.ebx;
                pDevice->CreatePixelShader(pFunction, ppShader);
                IDirect3DPixelShader9* pShader = *ppShader;

                if (pShader != nullptr)
                {
                    static std::vector<uint8_t> pbFunc;
                    UINT len;
                    pShader->GetFunction(nullptr, &len);
                    if (pbFunc.size() < len)
                        pbFunc.resize(len);

                    pShader->GetFunction(pbFunc.data(), &len);

                    //def c27, 1, 77, 88, 99            // FXAA Toggle
                    //def c28, 0, 77, 88, 99            // Console Gamma Toggle
                    auto pattern = hook::pattern((uintptr_t)pbFunc.data(), (uintptr_t)pbFunc.data() + pbFunc.size(), "05 ? 00 0F A0 ? ? ? ? 00 00 00 00 00 00 B0 42 00 00 C6 42");
                    if (!pattern.empty())
                    {
                        if (!shadermap.contains(pShader))
                        {
                            auto create_shader_variation = [&](int32_t fxaa, int32_t gamma) -> IDirect3DPixelShader9*
                            {
                                IDirect3DPixelShader9* shader;
                                pattern.for_each_result([&](hook::pattern_match match) {
                                    if (*match.get<uint8_t>(1) == 0x1B) // fxaa
                                        injector::WriteMemory(match.get<void>(5), fxaa, true);
                                    else if (*match.get<uint8_t>(1) == 0x1C) // gamma
                                        injector::WriteMemory(match.get<void>(5), gamma, true);
                                });
                                pDevice->CreatePixelShader((DWORD*)pbFunc.data(), &shader);
                                return shader;
                            };

                            static constexpr auto ENABLE  = 0x3F800000;
                            static constexpr auto DISABLE = 0x00000000;
                            auto fxaa_off_gamma_off = create_shader_variation(DISABLE, DISABLE);
                            auto fxaa_off_gamma_on = create_shader_variation(DISABLE, ENABLE);
                            auto fxaa_on_gamma_off = create_shader_variation(ENABLE, DISABLE);
                            auto fxaa_on_gamma_on = create_shader_variation(ENABLE, ENABLE);
                            shadermap.emplace(pShader, std::make_tuple(fxaa_off_gamma_off, fxaa_off_gamma_on, fxaa_on_gamma_off, fxaa_on_gamma_on));
                        }
                    }
                }
            }
        }; injector::MakeInline<CreatePixelShaderHook>(pattern.get_first(0), pattern.get_first(9));


        pattern = hook::pattern("8D A4 24 00 00 00 00 8B 40 04 8B 54 24 28");
        static auto ptr = pattern.get_first(0);

        pattern = hook::pattern("A1 ? ? ? ? 52 8B 08 50 89 15 ? ? ? ? FF 91 ? ? ? ? 8B 44 24 10");
        static auto pD3DDevice = *pattern.get_first<IDirect3DDevice9**>(1);
        struct SetPixelShaderHook
        {
            void operator()(injector::reg_pack& regs)
            {
                regs.eax = *(uint32_t*)pD3DDevice;
                auto pDevice = *pD3DDevice;
                {
                    auto pShader = (IDirect3DPixelShader9*)regs.edx;
                    if (shadermap.contains(pShader))
                    {
                        if (FusionFixSettings(bFXAA))
                            if (FusionFixSettings(bConsoleGamma))
                                regs.edx = (uint32_t)std::get<3>(shadermap.at(pShader));
                            else
                                regs.edx = (uint32_t)std::get<2>(shadermap.at(pShader));
                        else
                            if (FusionFixSettings(bConsoleGamma))
                                regs.edx = (uint32_t)std::get<1>(shadermap.at(pShader));
                            else
                                regs.edx = (uint32_t)std::get<0>(shadermap.at(pShader));
                    }
                }
            }
        }; injector::MakeInline<SetPixelShaderHook>(pattern.get_first(0));
    }
}

CEXP void InitializeASI()
{
    std::call_once(CallbackHandler::flag, []()
    {
        CallbackHandler::RegisterCallback(Init, hook::pattern("F3 0F 10 44 24 ? F3 0F 59 05 ? ? ? ? EB ? E8"));
    });
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        if (!IsUALPresent()) { InitializeASI(); }
    }
    if (reason == DLL_PROCESS_DETACH)
    {
        if (nFrameLimitType == FrameLimiter::FPSLimitMode::FPS_ACCURATE)
            timeEndPeriod(1);
    }
    return TRUE;
}
