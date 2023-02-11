#include "Settings.h"

Settings* Settings::GetSingleton() {
    static Settings singleton;
    return std::addressof(singleton);
}

void Settings::Load() {
    constexpr auto path = L"Data/SKSE/Plugins/DualWieldParryingSKSE.ini";

    CSimpleIniA ini;
    ini.SetUnicode();

    ini.LoadFile(path);

    dualWieldParryingSettings.Load(ini);

    ini.SaveFile(path);
}

void Settings::DualWieldParrying::Load(CSimpleIniA& a_ini) { 
    static const char* section = "DualWieldParrying";

    detail::get_value(a_ini, parryKey, section, "ParryKey",
                      ";Code of key to use for parrying. See https://www.creationkit.com/index.php?title=Input_Script#DXScanCodes");
}