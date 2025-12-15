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
#include "Enemy.h"

class GameManager;

class InGameState :public IGameState {
public:
	InGameState(GameManager* gameManager) :IGameState(gameManager)
		, m_gameFontHandle(0), m_gameGraphHandle(0), max_gemNum(10)
		, nowCollectCount(0), max_enemyNum(5), enemyGenerateTime(5.0f)
		, nowEnemyCount(0)
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

	//‚¨•óŠÖŒW
	void OnGemStoneHit(int id);
	void DeleteGemPos(int id);

	//“GŠÖŒW
	void OnEnemyHit(int id);

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

	//•óW‚ßŠÖŒW
	int max_gemNum;
	std::vector<int> gemImages;
	std::vector < std::tuple<int, float, float>> gem_pos;
	std::vector<int> deletePendingEntities;
	std::vector<int> deletedGemPos;
	int nowCollectCount;

	//“GŠÖŒW
	int max_enemyNum;
	float enemyGenerateTime;	//‰½•b‚²‚Æ‚É“G‚ª¶¬‚³‚ê‚é‚©
	int nowEnemyCount;
	std::vector < std::tuple<int, float, float>> enemy_pos;
};