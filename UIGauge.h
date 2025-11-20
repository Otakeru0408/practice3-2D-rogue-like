#pragma once
#include "IUIElement.h"
#include "DxLib.h"

#include <memory>

class UIGauge :public IUIElement {
private:
	float x, y, w, h;
	float paddingY;
	float runningValue;
public:
	UIGauge();
	void Update()override;

	void Draw()override;
	bool IsVisible()const override;

	void SetRunningValue(float val) { runningValue = val; }
};