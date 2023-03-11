//
// main.cpp
// ~~~~~~~~
//

#include "boost/asio.hpp"
#include "iostream"
#include "server/server.hpp"
#include "stdio.h"
#include "string"
#include "server/fiber/fiber_frame_context.hpp"

int main(int argc, char *argv[])
{

    FiberFrameContext &frame_cxt = FiberFrameContext::instance();
    frame_cxt.run_thread_count = 2;
    frame_cxt.init();

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

        // boost::fibers::fiber([&s, &frame_cxt](){
        // boost::this_fiber::sleep_for(std::chrono::seconds(5));
        
        // std::cout << "api stop()\n";
        // boost::this_fiber::sleep_for(std::chrono::seconds(5));
        
        // std::cout << "api start()\n"; 
        // }).detach();

		frame_cxt.notify_stop();
		frame_cxt.wait();
	}
	catch (std::exception &e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}

	return 0;
}