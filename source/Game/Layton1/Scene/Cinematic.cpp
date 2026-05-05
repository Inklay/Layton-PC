#include "Game/Layton1/Scene/Cinematic.h"
#include "Game/Game.h"

namespace Layton1Scene {
	Cinematic::Cinematic(Game* game, const fileUtils::path& videoFile, const fileUtils::path& audioFile) :
		Scene(game),
		m_audioFile(audioFile)
	{
	}

	void Cinematic::load() {
		playBGM(m_audioFile);
	}

	void Cinematic::render() {
	}
};
