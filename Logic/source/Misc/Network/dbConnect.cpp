#include <Misc\Network\dbConnect.h>
/*
   
    Add Score:              http://us-central1-stockman-highscore.cloudfunctions.net/addScore?name=TEST&score=5000&kills=5000&waves=1000&time=1000
    Get List of Scores:     http://us-central1-stockman-highscore.cloudfunctions.net/getTopScores?count=5

*/

#define GET_TOP_SCORES_ADRESS "http://us-central1-stockman-highscore.cloudfunctions.net/"
#define ADD_SCORE_ADRESS "http://us-central1-stockman-highscore.cloudfunctions.net/"

using namespace Network;

dbConnect::dbConnect() { }

dbConnect::~dbConnect() { }

bool dbConnect::addHighscore(std::string name, int score, int time, int wave, int kills)
{
    sf::Http http(ADD_SCORE_ADRESS);

    // Making the arguments for the http post
    std::string scoreStr;
    scoreStr += "name=" + name;
    scoreStr += "&score=" + std::to_string(score);
    scoreStr += "&kills=" + std::to_string(kills);
    scoreStr += "&waves=" + std::to_string(wave);
    scoreStr += "&time=" + std::to_string(time);

    // Setup the request
    sf::Http::Request request;
    request.setMethod(sf::Http::Request::Post);
    request.setUri("/addScore?" + scoreStr);
    request.setHttpVersion(1, 1); // HTTP 1.1
    request.setField("From", "me");
    request.setField("Content-Type", "application/x-www-form-urlencoded");

    // Send the request
    sf::Http::Response response = http.sendRequest(request);
    printf("Resonse from server: %s\n", response.getBody().c_str());

    // Return the response as a string
    return true;
}

std::vector<std::vector<std::string>> Network::dbConnect::getHigscoreStats(int count)
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

std::vector<std::string> Network::dbConnect::requestContestants(int count)
{
    std::string result = requestHighscoreBody(count);
    std::vector<std::string> contestants = splitIntoVector(result, ';');

    return contestants;
}

std::string dbConnect::requestHighscoreBody(int count)
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

std::vector<std::string> dbConnect::splitIntoVector(std::string full, char del)
{
    std::vector<std::string> str;

    for (size_t p = 0, q = 0; p != full.npos; p = q)
    {
        std::string result = (full.substr(p + (p != 0), (q = full.find(del, p + 1)) - p - (p != 0)));
        if (!result.empty())
            str.push_back(result);
    }

    return str;
}