#ifndef ANT_COLONY_EXCEPTIONS_H
#define ANT_COLONY_EXCEPTIONS_H

#include<exception>
#include<string>

class AntException : public std::exception {
	std::string msg;
	public:
	AntException(const std::string);
	const char * what() const noexcept override;
	virtual ~AntException();
	
};

#endif
