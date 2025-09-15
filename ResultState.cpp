#include "GameManager.h"
#include "DxLib.h"
#include "ResultState.h"
#include "InGameState.h"

void ResultState::Init() {
	//様々なもののロード
	SetBackgroundColor(255, 255, 255);
	//ご使用のパソコンに一時的にFontを読み込ませる
	AddFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
	m_resultFontHandle = CreateFontToHandle("YDW あおさぎ R", 30, 5);

	//UIを作成する
	auto button = std::make_shared<UIButton>(
		GameData::windowWidth / 2, GameData::windowHeight * 0.8f, 200, 100,
		"Start",
		[this]() {
			moveState = true;
		});

	m_uiManager->AddElement(button);
}

SceneTransition* ResultState::Update(const InputState* input, float deltaTime) {
	IGameState::Update(input, deltaTime);

	//Spaceを押したときはゲームシーンへ移行する
	if (moveState) {
		SceneTransition* trans = new SceneTransition{ TransitionType::Change,
			std::make_unique<InGameState>(m_gameManager) };
		return trans;
	}

	SceneTransition* trans = new SceneTransition{ TransitionType::None, nullptr };
	return trans;
}

void ResultState::Draw() {
	GameData::DrawStringWithAnchor(50, 100, 0, 0.5f,
		GetColor(0, 0, 0), m_resultFontHandle, "Escape : terminate game");

	IGameState::Draw();
}

void ResultState::Terminate() {
	//様々なもののアンロード
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
}