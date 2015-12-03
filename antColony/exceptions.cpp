#include "exceptions.h"

AntException::AntException(const std::string msg) : msg{msg} {
}

AntException::~AntException() {}

const char * AntException::what() const noexcept {
	return msg.c_str();
}
