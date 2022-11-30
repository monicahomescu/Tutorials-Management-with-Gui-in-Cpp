#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <shellapi.h>

class Tutorial
{
private:
	std::string title;
	std::string presenter;
	std::string duration;
	int likes;
	std::string link;

public:
	//default constructor
	Tutorial() : title{ "" }, presenter{ "" }, duration{ "" }, likes{ 0 }, link{ "" } {}

	//constructor with parameters
	Tutorial(const std::string& title, const std::string& presenter, const std::string& duration, const int& likes, const std::string& link)
		:title{ title }, presenter{ presenter }, duration{ duration }, likes{ likes }, link{ link } {}

	//getters
	std::string getTitle() const { return this->title; }
	std::string getPresenter() const { return this->presenter; }
	std::string getDuration() const { return this->duration; }
	int getLikes() const { return this->likes; }
	std::string getLink() const { return this->link; }

	//operator for equality
	bool operator==(const Tutorial& t) const { return this->title == t.title && this->presenter == t.presenter; }

	//plays video
	void play() { ShellExecuteA(NULL, NULL, "chrome.exe", this->getLink().c_str(), NULL, SW_SHOWMAXIMIZED); }

	//likes video
	void like() { this->likes += 1; }

	//tokenizes a string
	static std::vector<std::string> tokenize(std::string str, char delimiter);

	//operators for overrride
	friend std::istream& operator>>(std::istream& stream, Tutorial& t);
	friend std::ostream& operator<<(std::ostream& stream, const Tutorial& t);
};
