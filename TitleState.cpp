#include "GameManager.h"
#include "TitleState.h"
#include "InGameState.h"
#include "DxLib.h"

#include <Windows.h>

void TitleState::Init() {
	SetBackgroundColor(100, 100, 100);
	//ご使用のパソコンに一時的にFontを読み込ませる
	AddFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
	m_titleFontHandle = CreateFontToHandle("YDW あおさぎ R", 30, 3);

	//UIを作成する
	auto button = std::make_shared<UIButton>(
		GameData::windowWidth / 2, GameData::windowHeight * 0.8f, 200, 100,
		"Start",
		[this]() {
			moveState = true;
		});

	//button->OnFocusEnter = [button]() {button->SetColor(GetColor(50, 50, 50)); };
	m_uiManager->AddElement(button);
}

SceneTransition* TitleState::Update(const InputState* input, float deltaTime) {
	IGameState::Update(input, deltaTime);

	//Spaceを押したときはゲームシーンへ移行する
	if (input->IsKeyDown(KEY_INPUT_SPACE) || moveState) {
		SceneTransition* trans = new SceneTransition{ TransitionType::Change,
			std::make_unique<InGameState>(m_gameManager) };
		return trans;
	}

	SceneTransition* trans = new SceneTransition{ TransitionType::None, nullptr };
	return trans;
}

void TitleState::Draw() {
	SetFontSize(30);
	GameData::DrawStringWithAnchor(GameData::windowWidth / 2, GameData::windowHeight / 2,
		0.5f, 0.5f, GetColor(255, 255, 255), "Space : Game Start");

	IGameState::Draw();
}

void TitleState::Terminate() {
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
}