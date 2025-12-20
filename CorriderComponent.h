#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"

#include<functional>

class CorridorComponent :public Component {
public:
	//プレイヤーの位置を確認できる。Entity側で設定する必要あり
	TransformComponent* player_trans;
	float player_width, player_height;

	//当たった時の関数コールバック
	std::function<void()> OnHitEnter;
	std::function<void()> OnHitStay;
	std::function<void()> OnHitExit;

	CorridorComponent(Entity* owner, std::tuple<float, float> imageInfo) :Component(owner), isPlayerHitting(false), wasPlayerHitting(false)
		, player_width(0), player_height(0), image_width(0), image_height(0)
	{
		//自身の位置と幅高さを取得する
		myTrans = owner->GetComponent<TransformComponent>().get();
		std::tie(image_width, image_height) = imageInfo;

	}
	~CorridorComponent()override = default;

	void Update(const InputState* input, float deltaTime)override {
		if (!player_trans->GetOwner()->isActive)return;

		if (sumRadius_horizontal == 0 || sumRadius_vertical == 0) {
			//自身とプレイヤーの半分の合計を算出
			sumRadius_horizontal = (player_width + image_width) / 2.0f;
			sumRadius_vertical = (player_height + image_height) / 2.0f;
		}
		//自分自身とプレイヤーの当たり判定
		/*
		中心ベースの AABB（Axis-Aligned Bounding Box）判定は、
		・2つの矩形の中心の距離が、半分の幅の合計より小さい
		・2つの矩形の中心の距離が、半分の高さの合計より小さい
		が成り立てば判定が成立
		*/

		if (abs(player_trans->x - myTrans->x) <= sumRadius_horizontal
			&& abs(player_trans->y - myTrans->y) <= sumRadius_vertical) {
			//当たり判定がある！ということ
			isPlayerHitting = true;
		}
		else {
			isPlayerHitting = false;
		}

		//当たり判定があった時の処理
		if (isPlayerHitting) {
			if (wasPlayerHitting) {		//連続で当たり続けているとき
				if (OnHitStay)OnHitStay();
			}
			else {						//当たり判定の最初のとき
				if (OnHitEnter) OnHitEnter();
			}
		}
		//当たり判定がないとき
		else {
			if (wasPlayerHitting) {
				if (OnHitExit)OnHitExit();	//当たり判定を抜け出したとき
			}
		}


		wasPlayerHitting = isPlayerHitting;
	}

	void Draw() {
	}
private:
	TransformComponent* myTrans;
	bool isPlayerHitting;
	bool wasPlayerHitting;
	float image_width, image_height;
	float sumRadius_horizontal = 0;
	float sumRadius_vertical = 0;
};