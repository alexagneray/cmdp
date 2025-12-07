#include "pch.h"
#include "Display.h"


#define CLEAR_CONSOLE_LINE "                                                                                                   "
#define CLEAR_CONSOLE_LINE_SIZE 100

void Display::RefreshBuffer()
{
    std::unique_lock<std::mutex> lock(m_mutStrBuffer);
	std::stringstream ss;
    for (const auto& pair : m_mapVal) {
        m_mapValRefresh[pair.first] = false;
        ss << pair.first << " = ";
        if (pair.second.type() == typeid(int)) {
            ss << std::any_cast<int>(pair.second);
        }
        else if (pair.second.type() == typeid(std::string)) {
            ss << std::any_cast<std::string>(pair.second);
        }
        else if (pair.second.type() == typeid(double)) {
            ss << std::any_cast<double>(pair.second);
        }
        else if (pair.second.type() == typeid(bool)) {
            ss << (std::any_cast<bool>(pair.second) ? "true" : "false");
        }
        else {
            ss << "unknown type";
        }
        std::string strLine = ss.str();
        std::size_t hash = std::hash<std::string>{}(strLine);

        if (m_mapValHash[pair.first] != hash)
        {
            m_mapValHash[pair.first] = hash;
            m_mapValDisplay[pair.first] = strLine;
            m_mapValRefresh[pair.first] = true;
            
        }
        ss.str("");
        ss.clear();
    }
    m_strBuffer = ss.str();

}

Display::Display() :
    m_bDoRefresh(false),
    m_uLastHash(0)
{
}

void Display::Show()
{
    std::unique_lock<std::mutex> lock(m_mutStrBuffer);
    HANDLE m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    short i = 0;
    for (auto it = m_mapValDisplay.begin();
        it!=m_mapValDisplay.end();
        it++)
    {
        if (!m_mapValRefresh[it->first])
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
            it->second.c_str(),
            (DWORD)it->second.length(),
            &dwCharsWritten,
            NULL
        );
        i++;
    }
    
	//std::unique_lock<std::mutex> lock(m_mutStrBuffer);
 //   HANDLE m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 //   SetConsoleCursorPosition(m_hConsole, {0,0});
 //   DWORD dwCharsWritten = 0;
 //   WriteConsoleA(
 //       m_hConsole,
 //       m_strBuffer.c_str(),
 //       (DWORD)m_strBuffer.length(),
 //       &dwCharsWritten,
 //       NULL
 //   );
}

void Display::Set(const std::string&& name, const std::any&& val)
{
    std::unique_lock<std::mutex> lock(m_mutMapVal);
    m_mapVal[name] = val;
    lock.unlock();

    RefreshBuffer();
    Show();
}

Display display;