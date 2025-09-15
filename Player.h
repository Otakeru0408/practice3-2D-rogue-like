#pragma once
#include "Entity.h"

/*必要なコンポーネント
・TransformComponent
・SpriteComponent
・AnimateComponent
*/

class Player :public Entity {
public:
	Player() :Entity() {}
	~Player() = default;
private:
	void Update(const InputState* input, float deltaTime)override;
	void Draw()override;
};