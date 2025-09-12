#pragma once
#include <memory>
#include "IGameState.h"
#include "InputState.h"

class TitleState;
class InGameState;
class ResultState;

class GameManager{
	public:
	GameManager();
	~GameManager();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void ChangeState(std::unique_ptr<IGameState> newState);

	// ★追加: ゲームが終了状態かを確認するメソッド
	bool IsGameFinished() const
	{
		return m_isGameFinished;
	}
	// ★追加: ゲーム終了フラグをセットするメソッド
	void RequestGameFinish()
	{
		m_isGameFinished = true;
	}

	private:
	std::unique_ptr<IGameState> m_currentState;
	bool m_isGameFinished; // ★追加: ゲーム終了フラグ
	InputState m_inputState;

	void UpdateInputState();
};