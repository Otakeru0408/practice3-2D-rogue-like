#include "InGameState.h"
#include "GameManager.h"
#include "ResultState.h"
#include "DxLib.h"

void InGameState::Init() {
	SetBackgroundColor(200, 200, 200);
	//ご使用のパソコンに一時的にFontを読み込ませる
	AddFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
	m_gameFontHandle = CreateFontToHandle("YDW あおさぎ R", 25, 3);


	//プレイヤーのデータを設定
	bool isLoaded = LoadPlayer("Data/savedata1.csv");
	if (!isLoaded) {
		m_playerData = { "Hero",100,10,GameData::windowWidth / 2,GameData::windowHeight / 2 };
	}
}

SceneTransition* InGameState::Update(const InputState* input, float deltaTime) {
	IGameState::Update(input, deltaTime);

	//Spaceを押したときはゲームシーンへ移行する
	if (input->IsKeyDown(KEY_INPUT_SPACE)) {
		SceneTransition* trans = new SceneTransition{ TransitionType::Change,
			std::make_unique<ResultState>(m_gameManager) };
		return trans;
	}

	SceneTransition* trans = new SceneTransition{ TransitionType::None, nullptr };
	return trans;
}

void InGameState::Draw() {
	GameData::DrawStringWithAnchor(100, GameData::windowHeight / 2, 0, 0.5f,
		GetColor(0, 0, 0), m_gameFontHandle, "Press Space \nto See Result");

	GameData::DrawStringWithAnchor(GameData::windowWidth / 2, GameData::windowHeight / 2, 0.5f, 0.5f,
		GetColor(0, 0, 0), "name:%s,HP:%d", m_playerData.name.c_str(), m_playerData.hp);

	IGameState::Draw();
}

void InGameState::Terminate() {
	//読み込ませたフォントを開放する
	RemoveFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
}

//プレイヤーのデータを保存する
bool InGameState::LoadPlayer(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "読み込み失敗: " << filename << std::endl;
		return false;
	}
	std::string line;
	std::getline(file, line);
	m_playerData = PlayerData::FromCSV(line);
	return true;
}

//プレイヤーのデータをよみこむ
void InGameState::SavePlayer(const PlayerData& player, const std::string& filename) {
	std::ofstream file(filename);
	if (!file) {
		std::cerr << "保存失敗: " << filename << std::endl;
		return;
	}
	file << player.ToCSV() << "\n";
}