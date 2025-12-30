#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "../Exceptions/Exceptions.h"

class Validator {
public:
	static int year(const std::string&);
	static int likes(const std::string&);
	static int initialOption(const std::string&);
};

#endif
