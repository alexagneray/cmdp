#pragma once

class Display
{
private:
	struct ValInfos
	{
		std::any _val;
		std::string _strValDisplay;
		std::size_t _uValhash;
		bool _bValRefresh;
	};

	std::map<std::string, ValInfos> m_mapValInfos;

	std::mutex m_mutValInfos;

	std::size_t m_uLastHash;
	bool m_bDoRefresh;

	void RefreshBuffer();
public:
	Display();
	void Show();
	void Set(const std::string&& name, const std::any&& val);
};


extern Display display;
