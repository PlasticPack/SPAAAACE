#pragma once
#include <map>
#include <queue>
#include <string>
#include "Flags.h"
typedef std::pair<int, double> typeMsPair;
typedef std::multimap<std::string, typeMsPair> typeMessage;


class Message
{
private:
	typeMessage m_messages;

public:
	Message();
	~Message();

	void addMessage(std::string const& sender, std::string const& fromComponent, int const& msFlag, double const& message);
	void deleteMessage(std::string const& sender, std::string const& fromComponent, int const &msFlag = 0);
	
	//TODO: méthodes pour faire survivre un message à travers une frame

	std::queue<typeMsPair> getMessageQueue(std::string const& sender, std::string const& fromComponent);
	double getMessage(std::string const& sender, std::string const& fromComponent, int const& msFlag);
	
	void clearAll(void);

	//temporaire, bugfixing
	int getNumberOfMsg() { return m_messages.size(); }
	void printAllMsg();
};