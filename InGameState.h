#pragma once
#include "IGameState.h"
#include "PlayerData.h"

class GameManager;

class InGameState :public IGameState {
public:
	InGameState(GameManager* gameManager) :IGameState(gameManager)
		, m_gameFontHandle(0), m_gameGraphHandle(0) {
	}
	~InGameState()override = default;

	void Init()override;
	SceneTransition* Update(const InputState* input, float deltaTime)override;
	void Draw()override;
	void Terminate()override;

	bool LoadPlayer(const std::string& filename);
	void SavePlayer(const PlayerData& player, const std::string& filename);

private:
	int m_gameGraphHandle;
	int m_gameFontHandle;
	PlayerData m_playerData;
};