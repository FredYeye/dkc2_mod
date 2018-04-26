#include <fstream>
#include <sstream>
#include <iostream>

#include "file.hpp"


const std::vector<uint8_t> FileToU8Vec(const std::string inFile)
{
	std::ifstream iFile(inFile.c_str(), std::ios::in | std::ios::binary);
	if(iFile.is_open() == false)
	{
		return std::vector<uint8_t>{};
	}

	std::ostringstream contents;
    contents << iFile.rdbuf();
	iFile.close();

	const std::string contentStr = contents.str();
	const std::vector<uint8_t> contentVec(contentStr.begin(), contentStr.end());

	return contentVec;
}


void U8vecToFile(const std::vector<uint8_t> &outVec, const std::string outFile)
{
	std::ofstream result(outFile.c_str(), std::ios::out | std::ios::binary);
	result.write((char*)outVec.data(), outVec.size());
	result.close();
}
