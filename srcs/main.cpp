/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jwee <jwee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:46:34 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/26 13:08:17 by jwee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/IrcServ.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>


// void	check_leak(void)
// {
// 	system("leaks --list -- ircserv");
// }

static void errorHandle(std::string message, int exitCode)
{
    (void)message;
    exit(exitCode);
}

int main(int argc, char **argv)
{
    if (argc != 3)
        errorHandle("Usage : ./ircserv [port] [password]", 0);

    signal(SIGTSTP, SIG_IGN);
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
        return -1;
    } catch (...) {
    }
    close(hostFd);
    return 0;
}