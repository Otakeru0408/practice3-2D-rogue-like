#include "UIGauge.h"
#include "GameData.h"

UIGauge::UIGauge()
	:x(0), y(0), w(100), h(10), paddingY(60)
{
	x = GameData::windowWidth / 2;
	y = GameData::windowHeight / 2;
}

void UIGauge::Update() {

}

void UIGauge::Draw() {
	DrawBox(x - w / 2, y - h / 2 + paddingY, x + w / 2, y + h / 2 + paddingY,
		GetColor(0, 0, 0), TRUE);

	DrawBox(x - w / 2, y - h / 2 + paddingY, (x - w / 2) + (1 - runningValue) * w, y + h / 2 + paddingY,
		GetColor(255, 0, 0), TRUE);

	DrawBox(x - w / 2, y - h / 2 + paddingY, x + w / 2, y + h / 2 + paddingY,
		GetColor(255, 255, 255), FALSE);
}

bool UIGauge::IsVisible()const {
	return true;
}