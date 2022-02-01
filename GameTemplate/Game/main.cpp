#include "stdafx.h"
#include "system/system.h"
#include "TitleScene.h"
#include "RenderingEngine.h"
#include "GameScene.h"
#include "Fade.h"

namespace
{
	const float FADEIN_RATE = 1.0f;
	const int FADE_WAITTIME = 10;
	const float FADEOUT_RATE = 0.01f;
}

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
	EffectEngine::CreateInstance();
	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();
	//リソースバンクマネージャーのインスタンスを作成
	Engine::ResourceBankManager::CreateInstance();
	Graphics::RenderingEngine renderingEngine;
	renderingEngine.Init();

	PhysicsWorld::GetInstance()->SetGravity(GAME_GRAVITY);
	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	Game::Fade* fade = NewGO<Game::Fade>(0);
	fade->SetFadeInRate(FADEIN_RATE);
	fade->SetWaitFrame(FADE_WAITTIME);
	fade->SetFadeOutRate(FADEOUT_RATE);
	NewGO<Game::GameScene>(0,"gamescene");

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
		//////////////////////////////////////j

		GameObjectManager::GetInstance()->ExecuteUpdate();

#ifdef _DEBUGWIREFRAME
		PhysicsWorld::GetInstance()->DebugDrawWorld(renderContext);
#endif
		//GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		renderingEngine.Render(renderContext);

		EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());
		EffectEngine::GetInstance()->Draw();

		GameObjectManager::GetInstance()->ExecutePostRender(renderContext);
		
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
	//サウンドエンジンを削除
	CSoundEngine::DeleteInstance();
	//リソースバンクマネージャーを削除
	Engine::ResourceBankManager::DeleteInstance();
	return 0;
}

