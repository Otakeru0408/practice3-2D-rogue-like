#pragma once
#include "DxLib.h"
#include "IUIElement.h"
#include "GameData.h"

#include <string>

class UIButton : public IUIElement {
private:
	RECT m_rect;
	std::string m_text;
	std::function<void()> m_onClick;
	bool m_visible;
public:
	//RECTを使っているが、引数は{中心X、中心Y、幅、高さ}で取得したい
	UIButton(int cx, int cy, int w, int h, const std::string& text, std::function<void()> onClick)
		: m_text(text), m_onClick(onClick), m_visible(true) {
		m_rect.left = cx - w / 2;
		m_rect.top = cy - h / 2;
		m_rect.right = cx + w / 2;
		m_rect.bottom = cy + h / 2;
	}

	void Update() override {
		if (!m_visible) return;

		POINT mouse;
		GetCursorPos(&mouse);
		ScreenToClient(GetActiveWindow(), &mouse);

		bool hovering = PtInRect(&m_rect, mouse);

		// フォーカス処理
		if (hovering && !m_focused) {
			m_focused = true;
			if (OnFocusEnter) OnFocusEnter();
		}
		else if (hovering && m_focused) {
			if (OnFocusStay) OnFocusStay();
		}
		else if (!hovering && m_focused) {
			m_focused = false;
			if (OnFocusExit) OnFocusExit();
		}

		// クリック処理
		if (hovering && GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
			if (m_onClick) m_onClick();
		}
	}

	void Draw() override {
		if (!m_visible) return;
		int w = m_rect.right - m_rect.left;
		DrawBox(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom,
			m_focused ? GetColor(255, 255, 0) : GetColor(255, 255, 255), TRUE);
		GameData::DrawStringWithAnchor((m_rect.left + m_rect.right) / 2, (m_rect.top + m_rect.bottom) / 2,
			0.5f, 0.5f, GetColor(0, 0, 0), m_text.c_str());
		//DrawString(m_rect.left + m_rect.right / 2, m_rect.top + 5, m_text.c_str(), GetColor(0, 0, 0));
	}

	bool IsVisible() const override { return m_visible; }
	void SetVisible(bool v) { m_visible = v; }
};
