#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class MyException : public std::exception {
private:
	std::string message;

public:
	explicit MyException(const std::string& message_in) : message(message_in) {}
	const char* what() const noexcept override {
		return message.c_str();
	}
};


class ServiceException : public MyException {
public:
	explicit ServiceException(const std::string& message_in) : MyException(message_in) {}
};


class RepoException : public MyException {
public:
	explicit RepoException(const std::string& message_in) : MyException(message_in) {}
};


class ValidationException : public MyException {
public:
	explicit ValidationException(const std::string& message_in) : MyException(message_in) {}
};

#endif