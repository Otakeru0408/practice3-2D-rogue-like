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
	std::function<void()> OnHit;

	CorridorComponent(Entity* owner) :Component(owner), isPlayerHitting(false)
		, player_width(0), player_height(0), image_width(0), image_height(0) {
		//自身の位置と幅高さを取得する
		myTrans = owner->GetComponent<TransformComponent>().get();
		std::tie(image_width, image_height) = owner->GetComponent<SpriteRendererComponent>()
			->GetImageSize();

	}
	~CorridorComponent()override = default;

	void Update(const InputState* input, float deltaTime)override {
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
	}

	void Draw() {
		if (isPlayerHitting) {

		}
		DrawFormatString(10, 100, GetColor(0, 0, 0), "px:%.2f,py:%.2f",
			player_trans->x, player_trans->y);
		/*
		DrawFormatString(10, 130, GetColor(0, 0, 0), "myx:%.2f,myy:%.2f",
			myTrans->x, myTrans->y);
		DrawFormatString(10, 150, GetColor(0, 0, 0), "diff_x:%.2f,diff_y:%.2f,isHit?:%d",
			abs(player_trans->x - myTrans->x), abs(player_trans->y - myTrans->y),
			isPlayerHitting);*/

		DrawBox(myTrans->x - image_width / 2 - player_trans->x, myTrans->y - image_height / 2 - player_trans->y,
			myTrans->x + image_width / 2 - player_trans->x, myTrans->y + image_height / 2 - player_trans->y, GetColor(255, 0, 0), false);

		DrawBox(player_trans->x - player_width / 2, player_trans->y - player_height / 2,
			player_trans->x + player_width / 2, player_trans->y + player_height / 2,
			GetColor(0, 255, 0), false);
	}
private:
	TransformComponent* myTrans;
	bool isPlayerHitting;
	float image_width, image_height;
	float sumRadius_horizontal = 0;
	float sumRadius_vertical = 0;
};