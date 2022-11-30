#include "Tutorial.h"
#include <iostream>
#include <sstream>
#include <vector>

/// <summary>
/// tokenizes a string
/// </summary>
/// <param name="str">string to tokenize</param>
/// <param name="delimiter">delimiter for string</param>
/// <returns>vector of tokens</returns>
std::vector<std::string> Tutorial::tokenize(std::string str, char delimiter)
{
	std::vector<std::string> res;
	std::stringstream stream(str);
	std::string token;
	while (std::getline(stream, token, delimiter))
		res.push_back(token);
	return res;
}

/// <summary>
/// overrides >> operator
/// </summary>
/// <param name="stream">stream with data</param>
/// <param name="t">tutorial to create</param>
/// <returns>created stream</returns>
std::istream& operator>>(std::istream& is, Tutorial& t)
{
	std::string line;
	std::getline(is, line);
	std::vector<std::string> tokens = Tutorial::tokenize(line, ',');
	if (tokens.size() != 5)
		return is;
	t.title = tokens[0];
	t.presenter = tokens[1];
	t.duration = tokens[2];
	t.likes = std::stoi(tokens[3]);
	t.link = tokens[4];
	return is;
}

/// <summary>
/// overrides << operator
/// </summary>
/// <param name="stream">stream with data</param>
/// <param name="t">tutorial to display</param>
/// <returns>created stream</returns>
std::ostream& operator<<(std::ostream& os, const Tutorial& t)
{
	os << t.title << "," << t.presenter << "," << t.duration << "," << t.likes << "," << t.link << std::endl;
	return os;
}
