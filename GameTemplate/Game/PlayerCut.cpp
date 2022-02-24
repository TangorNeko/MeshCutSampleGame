#include "stdafx.h"
#include "PlayerCut.h"

namespace
{
	const char* PATH_CUTPLANEMODEL = "Assets/modelData/CutIndicator.tkm";
	const Vector3 PLAYER_TO_CUTPOINT = { 0.0f,120.0f,0.0f };
	const Vector3 CUT_FORCE_DOWN = Vector3::Down * 30.0f;
	const float CUT_RANGE = 600.0f;
	const float START_CUT_ANGLE = 45.0f;
	const float DEFAULT_CUT_ANGLE = 0.0f;
	const Vector3 DEFAULT_CUT_NORMAL = { 1.0f,0.0f,0.0f };
	const Vector2 INPUT_UP = { 0.0f,1.0f };
	const float PLAYER_CUTDEG = 0.7f;
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
		//Ø’fƒ‚[ƒh‚ªƒIƒ“‚©‚Ç‚¤‚©‚Å•ªŠò
		if (m_isCutMode == true || m_isCutEvent == true)
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
		//Ø’fƒ‚[ƒh’†‚ÉLB1‚Ì“ü—Í‚ª‚È‚­‚È‚é‚ÆØ’fƒ‚[ƒhI—¹
		if (g_pad[0]->IsPress(enButtonLB1) == false && m_isCutEvent == false)
		{
			DeleteGO(m_cutPlaneRender);
			m_cutPlaneRender = nullptr;
			m_isCutMode = false;

			return;
		}

		Vector3 cutPoint = playerPosition + PLAYER_TO_CUTPOINT;
		m_cutPlaneRender->SetPosition(cutPoint);
		Vector3 cutNormal = DEFAULT_CUT_NORMAL;

		//Ø’fƒ‚ƒfƒ‹‚ðƒvƒŒƒCƒ„[‚Ì‘€ì’Ê‚è‚É‰ñ“]‚³‚¹‚é
		Vector2 input = { g_pad[0]->GetRStickXF(),g_pad[0]->GetRStickYF() };
		if (input.LengthSq() > 0.0f)
		{
			input.Normalize();
			angle = Dot(input, INPUT_UP);
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

		//Ža‚Á‚½Û‚Ì—Í‚ÌŒü‚«
		//‰½‚àŒü‚«‚ð•Ï‚¦‚Ä‚¢‚È‚¢Žž‚Í‰ºŒü‚«
		Vector3 cutForce = CUT_FORCE_DOWN;
		//Ø’f–Ê‚Ì‰ñ“]‚É‡‚í‚¹‚Ä—Í‚ÌŒü‚«‚à‰ñ“]‚³‚¹‚é
		m_cutPlaneQRot.Apply(cutForce);

		//Ø’fƒ‚[ƒh’†‚ÉRB1ƒ{ƒ^ƒ“‚ÅØ’f
		if (g_pad[0]->IsTrigger(enButtonRB1))
		{
			bool hitCheck = false;

			Game::ModelCutManager::GetInstance()->QueryCut(cutNormal, cutPoint, cutForce, [cutPoint,&hitCheck, playerQRot](const SkinModelRender* cutObject)->bool
				{
					Vector3 distance = cutObject->GetPosition() - cutPoint;

					Vector3 front = Vector3::Front;
					playerQRot.Apply(front);

					Vector3 toCutObject = distance;
					toCutObject.y = 0.0f;
					toCutObject.Normalize();

					bool isInRange = false;
					float dot = Dot(toCutObject, front);
					if (dot >= PLAYER_CUTDEG)
					{
						isInRange = true;
					}

					if (distance.LengthSq() < CUT_RANGE * CUT_RANGE && isInRange)
					{
						hitCheck |= true;

						return true;
					}
					return false;
				}
			);
			
			//“–‚½‚Á‚Ä‚¢‚ê‚ÎØ’f‰¹‚ð—¬‚·
			if (hitCheck == true)
			{
				SoundOneShotPlay(L"Assets/sound/CutSE.wav");
			}
		}
	}

	void PlayerCut::NormalUpdate(const Vector3& playerPosition, const Quaternion& playerQRot)
	{
		//Ø’fƒ‚[ƒh‚Å‚È‚¢Žž‚ÍLB1‚ð‰Ÿ‚·‚ÆØ’fƒ‚[ƒh‚ÉˆÚs
		if (g_pad[0]->IsPress(enButtonLB1))
		{
			m_cutPlaneRender = NewGO<SkinModelRender>(Priority::High);
			m_cutPlaneRender->InitUnlit(PATH_CUTPLANEMODEL);
			m_cutPlaneRender->SetPosition(playerPosition);


			m_cutPlaneQRot = Quaternion::Identity;
			angle = DEFAULT_CUT_ANGLE;

			m_isCutMode = true;
		}
	}

	void PlayerCut::SetCutEvent(bool flag,const Vector3& playerPosition)
	{
		m_isCutEvent = flag;

		if (m_isCutEvent == true && m_cutPlaneRender == nullptr)
		{
			m_cutPlaneRender = NewGO<SkinModelRender>(Priority::High);
			m_cutPlaneRender->InitUnlit(PATH_CUTPLANEMODEL);
			m_cutPlaneRender->SetPosition(playerPosition);


			m_cutPlaneQRot = Quaternion::Identity;

			if (m_isStart == true)
			{
				angle = START_CUT_ANGLE;
				m_isStart = false;
			}
			else
			{
				angle = DEFAULT_CUT_ANGLE;
			}
		}

		if (m_isCutEvent == false && m_cutPlaneRender != nullptr)
		{
			DeleteGO(m_cutPlaneRender);
			m_cutPlaneRender = nullptr;
			m_isCutMode = false;

			return;
		}
	}
}
