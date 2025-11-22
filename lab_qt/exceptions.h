#pragma once
#include <string>
#include <stdexcept>

class EmptyListException : public std::runtime_error {
public:
	explicit EmptyListException(const std::string& msg) : std::runtime_error(msg) {}
};

class BookNotFoundException : public std::runtime_error {
public:
	explicit BookNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};

class BookAlreadyExistsException : public std::runtime_error {
public:
	explicit BookAlreadyExistsException(const std::string& msg) : std::runtime_error(msg) {}
};

class ValidationException : public std::runtime_error {
public:
	explicit ValidationException(const std::string& msg) : std::runtime_error(msg) {}
};

class FileException : public std::runtime_error {
public:
	explicit FileException(const std::string& msg) : std::runtime_error(msg) {}
};
