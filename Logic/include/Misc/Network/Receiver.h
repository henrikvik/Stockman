#ifndef REQUESTER_H
#define REQUESTER_H

#include <Misc\NonCopyable.h>
#include <SFML\Network.hpp>

namespace Network
{
    class Receiver : public NonCopyable
    {
    public:
        Receiver();
        ~Receiver();

        std::vector<std::vector<std::string>> getHigscoreStats(int count);
    
    private:
        std::string requestHighscoreBody(int count);
        std::vector<std::string> requestContestants(int count);

        std::vector<std::string> splitIntoVector(std::string full, char del);
    };

}

#endif // !REQUESTER_H
