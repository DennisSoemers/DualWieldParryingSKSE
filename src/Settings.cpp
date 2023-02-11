#include "Settings.h"

Settings* Settings::GetSingleton() {
    static Settings singleton;
    return std::addressof(singleton);
}

void Settings::Load() {
    constexpr auto path = L"Data/SKSE/Plugins/ParryingRPG.ini";

    CSimpleIniA ini;
    ini.SetUnicode();

    ini.LoadFile(path);

    core.Load(ini);
    scores.Load(ini);

    ini.SaveFile(path);
}

void Settings::Core::Load(CSimpleIniA& a_ini) { 
    static const char* section = "Core";

    detail::get_value(a_ini, useScoreSystem, section, "UseScoreSystem",
                      ";Use the score-based system to allow certain attacks to go through and ignore parries.");
}

void Settings::Scores::Load(CSimpleIniA& a_ini) {
    static const char* section = "Scores";

    detail::get_value(a_ini, scoreDiffThreshold, section, "ScoreDiffThreshold",
                      ";If the difference in scores is at least equal to this threshold, attacks are not parried.");

    detail::get_value(a_ini, weaponSkillWeight, section, "WeaponSkillWeight",
                      ";Weapon Skill is multiplied by this weight and then added to the score.");

    detail::get_value(a_ini, oneHandDaggerScore, section, "OneHandDaggerScore",
                      ";Bonus score for attacks with daggers.");
    detail::get_value(a_ini, oneHandSwordScore, section, "OneHandSwordScore",
                      ";Bonus score for attacks with one-handed swords.");
    detail::get_value(a_ini, oneHandAxeScore, section, "OneHandAxeScore",
                      ";Bonus score for attacks with one-handed axes.");
    detail::get_value(a_ini, oneHandMaceScore, section, "OneHandMaceScore",
                      ";Bonus score for attacks with one-handed maces.");
    detail::get_value(a_ini, oneHandKatanaScore, section, "OneHandKatanaScore",
                      ";Bonus score for attacks with katanas (from Animated Armoury).");
    detail::get_value(a_ini, oneHandRapierScore, section, "OneHandRapierScore",
                      ";Bonus score for attacks with rapiers (from Animated Armoury).");
    detail::get_value(a_ini, oneHandClawsScore, section, "OneHandClawsScore",
                      ";Bonus score for attacks with claws (from Animated Armoury).");
    detail::get_value(a_ini, oneHandWhipScore, section, "OneHandWhipScore",
                      ";Bonus score for attacks with whips (from Animated Armoury).");
    detail::get_value(a_ini, twoHandSwordScore, section, "TwoHandSwordScore",
                      ";Bonus score for attacks with two-handed swords.");
    detail::get_value(a_ini, twoHandAxeScore, section, "TwoHandAxeScore",
                      ";Bonus score for attacks with two-handed axes.");
    detail::get_value(a_ini, twoHandWarhammerScore, section, "TwoHandWarhammerScore",
                      ";Bonus score for attacks with two-handed warhammers.");
    detail::get_value(a_ini, twoHandPikeScore, section, "TwoHandPikeScore",
                      ";Bonus score for attacks with two-handed pikes (from Animated Armoury).");
    detail::get_value(a_ini, twoHandHalberdScore, section, "TwoHandHalberdScore",
                      ";Bonus score for attacks with two-handed halberds (from Animated Armoury).");
    detail::get_value(a_ini, twoHandQuarterstaffScore, section, "TwoHandQuarterstaffScore",
                      ";Bonus score for attacks with two-handed quarterstaffs (from Animated Armoury).");

    detail::get_value(a_ini, altmerScore, section, "AltmerScore",
                      ";Bonus score for Altmer.");
    detail::get_value(a_ini, argonianScore, section, "ArgonianScore",
                      ";Bonus score for Argonians.");
    detail::get_value(a_ini, bosmerScore, section, "BosmerScore",
                      ";Bonus score for Bosmer.");
    detail::get_value(a_ini, bretonScore, section, "BretonScore", 
                      ";Bonus score for Bretons.");
    detail::get_value(a_ini, dunmerScore, section, "DunmerScore",
                      ";Bonus score for Dunmer.");
    detail::get_value(a_ini, imperialScore, section, "ImperialScore",
                      ";Bonus score for Imperials.");
    detail::get_value(a_ini, khajiitScore, section, "KhajiitScore",
                      ";Bonus score for Khajiit.");
    detail::get_value(a_ini, nordScore, section, "NordScore",
                      ";Bonus score for Nords.");
    detail::get_value(a_ini, orcScore, section, "OrcScore",
                      ";Bonus score for Orcs.");
    detail::get_value(a_ini, redguardScore, section, "RedguardScore",
                      ";Bonus score for Redguard.");

    detail::get_value(a_ini, femaleScore, section, "FemaleScore",
                      ";Bonus score for female characters.");

    detail::get_value(a_ini, powerAttackScore, section, "PowerAttackScore",
                      ";Bonus score for power attacks.");

    detail::get_value(a_ini, playerScore, section, "PlayerScore", ";Bonus score for the Player.");
}