#include "stdafx.h"
#include "system/system.h"
#include "TriangleDivider.h"


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

	////NOTE:テストコード
	//Util::PlaneData plane;
	//Util::TriangleData triangle;

	//plane.SetPoint({ 0.0f,1.0f,0.0f });
	//plane.SetNormal({ 2.0f,1.0f,0.0f });

	//triangle.vertexIndexes[0] = 0;
	//triangle.vertexIndexes[1] = 1;
	//triangle.vertexIndexes[2] = 2;

	//std::array<Vector3, 2> newpointArray;
	//
	//Util::TriangleDivider divider;
	//std::map<std::pair<Vector3, Vector3>, Vector3> newvertexContainer;
	//divider.Init(plane,&newvertexContainer);
	//divider.Divide(triangle);
	//divider.GetCrossPoint({ 7.0f,-5.0f,0.0f }, { -2.0f,-5.0f,0.0f });
	//if (divider.IsPlaneDivideTriangle() == Util::Divided_1OnPlane)
	//{
	//	int newpoint = divider.GetDividedPoint(newpointArray);
	//	OutputDebugStringA("Divided");
	//}

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

