#pragma once
#include "IUIElement.h"

#include <memory>


class UIGauge :public IUIElement {
private:
	/*std::shared_ptr<InputComponent> m_input;
	std::shared_ptr<TransformComponent> m_trans;*/

	float x, y, w, h;
	float paddingY;
public:
	UIGauge();
	void Update()override;

	void Draw()override;
};