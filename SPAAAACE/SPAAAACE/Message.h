#pragma once
#include <map>
#include <queue>
#include <string>
typedef std::pair<int, double> typeMsPair;
typedef std::multimap<std::string, typeMsPair> typeMessage;

enum
{
	MS_COLLISION = 0x8500, 
	MS_X_CHANGED, 
	MS_Y_CHANGED,
	MS_SOUND_PLAYED
};

class Message
{
private:
	typeMessage m_messages;

public:
	Message();
	~Message();

	void addMessage(std::string const& sender, std::string const& fromComponent, int const& msFlag, double const& message);

	std::queue<typeMsPair> getMessageQueue(std::string const& sender, std::string const& fromComponent);
	double getMessage(std::string const& sender, std::string const& fromComponent, int const& msFlag);

	void clearAll(void);

};