#include "ConfExceptions.hpp"

using namespace Wbsv;

NoExistFileException::NoExistFileException(const std::string& message)
	: std::runtime_error(message)
{ }

EmptyFileException::EmptyFileException(const std::string& message)
	: std::runtime_error(message)
{ }

NotEnoughInfoFileException::NotEnoughInfoFileException(const std::string& message)
	: std::runtime_error(message)
{ }

InvalidContextException::InvalidContextException(const std::string& message)
	: std::runtime_error(message)
{ }

UnclosedBraceException::UnclosedBraceException(const std::string& message)
	: std::runtime_error(message)
{ }

DuplicatedDirectiveException::DuplicatedDirectiveException(const std::string& message)
	: std::runtime_error(message)
{ }

DuplicatedLocationException::DuplicatedLocationException(const std::string& message)
	: std::runtime_error(message)
{ }

NoValueException::NoValueException(const std::string& message)
	: std::runtime_error(message)
{ }