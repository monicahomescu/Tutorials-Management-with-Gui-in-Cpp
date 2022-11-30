#pragma once
#include "Repository.h"

class FileRepository : public Repository
{
private:
	std::string filename = "Tutorials.txt";

public:
	//constructor
	FileRepository() { read(); }

	//override functions for file
	void open() override {};
	void read() override;
	void write() override;
};
