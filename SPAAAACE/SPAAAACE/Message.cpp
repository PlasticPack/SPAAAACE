#include "Message.h"

using namespace std;

Message::Message()
{
	//There's nothing to gain
}
Message::~Message()
{
	//There's nothing to lose
}


void Message::addMessage(std::string const& sender, std::string const& fromComponent, int const& msFlag, double const& message)
{
	m_messages.insert(std::make_pair(sender + fromComponent, make_pair(msFlag, message)));
}


std::queue<typeMsPair> Message::getMessageQueue(std::string const& sender, std::string const& fromComponent)
{
	pair<typeMessage::iterator, typeMessage::iterator> messagePlage = m_messages.equal_range(sender + fromComponent);
	queue<typeMsPair> message;
	for (typeMessage::iterator it = messagePlage.first; it != messagePlage.second; ++it)
	{
		message.push(it->second);
	}
	return message;
}

double Message::getMessage(std::string const& sender, std::string const& fromComponent, int const& msFlag)
{
	pair<typeMessage::iterator, typeMessage::iterator> messagePlage = m_messages.equal_range(sender + fromComponent);

	for (typeMessage::iterator it = messagePlage.first; it != messagePlage.second; ++it)
	{
		if (it->second.first == msFlag)
			return it->second.second;
	}
	return 0;

}

void Message::clearAll(void)
{
	m_messages.clear();
}

