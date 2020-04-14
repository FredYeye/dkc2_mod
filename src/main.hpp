#pragma once

#include "dkc2.hpp"


void ImguiStuff();
void WindowValues();
void ShowHelpMarker(const char* desc);

// enum Base : int8_t{dec = 10, hex = 16};

// uint16_t CharHexWordToUint16(std::array<char, 5> charArray);
// uint8_t CharHexByteToUint8(std::array<char, 3> charArray);
// void WordToCharArray(std::array<char, 5> &charArray, uint16_t value, Base base);
// void ByteToCharArray(std::array<char, 3> &charArray, uint8_t value, Base base);

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
