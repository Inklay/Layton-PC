#pragma once

#include "Game/Scene/Scene.h"

extern "C" {
	#include <libavformat/avformat.h>
	#include <libavcodec/avcodec.h>
	#include <libswscale/swscale.h>
	#include <libavutil/imgutils.h>
}

class Cinematic : public Scene {
public:
	Cinematic(Game* game, const fileUtils::path& videoFile, const std::vector<fileUtils::path> audioFiles, const std::string& nextScene, float fadeIn = 0.0f, float fadeOut = 0.0f);
	void load();
	void render();
	void handleClick(const std::string&, SDL_Event) {};
	void unload();
	
private:
	const std::vector<fileUtils::path> m_audioFiles;
	AVFormatContext* m_fmtCtx = nullptr;
	AVCodecContext* m_codecCtx = nullptr;
	SDL_Texture* m_texture = nullptr;
	SwsContext* m_sws = nullptr;
	AVFrame* m_frame = nullptr;
	AVFrame* m_frameRGB = nullptr;
	AVPacket* m_packet = nullptr;
	int m_videoStream = -1;
	SDL_FRect m_rect;
	double m_frameDuration = 0.0;
	std::string m_nextSceneName;
};
