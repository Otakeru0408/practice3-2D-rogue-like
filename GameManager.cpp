#include "GameManager.h"
#include "DxLib.h"
#include "TitleState.h"
#include "InGameState.h"
#include "ResultState.h"

GameManager::GameManager() : m_currentState(nullptr), m_isGameFinished(false) {
	memset(m_inputState.key, 0, sizeof(m_inputState.key));
	memset(m_inputState.prevKey, 0, sizeof(m_inputState.prevKey));
}

GameManager::~GameManager() {

}

void GameManager::Initialize() {
	if (DxLib_Init() == -1) {
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	ChangeState(std::make_unique<TitleState>(this));

	if (m_currentState) {
		m_currentState->Init();
	}

	m_prevTime = GetNowCount();
}

void GameManager::Update() {
	if (ProcessMessage() != 0) {
		return;
	}

	UpdateInputState();

	//deltaTimeの計算
	float nowTime = GetNowCount();
	float deltaTime = (nowTime - m_prevTime) / 1000.0f;
	m_prevTime = nowTime;

	if (m_currentState) {
		m_currentState->Update(&m_inputState, deltaTime);
	}
}

void GameManager::Draw() {
	ClearDrawScreen();

	if (m_currentState) {
		m_currentState->Draw();
	}

	ScreenFlip();
}

void GameManager::ChangeState(std::unique_ptr<IGameState> newState) {
	if (m_currentState) {
		m_currentState->Terminate();
	}

	m_currentState = std::move(newState);
	if (m_currentState) {
		m_currentState->Init();
	}
}

void GameManager::Finalize() {
	if (m_currentState) {
		m_currentState->Terminate();
	}

	m_currentState.reset();

	DxLib_End();
}

void GameManager::UpdateInputState()
{
	// 現在のキー状態を前フレームのキー状態として保存
	memcpy(m_inputState.prevKey, m_inputState.key, sizeof(m_inputState.key));

	char tmpKey[256];
	// Dxライブラリから現在のキー状態を取得
	GetHitKeyStateAll(tmpKey);
	for (int i = 0; i < 256; i++) {
		if (tmpKey[i] != 0) {
			m_inputState.key[i]++;
		}
		else {
			m_inputState.key[i] = 0;
		}
	}

	// TODO: マウスやゲームパッドの入力もここに追加
}