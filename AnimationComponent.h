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
	float frameSpeed = 0.5f; // 何秒でアニメーションが進むか

public:
	//Playerで設定する値
	float scale = 0.05f;

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
		if (!direction || !transform)return;
		const auto& frames = animations[direction->dir];

		//カメラ位置の計算
		DrawRotaGraph(GameData::windowWidth / 2, GameData::windowHeight / 2, scale, 0.0f, frames[frame], TRUE);
		//DrawFormatString(10, 300, GetColor(0, 0, 0), "deltaTime:%.2f", frameCount);
	}
};
