#include "Exceptions.h"

/// <summary>
/// function to validate duration
/// </summary>
/// <param name="duration">duration to validate</param>
/// <returns>appropriate message</returns>
std::string InputValidator::validateDuration(const std::string& duration)
{
	std::string v = "";
	int ok = 1;
	if (duration.length() > 5 || duration.length() < 4)
		ok = 0;
	if (duration.length() == 5)
	{
		if (duration[2] != ':' || std::isdigit(duration[0]) == NULL || std::isdigit(duration[1]) == NULL || std::isdigit(duration[3]) == NULL || std::isdigit(duration[4]) == NULL)
			ok = 0;
		if ((duration[0] - '0') * 10 + (duration[1] - '0') > 59 || (duration[3] - '0') * 10 + (duration[4] - '0') > 59)
			ok = 0;
	}
	if (duration.length() == 4)
	{
		if (duration[1] != ':' || std::isdigit(duration[0]) == NULL || std::isdigit(duration[2]) == NULL || std::isdigit(duration[3]) == NULL)
			ok = 0;
		if ((duration[2] - '0') * 10 + (duration[3] - '0') > 59)
			ok = 0;
	}
	if (ok == 0)
		v += "The duration doesn't have a valid form!\n";
	return v;
}

/// <summary>
/// function to validate likes
/// </summary>
/// <param name="likes">likes to validate</param>
/// <returns>appropriate message</returns>
std::string InputValidator::validateLikes(const std::string& likes)
{
	std::string v = "";
	int ok = 1;
	for (int i = 0; i < likes.length(); i++)
	{
		if (std::isdigit(likes[i]) == NULL)
			ok = 0;
	}
	if (ok == 0)
		v += "The likes should be a number!\n";
	return v;
}

/// <summary>
/// function to validate link
/// </summary>
/// <param name="link">link to validate</param>
/// <returns>appropriate message</returns>
std::string InputValidator::validateLink(const std::string& link)
{
	std::string v = "";
	int ok = 1;
	if (link.length() != 43)
		ok = 0;
	if (link.find("https://www.youtube.com/watch?v=") != 0)
		ok = 0;
	if (ok == 0)
		v += "The link doesn't have a valid form!\n";
	return v;
}

/// <summary>
/// function to validate tutorial
/// </summary>
/// <param name="t">tutorial</param>
void InputValidator::errors(const Tutorial& t)
{
	std::string err;
	if (validateDuration(t.getDuration()).length() > 0)
		err += validateDuration(t.getDuration());
	if (validateLink(t.getLink()).length() > 0)
		err += validateLink(t.getLink());
	if (err.size() > 0)
		throw ValidationException(err);
}
