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
	player = std::make_shared<Player>(m_playerData, this);
	m_playerInput = player->GetComponent<InputComponent>();
	m_playerTrans = player->GetComponent<TransformComponent>();

	m_stageManager->m_player = player;
	//Initでm_playerを使用しているのでここで実行
	m_stageManager->Init();

	//出口のドアを作成・表示
	auto door = std::make_shared<ExitDoor>(this, player.get());
	entities.emplace_back(door);

	//ランダムな部屋の位置に出口を作成
	int door_x, door_y;
	auto [door_w, door_h] = door->GetComponent<SpriteRendererComponent>()->GetImageSize();

	m_stageManager->CulculateExitPos(door_w, door_h, door_x, door_y);
	door->SetPos(door_x, door_y);

	int handle_for_size = LoadGraph("Data/GemStone1.png");
	int gem_w, gem_h;
	GetGraphSize(handle_for_size, &gem_w, &gem_h);

	for (int i = 0; i < 5; i++) {
		auto gem = std::make_shared<GemStone>(this, player.get());
		entities.emplace_back(gem);
		int gem_x, gem_y;
		m_stageManager->CulculateExitPosForGem(gem_w, gem_h, gem_x, gem_y);
		gem->SetPos(gem_x, gem_y);
	}

	//UIを作成する
	m_gauge = std::make_shared<UIGauge>();
	m_uiManager->AddElement(m_gauge);
	auto miniMap = std::make_shared<UIMiniMap>(m_stageManager, player, 0, 0);
	m_uiManager->AddElement(miniMap);
	//ミニマップにドアの情報を渡す
	miniMap->SetDoorPos(door->GetDoorXYWH());

	//デバッグ用のワールド罫線　重い処理なのであまり使わないほうがいい
	m_GridLine = std::make_shared<GridLine>(this, player->GetComponent<TransformComponent>().get());
	//entities.emplace_back(m_GridLine);
}

SceneTransition* InGameState::Update(const InputState* input, float deltaTime) {
	IGameState::Update(input, deltaTime);

	//このシーンに存在する全てのEntityの更新をする
	player->Update(input, deltaTime);
	for (auto entity : entities) {
		entity->Update(input, deltaTime);
	}

	m_stageManager->Update(input);

	//ダッシュゲージのデータを渡す
	m_gauge->SetRunningValue(m_playerInput->GetRunningValue());

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