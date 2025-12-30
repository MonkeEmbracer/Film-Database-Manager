#include "Validator.h"

#include <stdexcept>

int Validator::year(const std::string& input) {
	int year = 0;
	try {
		year = std::stoi(input);
	} catch (std::invalid_argument&) {
		throw ValidationException("Year must be an integer!\n");
	}
	if (year < 0)
		throw ValidationException("Year must not be negative!\n");
	return year;
}

int Validator::likes(const std::string& input) {
	int likes = 0;
	try {
		likes = std::stoi(input);
	} catch (std::invalid_argument&) {
		throw ValidationException("Like count must not be negative!\n");
	}
	if (likes < 0)
		throw ValidationException("Like count must not be negative!\n");
	return likes;
}

int Validator::initialOption(const std::string& option) {
	int realOption = 0;
	try {
		realOption = std::stoi(option);
	} catch (std::invalid_argument&) {
		throw ValidationException("\n• What? Try again!\n\n");
	}
	if (realOption != 1 and realOption != 2)
		throw ValidationException("\n• What? Try again!\n\n");
	return realOption;
}
