//
// main.cpp
// ~~~~~~~~
//

#include "boost/asio.hpp"
#include "iostream"
#include "server/server.hpp"
#include "stdio.h"
#include "string"

int main(int argc, char *argv[])
{

	const std::string ip = "0.0.0.0";
	const std::string port = "7891";
	const std::string doc_root = ".";

	try
	{
		// Initialise the server.
		http::server::server s(ip, port, doc_root);

		std::cerr << "server is running on ip: " << ip << ", port: " << port << ", doc directory root is:" << doc_root << std::endl;

		// Run the server until stopped.
		s.run();
	}
	catch (std::exception &e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}

	return 0;
}