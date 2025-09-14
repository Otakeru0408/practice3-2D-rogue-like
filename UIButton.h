#pragma once
#include "DxLib.h"
#include "IUIElement.h"
#include "GameData.h"

#include <string>

class UIButton : public IUIElement {
private:
	RECT m_rect;
	int nowColor = GetColor(255, 255, 255);
	int unSelectedColor = GetColor(255, 255, 255);
	int selectedColor = GetColor(200, 200, 200);
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

		int x, y;
		GetMousePoint(&x, &y);

		bool hovering = HitTest(x, y);

		//ボタンの色を更新する
		nowColor = hovering ? selectedColor : unSelectedColor;

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
			nowColor, TRUE);
		GameData::DrawStringWithAnchor((m_rect.left + m_rect.right) / 2, (m_rect.top + m_rect.bottom) / 2,
			0.5f, 0.5f, GetColor(0, 0, 0), m_text.c_str());
	}

	bool IsVisible() const override { return m_visible; }
	void SetVisible(bool v) { m_visible = v; }
	void SetColor(int c) { nowColor = c; }
	void SetSelectedColor(int c) { selectedColor = c; }
	void SetUnSelectedColor(int c) { unSelectedColor = c; }

	bool HitTest(int mx, int my) {
		return (m_rect.left <= mx && m_rect.right >= mx && m_rect.top <= my && m_rect.bottom >= my);
	}
};
