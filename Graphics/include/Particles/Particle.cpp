#include "Particle.h"

namespace Graphics {

EaseFunc ease_funcs[] = {
	ease::Lerp,
	ease::EaseIn,
	ease::EaseOut,
	nullptr
};

EaseFuncV ease_funcs_xmv[] = {
	ease::Lerp,
	ease::EaseIn,
	ease::EaseOut,
	nullptr
};

}