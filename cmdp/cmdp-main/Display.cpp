#include "pch.h"
#include "Display.h"


void Display::RefreshBuffer()
{
    std::unique_lock<std::mutex> lock(m_mutStrBuffer);
	std::stringstream ss;
    for (const auto& pair : m_mapVal) {
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
        ss << std::endl;
    }
    m_strBuffer = ss.str();
}

void Display::Show()
{
	std::unique_lock<std::mutex> lock(m_mutStrBuffer);
    HANDLE m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(m_hConsole, {0,0});
    DWORD dwCharsWritten = 0;
    WriteConsoleA(
        m_hConsole,
        m_strBuffer.c_str(),
        (DWORD)m_strBuffer.length(),
        &dwCharsWritten,
        NULL
    );
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