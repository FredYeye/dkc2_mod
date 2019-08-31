#include "dkc2.hpp"
#include "file.hpp"

#include <cstring>
#include <iostream>
#include <sstream>


bool DKC2::OpenRom()
{
    rom = FileToU8Vec(romName.data());
    if(rom.size())
    {
        romStatus = "success";

        if(!GetVersion())
        {
            romStatus = "unrecognized version";
            return false;
        }

        for(int x = 0; x < 3; ++x)
        {
            DKCoinThresholds[x] = GetU16(versionData.at(version).DKCoin + x*8);
        }

        if(GetU16(versionData.at(version).egg) == 0x0FB0)
        {
            goodEggs = false;
        }
        else if(GetU16(versionData.at(version).egg) == 0xEAEA)
        {
            goodEggs = true;
        }
        else
        {
            std::cout << "unexpected goodEggs value!\n";
        }

        for(auto &v : world)
        {
            for(auto &w : v)
            {
                for(int x = 0; x < 6; ++x)
                {
                    w.waterLevelTarget[x] = GetWaterLevelTarget(w.ID, x);

                    GetLevelData(w, x);
                }
            }
        }

        return true;
    }

    romStatus = "file not found";
    return false;
}


void DKC2::Save()
{
    //maybe save changes to a separate vector and mention how many bytes were changed or something (for sanity checking)
    const std::vector<uint8_t> romCopy = rom;


    for(int x = 0; x < 3; ++x)
    {
        SetWord(versionData.at(version).DKCoin + x*8, DKCoinThresholds[x]);
    }

    const uint16_t eggCode = (goodEggs) ? 0xEAEA : 0x0FB0;
    SetWord(versionData.at(version).egg, eggCode); //todo: saves value even if offset is wrong

    std::map<uint8_t, LevelAttributes> sortedLevels;
    for(const auto &v : world)
    {
        for(const auto &w : v)
        {
            sortedLevels.insert({w.ID, w});
        }
    }

    SetWaterLevelTarget(sortedLevels);
    SetLevelData(sortedLevels);

    uint32_t diffCount = 0;

    for(int x = 0; x < rom.size(); ++x)
    {
        if(rom[x] != romCopy[x])
        {
            ++diffCount;
        }
    }
    std::cout << "diff: " << diffCount << " bytes\n";

    U8vecToFile(rom, "modified.sfc");
}


std::vector<uint8_t> DKC2::GetConnectedLevels(uint8_t LevelID) const
{
    uint16_t a = GetU16(0x3D0000 | LevelID*2);
    while(GetU8(0x3D0000 | a) != 0xFF) { ++a; } //look over this
    a += 3;

    std::vector<uint8_t> connectedLevels;
    while(GetS16(0x3D0000 | a) > 0)
    {
        connectedLevels.push_back(GetU8(0x3D0000 | a));
        a += 2;
    }
    return connectedLevels;
}


int16_t DKC2::GetWaterLevelTarget(uint8_t levelID, uint8_t entryPoint) const
{
    --levelID;
    int16_t x = 0;
    while(levelID >= GetU8(0x3BAC3D + x)) { x += 2; }

    if(~levelID + GetU8(0x3BAC3D + x) != 0)
    {
        /*water speed = 0xF000, 0x0D54 = 0*/
        return 0x8000; //previous value in 0x0D52 i think
    }

    uint16_t a = GetU8(0x3BAC3E + x) + entryPoint;
    int16_t target = GetS16(0x3BAC69 + a*2);

    if(target < 0)
    {
        // 0x0D54 = target = ~target + 1;
        // water speed = 0xFF40;
        // water speed target = 0xFF40;
        // 0x0D56 = 0x08C2;
        return 0x012B;
    }

    return target;
}

void DKC2::SetWaterLevelTarget(const std::map<uint8_t, LevelAttributes> &sortedLevels)
{
    for(auto &w : sortedLevels)
    {
        uint8_t id = w.first - 1;
        int16_t x = 0;
        while(id >= GetU8(0x3BAC3D + x)) { x += 2; }

        if(~id + GetU8(0x3BAC3D + x) == 0)
        {
            for(int y = 0; y < 6; ++y)
            {
                uint16_t a = GetU8(0x3BAC3E + x) + y;
                int16_t target = GetS16(0x3BAC69 + a*2);

                if(target >= 0)
                {
                    SetWord(0x3BAC69 + a*2, w.second.waterLevelTarget[y]);
                }
            }
        }
    }
}


