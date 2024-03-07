#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

const unsigned short file_size = 10000;

void generateFile(const std::vector<std::string>& init_dict, const std::string& file_name);

void LZW_encoder(const std::vector<std::string>& init_dict, const std::string& fin_name, const std::string& fout_name);
void LZW_decoder(const std::vector<std::string>& init_dict, const std::string& fin_name, const std::string& fout_name);

int getIndex(const std::vector<std::string>& dictionary, const std::string& str);
int getNumFromBinStr(std::string binary_string);

int nearestPower2(int x);
std::vector<bool> binaryRepresentation(int num, int dict_size);