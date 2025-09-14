#pragma once
#include "DxLib.h"
#include "GameData.h"
#include <string>

#include <cstdarg>

class GameManager;
struct InputState;

class IGameState {
public:
	IGameState(GameManager* gameManager) : m_gameManager(gameManager) {}

	virtual ~IGameState() = default;

	virtual void Init() = 0;
	virtual void Update(const InputState* input, float deltaTime) = 0;
	virtual void Draw() = 0;
	virtual void Terminate() = 0;

protected:
	GameManager* m_gameManager;
};