void DKC2::GetLevelData(LevelAttributes &levelAttributes, int8_t entry)
{
	uint16_t y  = GetU16(0x3D0000 | levelAttributes.ID * 2);
	uint16_t y2 = GetU16(0x3D0000 | y);
	y += 2;

    if(y2 == 1)
	{
        y2 = 0; //do that too
		//AF4B
        ++y;
	}

	if(y2 == 0 || y2 == 2 || y2 == 3 || y2 == 6)
	{
		//AF58

		//jsr AFD4
		y += 2;
		++y;

		while(GetU16(0x3D0000 | y) != 0)
		{
			y += 2;
			++y;
		}
		y += 2;
		//end AFD4

		//jsr B04F
		++y;
		//end B04F

		//jsr B059
		y += 2;
		++y;
		//end B059

        //AF6A - AF72 - AF7F
        int8_t tempEntry = entry;
		while(GetU8(0x3D0005 + y) != 0xFF)
		{
			if(--tempEntry >= 0) //int8_t entry;
			{
				y += 5;
			}
			else
			{
				break;
			}
		}

		//AF81
        levelAttributes.flags[entry] = GetU8(0x3D0000 | y);
		//... phy ... ply

		//AFA8
        levelAttributes.posX[entry] = GetU16(0x3D0001 + y);
        levelAttributes.posY[entry] = GetU16(0x3D0003 + y);
        return;
	}
	else if(y2 == 4 || y2 == 5)
	{
		//AF48
        std::cout << "unfinished (GetLevelData 2) ID=" << std::hex << +levelAttributes.ID << "\n";
        levelAttributes.flags[entry] = 0;
        levelAttributes.posX[entry] = 0;
        levelAttributes.posY[entry] = 0;
        return;
	}

	//else clear carry and return
    std::cout << "unfinished (GetLevelData 3) ID=" << std::hex << +levelAttributes.ID << "\n";
    levelAttributes.flags[entry] = 0;
    levelAttributes.posX[entry] = 0;
    levelAttributes.posY[entry] = 0;
	return;
}

void DKC2::SetLevelData(const std::map<uint8_t, LevelAttributes> &sortedLevels)
{
    for(const auto &v : sortedLevels)
    {
        uint16_t y  = GetU16(0x3D0000 | v.first * 2);
        uint16_t y2 = GetU16(0x3D0000 | y);
        y += 2;

        if(y2 == 1)
        {
            y2 = 0; //do that too
            //AF4B
            ++y;
        }

        if(y2 == 0 || y2 == 2 || y2 == 3 || y2 == 6)
        {
            //AF58

            //jsr AFD4
            y += 2;
            ++y;

            while(GetU16(0x3D0000 | y) != 0)
            {
                y += 2;
                ++y;
            }
            y += 2;
            //end AFD4

            //jsr B04F
            ++y;
            //end B04F

            //jsr B059
            y += 2;
            ++y;
            //end B059

            for(int x = 0; x < 6; ++x)
            {
                //AF6A - AF72 - AF7F
                int8_t tempEntry = x;
                uint16_t tempY = y;
                while(GetU8(0x3D0005 + tempY) != 0xFF)
                {
                    if(--tempEntry >= 0)
                    {
                        tempY += 5;
                    }
                    else
                    {
                        break;
                    }
                }

                //AF81
                SetU8(0x3D0000 | tempY, v.second.flags[x]);
                //... phy ... ply

                //AFA8
                SetWord(0x3D0001 + tempY, v.second.posX[x]);
                SetWord(0x3D0003 + tempY, v.second.posY[x]);
            }
        }
        else if(y2 == 4 || y2 == 5)
        {
            //AF48
            std::cout << "unfinished (SetLevelData 2) ID=" << std::hex << +v.second.ID << "\n";
        }
        else if(y2 > 6)
        {
            //else clear carry and return
            std::cout << "unfinished (SetLevelData 3) ID=" << std::hex << +v.second.ID << "\n";
        }
    }
}


bool DKC2::GetVersion()
{
    const std::string buildDate = {char(rom[0x3F0029]), char(rom[0x3F002A])}; //seconds of build string

    if(dateToVersion.count(buildDate))
    {
        version = dateToVersion.at(buildDate);
    }
    else
    {
        std::cout << "unrecognized version of dkc2!\n";
        return false;
    }

    if(version != US_rev0 && version != JP_rev0)
    {
        std::cout << "currently unsupported version of dkc2!\n";
        return false;
    }

    return true;
}


const uint8_t DKC2::GetU8(uint32_t address) const
{
    return rom[address];
}

void DKC2::SetU8(uint32_t address, uint8_t value)
{
    rom[address] = value;
}

const uint16_t DKC2::GetU16(uint32_t address) const
{
    return rom[address] | (rom[address+1] << 8);
}
const int16_t DKC2::GetS16(uint32_t address) const
{
    return rom[address] | (rom[address+1] << 8);
}

void DKC2::SetWord(uint32_t address, uint16_t value)
{
    rom[address]     = value;
    rom[address + 1] = value >> 8;
}
