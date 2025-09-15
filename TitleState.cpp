#include "GameManager.h"
#include "TitleState.h"
#include "InGameState.h"
#include "DxLib.h"

#include <Windows.h>

void TitleState::Init() {
	SetBackgroundColor(100, 100, 100);
	//ご使用のパソコンに一時的にFontを読み込ませる
	AddFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
	m_titleFontHandle = CreateFontToHandle("YDW あおさぎ R", 100, 30);



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
	SetFontSize(60);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);

	//角度を計算
	float angle = DX_PI_F / 180.0f * -30.0f;

	//表示座標を中心にするため計算
	float nx =
		DrawRotaStringToHandle(50, GameData::windowHeight / 2, 1.0f, 1.0f, 1.0f, 1.0f, angle,
			GetColor(255, 255, 255), m_titleFontHandle, 0, 0, "Rogue like Game");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	IGameState::Draw();
}

void TitleState::Terminate() {
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
}