#include <Misc/CommandsFile.h>
#include <Misc/FileLoader.h>
#include <sstream>
#include <Singletons/DebugWindow.h>
using namespace Logic;

CommandsFile::CommandsFile()
{
}

CommandsFile::~CommandsFile()
{
}

void CommandsFile::doCommandsFromFile()
{
    DebugWindow *win = DebugWindow::getInstance();

    std::istringstream istr;
    std::string cmd;

    std::vector<FileLoader::LoadedStruct> structs;
    FileLoader::singleton().loadStructsFromFile(structs, "Cmd", 0, 0, 0, R"({
    "cmd" : "
        'Edit the Cmd.lw file to make commands execute at startup'
    ";
})");

    istr.str(structs[0].strings["cmd"]);
    int first, last;
    while (getline(istr, cmd, ','))
    {
        first = (int)cmd.find('\'') + 1;
        last = (int)cmd.find_last_of('\'') - first;
        win->doCommand(cmd.substr(first, last).c_str());
    }
}