#include "FileRepository.h"
#include <iostream>
#include <sstream>
#include <fstream>

/// <summary>
/// overrides read function
/// </summary>
void FileRepository::read()
{
	std::ifstream f(this->filename);
	if (!f.is_open())
		return;
	Tutorial t{};
	while (f >> t)
		this->addRepo(t);
	f.close();
}

/// <summary>
/// overrides write function
/// </summary>
void FileRepository::write()
{
	std::ofstream f(this->filename);
	if (!f.is_open())
		return;
	for (const Tutorial& t : this->tutorials)
		f << t;
	f.close();
}
