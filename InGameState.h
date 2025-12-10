#pragma once
#include "IGameState.h"
#include "PlayerData.h"
#include "Entity.h"
#include "Player.h"
#include "StageManager.h"
#include "UIGauge.h"
#include "ExitDoor.h"
#include "GridLine.h"
#include "UIMiniMap.h"
#include "GemStone.h"


class GameManager;

class InGameState :public IGameState {
public:
	InGameState(GameManager* gameManager) :IGameState(gameManager)
		, m_gameFontHandle(0), m_gameGraphHandle(0), max_gemNum(10)
	{
	}
	~InGameState()override = default;

	void Init()override;
	void LoadData();
	SceneTransition* Update(const InputState* input, float deltaTime)override;
	void Draw()override;
	void Terminate()override;

	bool LoadPlayer(const std::string& filename);
	void SavePlayer(const PlayerData& player, const std::string& filename);

	TransformComponent* GetPlayerTrans() {
		return m_playerTrans.get();
	}

	float GetPX() {
		return m_playerTrans->x;
	}

	float GetPY() {
		return m_playerTrans->y;
	}

private:
	int m_gameGraphHandle;
	int m_gameFontHandle;
	PlayerData m_playerData;
	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Entity>> entities;
	std::shared_ptr<StageManager> m_stageManager;
	std::shared_ptr<UIGauge> m_gauge;
	std::shared_ptr<InputComponent> m_playerInput;
	std::shared_ptr<TransformComponent> m_playerTrans;
	std::shared_ptr<GridLine> m_GridLine;

	int max_gemNum;
	std::vector<int> gemImages;
	std::vector < std::tuple<float, float>> gem_pos;
};