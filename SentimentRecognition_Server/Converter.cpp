#pragma once
#include <iostream>
#include <vector>
#include <string>
#define _CRT_SECURE_NO_WARNINGS

using std::vector;
using std::cout;
using std::endl;
using std::string;

static class Converter
{
public:
	static void ConvertVectorToBytes(vector<double> vec, char* outPacket)
	{
		string binaryVectorStr;
		for (double num : vec)
			binaryVectorStr += std::to_string(static_cast<int>(num));
		memcpy(outPacket, binaryVectorStr.c_str(), binaryVectorStr.length());
	}

	static vector<double> ConvertBytesToVector(char* inPacket, int dataSize)
	{
		vector<double> vec;
		for (int i = 0; i < dataSize; i++)
			vec.push_back(inPacket[i] - '0');
		return vec;
	}

	static char* ToCharArray(std::string str)
	{
		char* S = new char[str.length() + 1];
		strcpy_s(S, str.length() + 1, str.c_str());
		return S;
	}
};
