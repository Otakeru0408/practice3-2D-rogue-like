#include "DxLib.h"
#include "GameManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(400,600, 32);
    GameManager gameManager; // GameManagerオブジェクトを生成

    gameManager.Initialize(); // ゲームの初期化

    // ゲームループ
    while ( ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0
        && !gameManager.IsGameFinished()) {
        gameManager.Update(); // 更新
        gameManager.Draw();   // 描画
    }

    gameManager.Finalize(); // ゲームの終了処理
}