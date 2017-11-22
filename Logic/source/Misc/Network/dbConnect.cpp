#include <Misc\Network\dbConnect.h>

#define DB_ADRESS "http://us-central1-stockman-highscore.cloudfunctions.net/"

using namespace Network;

dbConnect::dbConnect() { }
dbConnect::~dbConnect() { }

// Simply uploads a new score to the database, will be sorted in the database itself
bool dbConnect::addHighscore(std::string name, int score, int time, int wave, int kills)
{
    sf::Http http(DB_ADRESS);

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

// Returns a 2D Vector containing each player and their stats, need's to be parsed
std::vector<std::vector<std::string>> dbConnect::getHigscoreStats(int count)
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

// For internal use only, returns a vector with one string each, containing all their stats, un-parsed
std::vector<std::string> dbConnect::requestContestants(int count)
{
    std::string result = requestHighscoreBody(count);
    std::vector<std::string> contestants = splitIntoVector(result, ';');

    return contestants;
}

// Returns the full response-body from the HTTP-GET call
std::string dbConnect::requestHighscoreBody(int count)
{
    sf::Http http(DB_ADRESS);
    
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

// For internal use only, because it's confusing, splits a string into a vector with strings, divided by a del
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