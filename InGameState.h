#pragma once
#include "IGameState.h"
#include "PlayerData.h"
#include "Entity.h"
#include "Player.h"
#include "StageManager.h"
#include "UIGauge.h"
#include "ExitDoor.h"

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
	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Entity>> entities;
	std::shared_ptr<StageManager> m_stageManager;
	std::shared_ptr<UIGauge> m_gauge;
	std::shared_ptr<InputComponent> m_playerInput;
};