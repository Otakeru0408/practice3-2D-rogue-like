#pragma once
#include "DxLib.h"
#include <string>

class GameManager;
struct InputState;

class IGameState{
	public:
	IGameState(GameManager* gameManager): m_gameManager(gameManager){}

	virtual ~IGameState() = default;

	virtual void Init() = 0;
	virtual void Update(const InputState* input) = 0;
	virtual void Draw() = 0;
	virtual void Terminate() = 0;
	//charX = 0.0 ~ 1.0, charY = 0.0 ~ 1.0 左上が(0,0)で右下が(1.0,1.0)
	void SetStringAlign(float posX, float posY, std::string context,unsigned int color,
		float charX,float charY, int fontHandle = -1){

		//fontHandleを指定しているなら
		if(fontHandle >= 0){
			//x,yを(0,0)から(charX,charY)の位置に変更する
			float x = GetDrawStringWidthToHandle(context.c_str(), -1, fontHandle) * charX;
			float y = GetFontSizeToHandle(fontHandle) * charY;

			DrawStringToHandle(posX - x, posY - y, context.c_str(), color, fontHandle);
		}

		//fonthandleを指定していないなら
		else{
			//x,yを(0,0)から(charX,charY)の位置に変更する
			float x = GetDrawStringWidth(context.c_str(), -1) * charX;
			float y = GetFontSize() * charY;

			DrawString(posX - x, posY - y, context.c_str(), color);
		}
		
	}
	/*
	SetStringAlign(200,300,"Hello, World",0.5,0.5);
	200,300が画面の中心で、文字の中心もそれにしたい。
	*/

	protected:
	GameManager* m_gameManager;
};
