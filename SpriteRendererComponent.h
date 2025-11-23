#pragma once
#include "DxLib.h"
#include "Component.h"
#include "TransformComponent.h"
#include "InGameState.h"

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
	//プレイヤーの位置を確認できる。Entity側で設定する必要あり
	TransformComponent* player_trans;

	SpriteRendererComponent(Entity* _parent, int handle, RenderLayer _layer, TransformComponent* trans, float scale)
		: Component(_parent), imageHandle(handle), transform(trans), layer(_layer), imageScale(scale)
	{

	}

	void Draw()override {
		int px = player_trans->x;
		int py = player_trans->y;
		DrawRotaGraph(
			static_cast<int>(transform->x) - px,
			static_cast<int>(transform->y) - py,
			imageScale, 0, imageHandle, TRUE);

		DrawFormatString(10, 30, GetColor(0, 0, 0), "x:%d,y:%d", static_cast<int>(transform->x) - px,
			static_cast<int>(transform->y) - py);
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