#include "Game/Layton1/Scene/Cinematic.h"
#include "Game/Game.h"

extern "C" {
	#include <libavformat/avformat.h>
	#include <libavcodec/avcodec.h>
	#include <libswscale/swscale.h>
}

namespace Layton1Scene {
	Cinematic::Cinematic(Game* game, const fileUtils::path& videoFile, const fileUtils::path& audioFile) :
		Scene(game),
		m_audioFile(audioFile)
	{
		AVFormatContext* fmtCtx = NULL;
		avformat_open_input(&fmtCtx, (m_game->m_gameFolder / videoFile).string().c_str(), NULL, NULL);
		avformat_find_stream_info(fmtCtx, NULL);

		int videoStream = av_find_best_stream(fmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	}

	void Cinematic::load() {
		playBGM(m_audioFile);
	}

	void Cinematic::render() {
	}
};
