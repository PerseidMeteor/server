/**
 * @file request.cc
 * @author breezeTuT (y_q_email@163.com)
 * @brief
 * @date 2023-02-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "request.h"
#include "boost/asio/buffer.hpp"
#include "regex"
namespace http
{
    namespace server
    {

        std::string request::path() const
        {
            return path_;
        }

        std::string request::method() const
        {
            return method_;
        }

        std::string request::version() const
        {
            return version_;
        }

        std::unordered_map<std::string, std::string> request::headers() const
        {
            return headers_;
        }

        std::string request::body() const
        {
            return body_;
        }

        request::request() : path_(""), method_(""), state_(REQUEST_LINE)
        {
        }

        request::~request()
        {
        }

        bool request::parse(std::array<char, 8192> buffer_)
        {
            const char CRLF[] = "\r\n";

            std::string content(buffer_.begin(), buffer_.end());

            while (state_ != FINISH)
            {
                std::size_t CRLF_pos_ = content.find_first_of(CRLF);
                std::string line = content.substr(0, CRLF_pos_);
                content = content.substr(CRLF_pos_ + 2, content.length());

                switch (state_)
                {
                case REQUEST_LINE:

                    if (!parse_request_line_(line))
                        return false; // bad request
                    break;
                case HEADERS:
                    if (line == "")
                    {
                        state_ = BODY;
                        break;
                    }

                    if (!parse_header_(line))
                        return false; // bad request
                    break;

                case BODY:
                    if (!parse_body_(line))
                        return false; // bad request

                    break;
                case FINISH:
                    break;
                }
            }
            
            // std::printf("****%s", content.c_str());
            return true;
        }

        bool request::parse_request_line_(std::string content)
        {
            std::regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
            std::smatch subMatch;

            if (std::regex_match(content, subMatch, patten))
            {
                method_ = subMatch[1];
                path_ = subMatch[2];
                version_ = subMatch[3];
                state_ = HEADERS;
                return true;
            }
            // LOG_ERROR("RequestLine Error");
            return false;
        }

        bool request::parse_header_(std::string line)
        {
            std::regex patten("^([^:]*): ?(.*)$");
            std::smatch subMatch;
            if (std::regex_match(line, subMatch, patten))
            {
                headers_[subMatch[1]] = subMatch[2];
            }
            else
            {
                state_ = BODY;
            }

            return true;
        }

        bool request::parse_body_(std::string line)
        {
            body_ = line;
            // ParsePost_();
            state_ = FINISH;
            // LOG_DEBUG("Body:%s, len:%d", line.c_str(), line.size());
            return true;
        }

    }

}
