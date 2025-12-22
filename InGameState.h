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
#include "Easing.h"

class GameManager;

class InGameState :public IGameState {
public:
	InGameState(GameManager* gameManager) :IGameState(gameManager)
		, m_gameFontHandle(0), m_gameGraphHandle(0), max_gemNum(10)
		, nowCollectCount(0), max_enemyNum(5), enemyGenerateTime(5.0f)
		, nowEnemyCount(0), gameStartCountDownTime(3), startCountdownElapsedTime(0)
		, cd_rotRate(0), waitFrame(5), nowWaitFrame(0), emptyImageScale(0.2f)
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
	void GenerateEnemy();

	TransformComponent* GetPlayerTrans() {
		return m_playerTrans.get();
	}

	float GetPX() {
		return m_playerTrans->x;
	}

	float GetPY() {
		return m_playerTrans->y;
	}

	//お宝関係
	void OnGemStoneHit(int id);
	void DeleteGemPos(int id);

	//敵関係
	void OnEnemyHit(int id);

	//ドア関係
	void OnDoorHit();

	//透明化ボタンの関数
	void OnInvisibleButtonPressed();
	void OnInvisibleButtonFinished();

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
	std::shared_ptr<UIButton> invisibleButton;
	float startCount;		//クリアまで何秒かかったかの計測用

	//宝集め関係
	int max_gemNum;
	std::vector<int> gemImages;
	std::vector<int> gainedGems;
	int nowGainedGemIndex;
	int gemEmptyImage;
	float width_gemEmptyImage;
	float height_gemEmptyImage;
	float emptyImageScale;
	std::vector < std::tuple<int, float, float>> gem_pos;
	std::vector<int> deletePendingEntities;
	std::vector<int> deletedGemPos;
	int nowCollectCount;

	//敵関係
	int enemy_w, enemy_h;
	int max_enemyNum;
	float enemyGenerateTime;	//何秒ごとに敵が生成されるか
	float nowElapsedTimeForEnemy;
	int nowEnemyCount;
	std::vector < std::tuple<int, float, float>> enemy_pos;
	std::vector<int> enemyRoomIndex;	//敵の配置された部屋のインデックス


	//スタート時のカウントダウン用
	float gameStartCountDownTime;		//何秒のカウントダウンにするか
	float startCountdownElapsedTime;	//0秒から何秒カウントダウンしているか
	float cd_rotRate;
	int waitFrame;						//最初に数フレーム待たせないといけないので
	int nowWaitFrame;
	int cd_w = 200;
	int cd_h = 100;
	int cd_x = GameData::windowWidth / 2;
	int cd_y = GameData::windowHeight / 2;
	float rotSpeed = 2.0f;

	//透明化ボタン用
	float reuseInvisibleCount = 3.0f;	//3秒まったらもう一度使える
	float invisibleElapsedTime = 0.0f;

};