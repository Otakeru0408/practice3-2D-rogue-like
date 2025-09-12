#pragma once
#include "IGameState.h"

class GameManager;

class InGameState:public IGameState{
	public:
	InGameState(GameManager* gameManager):IGameState(gameManager){}
	~InGameState()override = default;

	void Init()override;
	void Update(const InputState* input)override;
	void Draw()override;
	void Terminate()override;

	private:
	int m_gameGraphHandle;
	int m_gameFontHandle;
};