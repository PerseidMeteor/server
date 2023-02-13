//
// main.cpp
// ~~~~~~~~
//

#include "server/server.hpp"
#include "stdio.h"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	try
	{
		std::string ip = "0.0.0.0";
		std::string port = "7891";
		std::string doc_root = ".";

		printf("server is running on %s %s,doc_root is <%s> ...", ip.c_str(), port.c_str(), doc_root.c_str());

		// Initialise the server.
		http::server::server s(ip, port, doc_root);

		// Run the server until stopped.
		s.run();
	}
	catch (std::exception &e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}

	return 0;
}