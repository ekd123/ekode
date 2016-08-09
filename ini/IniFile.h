#pragma once
#include <vector>
#include <string>
#include <map>
#include <exception>
#include <iostream>
#include <memory>

namespace mylib {

using IniSection = std::map<std::string, std::string>;

typedef enum {
    INI_SECTION,
    INI_KEY,
    INI_BEGIN,
    INI_END
} IniState;

class IniFile
{
public:
    IniFile(std::istream &is);
    ~IniFile();
    void Parse(IniState next = INI_END);
    const IniSection *GetLastSection(std::string &secname) const;
    IniSection *TakeLastSection(std::string &secname);
    const IniSection *GetSection(std::string secname) const;
    IniSection *TakeSection(std::string secname);
    bool End(void) const;
private:
    std::istream &stream;
    std::vector<IniSection *> sections;
    std::vector<std::string> secnames;
    IniSection *cursection;
    IniState curstate;
};



}
