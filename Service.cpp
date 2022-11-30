#include "Service.h"
#include "Exceptions.h"
#include <algorithm>

/// <summary>
/// lists tutorials
/// </summary>
/// <returns>string with tutorials</returns>
std::string Service::listTutorials() const
{
	std::string s = "";
	std::vector<Tutorial> tutorials = this->repo.getTutorials();
	for (const Tutorial& t : tutorials)
		s += "Title: " + t.getTitle() + " | Presenter: " + t.getPresenter() + " | Duration: " + t.getDuration() + " | Likes: " + std::to_string(t.getLikes()) + " | Link: " + t.getLink() + "\n";
	return s;
}

/// <summary>
/// gets tutorials by presenter
/// </summary>
/// <param name="presenter"></param>
/// <returns></returns>
std::vector<Tutorial> Service::seePresenterTutorials(const std::string& presenter)
{
	std::vector<Tutorial> res;
	std::vector<Tutorial> all = this->repo.getTutorials();
	if (presenter == "")
		return all;
	auto f = copy_if(all.begin(), all.end(), std::back_inserter(res), [presenter](Tutorial& t) {  return t.getPresenter() == presenter; });
	if (res.size() == 0)
		throw RepositoryException("There is no tutorial with that presenter!\n");
	return res;
}

void Service::addWlist(const Tutorial& t)
{
	std::vector<Tutorial> wl = this->wlist.getAll();
	auto f = std::find(wl.begin(), wl.end(), t);
	if (f != wl.end())
		throw RepositoryException("The tutorial was already added!\n");
	this->wlist.addRepo(t);
}

void Service::deleteWlist(const Tutorial& t)
{
	std::vector<Tutorial> wl = this->wlist.getAll();
	auto f = std::find(wl.begin(), wl.end(), t);
	if (f == wl.end())
		throw RepositoryException("The tutorial was already deleted!\n");
	this->wlist.deleteRepo(t.getTitle(), t.getPresenter());
}

void Service::likeTutorial(Tutorial& t)
{
	t.like();
	this->wlist.updateRepo(t.getTitle(), t.getPresenter(), t);
	this->repo.updateRepo(t.getTitle(), t.getPresenter(), t);
}

Repository& Service::getWlist()
{
	if (this->wlist.getAll().size() == 0)
		throw RepositoryException("There are no tutorials in the watchlist!\n");
	return this->wlist;
}
