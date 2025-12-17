#pragma once

#include <algorithm> // std::clamp
#include <cmath>

enum class EaseType
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut
};

class Easing
{
public:
	static float Calculate(
		float start,
		float end,
		float t,
		EaseType type,
		float power = 2.0f   // Å© ÉCÅ[ÉYÇÃã≠Ç≥
	)
	{
		t = std::clamp(t, 0.0f, 1.0f);

		float easedT = t;

		switch (type)
		{
		case EaseType::Linear:
			easedT = t;
			break;

		case EaseType::EaseIn:
			easedT = std::pow(t, power);
			break;

		case EaseType::EaseOut:
			easedT = 1.0f - std::pow(1.0f - t, power);
			break;

		case EaseType::EaseInOut:
			if (t < 0.5f)
			{
				easedT = std::pow(2.0f * t, power) * 0.5f;
			}
			else
			{
				easedT = 1.0f - std::pow(2.0f * (1.0f - t), power) * 0.5f;
			}
			break;
		}

		return start + (end - start) * easedT;
	}
};
