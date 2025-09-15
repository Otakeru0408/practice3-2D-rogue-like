#include "GameManager.h"
#include "DxLib.h"
#include "TitleState.h"
#include "InGameState.h"
#include "ResultState.h"

GameManager::GameManager() :m_isGameFinished(false) {
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

	//ChangeState(std::make_unique<TitleState>(this));
	PushState(std::make_unique<TitleState>(this));
	currentState()->Init();

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

	//SceneのUpdate
	SceneTransition* transition = m_currentState.top()->Update(&m_inputState, deltaTime);
	if (transition->type == TransitionType::Change) {
		m_currentState.top() = std::move(transition->nextState);
		m_currentState.top()->Init();
	}
	else if (transition->type == TransitionType::Popup) {
		PushState(std::move(transition->nextState));
		currentState()->Init();
	}
	else if (transition->type == TransitionType::Resume) {
		if (!m_currentState.empty())
			PopState();
		//Initializeはしない。画面が元に戻る
	}

	//デバッグ用
	if (m_inputState.IsKeyStay(KEY_INPUT_LSHIFT) && m_inputState.IsKeyDown(KEY_INPUT_1)) {
		isDisplayMousePos = !isDisplayMousePos;
	}
}

void GameManager::Draw() {
	ClearDrawScreen();

	if (!m_currentState.empty())m_currentState.top()->Draw();
	else {
		DrawString(100, 100, "current Scene not exist", GetColor(0, 0, 0));
	}

	if (isDisplayMousePos) {
		int x, y;
		GetMousePoint(&x, &y);
		DrawFormatString(10, 10, GetColor(0, 0, 0), "x:%d y:%d", x, y);
	}


	ScreenFlip();
}

void GameManager::Finalize() {
	m_currentState.top()->Terminate();
	while (!m_currentState.empty()) {
		m_currentState.pop();
	}

	DxLib_End();
}

void GameManager::UpdateInputState()
{
	// 現在のキー状態を前フレームのキー状態として保存
	memcpy(m_inputState.prevKey, m_inputState.key, sizeof(m_inputState.key));
	memcpy(m_inputState.prevMouseVal, m_inputState.mouseVal, sizeof(m_inputState.mouseVal));

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
	int input = GetMouseInput();

	//左クリック
	if ((input & MOUSE_INPUT_LEFT) != 0) {
		m_inputState.mouseVal[0]++;
	}
	else {
		m_inputState.mouseVal[0] = 0;
	}
	//右クリック
	if ((input & MOUSE_INPUT_RIGHT) != 0) {
		m_inputState.mouseVal[1]++;
	}
	else {
		m_inputState.mouseVal[1] = 0;
	}
	//真ん中クリックはいらない？

	//もしどこかのマウスボタンが押されていたら
	if (input != 0) {
		m_inputState.mousePressed = true;
		int x, y;
		GetMousePoint(&x, &y);
		m_inputState.mouseX = x;
		m_inputState.mouseY = y;
	}
	else {
		m_inputState.mousePressed = false;
	}
}