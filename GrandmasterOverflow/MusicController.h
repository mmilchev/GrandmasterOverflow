#ifndef MUSIC_CONTROLLER_H

#include <DynamicBehaviour.h>
#include <SFML/Audio/Music.hpp>

class MusicController :
	public DynamicBehaviour
{
public:
	MusicController();

	virtual void Start();

private:
	sf::Music m_Music;
};

#endif // !MUSIC_CONTROLLER_H

