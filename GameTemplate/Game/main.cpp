#include "stdafx.h"
#include "system/system.h"
#include "TriangleDivider.h"
#include "GameScene.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	Light::LightManager::CreateInstance();
	Game::ModelCutManager::CreateInstance();
	//リソースバンクマネージャーのインスタンスを作成
	Engine::ResourceBankManager::CreateInstance();

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	NewGO<Game::GameScene>(0);

	//FPS計測用ストップウォッチ
	Stopwatch stopwatch;
	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//FPS計測(固定)用ストップウォッチの計測開始
		stopwatch.Start();

		//レンダリング開始。
		g_engine->BeginFrame();


		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////

		GameObjectManager::GetInstance()->ExecuteUpdate();
		PhysicsWorld::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());

#ifdef _DEBUGWIREFRAME
		PhysicsWorld::GetInstance()->DebugDrawWorld(renderContext);
#endif
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();

		//スピンロックを行う。
		int restTime = 0;
		do {
			stopwatch.Stop();
			restTime = 16 - (int)stopwatch.GetElapsedMillisecond();
		} while (restTime > 0);


		//ストップウォッチの計測終了
		stopwatch.Stop();
		//デルタタイムをストップウォッチの計測時間から、計算する
		g_gameTime->PushFrameDeltaTime((float)stopwatch.GetElapsed());
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();

	//リソースバンクマネージャーを削除
	Engine::ResourceBankManager::DeleteInstance();
	return 0;
}

