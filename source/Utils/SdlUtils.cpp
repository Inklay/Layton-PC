#include "SdlUtils.h"

namespace sdlUtils {
	void SDL_Deleter::operator()(SDL_Window* ptr) {
		if (ptr != nullptr) {
			SDL_DestroyWindow(ptr);
		}
	}
};
