#include "stdafx.h"
#include "PlayerCut.h"

namespace
{
	const char* PATH_CUTPLANEMODEL = "Assets/modelData/CutIndicator.tkm";
	const Vector3 PLAYER_TO_CUTPOINT = { 0.0f,120.0f,0.0f };
	const float CUT_RANGE = 500.0f;
}

namespace Game
{
	PlayerCut::~PlayerCut()
	{
		if (m_cutPlaneRender != nullptr)
		{
			DeleteGO(m_cutPlaneRender);
		}
	}

	void PlayerCut::Update(const Vector3& playerPosition, const Quaternion& playerQRot)
	{
		//切断モードがオンかどうかで分岐
		if (m_isCutMode == true)
		{
			CutUpdate(playerPosition, playerQRot);
		}
		else
		{
			NormalUpdate(playerPosition, playerQRot);
		}
	}

	void PlayerCut::CutUpdate(const Vector3& playerPosition, const Quaternion& playerQRot)
	{
		//切断モード中にLB1の入力がなくなると切断モード終了
		if (g_pad[0]->IsPress(enButtonLB1) == false)
		{
			DeleteGO(m_cutPlaneRender);
			m_cutPlaneRender = nullptr;
			m_isCutMode = false;

			return;
		}

		Vector3 cutPoint = playerPosition + PLAYER_TO_CUTPOINT;
		m_cutPlaneRender->SetPosition(cutPoint);
		Vector3 cutNormal = { 1.0f,0.0f,0.0f };

		//切断モデルをプレイヤーの操作通りに回転させる
		Vector2 input = { g_pad[0]->GetRStickXF(),g_pad[0]->GetRStickYF() };
		if (input.LengthSq() > 0.0f)
		{
			input.Normalize();
			angle = Dot(input, { 0.0f,1.0f });
			angle = acos(angle);

			if (input.x > 0.0f)
			{
				angle *= -1;
			}
		}
		m_cutPlaneQRot.SetRotationZ(angle);
		m_cutPlaneQRot.Multiply(playerQRot);
		m_cutPlaneQRot.Apply(cutNormal);
		m_cutPlaneRender->SetRotation(m_cutPlaneQRot);


		//切断モード中にRB1ボタンで切断
		if (g_pad[0]->IsTrigger(enButtonRB1))
		{
			Game::ModelCutManager::GetInstance()->QueryCut(cutNormal, cutPoint, [cutPoint](const SkinModelRender* cutObject)->bool
				{
					Vector3 distance = cutObject->GetPosition() - cutPoint;

					if (distance.LengthSq() < CUT_RANGE * CUT_RANGE)
					{
						return true;
					}
					return false;
				}
			);
		}
	}

	void PlayerCut::NormalUpdate(const Vector3& playerPosition, const Quaternion& playerQRot)
	{
		//切断モードでない時はLB1を押すと切断モードに移行
		if (g_pad[0]->IsPress(enButtonLB1))
		{
			m_cutPlaneRender = NewGO<SkinModelRender>(0);
			m_cutPlaneRender->InitUnlit(PATH_CUTPLANEMODEL);
			m_cutPlaneRender->SetPosition(playerPosition);


			m_cutPlaneQRot = Quaternion::Identity;
			angle = 0.0f;

			m_isCutMode = true;
		}
	}
}
