#include "InGameState.h"
#include "GameManager.h"
#include "ResultState.h"
#include "DxLib.h"

void InGameState::Init() {
	//最初にデータのロードを行う
	LoadData();

	m_stageManager = std::make_shared<StageManager>(GameData::windowWidth, GameData::windowHeight);

	//プレイヤーのデータを設定
	//TODO : この機能がいるかどうか検討
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

	//敵を生成
	int enemy_handle = LoadGraph("Data/Rogue-Enemy1.png");
	GetGraphSize(enemy_handle, &enemy_w, &enemy_h);
	GenerateEnemy();		//まずは1体生成する

	//出口のドアを作成・表示
	auto door = std::make_shared<ExitDoor>(this, player.get());
	entities.emplace_back(door);

	//ランダムな部屋の位置に出口を作成
	int door_x, door_y;
	auto [door_w, door_h] = door->GetComponent<SpriteRendererComponent>()->GetImageSize();

	m_stageManager->CulculateRandomEntityPos(door_w, door_h, door_x, door_y);
	door->SetPos(door_x, door_y);

	//gemの生成をする
	int handle_for_size = LoadGraph("Data/GemStone1.png");
	int gem_w, gem_h;
	GetGraphSize(handle_for_size, &gem_w, &gem_h);

	//宝をmax_gemNum分生成する
	for (int i = 0; i < max_gemNum; i++) {
		auto gem = std::make_shared<GemStone>(this, player.get(), gemImages[i % 5], i);
		entities.emplace_back(gem);
		int gem_x, gem_y;
		float scale = gem->GetImageScale();
		//ランダムな部屋に配置する
		m_stageManager->CulculateRandomEntityPos(gem_w * scale, gem_h * scale, gem_x, gem_y);
		gem->SetPos(gem_x, gem_y);
		//gemの位置を保存する
		gem_pos.emplace_back(std::make_tuple(i, gem_x, gem_y));
	}

	//UIを作成する
	m_gauge = std::make_shared<UIGauge>();
	m_uiManager->AddElement(m_gauge);
	auto miniMap = std::make_shared<UIMiniMap>(m_stageManager, player, 0, 0, gem_pos, enemy_pos);
	m_uiManager->AddElement(miniMap);
	//ミニマップにドアの情報を渡す
	miniMap->SetDoorPos(door->GetDoorXYWH());

	//デバッグ用のワールド罫線　重い処理なのであまり使わないほうがいい
	m_GridLine = std::make_shared<GridLine>(this, player->GetComponent<TransformComponent>().get());
	//entities.emplace_back(m_GridLine);
}

void InGameState::LoadData() {
	SetBackgroundColor(200, 200, 200);
	//ご使用のパソコンに一時的にFontを読み込ませる
	AddFontResourceEx("Data/YDWaosagi.otf", FR_PRIVATE, 0);
	m_gameFontHandle = CreateFontToHandle("YDW あおさぎ R", 25, 3);

	int handle = 0;
	for (int i = 1; i <= 5; i++) {
		std::string fileName = "Data/gemStone" + std::to_string(i) + ".png";


		handle = LoadGraph(fileName.c_str());
		if (handle != 0) {
			gemImages.emplace_back(handle);
		}
		handle = 0;
	}

}

