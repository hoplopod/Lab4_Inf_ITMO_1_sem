#include <iostream>
#include <fstream>
#include <chrono>

#include "Lexer.h"
#include "Parser.h"
#include "BinarySerializer.h"
#include "XmlSerializer.h"
#include "IniSerializer.h"

const std::string FILE_NAME = "Schedule.json";
const std::string FILE_PATH = "jsons/" + FILE_NAME;

const std::string END_FILE_NAME = "schedule";

const int RUNS = 100;

std::string get_file_data() {
	std::ifstream file(FILE_PATH, std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		throw std::runtime_error("Cannot open file " + FILE_PATH);
	}

	auto file_size = file.tellg();
	std::string file_data(file_size, '\0');
	file.seekg(0);

	file.read(file_data.data(), file_size);

	if (file_data.size() >= 3 && static_cast<unsigned char>(file_data[0]) == 0xEF &&
		static_cast<unsigned char>(file_data[1]) == 0xBB &&
		static_cast<unsigned char>(file_data[2]) == 0xBF) {
		file_data.erase(0, 3);
	}

	return file_data;
}

template <typename Func>
static double measureMs(int runs, Func func)
{
	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < runs; i++) func();
	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
	try {

		auto start = std::chrono::steady_clock::now();
		for (int i = 0; i < RUNS; i++) {
			std::string data = get_file_data();
			Lexer lexer(data);
			std::vector<Token> tokens = lexer.tokenize();
			Parser parser(tokens);
			Value vaule = parser.parse();
			IniSerializer::save(vaule, END_FILE_NAME + ".ini");
		}
		auto end = std::chrono::steady_clock::now();
		double libTime = std::chrono::duration<double, std::milli>(end - start).count();
		std::cout << "JSON -> INI (" << RUNS << "x): " << libTime << " ms\n";
	}
	catch (const std::exception& e) { std::cerr << e.what() << "\n"; }
}