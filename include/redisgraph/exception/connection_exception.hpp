#ifndef REDISGRAPH_CPP_CONNECTION_EXCEPTION_H
#define REDISGRAPH_CPP_CONNECTION_EXCEPTION_H
#include <exception>
class connection_exception : public std::system_error
{
public:
	connection_exception(std::error_code ec, const char* what_arg): std::system_error(ec, what_arg)
	{
	}
};
#endif // !REDISGRAPH_CPP_CONNECTION_EXCEPTION_H
