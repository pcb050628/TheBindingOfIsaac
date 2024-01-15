#pragma once

class GUI
{
private:
	std::string m_strName;
	std::string m_strID;

public:
	void SetName(const std::string& _name) { m_strName = _name; }
	const std::string& GetName() { return m_strName; }

public:
	GUI();
	~GUI();
};

