/**
 *  header.hpp
 */
#pragma once

#ifndef HTTP_HEADER_HPP
#define HTTP_HEADER_HPP

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

        public:
            header(){};
            
            ~header(){};
        };

    } // namespace server

} // namespace http

#endif