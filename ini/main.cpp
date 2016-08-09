#include <iostream>
#include <fstream>
#include <string>
#include "IniFile.h"
using namespace mylib;
using namespace std;

int main()
{
    ifstream f("test.ini");
    IniFile parser(f);
    const IniSection *cursec;
    string secname;
    parser.Parse();
    cursec = parser.GetLastSection(secname);
    cout << '[' << secname << ']' << endl;
    for (auto i = cursec->begin(); i != cursec->end(); i++)
    {
        cout << i->first << " = " << i->second << endl;
    }
    return 0;
}

