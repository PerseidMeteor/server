/**
 *
 */
#include <map>

namespace http
{
    namespace server
    {
        class header
        {
        public:
            std::string name;

            std::string value;

            std::map<std::string, std::string> h_;

        public:
            header(/* args */);
            ~header();
        };

        header::header(/* args */)
        {
        }

        header::~header()
        {
        }

    } // namespace server

} // namespace http
