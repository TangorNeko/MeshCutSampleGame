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
		//�ؒf���[�h���I�����ǂ����ŕ���
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
		//�ؒf���[�h����LB1�̓��͂��Ȃ��Ȃ�Ɛؒf���[�h�I��
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

		//�ؒf���f�����v���C���[�̑���ʂ�ɉ�]������
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

		//�a�����ۂ̗͂̌���
		//����������ς��Ă��Ȃ����͉�����
		Vector3 cutForce = CUT_FORCE_DOWN;
		//�ؒf�ʂ̉�]�ɍ��킹�ė͂̌�������]������
		m_cutPlaneQRot.Apply(cutForce);

		//�ؒf���[�h����RB1�{�^���Őؒf
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
			
			//�������Ă���ΐؒf���𗬂�
			if (hitCheck == true)
			{
				SoundOneShotPlay(L"Assets/sound/CutSE.wav");
			}
		}
	}

	void PlayerCut::NormalUpdate(const Vector3& playerPosition, const Quaternion& playerQRot)
	{
		//�ؒf���[�h�łȂ�����LB1�������Ɛؒf���[�h�Ɉڍs
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
