#include "RLE_coder.h"

void RLE_encoder(const std::vector<std::string>& init_dict, const std::string& fin_name, const std::string& fout_name) {
	std::ifstream fin;
	std::ofstream fout;

	fin.open(fin_name);
	fout.open(fout_name);

	std::string different = "";
	char prev;
	char next;
	int count = 1;
	fin >> std::noskipws >> prev;
	// посимвольное считывание файла
	while (fin >> std::noskipws >> next) {
		if (next == prev) {
			count++;

			if (different != "") { 
				fout << "0" << binaryRepresentationStr(different.size(), pow(2, bitsSize - 1));
				for (int i = 0; i < different.size(); i++) {
					fout << binaryRepresentationStr(getIndex(init_dict, std::string(1, different[i])), init_dict.size());
				}
				different = "";
			}
		}
		else {
			
			if (count == 1) {
				different += prev;
			}
			else {
				fout << "1" << binaryRepresentationStr(count, pow(2, bitsSize - 1))
					<< binaryRepresentationStr(getIndex(init_dict, std::string(1, prev)), init_dict.size());
			}

			count = 1;
			prev = next;
		}
	}

	if (different == "") {
		fout << "1" << binaryRepresentationStr(count, pow(2, bitsSize - 1))
			<< binaryRepresentationStr(getIndex(init_dict, std::string(1, prev)), init_dict.size());
	}
	else {
		different += prev;
		fout << "0" << binaryRepresentationStr(different.size(), pow(2, bitsSize - 1));
		for (int i = 0; i < different.size(); i++) {
			fout << binaryRepresentationStr(getIndex(init_dict, std::string(1, different[i])), init_dict.size());
		}
	}
	
	fin.close();
	fout.close();
}

void RLE_decoder(const std::vector<std::string>& init_dict, const std::string& fin_name, const std::string& fout_name) {
	std::ifstream fin;
	std::ofstream fout;


	fin.open(fin_name);
	fout.open(fout_name);

	char buff[50];
	while (!fin.eof()) {
		fin.get(buff, bitsSize + 1);
		std::string binary_string(buff);
		if (binary_string == "") break;
		
		if (binary_string[0] == '1') {
			int num = 0;
			num = getNumFromBinStr(binary_string.substr(1, binary_string.size() - 1));

			fin.get(buff, nearestPower2(init_dict.size()) + 1);
			binary_string = buff;
			for (int i = 0; i < num; i++) {
				fout << init_dict[getNumFromBinStr(binary_string)];
			}
		}
		else {
			int num = 0;
			num = getNumFromBinStr(binary_string);

			for (int i = 0; i < num; i++) {
				fin.get(buff, nearestPower2(init_dict.size()) + 1);
				binary_string = buff;
				fout << init_dict[getNumFromBinStr(binary_string)];
			}
		}

	}

	fin.close();
	fout.close();
}


///////////////////////////////////

std::string binaryRepresentationStr(int num, int dict_size) {
	std::vector<bool> code(nearestPower2(dict_size));
	int i = 0;
	while (num > 0) {
		code[code.size() - 1 - i] = num % 2;
		num /= 2;
		i++;
	}

	std::string result;
	for (int i = 0; i < code.size(); i++) {
		result += code[i] ? "1" : "0";
	}

	return result;
}

bool compareFiles(const std::string& file1_name, const std::string& file2_name) {
	std::ifstream file1(file1_name);
	std::ifstream file2(file2_name);

	const int buff_size = 500;
	char buff[buff_size];
	while (!file1.eof() and !file2.eof()) {
		file1.get(buff, buff_size);
		std::string line1(buff);

		file2.get(buff, buff_size);
		std::string line2(buff);

		if (line1 != line2) {
			return false;
		}
	}

	return !(file1.eof() != file2.eof());
}


int numberOfCharacters(const std::string& file_name) {
	std::ifstream fin;
	fin.open(file_name);

	char symbol;
	int result = 0;
	// посимвольное считывание файла
	while (fin >> std::noskipws >> symbol) {
		result++;
	}

	fin.close();
	return result;
}
