#include <Misc\Network\Receiver.h>
/*
   
    Add Score:              http://us-central1-stockman-highscore.cloudfunctions.net/addScore?name=TEST&score=5000&kills=5000&waves=1000&time=1000
    Get List of Scores:     http://us-central1-stockman-highscore.cloudfunctions.net/getTopScores?count=5

*/

#define GET_TOP_SCORES_ADRESS "http://us-central1-stockman-highscore.cloudfunctions.net/"

using namespace Network;

Receiver::Receiver() 
{
    //std::vector<std::vector<std::string>> contestants = getHigscoreStats(10);


    //printf("\n\n************************\nParsed from database request:");
    //for (size_t i = 0; i < contestants.size(); i++)
    //{
    //    printf("\nPlace: %d", i);
    //    for (size_t j = 0; j < contestants[i].size(); j++)
    //    {
    //        printf(" *-. %s", contestants[i][j].c_str());
    //    }
    //}
}

Receiver::~Receiver() { }

std::vector<std::vector<std::string>> Network::Receiver::getHigscoreStats(int count)
{
    std::vector<std::vector<std::string>> entries;
    std::vector<std::string> contestants = requestContestants(count);

    for (size_t i = 0; i < contestants.size(); i++)
    {
        std::vector<std::string> entry = splitIntoVector(contestants[i], ',');
        entries.push_back(entry);
    }

    return entries;
}

std::vector<std::string> Network::Receiver::requestContestants(int count)
{
    std::string result = requestHighscoreBody(count);
    std::vector<std::string> contestants = splitIntoVector(result, ';');

    return contestants;
}

std::string Receiver::requestHighscoreBody(int count)
{
    sf::Http http(GET_TOP_SCORES_ADRESS);
    
    // Setup the request
    sf::Http::Request request;
    request.setMethod(sf::Http::Request::Get);
    request.setUri("/getTopScores?count=" + std::to_string(count));
    request.setHttpVersion(1, 1); // HTTP 1.1
    request.setField("From", "me");
    request.setField("Content-Type", "application/x-www-form-urlencoded");

    // Send the request
    sf::Http::Response response = http.sendRequest(request);

    // Return the response as a string
    return response.getBody();
}

std::vector<std::string> Receiver::splitIntoVector(std::string full, char del)
{
    std::vector<std::string> str;

    for (size_t p = 0, q = 0; p != full.npos; p = q)
        str.push_back((full.substr(p + (p != 0), (q = full.find(del, p + 1)) - p - (p != 0))));

    return str;
}