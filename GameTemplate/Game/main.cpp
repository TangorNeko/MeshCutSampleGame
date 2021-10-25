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
	Game::ModelCutManager::CreateInstance();

	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//NOTE:テストコード
	auto* cutPlane = NewGO<Game::SkinModelRender>(0);
	cutPlane->Init("Assets/modelData/testCutPlane.tkm");

	auto* model = NewGO<Game::SkinModelRender>(0);
	model->Init("Assets/modelData/ball.tkm");

	float modelDeg = 0.0f;
	float modelX = 0.0f;
	Quaternion modelQRot = g_quatIdentity;

	Quaternion cutPlaneQRot = g_quatIdentity;
	float cutDeg = 0.0f;

	Vector3 cutNormal = { 1.0f,0.0f,0.0f };
	cutNormal.Normalize();
	Vector3 cutPoint = { 0.0f, 0.0f, 0.0f };

	g_camera3D->SetPosition(0.0f, 0.0f, -1000.0f);
	g_camera3D->SetTarget(0.0f, 0.0f, 0.0f);
	g_camera3D->SetUpdateProjMatrixFunc(g_camera3D->enUpdateProjMatrixFunc_Ortho);
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

		cutDeg += g_pad[0]->GetRStickXF();
		cutPoint.x += g_pad[0]->GetRStickYF();

		cutNormal = { 1.0f,0.0f,0.0f };
		cutPlaneQRot.SetRotationDegZ(cutDeg);
		cutPlaneQRot.Apply(cutNormal);

		cutPlane->SetPosition(cutPoint);
		cutPlane->SetRotation(cutPlaneQRot);
		cutPlane->SetScale({ 1.0f,5.0f,1.0f });

		modelDeg += g_pad[0]->GetLStickYF();
		modelX += g_pad[0]->GetLStickXF();

		modelQRot.SetRotationDegY(modelDeg);

		//現状、メッシュパーツ、マテリアルごとに切断後の頂点インデックスバッファのサイズが0ならエラーを吐く。
		//0サイズのインデックスバッファをGPUにコピーしようとするため?
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			Game::ModelCutManager::GetInstance()->QueryCut(cutNormal, cutPoint,100.0f);
		}

		if (g_pad[0]->IsTrigger(enButtonB))
		{
			model->SetDivideFlag(true);
			Game::ModelCutManager::GetInstance()->AddCuttable(model);
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