SceneTransition* InGameState::Update(const InputState* input, float deltaTime) {
	//最初にカウントダウンをする
	if (startCountdownElapsedTime <= gameStartCountDownTime) {
		//ただし最初に数フレーム待つべき
		if (nowWaitFrame <= waitFrame) {
			nowWaitFrame++;
		}
		else {
			//数フレーム待ったらカウントダウン開始
			startCountdownElapsedTime += deltaTime;
		}
		return new SceneTransition{ TransitionType::None, nullptr };
	}
	//適切なタイミングで敵を随時生成する
	nowElapsedTimeForEnemy += deltaTime;
	if (nowElapsedTimeForEnemy >= enemyGenerateTime
		&& nowEnemyCount < max_enemyNum) {
		nowElapsedTimeForEnemy = 0.0f;
		GenerateEnemy();
	}

	IGameState::Update(input, deltaTime);

	//このシーンに存在する全てのEntityの更新をする
	player->Update(input, deltaTime);
	for (auto entity : entities) {
		entity->Update(input, deltaTime);
	}
	//もしも削除するものがあったら削除する
	std::sort(deletePendingEntities.rbegin(), deletePendingEntities.rend());
	for (int id : deletePendingEntities) {

		entities.erase(entities.begin() + id);
		nowCollectCount++;
	}
	deletePendingEntities.clear();
	//削除されたGemのPosを削除する→MiniMap反映用
	for (int id : deletedGemPos) {
		DeleteGemPos(id);
	}
	deletedGemPos.clear();

	//敵の位置情報を更新する
	for (auto entity : entities) {
		if (Enemy* enemy = dynamic_cast<Enemy*>(entity.get())) {
			for (auto& [id, x, y] : enemy_pos) {
				if (id == enemy->GetMyId()) {
					std::tie(x, y) = enemy->GetPos();
				}
			}
		}
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

	//その他こまごましたUIを表示する
	int margin = 10;
	int height = 60;
	int width = 600;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	SetFontSize(height);
	DrawBox(0, GameData::windowHeight - (height + margin),
		width + margin, GameData::windowHeight + (height + margin), GetColor(255, 255, 255), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(0, GameData::windowHeight - (height + margin),
		width + margin, GameData::windowHeight + (height + margin), GetColor(0, 0, 0), false);
	DrawFormatString(margin, GameData::windowHeight - height, GetColor(0, 0, 0), "Collect Now : %d/%d", nowCollectCount, max_gemNum);


	//最初のカウントダウン中なら
	if (startCountdownElapsedTime <= gameStartCountDownTime) {
		//最初に数フレーム待たせる必要がある→カウントダウンの数値が早すぎる
		if (nowWaitFrame <= waitFrame) {
			return;
		}
		SetFontSize(500);
		//cd_w = GetDrawStringWidthToHandle("0", lstrlen("0"), m_gameFontHandle);

		//文字を回転させる 小数点以下を取り出すことで、各文字の0.0～0.9を取り出せる
		float oneCharSize = (startCountdownElapsedTime - std::floor(startCountdownElapsedTime));
		cd_rotRate = oneCharSize * rotSpeed;
		if (cd_rotRate >= 1.0f)cd_rotRate = 1.0f;
		float cd_rotAngle = Easing::Calculate(
			DX_PI_F / 6.0, 2 * DX_PI_F + DX_PI_F / 6.0, cd_rotRate, EaseType::EaseIn, 2.0f
		);
		float cd_Scale = Easing::Calculate(
			0.0f, 1.0f, cd_rotRate, EaseType::EaseIn
		);

		DrawRotaFormatString(cd_x + 10, cd_y + 10, cd_Scale, cd_Scale, 100, 250,
			cd_rotAngle, GetColor(0, 0, 0), 0, 0,
			"%d", (int)(gameStartCountDownTime - startCountdownElapsedTime) + 1);
		DrawRotaFormatString(cd_x, cd_y, cd_Scale, cd_Scale, 100, 250,
			cd_rotAngle, GetColor(200, 0, 0), 0, 0,
			"%d", (int)(gameStartCountDownTime - startCountdownElapsedTime) + 1);
	}
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

void InGameState::OnGemStoneHit(int id) {
	deletedGemPos.emplace_back(id);
	for (int i = 0; i < entities.size(); i++) {
		if (std::shared_ptr<GemStone> g = std::dynamic_pointer_cast<GemStone>(entities[i])) {
			if (g->GetMyId() == id) {
				deletePendingEntities.emplace_back(i);

				return;
			}
		}
	}
}

void InGameState::DeleteGemPos(int id) {
	gem_pos.erase(
		std::remove_if(
			gem_pos.begin(),
			gem_pos.end(),
			[id](const std::tuple<int, float, float>& t) {
				return std::get<0>(t) == id;  // 1番目の要素（int）が一致したら削除対象
			}
		),
		gem_pos.end()
	);
}

void InGameState::OnEnemyHit(int id) {
	for (int i = 0; i < entities.size(); i++) {
		if (std::shared_ptr<Enemy> g = std::dynamic_pointer_cast<Enemy>(entities[i])) {
			if (g->GetMyId() == id) {
				moveState = true;
				return;
			}
		}
	}
}

void InGameState::GenerateEnemy() {
	int enemy_x, enemy_y;
	auto enemy = std::make_shared<Enemy>(this, player.get(), nowEnemyCount);
	entities.emplace_back(enemy);
	enemy_pos.emplace_back(std::make_tuple(nowEnemyCount++, enemy_x, enemy_y));
	float enemy_scale = enemy->GetImageScale();

	//できれば同じ個所に2体以上は重なってほしくない
	do {
		int decidedIndex = m_stageManager->CulculateRandomEntityPos(enemy_w * enemy_scale, enemy_h * enemy_scale, enemy_x, enemy_y);
		enemy->SetPos(enemy_x, enemy_y);
		if (std::count(enemyRoomIndex.begin(), enemyRoomIndex.end(), decidedIndex) <= 1) {
			enemyRoomIndex.emplace_back(decidedIndex);
			break;
		}

	} while (1);
}