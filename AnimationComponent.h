#pragma once
#include "Component.h"
#include "DirectionComponent.h"

#include <map>

class AnimationComponent : public Component {
private:
	std::map<Direction, std::vector<int>> animations; // 方向ごとのフレーム画像
	std::shared_ptr<DirectionComponent> direction;
	std::shared_ptr<TransformComponent> transform;
	int frame = 0;
	float frameCount = 0;
	float frameSpeed = 0.2f; // 何秒でアニメーションが進むか
	bool isOtherAnim = false;	//プレイヤーかそれ以外かでアニメーションの仕方を変える

public:
	//Playerで設定する値
	float scale = 0.05f;
	TransformComponent* player_trans;

	//1だったらプレイヤー以外のアニメーションとして常に動かし続けたい
	void SetAnimationMode(int i) {
		if (i == 1)isOtherAnim = true;
		else {
			isOtherAnim = false;
			//それ以外の処理...
		}
	}

	AnimationComponent(Entity* e, float _scale)
		:Component(e), scale(_scale) {
		direction = owner->GetComponent<DirectionComponent>();
		transform = owner->GetComponent<TransformComponent>();
	}
	void AddAnimation(Direction dir, const std::vector<int>& frames) {
		animations[dir] = frames;
	}

	void Update(const InputState* input, float deltaTime) override {
		if (!transform) return;


		bool moving = input->IsMouseStay(0);
		if (isOtherAnim)moving = true;	//プレイヤー以外のアニメは常に動かす

		if (moving) {
			frameCount += deltaTime;
			if (frameCount >= frameSpeed) {
				frameCount = 0;
				frame = (frame + 1) % animations[direction->dir].size();
			}
		}
		else {
			frame = 0; // 停止中は最初のフレーム
		}
	}

	void Draw() override {

		if (!direction || !transform || !player_trans)return;

		const auto& frames = animations[direction->dir];

		if (isOtherAnim) {
			int px = player_trans->x - GameData::windowWidth / 2;
			int py = player_trans->y - GameData::windowHeight / 2;
			DrawRotaGraph(
				static_cast<int>(transform->x) - px,
				static_cast<int>(transform->y) - py,
				scale, 0, frames[frame], TRUE);
			return;
		}

		//カメラ位置の計算
		DrawRotaGraph(GameData::windowWidth / 2, GameData::windowHeight / 2, scale, 0.0f, frames[frame], TRUE);
		//DrawFormatString(10, 300, GetColor(0, 0, 0), "deltaTime:%.2f", frameCount);
	}

	auto GetImageSize() {
		int width, height;
		GetGraphSize(animations[Direction::Up][0], &width, &height);
		return std::make_tuple(width * scale, height * scale);
	}

	void SetAnimationSpeed(float animSpeed) {
		frameSpeed = animSpeed;
	}
};
