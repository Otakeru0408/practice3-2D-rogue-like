#pragma once
#include "IGameState.h"

class GameManager;

class ResultState : public IGameState {
public:
	ResultState(GameManager* gameManager) :IGameState(gameManager) {}
	~ResultState() override = default;

	void Init()override;
	void Update(const InputState* input, float deltaTime)override;
	void Draw()override;
	void Terminate()override;

private:
	int m_resultFontHandle;
};