#pragma once
#include "Tutorial.h"
#include <vector>
#include <string>

class Repository
{
protected:
	std::vector<Tutorial> tutorials;

public:
	//gets size
	int getSize() const { return this->tutorials.size(); }

	//finds tutorial by title and presenter
	Tutorial findTutorial(const std::string& title, const std::string& presenter) const;

	//adds a tutorial to repo
	void addRepo(const Tutorial& t);

	//deletes a tutorial from repo
	void deleteRepo(const std::string& title, const std::string& presenter);

	//updates a tutorial from repo
	void updateRepo(const std::string& title, const std::string& presenter, const Tutorial& t);

	//gets tutorials
	std::vector<Tutorial> getTutorials() const;

	std::vector<Tutorial> getAll() { return this->tutorials; }

	//virtual functions for files
	virtual void read() { };
	virtual void write() { };
	virtual void open() { };
};
