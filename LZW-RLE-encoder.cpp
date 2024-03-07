/*
а) Случайно сгенерировать файл в 10 тысяч символов, используя указанный алфавит
(заглавные и строчные буквы).Закодировать текстовую информацию, используя указанный в
задании алгоритм.Определить цену кодирования.Декодировать информацию, определить
коэффициент сжатия.Программно проверить, что декодирование произошло верно.

б) Закодировать информацию алгоритмом RLE.Декодировать информацию, определить
коэффициент сжатия.Программно проверить, что декодирование произошло верно.

в) Аналогично пункту а) закодировать информацию, применив двухступенчатое кодирование
(RLE + алгоритм варианта и алгоритм варианта + RLE).Показать, какой из способов более
эффективный.

вариант 9 - {а, ы, и, б, к, с, пробел, 0-9, ), .} Алгоритм LZW
*/
#include <iostream>
#include "LZW_encoder/LZW_coder.h"
#include "RLE_encoder/RLE_coder.h"


int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    std::vector<std::string> bin_dict = { "0", "1" };
    std::vector<std::string> dict = { "А", "Ы", "И", "Б", "К", "С", "а", "ы", "и", "б", "к", "с",
                        " ", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ")", "." };

    //std::vector<std::string> dict = { "p", "o", "l", "y", " " };

    std::string file_name = "file.txt";
    generateFile(dict, file_name);

    LZW_encoder(dict, file_name, "coded/LZW_coded.txt");
    LZW_decoder(dict, "coded/LZW_coded.txt", "decoded/LZW_decoded.txt");

    RLE_encoder(dict, file_name, "coded/RLE_coded.txt");
    RLE_decoder(dict, "coded/RLE_coded.txt", "decoded/RLE_decoded.txt");

    // сравнение исходного файла и декодированных
    std::cout << "LZW decoding was " << (compareFiles(file_name, "decoded/LZW_decoded.txt") ? "correct" : "incorrect");
    std::cout << "\nRLE decoding was " << (compareFiles(file_name, "decoded/RLE_decoded.txt") ? "correct" : "incorrect");

    // коэффициенты сжатия
    std::cout << "\n\nLZW compression ratio: " << float(file_size) / (numberOfCharacters("coded/LZW_coded.txt") / 8) << std::endl;
    std::cout << "RLE compression ratio: " << float(file_size) / (numberOfCharacters("coded/RLE_coded.txt") / 8) << std::endl;

    // двухступенчатое кодирование и сравнение с исходным файлом
    LZW_encoder(bin_dict, "coded/RLE_coded.txt", "coded/RLE_LZW_coded.txt");
    LZW_decoder(bin_dict, "coded/RLE_LZW_coded.txt", "halfdecoded/RLE_coded_LZW_decoded.txt");
    RLE_decoder(dict, "halfdecoded/RLE_coded_LZW_decoded.txt", "decoded/RLE_LZW_decoded.txt");
    std::cout << "\nRLE_LZW decoding was " << (compareFiles(file_name, "decoded/RLE_LZW_decoded.txt") ? "correct" : "incorrect");

    RLE_encoder(bin_dict, "coded/LZW_coded.txt", "coded/LZW_RLE_coded.txt");
    RLE_decoder(bin_dict, "coded/LZW_RLE_coded.txt", "halfdecoded/LZW_coded_RLE_decoded.txt");
    LZW_decoder(dict, "halfdecoded/LZW_coded_RLE_decoded.txt", "decoded/LZW_RLE_decoded.txt");
    std::cout << "\nLZW_RLE decoding was " << (compareFiles(file_name, "decoded/LZW_RLE_decoded.txt") ? "correct" : "incorrect");

    std::cout << "\n\nRLE_LZW compression ratio: " << float(file_size) / (numberOfCharacters("coded/RLE_LZW_coded.txt") / 8) << std::endl;
    std::cout << "LZW_RLE compression ratio: " << float(file_size) / (numberOfCharacters("coded/LZW_RLE_coded.txt") / 8) << std::endl;

    std::cout << "\n\nFinish";
}
