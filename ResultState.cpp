#include "GameManager.h"
#include "DxLib.h"
#include "ResultState.h"

void ResultState::Init(){
	//様々なもののロード
	SetBackgroundColor(150,0,0);
	//ご使用のパソコンに一時的にFontを読み込ませる
	AddFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
	m_resultFontHandle = CreateFontToHandle("YDW あおさぎ R", 20, 5);
}

void ResultState::Update(const InputState* input){
	if(input->IsKeyStay(KEY_INPUT_ESCAPE)|| input->IsKeyStay(KEY_INPUT_RETURN)){
		m_gameManager->RequestGameFinish();
	}
}

void ResultState::Draw(){
	SetStringAlign(30, 400, "Your Rank : A", GetColor(0, 255, 0), 0, 0.5f,m_resultFontHandle);
	SetStringAlign(30, 440, "Press Escape or Enter \nto Terminate Game",
		GetColor(0, 255, 0), 0, 0.5f,m_resultFontHandle);
}

void ResultState::Terminate(){
	//様々なもののアンロード
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
}