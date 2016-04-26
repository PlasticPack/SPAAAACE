#include "cinematique.h"
#include "PhysicsComponent.h"

using namespace std;


Cinematique::Cinematique() //: m_stop(10), m_rev(10000, 10), m_rev2(0, 10), m_text("STHAP IT")
{

}
Cinematique::~Cinematique() 
{

}

bool Cinematique::update(GameObject* objet, Message &postman)
{
	m_isPlaying = true;
	/*if (m_stop.execute(objet))
	{

		if (m_rev.isDone())
		{
			if (m_text.isDone())
			{

				if (m_rev2.isDone())
				{

					m_isPlaying = false;
					m_rev.setIsDone(false);
					m_rev2.setIsDone(false);
					m_text.setIsDone(false);
					return true;
				}
				else
					m_rev2.execute(objet);
			}
			else
				m_text.execute(objet);
		}
		else
			m_rev.execute(objet);

	}

*/
	return false;
}
