#pragma once

class HudElements {
public:
    // This enum matches the bitwise mask values used in game
    enum class KCD1Elements : int8_t {
        UNKNOWN = -1,
        COMPASS = 0,
        STATS = 1,
        SUBTITLES = 2,
        INFOTEXT = 3,
        EVENTLOG = 4,
        HINTS = 5,
        CURSOR = 7,
        WANTED = 9,
        TUTORIALMESSAGE = 11,
        QUESTEVENT = 12,
        STATCHECK = 13,
        BUFFS = 14,
        LEVELUP = 15,
        TRESSPASING = 18,
        COMBAT = 21
    };

    static auto getHudElemFromStr(const std::string& str) -> KCD1Elements
    {
        if (str == "compass") {
            return KCD1Elements::COMPASS;
        }
        if (str == "stats") {
            return KCD1Elements::STATS;
        }
        if (str == "subtitles") {
            return KCD1Elements::SUBTITLES;
        }
        if (str == "infotext") {
            return KCD1Elements::INFOTEXT;
        }
        if (str == "eventlog") {
            return KCD1Elements::EVENTLOG;
        }
        if (str == "hints") {
            return KCD1Elements::HINTS;
        }
        if (str == "crosshair") {
            return KCD1Elements::CURSOR;
        }
        if (str == "wanted") {
            return KCD1Elements::WANTED;
        }
        if (str == "tutorialmessage") {
            return KCD1Elements::TUTORIALMESSAGE;
        }
        if (str == "questevent") {
            return KCD1Elements::QUESTEVENT;
        }
        if (str == "statcheck") {
            return KCD1Elements::STATCHECK;
        }
        if (str == "buffs") {
            return KCD1Elements::BUFFS;
        }
        if (str == "levelup") {
            return KCD1Elements::LEVELUP;
        }
        if (str == "tresspasing") {
            return KCD1Elements::TRESSPASING;
        }
        if (str == "combat") {
            return KCD1Elements::COMBAT;
        }
        return KCD1Elements::UNKNOWN;
    }
};
