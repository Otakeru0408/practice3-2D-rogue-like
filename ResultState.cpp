#include "GameManager.h"
#include "DxLib.h"
#include "ResultState.h"
#include "InGameState.h"

void ResultState::Init() {
	//様々なもののロード
	SetBackgroundColor(100, 150, 255);
	//ご使用のパソコンに一時的にFontを読み込ませる
	AddFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
	m_resultFontHandle = CreateFontToHandle("YDW あおさぎ R", 30, 5);

	//UIを作成する
	auto button = std::make_shared<UIButton>(
		GameData::windowWidth / 2, GameData::windowHeight * 0.8f, 200, 100,
		"Retry",
		[this]() {
			isClick = true;
		});
	button->SetUnSelectedColor(GetColor(100, 150, 255));
	button->SetSelectedColor(GetColor(80, 130, 235));

	button->SetOnHoverCallBack(
		[this]() {
			isHovering = true;
		});
	button->SetOnClickReleaseCallBack(
		[this]() {
			moveState = true;
		}
	);

	m_uiManager->AddElement(button);

	//結果を取得する
	resultData = m_gameManager->resultData;
	resultText = resultData.isClear ? "Game Clear !" : "Game Over...";
	resultColor = resultData.isClear ? GetColor(255, 255, 100) : GetColor(0, 0, 0);
}

SceneTransition* ResultState::Update(const InputState* input, float deltaTime) {
	isHovering = false;
	isClick = false;
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
	startX = 50;
	startY = GameData::windowHeight / 2;
	SetFontSize(100);
	GameData::DrawStringWithAnchor(55, 55, 0, 0.0f, GetColor(255, 255, 255),
		"YOUR Result:");
	GameData::DrawStringWithAnchor(50, 50, 0, 0.0f, GetColor(0, 0, 0),
		"YOUR Result:");

	DrawBox(480, startY + 25, 590, startY + 75, GetColor(255, 255, 255), TRUE);
	DrawBox(430, startY + 80, 590, startY + 130, GetColor(255, 255, 255), TRUE);

	SetFontSize(70);
	GameData::DrawStringWithAnchor(startX, startY - 100, 0, 0.5f, resultColor,
		resultText.c_str());

	SetFontSize(50);
	GameData::DrawStringWithAnchor(startX, startY + 50, 0, 0.5f, GetColor(0, 0, 0),
		"Number of Gems : %d", resultData.numOfGems);
	GameData::DrawStringWithAnchor(startX, startY + 105, 0, 0.5f, GetColor(0, 0, 0),
		"Elapsed Time : %.3f s.", resultData.elapsedTime);

	IGameState::Draw();
	int diff = 10;

	//ボタンのアニメーション
	if (isHovering) {
		if (isClick) {
			DrawBox(GameData::windowWidth / 2 - 100, GameData::windowHeight * 0.8f - 50,
				GameData::windowWidth / 2 + 100, GameData::windowHeight * 0.8f + 50,
				GetColor(0, 0, 0), false);
		}
		DrawBox(GameData::windowWidth / 2 - 100 - diff, GameData::windowHeight * 0.8f - 50 - diff,
			GameData::windowWidth / 2 + 100 - diff, GameData::windowHeight * 0.8f + 50 - diff,
			GetColor(0, 0, 0), false);
	}
	else {
		DrawBox(GameData::windowWidth / 2 - 100, GameData::windowHeight * 0.8f - 50,
			GameData::windowWidth / 2 + 100, GameData::windowHeight * 0.8f + 50,
			GetColor(0, 0, 0), false);
	}
}

void ResultState::Terminate() {
	//様々なもののアンロード
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
}
