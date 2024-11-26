#ifndef CONFEXCEPTIONS_HPP
#define CONFEXCEPTIONS_HPP

#include <stdexcept>

namespace Wbsv
{
	class NoExistFileException : public std::runtime_error
	{
	public:
		NoExistFileException(const std::string& message);
		// ~NoExistFileException(void) throw();
	};

	class EmptyFileException : public std::runtime_error
	{
	public:
		EmptyFileException(const std::string& message);
		// ~EmptyFileException(void) throw();
	};

	class NotEnoughInfoFileException : public std::runtime_error
	{
	public:
		NotEnoughInfoFileException(const std::string& message);
		// ~NoExistFileException(void) throw();
	};

	class InvalidContextException : public std::runtime_error
	{
	public:
		InvalidContextException(const std::string& message);
		// ~NoExistFileException(void) throw();
	};

	class UnclosedBraceException : public std::runtime_error
	{
	public:
		UnclosedBraceException(const std::string& message);
		// ~NoExistFileException(void) throw();
	};

	class DuplicatedDirectiveException : public std::runtime_error
	{
	public:
		DuplicatedDirectiveException(const std::string& message);
		// ~NoExistFileException(void) throw();
	};

	class DuplicatedLocationException : public std::runtime_error
	{
	public:
		DuplicatedLocationException(const std::string& message);
		// ~NoExistFileException(void) throw();
	};

	class NoValueException : public std::runtime_error
	{
	public:
		NoValueException(const std::string& message);
		// ~NoExistFileException(void) throw();
	};
}; // namespace Wbsv

#endif