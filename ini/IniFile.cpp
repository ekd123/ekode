#include <string>
#include <exception>
#include <algorithm>
#include "IniFile.h"
using namespace std;

namespace mylib {

IniFile::IniFile(istream &is) : 
    stream(is), 
    cursection(nullptr), 
    curstate(INI_BEGIN)
{
}

/// trim
/// 
/// remove trailing spaces and comments
static std::string trim(std::string s)
{
    if (s.empty())
    {
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

/// IniFile::Parse
/// 
/// Parse the file until next state @next.
void IniFile::Parse(IniState next)
{
    string line;
    while (curstate != next)
    {
        if (stream.eof())
        {
            curstate = INI_END;
            break;
        }
        getline(stream, line);
        line = trim(line);
        if (line.size() == 0)
            continue;
        if (line[0] == '[')
        {
            string secname = line.substr(1, line.size() - 2);
            curstate = INI_SECTION;
            cursection = new IniSection;
            sections.push_back(cursection);
            secnames.push_back(secname);
            
        }
        else
        {
            curstate = INI_KEY;
            string keyname = trim(line.substr(0, line.find('=')));
            string keyvalue = trim(line.substr(line.find('=') + 1, line.size()));
            cursection->insert(pair<string, string>(keyname, keyvalue));
        }
    }
}

/// GetLastSection
///
/// Get last section.
///
/// Arguments:
/// secname: [output] name of the section
///
/// Return value: last section
const IniSection *IniFile::GetLastSection(string &secname) const
{
    secname = secnames[secnames.size() - 1];
    return sections[sections.size() - 1];
}

/// TakeLastSection
///
/// Get last section and take its ownership.
///
/// Arguments:
/// secname: [output] name of the section
/// 
/// Return value: last section
IniSection *IniFile::TakeLastSection(string &secname)
{
    secname = secnames[secnames.size() - 1];
    auto it = secnames.end() - 1;
    secnames.erase(it);
    auto ret = sections[sections.size() - 1];
    auto secit = sections.end() - 1;
    sections.erase(secit);
    return ret;
}

const IniSection *IniFile::GetSection(string secname) const
{
    auto it = find(secnames.begin(), secnames.end(), secname);
    if (it == secnames.end())
        throw runtime_error("no such section found");
    auto secit = sections.begin() + (it - secnames.begin());
    return *secit;
}

IniSection *IniFile::TakeSection(string secname)
{
    auto it = find(secnames.begin(), secnames.end(), secname);
    if (it == secnames.end())
        throw runtime_error("no such section found");
    secnames.erase(it);
    auto secit = sections.begin() + (it - secnames.begin());
    auto ret = *secit;
    sections.erase(secit);
    return ret;
}

bool IniFile::End() const
{
    return curstate == INI_END;
}

}
