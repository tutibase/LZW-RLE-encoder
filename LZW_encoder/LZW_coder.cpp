#include "LZW_coder.h"

// ��������� ����� �� ������ ��������
void generateFile(const std::vector<std::string>& init_dict, const std::string& file_name) {
	std::ofstream file;
	file.open(file_name);
	int alphabet_size = init_dict.size();
	for (int i = 0; i < file_size; i++) {
		file << init_dict[rand() % alphabet_size];
	}

	file.close();
}


// ����������� LZW
void LZW_encoder(const std::vector<std::string>& init_dict, const std::string& fin_name, const std::string& fout_name) {
	std::ifstream fin;
	std::ofstream fout;

	fin.open(fin_name);
	fout.open(fout_name);

	std::vector<std::string> dictionary(init_dict);
	
	char symbol;
	std::string str = "";
	// ������������ ���������� �����
	while (fin >> std::noskipws >> symbol) {
		str += symbol;
		
		// ����� ������ � �������
		if (std::find(dictionary.begin(), dictionary.end(), str) == dictionary.end()) {
			// not found
			char eChar = str[str.size() - 1];
			str.pop_back();

			// �������� � �����
			std::vector<bool> tmp_code = binaryRepresentation(getIndex(dictionary, str), dictionary.size());
			for (int i = 0; i < tmp_code.size(); i++) {
				fout << tmp_code[i];
			}

			// ���������� ������ � �������
			str.push_back(eChar);
			dictionary.push_back(str);
			
			// ��������� ������ ���������� � ���������� ������� ����� ��� ������� ������
			str = eChar;
		}
	}
	
	// �������� � ����� ���������� �������
	std::vector<bool> tmp_code = binaryRepresentation(getIndex(dictionary, str), dictionary.size());
	for (int i = 0; i < tmp_code.size(); i++) {
		fout << tmp_code[i];
	}

	fin.close();
	fout.close();
}

// ������������� LZW
void LZW_decoder(const std::vector<std::string>& init_dict, const std::string& fin_name, const std::string& fout_name) {
	std::ifstream fin;
	std::ofstream fout;

	fin.open(fin_name);
	fout.open(fout_name);

	bool first_sym = 1;
	bool flag = 1;
	std::vector<std::string> dictionary(init_dict);
	std::string str = "";
	char buff[50];
	while (!fin.eof()) {
		flag = 1;
		// ��������� x ��� �� �����
		if (dictionary.size() == 2 and first_sym) {
			fin.get(buff, 2);
			first_sym = 0;
		}
		else {
			fin.get(buff, nearestPower2(dictionary.size() + 1) + 1);
		}
		
		std::string binary_string(buff);
		if (binary_string == "") break;
		std::string tmp; 

		// ���� ������ ��� ��� � �������, �� ��������� � �������������� (������ �� ����������)
		// http://vmath.ru/vf5/codes/lzw#dekodirovanie:~:text=%D0%9F-,%D0%9F%D1%80%D0%B8%D0%BC%D0%B5%D1%80,-.%20%D0%97%D0%B0%D0%BA%D0%BE%D0%B4%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D1%82%D1%8C%20%D0%B8
		if (getNumFromBinStr(binary_string) == dictionary.size())
			tmp = str + str[0];
		else
			tmp = dictionary[getNumFromBinStr(binary_string)];

		// ������ �� ���� �������� ��������� ������
		for (int i = 0; i < tmp.size(); i++) {
			str += tmp[i];
			// ����� ������ � �������
			if (std::find(dictionary.begin(), dictionary.end(), str) == dictionary.end() and flag) {
				// not found

				char eChar = str[str.size() - 1];
				str.pop_back();

				// �������� � �����
				fout << str;

				// ���������� ������ � �������
				str.push_back(eChar);
				dictionary.push_back(str);

				// ��������� ������ ���������� � ���������� �������� tmp
				str = tmp.substr(i, tmp.size() - i); //tmp.size() - i �������� ������� � i-�� 
				flag = false;
			}
			if (!flag) break;
		}
	}
	
	// �������� � ����� ���������� �������
	fout << str;

	fin.close();
	fout.close();
}


/////////////////////////////////////////////

// ������ �������� � �������
int getIndex(const std::vector<std::string>& dictionary, const std::string& str) {
	auto it = std::find(dictionary.begin(), dictionary.end(), str);
	if (it != dictionary.end()) {
		int ind = std::distance(dictionary.begin(), it);
		return ind;
	}
	else {
		std::cout << "������� " << str << " �� ������ � �������." << std::endl;
	}
}

// ����� � 10-���� ��, ���������� �� ��� ��������� ������������� 
int getNumFromBinStr(std::string binary_string) {
	std::reverse(binary_string.begin(), binary_string.end());
	int decimal_number = 0;
	int base = 2;

	for (int i = 0; i < binary_string.size(); i++) {
		if (binary_string[i] == '1') {
			decimal_number += pow(base, i);
		}
	}
	return decimal_number;
}

// ���� � ���������� ��������� � x ������ ������� ������
int nearestPower2(int x) {
	int result = 0;
	int i = 1;
	while (i < x) {
		result++;
		i *= 2;
	}
	return result;
}

// �������� ������������� ����� � ������������ x, ��� 2^x >= dict_size
std::vector<bool> binaryRepresentation(int num, int dict_size) {
	std::vector<bool> code(nearestPower2(dict_size));
	int i = 0;
	while (num > 0) {
		code[code.size() - 1 - i] = num % 2;
		num /= 2;
		i++;
	}

	return code;
}