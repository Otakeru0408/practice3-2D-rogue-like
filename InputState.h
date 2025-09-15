#pragma once
struct InputState {
	int key[256];
	int prevKey[256];
	int mouseVal[2];
	int prevMouseVal[2];
	int mouseX;
	int mouseY;
	bool mousePressed;

	bool IsKeyDown(int key_code)const
	{
		return key[key_code] >= 1 && prevKey[key_code] == 0;
	}
	bool IsKeyStay(int key_code)const
	{
		return key[key_code] >= 1 && prevKey[key_code] >= 1;
	}
	bool IsKeyUp(int key_code)const
	{
		return key[key_code] == 0 && prevKey[key_code] >= 1;
	}

	bool IsMouseDown(int key_code)const
	{
		return mouseVal[key_code] >= 1 && prevMouseVal[key_code] == 0;
	}
	bool IsMouseStay(int key_code)const
	{
		return mouseVal[key_code] >= 1 && prevMouseVal[key_code] >= 1;
	}
	bool IsMouseUp(int key_code)const
	{
		return mouseVal[key_code] == 0 && prevMouseVal[key_code] >= 1;
	}

	int GetMouseX()const { return mouseX; }
	int GetMouseY()const { return mouseY; }
};