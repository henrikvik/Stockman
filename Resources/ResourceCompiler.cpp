#include <filesystem>
#include <iostream>
#include <set>
#include <fstream>

namespace fs = std::experimental::filesystem;

struct Pad
{
    size_t tabs = 0;
    std::string str()
    {
        std::string s;
           
        for (size_t i = 0; i < tabs; i++)
        {
            s += "    ";
        }

        return s;
    }

    void inc() { tabs++; }
    void dec() { tabs--; }
};

std::set<std::wstring> const whitelist =
{
    L".hyb",
    L".hlsl",
    L".dds",
    L".png",
    L".spritefont"
};

struct Dir
{
    Dir(fs::path path) : path(path)
    {
        for (auto & entry : fs::directory_iterator(path))
        {
            if (fs::is_directory(entry.status()))
            {
                dirs.emplace_back(entry.path());
            }
            else
            {
                std::wstring ext = entry.path().extension().c_str();
                if (whitelist.find(ext) != whitelist.end())
                {
                    files.emplace_back(entry.path());
                }
            }
        }
    }

    fs::path path;
    std::vector<fs::path> files;
    std::vector<Dir> dirs;

    void h(std::ostream & ostream, Pad & pad)
    {
        ostream << pad.str() << "namespace " << path.filename() << "\n";
        ostream << pad.str() << "{\n";
        pad.inc();

        if (files.size() > 0)
        {
            ostream << pad.str() << "enum Files\n";
            ostream << pad.str() << "{\n";
            pad.inc();

            for (auto & file : files)
            {
                ostream << pad.str() << file.stem();
                if (file != files.back())
                    ostream << ",";
                ostream << "\n";

            }
            pad.dec();
            ostream << pad.str() << "};\n";

            ostream << pad.str() << "const std::map<Files, const char *> Paths =\n";
            ostream << pad.str() << "{\n";
            pad.inc();

            for (auto & file : files)
            {
                ostream << pad.str() << "{" << file.stem() << ", R\"(..\\" << file << ")\"}";
                if (file != files.back())
                    ostream << ",";
                ostream << "\n";
            }

            pad.dec();
            ostream << pad.str() << "};\n";

            ostream << pad.str() << "\n";
            ostream << pad.str() << "Files toEnum(const char * str)\n";
            ostream << pad.str() << "{\n";
            pad.inc();

            for (auto & file : files)
            {
                ostream << pad.str() << "if (strcmp(str, \"" << file.stem() << "\") == 0) return " << file.stem() << ";\n";
            } 
            ostream << pad.str() << "throw \"Could not find matching enum.\";\n";
            pad.dec();
            ostream << pad.str() << "}\n";
        }

        for (auto & dir : dirs) 
        { 
            if (dir.files.size() > 0 || dir.dirs.size() > 0) 
            { dir.h(ostream, pad); } 
        }

        pad.dec();
        ostream << pad.str() << "}\n";
    }
};

void buildModelFileList()
{
    std::set<fs::path> hybrisFiles;

    Dir resources("Resources");

    std::ofstream hfile("Resources/Resources.h", std::ios::trunc);
    hfile << "#pragma once\n";
    hfile << "#include <map>\n";
    resources.h(hfile, Pad());
    hfile.close();

    std::cout << "Done\n";
}

//#include "Resources.h"
int main()
{
    buildModelFileList();

    //auto Hammer = Resources::Models::toEnum("Hammer");
    return 0;
}