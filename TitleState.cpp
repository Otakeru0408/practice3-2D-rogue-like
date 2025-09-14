#include "GameManager.h"
#include "TitleState.h"
#include "InGameState.h"
#include "DxLib.h"
#include <Windows.h>

void TitleState::Init() {
	SetBackgroundColor(0, 0, 150);
	//ご使用のパソコンに一時的にFontを読み込ませる
	AddFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
	m_titleFontHandle = CreateFontToHandle("YDW あおさぎ R", 30, 3);
}

void TitleState::Update(const InputState* input, float deltaTime) {
	// キー入力検出
	if (input->IsKeyDown(KEY_INPUT_SPACE)) {
		// スペースキーまたは左クリックでゲーム開始フェーズへ遷移
		m_gameManager->ChangeState(std::make_unique<InGameState>(m_gameManager));
	}
}

void TitleState::Draw() {
	SetFontSize(30);
	GameData::DrawStringWithAnchor(GameData::windowWidth / 2, GameData::windowHeight / 2,
		0.5f, 0.5f, GetColor(255, 255, 255), "Space : Game Start");
}

void TitleState::Terminate() {
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
}