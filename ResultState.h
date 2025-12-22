#pragma once
#include "IGameState.h"
#include "InGameState.h"

#include <string>

class GameManager;

class ResultState : public IGameState {
public:
	ResultState(GameManager* gameManager) :IGameState(gameManager), m_resultFontHandle(0)
		, startX(0), startY(0), isHovering(false), isClick(false), resultColor(0)
	{
	}
	~ResultState() override = default;

	void Init()override;
	SceneTransition* Update(const InputState* input, float deltaTime)override;
	void Draw()override;
	void Terminate()override;

private:
	int m_resultFontHandle;
	ResultData resultData;
	std::string resultText;
	int startX, startY;
	bool isHovering;
	bool isClick;
	int resultColor;
};