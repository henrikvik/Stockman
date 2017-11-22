#include <Misc\Test\TestingWork.h>
#include <Misc\HighScoreManager.h>
#include <Engine\DebugWindow.h>

using namespace Logic;

TestingWork::TestingWork()
{
    DebugWindow::getInstance()->registerCommand("TEST_DO_TESTS", [&](std::vector<std::string> &para) -> std::string {
        testEverything();
        return "Everything worked!";
    });
}

void TestingWork::testEverything()
{
    // this is my testing grounds for making sure things actually work
    HighScoreManager manager;
    manager.addNewHighScore(0);
    manager.addNewHighScore(6);
    manager.addNewHighScore(22);
    manager.addNewHighScore(64);
    manager.addNewHighScore(64);
    manager.addNewHighScore(0);
    manager.addNewHighScore(9999);
    manager.addNewHighScore(23);
    manager.addNewHighScore(66);
    manager.addNewHighScore(44);
    manager.addNewHighScore(99);
    manager.addNewHighScore(128);

    assert(manager.getHighScores()[0].score == 9999);
    assert(manager.getHighScores()[5].score == 64);

    manager.saveToFile();
    manager.loadFromFile();

    assert(manager.getHighScores()[0].score == 9999);
    assert(manager.getHighScores()[5].score == 64);
}