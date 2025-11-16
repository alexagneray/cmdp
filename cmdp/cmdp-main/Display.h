#pragma once

class Display
{
private:
	std::mutex m_mutStrBuffer;
	std::string m_strBuffer;

	std::mutex m_mutMapVal;
	std::map<std::string, std::any> m_mapVal;

	void RefreshBuffer();
public:
	
	void Show();
	void Set(const std::string&& name, const std::any&& val);
};


extern Display display;
