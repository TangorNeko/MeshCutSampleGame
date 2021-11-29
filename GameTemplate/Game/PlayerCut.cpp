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
		//Ø’fƒ‚[ƒh‚ªƒIƒ“‚È‚çØ’fƒ‚ƒfƒ‹‚ð³‚µ‚­‰ñ“]‚³‚¹‚é
		if (m_isCutMode == true)
		{
			Vector3 cutPoint = playerPosition + PLAYER_TO_CUTPOINT;
			m_cutPlaneRender->SetPosition(cutPoint);
			Vector3 cutNormal = { 1.0f,0.0f,0.0f };
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

			//Ø’fƒ‚[ƒh’†‚ÉRB1ƒ{ƒ^ƒ“‚ÅØ’f
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

		//Ø’fƒ‚[ƒh‚Å‚È‚¢Žž‚ÍLB1‚ð‰Ÿ‚·‚ÆØ’fƒ‚[ƒh‚ÉˆÚs
		if (g_pad[0]->IsPress(enButtonLB1) && m_isCutMode == false)
		{
			m_cutPlaneRender = NewGO<SkinModelRender>(0);
			m_cutPlaneRender->InitUnlit(PATH_CUTPLANEMODEL);
			m_cutPlaneRender->SetPosition(playerPosition);


			m_cutPlaneQRot = Quaternion::Identity;
			angle = 0.0f;

			m_isCutMode = true;
		}

		//Ø’fƒ‚[ƒh’†‚ÉLB1‚Ì“ü—Í‚ª‚È‚­‚È‚é‚ÆØ’fƒ‚[ƒhI—¹
		if (g_pad[0]->IsPress(enButtonLB1) == false && m_isCutMode == true)
		{
			DeleteGO(m_cutPlaneRender);
			m_cutPlaneRender = nullptr;
			m_isCutMode = false;
		}
	}
}
