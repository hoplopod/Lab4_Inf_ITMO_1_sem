#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

const std::string FILE_NAME = "Schedule.json";
const std::string FILE_PATH = "jsons/" + FILE_NAME;

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

int main() {
	try {
		auto data = get_file_data();
		std::cout << data;
	}
	catch (const std::exception& e) { std::cerr << e.what() << "\n"; }
}