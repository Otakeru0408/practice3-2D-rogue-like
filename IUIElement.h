#pragma once
#include <functional>

class IUIElement {
protected:
	bool m_focused = false; // 現在フォーカスしているか
public:
	virtual ~IUIElement() = default;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual bool IsVisible() const = 0;

	// フォーカスコールバック
	std::function<void()> OnFocusEnter;
	std::function<void()> OnFocusStay;
	std::function<void()> OnFocusExit;
};
