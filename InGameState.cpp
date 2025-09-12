#include "InGameState.h"
#include "GameManager.h"
#include "ResultState.h"
#include "DxLib.h"

void InGameState::Init(){
	SetBackgroundColor(0, 150, 0);
	//ご使用のパソコンに一時的にFontを読み込ませる
	AddFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
	m_gameFontHandle = CreateFontToHandle("YDW あおさぎ R", 25, 3);
}

void InGameState::Update(const InputState* input){
	// キー入力検出
	if (input->IsKeyDown(KEY_INPUT_SPACE)) {
		// スペースキーまたは左クリックでゲーム開始フェーズへ遷移
		m_gameManager->ChangeState(std::make_unique<ResultState>(m_gameManager));
	}
}

void InGameState::Draw(){
	SetStringAlign(10, 20, "Press Space \nto See Result", GetColor(255, 255, 0), 0, 0,m_gameFontHandle);
}

void InGameState::Terminate(){
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
}