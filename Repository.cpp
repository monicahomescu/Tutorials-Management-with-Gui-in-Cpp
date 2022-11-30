#include "Repository.h"
#include "Exceptions.h"
#include <algorithm>
#include <string>

/// <summary>
/// finds tutorial by title and presenter
/// </summary>
/// <param name="title">title of tutorial to find</param>
/// <param name="presenter">presenter of tutorial to find</param>
/// <returns>tutorial found or empty tutorial</returns>
Tutorial Repository::findTutorial(const std::string& title, const std::string& presenter) const
{
	for (const Tutorial& t : this->tutorials)
		if (t.getTitle() == title && t.getPresenter() == presenter)
			return t;
	return Tutorial{};
}

/// <summary>
/// adds a tutorial to repo
/// </summary>
/// <param name="t">tutorial to add</param>
void Repository::addRepo(const Tutorial& t)
{
	auto f = std::find(this->tutorials.begin(), this->tutorials.end(), t);
	if (f != this->tutorials.end())
		throw RepositoryException("The tutorial already exists!\n");
	this->tutorials.push_back(t);
}

/// <summary>
/// deletes a tutorial from repo
/// </summary>
/// <param name="title">title of tutorial to delete</param>
/// <param name="presenter">presenter of tutorial to delete</param>
void Repository::deleteRepo(const std::string& title, const std::string& presenter)
{
	Tutorial t = findTutorial(title, presenter);
	auto f = std::find(this->tutorials.begin(), this->tutorials.end(), t);
	if (f == this->tutorials.end())
		throw RepositoryException("The tutorial doesn't exist!\n");
	this->tutorials.erase(f);
}

/// <summary>
/// updates a tutorial from repo
/// </summary>
/// <param name="title">title of tutorial to update</param>
/// <param name="presenter">presenter of tutorial to update</param>
/// <param name="t2">tutorial to update with</param>
void Repository::updateRepo(const std::string& title, const std::string& presenter, const Tutorial& t2)
{
	Tutorial t1 = findTutorial(title, presenter);
	auto f = std::find(this->tutorials.begin(), this->tutorials.end(), t1);
	if (f == this->tutorials.end())
		throw RepositoryException("The tutorial doesn't exist!\n");
	std::replace(this->tutorials.begin(), this->tutorials.end(), t1, t2);
}

/// <summary>
/// gets tutorials
/// </summary>
/// <returns>vector of tutorials</returns>
std::vector<Tutorial> Repository::getTutorials() const
{
	if (this->tutorials.size() == 0)
		throw RepositoryException("There are no tutorials in the database!\n");
	return this->tutorials;
}
