#pragma once
#include "Repository.h"
#include "Tutorial.h"
#include <string>
#include <vector>

class Service
{
private:
	Repository& repo;
	Repository& wlist;

public:
	//constructor
	Service(Repository& r, Repository& w) : repo(r), wlist(w) {}

	//gets repo
	Repository& getRepo() const { return this->repo; }

	//gets size of repo
	int getRepoSize() const { return this->repo.getSize(); }

	//adds a tutorial
	void addServ(const Tutorial& t) { this->repo.addRepo(t); }

	//deletes a tutorial
	void deleteServ(const std::string& title, const std::string& presenter) { this->repo.deleteRepo(title, presenter); }

	//updates a tutorial
	void updateServ(const std::string& findTitle, const std::string& findPresenter, const Tutorial& t) { this->repo.updateRepo(findTitle, findPresenter, t); }

	//lists tutorials
	std::string listTutorials() const;

	//gets tutorials by presenter
	std::vector<Tutorial> seePresenterTutorials(const std::string& presenter);

	//adds tutorial to wlist
	void addWlist(const Tutorial& t);

	//deletes tutorial from wlist
	void deleteWlist(const Tutorial& t);

	//likes current tutorial
	void likeTutorial(Tutorial& t);

	//gets wlist
	Repository& getWlist();
};
