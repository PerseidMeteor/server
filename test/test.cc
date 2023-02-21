#include "../server/request.h"
#include "iostream"

int main()
{

    http::server::request r;
    std::array<char, 8192> buffer{"GET /a.txt HTTP/1.1\r\nHost: 0.0.0.0:7891\r\nUser-Agent: curl/7.81.0\r\nAccept: */*\r\n\r\nasdasdasd"};

    r.parse(buffer);
    std::cout << r.method() << "    " << r.path() << "    " << r.version() << std::endl;

    std::cout << r.headers().size() << "     " << r.headers()["Host"]<<r.headers()["User-Agent"] << std::endl;

    auto tmp = r.body();
    std::cout << tmp.size();
    std::cout << tmp;
    const std::string s = "asdasdasd";
    std::cout << s.length() << std::endl;

    return 0;
}