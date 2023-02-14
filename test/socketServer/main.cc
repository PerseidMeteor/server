/**
 * @file main.cc
 * @author breezeTuT : YuQiang (y_q_email@163.com)
 * @brief socket Server main.cc
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "boost/asio.hpp"
#include "iostream"

int main(int argc, char *argv[])
{
    if(argc != 3){
        std::cerr << "usage: ./socketServer <ip> <port>";
    }

    return 0;
}