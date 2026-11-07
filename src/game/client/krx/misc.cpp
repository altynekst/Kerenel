#include "misc.h"
namespace OpenKrx {
	vec2 CMiscFeatures::QuantizedAim(vec2 Direction, float Radius)
	{
		// 0x1402ba970 normalizes to the full mouse maximum, rather than
		// clamping only vectors beyond it. Then MSVC cvttss2si truncates both
		// components to integers before returning them as floats.
		const float Length = length(Direction);
		if(Length == 0.0f)
			return vec2(0, 0);
		const vec2 Scaled = (Direction / Length) * Radius;
		return vec2((int)Scaled.x, (int)Scaled.y);
	}
}
