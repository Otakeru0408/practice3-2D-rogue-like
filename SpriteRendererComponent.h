#pragma once
#include "DxLib.h"
#include "Component.h"
#include "TransformComponent.h"

enum class RenderLayer {
	BackgroundFar, BackgroundMid, BackgroundNear,
	Enemy, Player, EnemyBullet, PlayerBullet,
	Effect, UI, MAX_LAYERS
};

class SpriteRendererComponent :public Component {
public:
	int imageHandle;
	float imageScale;
	RenderLayer layer;          // 所属するレイヤー
	TransformComponent* transform;

	SpriteRendererComponent(Entity* _parent, int handle, RenderLayer _layer, TransformComponent* trans, float scale)
		: Component(_parent), imageHandle(handle), transform(trans), layer(_layer), imageScale(scale)
	{
	}

	void Draw()override {
		DrawRotaGraph(static_cast<int>(transform->x), static_cast<int>(transform->y), imageScale, 0, imageHandle, TRUE);
	}

	// 描画レイヤーを取得するメソッド
	RenderLayer GetLayer() const {
		return layer;
	}

	int GetImageHandle()const {
		return imageHandle;
	}
	void SetImageHandle(int handle) {
		imageHandle = handle;
	}
};