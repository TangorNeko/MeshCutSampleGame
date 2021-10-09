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

	//NOTE:テストコード

	ModelInitData modelInitData;
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	modelInitData.m_tkmFilePath = "Assets/modelData/teapot.tkm";
	modelInitData.m_modelUpAxis = enModelUpAxisZ;

	Model testModel;
	testModel.Init(modelInitData);
	g_camera3D->SetFar(100000.0f);

	float deg = 0.0f;
	float x = 0.0f;
	Quaternion testQRot = g_quatIdentity;

	Vector3 cutNormal = { 1.0f,0.0f,0.0f };
	cutNormal.Normalize();
	Vector3 cutPoint = { 0.0f, 25.0f, 0.0f };

	g_camera3D->SetPosition(0.0f, 0.0f, -1000.0f);
	g_camera3D->SetTarget(0.0f, 0.0f, 0.0f);

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

		testModel.Draw(renderContext);

		deg += g_pad[0]->GetRStickXF();
		x += g_pad[0]->GetLStickXF();

		testQRot.SetRotationDegY(deg);

		testModel.UpdateWorldMatrix({ x,0.0f,0.0f }, testQRot, { 1.0f,1.0f,1.0f });

		if (g_pad[0]->IsTrigger(enButtonA))
		{
			testModel.Divide(modelInitData, cutNormal, cutPoint);
		}
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}

