#include <Misc\Test\TestingWork.h>
#include <Engine\DebugWindow.h>

// using namespace Logic;

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

}