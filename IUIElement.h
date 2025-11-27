#pragma once
#include <functional>
#include<memory>

class IUIElement {
protected:
	bool m_focused = false; // 現在フォーカスしているか
	bool m_isVisible = true;	//表示するかどうか
public:
	virtual ~IUIElement() = default;
	virtual void Update() {
		if (!m_isVisible)return;
	}
	virtual void Draw() {
		if (!m_isVisible)return;
	}

	bool IsVisible() const { return m_isVisible; }
	void SetVisible(bool v) { m_isVisible = v; }

	// フォーカスコールバック
	std::function<void()> OnFocusEnter;
	std::function<void()> OnFocusStay;
	std::function<void()> OnFocusExit;
};
