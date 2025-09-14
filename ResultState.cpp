#include "GameManager.h"
#include "DxLib.h"
#include "ResultState.h"

void ResultState::Init() {
	//様々なもののロード
	SetBackgroundColor(150, 0, 0);
	//ご使用のパソコンに一時的にFontを読み込ませる
	AddFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
	m_resultFontHandle = CreateFontToHandle("YDW あおさぎ R", 20, 5);
}

void ResultState::Update(const InputState* input, float deltaTime) {
	if (input->IsKeyStay(KEY_INPUT_ESCAPE) || input->IsKeyStay(KEY_INPUT_RETURN)) {
		m_gameManager->RequestGameFinish();
	}
}

void ResultState::Draw() {
	GameData::DrawStringWithAnchor(GameData::windowWidth / 2, 100, 0.5f, 0.5f,
		GetColor(255, 255, 255), m_resultFontHandle, "Press Escape to terminate game");
}

void ResultState::Terminate() {
	//様々なもののアンロード
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
}