#pragma once

class Display
{
private:
	std::mutex m_mutStrBuffer;
	std::string m_strBuffer;

	std::mutex m_mutMapVal;
	std::map<std::string, std::any> m_mapVal;
	std::map<std::string, std::string> m_mapValDisplay;
	std::map<std::string, std::size_t> m_mapValHash;
	std::map<std::string, bool> m_mapValRefresh;

	std::size_t m_uLastHash;
	bool m_bDoRefresh;

	void RefreshBuffer();
public:
	Display();
	void Show();
	void Set(const std::string&& name, const std::any&& val);
};


extern Display display;
