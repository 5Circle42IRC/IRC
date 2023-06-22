/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:46:34 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/21 18:50:05 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/IrcServ.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>

static void errorHandle(std::string message, int exitCode)
{
    std::cout << message << std::endl;
    exit(exitCode);
}

int main(int argc, char **argv)
{
    if (argc != 3)
        errorHandle("Usage : ./ircserv [port] [password]", 0);

    char *isError = NULL;
    long port = std::strtol(argv[1], &isError, 10);
    int hostFd;

    if (*isError || (0 > port || 65535 < port))
        errorHandle("port error", -1);
    try {
        IrcServ serv(port, argv[2]);
        hostFd = serv.on();
        serv.run();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cout << "unkown exception" << std::endl;
    }
    close(hostFd);
    return 0;
}