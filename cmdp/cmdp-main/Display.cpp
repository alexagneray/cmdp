#include "pch.h"
#include "Display.h"


#define CLEAR_CONSOLE_LINE "                                                                                                   "
#define CLEAR_CONSOLE_LINE_SIZE 100

void Display::RefreshBuffer()
{
    std::unique_lock<std::mutex> lock(m_mutValInfos);
	std::stringstream ss;
    for (auto& pair : m_mapValInfos) {
        m_mapValInfos[pair.first]._bValRefresh = false;
        ss << pair.first << " = ";
        if (pair.second._val.type() == typeid(int)) {
            ss << std::any_cast<int>(pair.second._val);
        }
        else if (pair.second._val.type() == typeid(std::string)) {
            ss << std::any_cast<std::string>(pair.second._val);
        }
        else if (pair.second._val.type() == typeid(double)) {
            ss << std::any_cast<double>(pair.second._val);
        }
        else if (pair.second._val.type() == typeid(bool)) {
            ss << (std::any_cast<bool>(pair.second._val) ? "true" : "false");
        }
        else {
            ss << "unknown type";
        }
        std::string strLine = ss.str();
        std::size_t hash = std::hash<std::string>{}(strLine);

        if (pair.second._uValhash != hash)
        {
            pair.second._uValhash = hash;
            
            pair.second._strValDisplay = strLine;
            pair.second._bValRefresh = true;
            
        }
        ss.str("");
        ss.clear();
    }
}

Display::Display() :
    m_bDoRefresh(false),
    m_uLastHash(0)
{
}

void Display::Show()
{
    HANDLE m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    short i = 0;
    std::string strLineToDisplay;
    std::unique_lock<std::mutex> lock(m_mutValInfos);

    for (auto it = m_mapValInfos.begin();
        it!= m_mapValInfos.end();
        it++)
    {
        strLineToDisplay = it->second._strValDisplay;

        if (!it->second._bValRefresh)
        {
            i++;
            continue;
        }
        SetConsoleCursorPosition(m_hConsole, { 0,i });

        DWORD dwCharsWritten = 0;
        WriteConsoleA(m_hConsole,
            CLEAR_CONSOLE_LINE,
            CLEAR_CONSOLE_LINE_SIZE,
            &dwCharsWritten,
            NULL);
        SetConsoleCursorPosition(m_hConsole, { 0,i });
        WriteConsoleA(
            m_hConsole,
            strLineToDisplay.c_str(),
            strLineToDisplay.length(),
            &dwCharsWritten,
            NULL
        );
        i++;
    }
    
}

void Display::Set(const std::string&& name, const std::any&& val)
{
    std::unique_lock<std::mutex> lock(m_mutValInfos);
    m_mapValInfos[name]._val = val;
    lock.unlock();

    RefreshBuffer();
    Show();
}

Display display;