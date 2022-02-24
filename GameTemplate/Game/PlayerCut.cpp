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
		//切断モードがオンかどうかで分岐
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
		//切断モード中にLB1の入力がなくなると切断モード終了
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

		//切断モデルをプレイヤーの操作通りに回転させる
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

		//斬った際の力の向き
		//何も向きを変えていない時は下向き
		Vector3 cutForce = CUT_FORCE_DOWN;
		//切断面の回転に合わせて力の向きも回転させる
		m_cutPlaneQRot.Apply(cutForce);

		//切断モード中にRB1ボタンで切断
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
			
			//当たっていれば切断音を流す
			if (hitCheck == true)
			{
				SoundOneShotPlay(L"Assets/sound/CutSE.wav");
			}
		}
	}

	void PlayerCut::NormalUpdate(const Vector3& playerPosition, const Quaternion& playerQRot)
	{
		//切断モードでない時はLB1を押すと切断モードに移行
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
