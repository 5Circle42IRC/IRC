/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcClient.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysungwon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:45:38 by ysungwon          #+#    #+#             */
/*   Updated: 2023/06/21 18:45:40 by ysungwon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __IrcCLIENT__
#define __IrcCLIENT__

#include <string>

class IrcClient
{
public:
    IrcClient();
    IrcClient(int fd, std::string nickname, std::string password, std::string buffer);
    ~IrcClient();

    int getFd() const;
    int getPasswordFlag() const;
    const std::string &getNickname() const;
    const std::string &getUsername() const;
    const std::string &getHostname() const;
    const std::string &getServername() const;
    const std::string &getRealname() const;
    const std::string &getPassword() const;
    const std::string &getBuffer() const;
    const std::string getNextLineReadBuffer(); 

    void setPasswordFlag(int number); 
    void setNickname(std::string newNickname);
    void setUsername(std::string newUsername);
    void setHostname(std::string newHostname);
    void setServername(std::string newServername);
    void setRealname(std::string newRealname);
    void setPassword(std::string newPassword);
    void addBackReadBuffer(const std::string readMassage);
    void addBackBuffer(const std::string str);
    void addBackCarriageBuffer(const std::string str);
    void reduceBuffer(int result);
    void reduceReadBuffer(int result);

    void Display();
    
    IrcClient(const IrcClient &copy);
    const IrcClient &operator=(const IrcClient &copy);

protected:
	int         _fd;
    int         _passwordFlag;
	std::string _nickname;
    std::string _username;
    std::string _hostname;
    std::string _servername;
    std::string _realname;
    std::string _readBuffer;

	std::string _password;
	std::string _buffer;

private :

    
};

#endif
