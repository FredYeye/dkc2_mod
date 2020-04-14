#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include <map>


enum Version : uint8_t
{
    US_rev0, JP_rev0, DE_rev0,
    JP_rev1, EU_rev1, DE_rev1, US_rev1,
};

struct LevelAttributes
{
    std::string name;
    uint8_t ID;
    std::array<uint16_t, 6> posX, posY;
    std::array<uint8_t, 6> flags;
    std::array<uint16_t, 6> waterLevelTarget;
};

struct VersionData
{
    uint32_t egg, DKCoin;
    std::string name;
};


class DKC2
{
    public:
        bool OpenRom();
        void Save();

        std::vector<uint8_t> GetConnectedLevels(uint8_t LevelID) const;
        int16_t GetWaterLevelTarget(uint8_t levelID, uint8_t entryPoint) const;
        void SetWaterLevelTarget(const std::map<uint8_t, LevelAttributes> &sortedLevels);

        void GetLevelData(LevelAttributes &levelAttributes, int8_t entry);
        void SetLevelData(const std::map<uint8_t, LevelAttributes> &sortedLevels);

        bool GetVersion();

        const uint8_t  GetU8(uint32_t address) const;
        void SetU8(uint32_t address, uint8_t value);
        const uint16_t GetU16(uint32_t address) const;
        const int16_t GetS16(uint32_t address) const;
        void SetWord(uint32_t address, uint16_t value);

        std::array<char, 80> romName{"Super Donkey Kong 2 - Dixie & Diddy (Japan).sfc"};
        std::string romStatus{};
        std::vector<uint8_t> rom{};

        std::array<uint16_t, 3> DKCoinThresholds;
        const std::array<std::string, 3> DKCoinNames{"Mario", "Yoshi", "Link"};
        bool goodEggs;
        Version version;

        const std::map<Version, VersionData> versionData
        {
            {US_rev0, {0x36F46E, 0x34BE8F, "US rev 0"}},
            {JP_rev0, {0x36F43D, 0x34BE7D, "JP rev 0"}},
        };

        const std::array<std::string, 8> worldNames
        {
            "1: Gangplank Galleon", "2: Crocodile Cauldron", "3: Krem Quay", "4: Krazy Kremland",
            "5: Gloomy Gulch", "6: K. Rool's Keep", "7: The Flying Krock", "8: Lost World",
        };

        std::vector<std::vector<LevelAttributes>> world
        {
            { //W1
                {"1. Pirate Panic"    , 0x03},
                {"2. Mainbrace Mayhem", 0x0C},
                {"3. Gangplank Galley", 0x04},
                {"4. Lockjaw's Locker", 0x15},
                {"5. Topsail Trouble" , 0x0B},
                {"6. Krow's Nest"     , 0x09},

                {"Pirate Panic Letter", 0x1B},
                {"Pirate Panic/Gangplank Galley Secret", 0x1D},
                {"Lockjaw's Locker Secret", 0x68},
                {"Topsail Trouble Secret", 0x1A},

                {"Pirate Panic Bonus 1", 0x6F},
                {"Pirate Panic Bonus 2", 0x70},
                {"Mainbrace Mayhem Bonus 1", 0x78},
                {"Mainbrace Mayhem Bonus 2", 0x79},
                {"Mainbrace Mayhem Bonus 3", 0x7D},
                {"Gangplank Galley Bonus 1", 0xA8},
                {"Gangplank Galley Bonus 2", 0x71},
                {"Lockjaw's Locker Bonus 1", 0x81},
                {"Topsail Trouble Bonus 1", 0x7B},
                {"Topsail Trouble Bonus 2", 0x7C},
            },
            { //W2
                {"1. Hot-Head Hop"  , 0x07},
                {"2. Kannon's Klaim", 0x25},
                {"3. Lava Lagoon"   , 0x14},
                {"4. Red-Hot Ride"  , 0x08},
                {"5. Squawk's Shaft", 0x24},
                {"6. Kleever's Kiln", 0x21},
            },
            { //W3
                {"1. Barrel Bayou"     , 0x28},
                {"2. Glimmer's Galleon", 0x01},
                {"3. Krockhead Klamber", 0x29},
                {"4. Rattle Battle"    , 0x05},
                {"5. Slime Climb"      , 0x0A},
                {"6. Bramble Blast"    , 0x2D},
                {"7. Kudgel's Kontest" , 0x63},

				{"Rattle Battle room", 0x22},
            },
            { //W4
                {"1. Hornet Hole"     , 0x11},
                {"2. Target Terror"   , 0x0E},
                {"3. Bramble Scramble", 0x2E},
                {"4. Rickety Race"    , 0x0F},
                {"5. Mudhole Marsh"   , 0x2C},
                {"6. Rambi Rumble"    , 0x02},
                {"7. King Zing Sting" , 0x60},

                {"Rambi Rumble rambi room", 0x12},

				{"Rickety Race Bonus 1", 0xC3},
                {"Rambi Rumble Bonus 1", 0xB4},
                {"Rambi Rumble Bonus 2", 0xB2},
            },
            { //W5
                {"1. Ghostly Grove"     , 0x19},
                {"2. Haunted Hall"      , 0x10},
                {"3. Gusty Glade"       , 0x18},
                {"4. Parrot Chute Panic", 0x13},
                {"5. Web Woods"         , 0x17},
                {"6. Kreepy Crow"       , 0x0D},
            },
            { //W6
                {"1. Arctic Abyss"       , 0x6C},
                {"2. Windy Well"         , 0x23},
                {"3. Castle Crush"       , 0x62},
                {"4. Clapper's Cavern"   , 0x8F},
                {"5. Chain Link Chamber" , 0x6D},
                {"6. Toxic Tower"        , 0x6E},
                {"7. Stronghold Showdown", 0xB9},

				{"Clapper's Cavern Bonus 1", 0x91},
				{"Clapper's Cavern Bonus 2", 0x92},
            },
            { //W7
                {"1. Screech's Sprint", 0x2F},
                {"2. K.Rool Duel"     , 0x61},
            },
            { //W8
                {"1. Jungle Jinx"     , 0x99},
                {"2. Black Ice Battle", 0x96},
                {"3. Klobber Karnage" , 0x80},
                {"4. Fiery Furnace"   , 0x16},
                {"5. Animal Antics"   , 0x9A},
                // {"Krocodile Kore"    , 0x},

				{"Animal Antics Bonus 1", 0x9D},
            },
        };

// /-----------------------------------------------\ level IDs found so far
// |   01 02 03 04 05    07 08 09 0A 0B 0C 0D 0E 0F|
// |10 11 12 13 14 15 16 17 18 19 1A 1B    1D      |
// |   21 22 23 24 25       28 29       2C 2D 2E 2F|
// |                                               |
// |                                               |
// |                                               |
// |60 61 62 63             68          6C 6D 6E 6F|
// |70 71                   78 79    7B 7C 7D      |
// |80 81                                        8F|
// |   91 92          96       99 9A               |
// |                        A8                     |
// |      B2    B4             B9                  |
// |         C3                                    |
// |                                               |
// |                                               |
// |                                               |
// \-----------------------------------------------/

    private:
        const std::map<std::string, Version> dateToVersion
        {
            {"46", US_rev0},
            {"44", JP_rev0},
            {"03", DE_rev0},
            {"48", JP_rev1},
            {"54", EU_rev1},
            {"00", DE_rev1},
            {"06", US_rev1},
        };
};
