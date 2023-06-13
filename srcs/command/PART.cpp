#include "../../include/IrcCommand.hpp"
#include "../../include/IrcClient.hpp"
#include "../../include/IrcChannel.hpp"

static int checkValidChname(std::string chname)
{
    int size = chname.size();
    if (size > 200 || size < 2)
    {   
        std::cout << "chname size is wrong, chname : <" << chname << ">" << std::endl;
        return -1;
    }
    if (chname.front() != '#')
    {
        std::cout << "chname not start with #,   chname : <" << chname << ">" << std::endl;
        return -1;        
    }

    return 0;
}
void IrcCommand::PART(int clientfd){
    try
    {
        if (getArgs()[0].size() < 1)
        {
            std::cout << "PART need more than 1 args.  args size : <" << getArgs()[0].size() << ">" << std::endl;
            return;
        }
        std::string chname = getArgs()[0];
        if (checkValidChname(chname) != -1)//check chname valid
        {
            std::cout << "t1" << std::endl;    
            IrcChannel* ch = _db->findChannel(chname);
            std::cout << "t2" << std::endl;
            ch->deleteUser(clientfd);
            std::cout << "t3" << std::endl;
        }
        return ;
    }
    catch (const std::exception& e)
    {
        //std::fprintf(stderr, "Error: %s\n", e.what());
        std::cout << "Exception : < " << e.what() << ">" << std::endl;
        return ;
    }

   
}