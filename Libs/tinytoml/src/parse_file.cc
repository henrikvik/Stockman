#include "toml/toml.h"

#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "Usage:" << endl
             << "  " << argv[0] << " <input-file>" << endl;
        return 1;
    }

    toml::ParseResult pr = toml::parseFile(argv[1]);
    if (pr.valid()) {
        cout << pr.value;
    } else {
        cout << pr.errorReason << endl;
    }
}
