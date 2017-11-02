#include <Misc/CommandsFile.h>
#include <Misc/FileLoader.h>
#include <sstream>
#include <Engine/DebugWindow.h>
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
    FileLoader::singleton().loadStructsFromFile(structs, "Cmd");

    istr.str(structs[0].strings["cmd"]);
    int first, last;
    while (getline(istr, cmd, ','))
    {
        first = cmd.find('\'') + 1;
        last = cmd.find_last_of('\'') - first;
        win->doCommand(cmd.substr(first, last).c_str());
    }
}