#include "InGameState.h"
#include "GameManager.h"
#include "ResultState.h"
#include "DxLib.h"

void InGameState::Init() {
	SetBackgroundColor(200, 200, 200);
	//ご使用のパソコンに一時的にFontを読み込ませる
	AddFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
	m_gameFontHandle = CreateFontToHandle("YDW あおさぎ R", 25, 3);

	m_stageManager = std::make_shared<StageManager>(GameData::windowWidth, GameData::windowHeight);

	//プレイヤーのデータを設定
	bool isLoaded = LoadPlayer("Data/savedata1.csv");
	if (!isLoaded) {
		m_playerData.hp = 100;
		m_playerData.mp = 100;
		m_playerData.posX = GameData::windowWidth / 2;
		m_playerData.posY = GameData::windowHeight / 2;
	}

	//PlayerをPlayerDataから作成する
	player = std::make_shared<Player>(m_playerData);

	m_stageManager->m_player = player;
	//Initでm_playerを使用しているのでここで実行
	m_stageManager->Init();
	//UIを作成する
	/*
	auto button = std::make_shared<UIButton>(
		100, 100, 200, 100,
		"Start",
		[this]() {
			moveState = true;
		});

	m_uiManager->AddElement(button);
	*/
}

SceneTransition* InGameState::Update(const InputState* input, float deltaTime) {
	IGameState::Update(input, deltaTime);

	//このシーンに存在する全てのEntityの更新をする
	player->Update(input, deltaTime);
	for (auto entity : entities) {
		entity->Update(input, deltaTime);
	}

	//m_stageManager->Update();

	//Spaceを押したときはゲームシーンへ移行する
	if (moveState || input->IsKeyDown(KEY_INPUT_SPACE)) {
		SceneTransition* trans = new SceneTransition{ TransitionType::Change,
			std::make_unique<ResultState>(m_gameManager) };
		return trans;
	}

	SceneTransition* trans = new SceneTransition{ TransitionType::None, nullptr };
	return trans;
}

void InGameState::Draw() {

	m_stageManager->Draw();

	player->Draw();
	for (auto entity : entities) {
		entity->Draw();
	}
	//UIを表示するので最後に表示
	IGameState::Draw();
}

void InGameState::Terminate() {
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);

	if (player) {
		m_playerData = player->SavePlayerData();
	}
	SavePlayer(m_playerData, "Data/savedata1.csv");
}

//プレイヤーのデータをよみこむ
bool InGameState::LoadPlayer(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "読み込み失敗: " << filename << std::endl;
		file.close();
		return false;
	}
	std::string line;
	std::getline(file, line);
	m_playerData = PlayerData::FromCSV(line);
	file.close();
	return true;
}

//プレイヤーのデータを保存する
void InGameState::SavePlayer(const PlayerData& player, const std::string& filename) {
	std::ofstream file(filename);
	if (!file) {
		std::cerr << "保存失敗: " << filename << std::endl;
		return;
	}
	file << player.ToCSV() << "\n";
	file.close();
}