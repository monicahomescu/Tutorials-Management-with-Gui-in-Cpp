#pragma once
#include "Tutorial.h"
#include <string>

class RepositoryException : public std::exception
{
private:
	std::string message;

public:
	RepositoryException(std::string msg) : message(msg) {};
	std::string getMessage() { return this->message; }
};

class ValidationException : public std::exception
{
private:
	std::string message;

public:
	ValidationException(std::string msg) : message(msg) {};
	std::string getMessage() { return this->message; }
};

class InputValidator
{
public:
	//constructor
	InputValidator() {};

	//validation functions
	std::string validateDuration(const std::string& duration);
	std::string validateLikes(const std::string& likes);
	std::string validateLink(const std::string& link);

	//validates tutorial
	void errors(const Tutorial& t);
};
