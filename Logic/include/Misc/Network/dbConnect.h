#ifndef dbCONNECT_H
#define dbCONNECT_H

#include <Misc\NonCopyable.h>
#include <SFML\Network.hpp>

namespace Network
{
    class dbConnect : public NonCopyable
    {
    public:
        dbConnect();
        ~dbConnect();

        bool addHighscore(std::string name, int score, int time, int wave, int kills);
        std::vector<std::vector<std::string>> getHigscoreStats(int count);
    
    private:
        std::string requestHighscoreBody(int count);
        std::vector<std::string> requestContestants(int count);

        std::vector<std::string> splitIntoVector(std::string full, char del);
    };

}

#endif // !dbCONNECT_H
