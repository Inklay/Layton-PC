#include "Game/Scene/Cinematic.h"
#include "Game/Game.h"

Cinematic::Cinematic(Game* game, const fileUtils::path& videoFile, const std::vector<fileUtils::path> audioFiles, const std::string& nextScene, float fadeIn, float fadeOut) :
	Scene(game, fadeIn, fadeOut),
	m_audioFiles(audioFiles),
	m_nextSceneName(nextScene)
{
	m_fmtCtx = NULL;
	avformat_open_input(&m_fmtCtx, (m_game->m_gameFolder / videoFile).string().c_str(), NULL, NULL);
	avformat_find_stream_info(m_fmtCtx, NULL);

	m_videoStream = av_find_best_stream(m_fmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	const AVCodec* codec = avcodec_find_decoder(m_fmtCtx->streams[m_videoStream]->codecpar->codec_id);

	m_codecCtx = avcodec_alloc_context3(codec);
	avcodec_parameters_to_context(m_codecCtx, m_fmtCtx->streams[m_videoStream]->codecpar);
	avcodec_open2(m_codecCtx, codec, NULL);

	int h = m_codecCtx->height;
	int w = m_codecCtx->width;
	m_sws = sws_getContext( w, h, m_codecCtx->pix_fmt, w, h, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);

	m_texture = SDL_CreateTexture(m_game->m_renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, w, h);
	m_packet = av_packet_alloc();
	m_frame = av_frame_alloc();
	m_frameRGB = av_frame_alloc();
	av_image_alloc(m_frameRGB->data, m_frameRGB->linesize, w, h, AV_PIX_FMT_RGB24, 1);
	m_rect = SDL_FRect{ 0, HALF_HEIGHT, WIDTH, HALF_HEIGHT };
	AVRational fps = m_fmtCtx->streams[m_videoStream]->avg_frame_rate;
	m_frameDuration = 1000.0 * fps.den / fps.num;
	sdlUtils::multiply(m_rect, m_game->m_windowMultiplier);
}

void Cinematic::load() {
	m_game->setFrameDuration((int)m_frameDuration);

	for (size_t i = 0; i < m_audioFiles.size(); i++) {
		playBGM(m_audioFiles.at(i), i);
	}

	m_fading = true;

	Scene::load();
}

void Cinematic::render() {
	while (av_read_frame(m_fmtCtx, m_packet) >= 0) {
		if (m_packet->stream_index != m_videoStream) {
			av_packet_unref(m_packet);
			continue;
		}

		avcodec_send_packet(m_codecCtx, m_packet);
			
		AVStream* stream = m_fmtCtx->streams[m_videoStream];
		double currentTime = m_packet->pts * av_q2d(stream->time_base);
		double totalDuration = m_fmtCtx->duration / (double)AV_TIME_BASE;
		double remainingTime = totalDuration - currentTime;

		if (remainingTime <= m_fadeOutSeconds && m_fadeOutSeconds > 0.0f) {
			fadeToNextScene(m_nextSceneName);
		}

		av_packet_unref(m_packet);

		if (avcodec_receive_frame(m_codecCtx, m_frame) == 0) {
			sws_scale(m_sws,
				m_frame->data, m_frame->linesize, 0, m_codecCtx->height,
				m_frameRGB->data, m_frameRGB->linesize);

			SDL_UpdateTexture(m_texture, NULL, m_frameRGB->data[0], m_frameRGB->linesize[0]);
			break;
		}
	}

	SDL_RenderTexture(m_game->m_renderer, m_texture, NULL, reinterpret_cast<const SDL_FRect*>(&m_rect));

	Scene::render();
}

void Cinematic::unload() {
	av_freep(&m_frameRGB->data[0]);
	av_frame_free(&m_frame);
	av_frame_free(&m_frameRGB);
	av_packet_free(&m_packet);
	sws_freeContext(m_sws);
	avcodec_free_context(&m_codecCtx);
	avformat_close_input(&m_fmtCtx);
	SDL_DestroyTexture(m_texture);
	m_game->setFrameDuration(16);

	Scene::unload();
}
