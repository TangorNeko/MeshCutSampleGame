#include "stdafx.h"
#include "PlayerCut.h"

namespace
{
	const char* PATH_CUTPLANEMODEL = "Assets/modelData/CutIndicator.tkm";
	const Vector3 PLAYER_TO_CUTPOINT = { 0.0f,120.0f,0.0f };
	const Vector3 CUT_FORCE_DOWN = Vector3::Down * 30.0f;
	const float CUT_RANGE = 600.0f;
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
		Vector3 cutNormal = { 1.0f,0.0f,0.0f };

		//�ؒf���f�����v���C���[�̑���ʂ�ɉ�]������
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

		//�a�����ۂ̗͂̌���
		//����������ς��Ă��Ȃ����͉�����
		Vector3 cutForce = CUT_FORCE_DOWN;
		//�ؒf�ʂ̉�]�ɍ��킹�ė͂̌�������]������
		m_cutPlaneQRot.Apply(cutForce);

		//�ؒf���[�h����RB1�{�^���Őؒf
		if (g_pad[0]->IsTrigger(enButtonRB1))
		{
			bool hitCheck = false;

			Game::ModelCutManager::GetInstance()->QueryCut(cutNormal, cutPoint, cutForce, [cutPoint,&hitCheck](const SkinModelRender* cutObject)->bool
				{
					Vector3 distance = cutObject->GetPosition() - cutPoint;

					if (distance.LengthSq() < CUT_RANGE * CUT_RANGE)
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
			m_cutPlaneRender = NewGO<SkinModelRender>(0);
			m_cutPlaneRender->InitUnlit(PATH_CUTPLANEMODEL);
			m_cutPlaneRender->SetPosition(playerPosition);


			m_cutPlaneQRot = Quaternion::Identity;
			angle = 0.0f;

			m_isCutMode = true;
		}
	}

	void PlayerCut::SetCutEvent(bool flag,const Vector3& playerPosition)
	{
		m_isCutEvent = flag;

		if (m_isCutEvent == true && m_cutPlaneRender == nullptr)
		{
			m_cutPlaneRender = NewGO<SkinModelRender>(0);
			m_cutPlaneRender->InitUnlit(PATH_CUTPLANEMODEL);
			m_cutPlaneRender->SetPosition(playerPosition);


			m_cutPlaneQRot = Quaternion::Identity;
			angle = 0.0f;
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
