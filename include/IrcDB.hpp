#ifndef __IRC_DB_HPP__
#define __IRC_DB_HPP__

#include "../InterfaceClass/IDB.hpp"

template <typename T, typename S>
class DB : private IDB
{
public:
    DB();
    ~DB();
    virtual const S &find(const T &t) const;
    virtual void deleteTarget(const int targetFd);
    virtual void addTarget(const int targetFd);
private:
    const DB& operator=(const DB& copy);
    const DB& DB(const DB& copy);


}

#endif