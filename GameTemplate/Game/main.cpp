#include "stdafx.h"
#include "system/system.h"
#include "HitTest.h"


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

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//NOTE:テストコード
	Util::PlaneData plane;
	Util::TriangleData triangle;
	Util::VerticesPack vertices;

	plane.planePoint = { 0.0f,0.0f,0.0f };
	plane.planeNormal = { 1.0f,0.0f,0.0f };

	triangle.vertices[0] = { -1.0f,0.0f,0.0f };
	triangle.vertices[1] = { 1.0f,0.0f,0.0f };
	triangle.vertices[2] = { 0.0f,0.0f,0.0f };

	if (Util::IsPlaneDivideTriangle(plane, triangle, vertices) == true)
	{
		OutputDebugStringA("Divided");
	}

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();


		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////

		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}

