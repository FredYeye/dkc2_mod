#pragma once

#include "dkc2.hpp"


enum Base : int8_t{dec = 10, hex = 16};

void WindowValues();
// uint16_t CharHexWordToUint16(std::array<char, 5> charArray);
// uint8_t CharHexByteToUint8(std::array<char, 3> charArray);
// void WordToCharArray(std::array<char, 5> &charArray, uint16_t value, Base base);
// void ByteToCharArray(std::array<char, 3> &charArray, uint8_t value, Base base);
void ShowHelpMarker(const char* desc);

bool show_demo_window = false;
bool showMain = true;
bool showValues = false;
bool showCode = false;

DKC2 dkc2;

uint8_t currentWorld = 0, currentLevel = 0;
uint16_t posX, posY, waterTarget;
uint8_t flags;
std::array<uint16_t, 3> DKCoinThresholds;
int entry = 0;

const std::array<std::string, 8> worldNames
{
    "1: Gangplank Galleon", "2: Crocodile Cauldron", "3: Krem Quay", "4: Krazy Kremland",
    "5: Gloomy Gulch", "6: K. Rool's Keep", "7: The Flying Krock", "8: Lost World",
};

int8_t version = 0;
const std::array<std::string, 2> versionNames {"JPrev0", "USrev0"};
