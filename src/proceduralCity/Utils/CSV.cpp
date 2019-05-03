///
/// @file Curve.cpp
/// @brief
///
/// Převzato z https://github.com/dormon/Shadows/blob/master/src/CSV.cpp
///
#include <Utils/CSV.h>
#include <sstream>
#include <stdexcept>

std::vector<std::vector<std::string>>loadCSV(std::string const&fileName) {
	std::ifstream f(fileName.c_str());
	std::vector<std::vector<std::string>>ret;
	if (!f.is_open())
		throw std::runtime_error(std::string("file cannot be opened: ") + fileName);
	std::string line;
	size_t largestLineLength = 0;
	while (std::getline(f, line)) {
		std::istringstream linestream(line);
		std::string word;
		std::vector<std::string>words;
		while (std::getline(linestream, word, ',')) {
			words.push_back(word);
		}
		if (largestLineLength < words.size())largestLineLength = words.size();
		ret.push_back(words);
	}
	f.close();
	for (auto&line : ret)
		line.resize(largestLineLength, "");
	return ret;
}
