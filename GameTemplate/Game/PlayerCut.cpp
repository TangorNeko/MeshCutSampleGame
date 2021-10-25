#include "stdafx.h"
#include "PlayerCut.h"

namespace
{
	const char* PATH_CUTPLANEMODEL = "Assets/modelData/testCutPlane.tkm";
	const Vector3 SCALE_CUTPLANE = { 0.25f,1.0f,1.0f };
	const Vector3 PLAYER_TO_CUTPOINT = { 0.0f,50.0f,0.0f };
	const float CUT_RANGE = 500.0f;
}

namespace Game
{
	PlayerCut::~PlayerCut()
	{
		if (m_testCutPlane != nullptr)
		{
			DeleteGO(m_testCutPlane);
		}
	}

	void PlayerCut::Update(const Vector3& playerPosition, const Quaternion& playerQRot)
	{
		//切断モードがオンなら切断モデルを正しく回転させる
		if (m_isCutMode == true)
		{
			Vector3 CutPoint = playerPosition + PLAYER_TO_CUTPOINT;
			m_testCutPlane->SetPosition(CutPoint);
			m_cutDeg += g_pad[0]->GetRStickXF();

			Vector3 cutNormal = { 1.0f,0.0f,0.0f };
			m_cutPlaneQRot.SetRotationDegZ(m_cutDeg);
			m_cutPlaneQRot.Multiply(playerQRot);
			m_cutPlaneQRot.Apply(cutNormal);
			m_testCutPlane->SetRotation(m_cutPlaneQRot);

			//NOTE:プロト用　切断した残りが常に下側になるようにする
			Vector3 up = Vector3::Up;
			float deg = up.Dot(cutNormal);
			if (deg > 0)
			{
				cutNormal *= -1.0f;
			}

			//切断モード中にYボタンで切断
			if (g_pad[0]->IsTrigger(enButtonY))
			{
				Game::ModelCutManager::GetInstance()->QueryCut(cutNormal, CutPoint, CUT_RANGE);
			}
		}

		//切断モードでない時はLB1を押すと切断モードに移行
		if (g_pad[0]->IsPress(enButtonLB1) && m_isCutMode == false)
		{
			m_testCutPlane = NewGO<SkinModelRender>(0);
			m_testCutPlane->Init(PATH_CUTPLANEMODEL);
			m_testCutPlane->SetScale(SCALE_CUTPLANE);
			m_testCutPlane->SetPosition(playerPosition);


			m_cutPlaneQRot = Quaternion::Identity;
			m_cutDeg = 0.0f;


			m_isCutMode = true;
		}

		//切断モード中にLB1の入力がなくなると切断モード終了
		if (g_pad[0]->IsPress(enButtonLB1) == false && m_isCutMode == true)
		{
			DeleteGO(m_testCutPlane);
			m_testCutPlane = nullptr;
			m_isCutMode = false;
		}
	}
}
