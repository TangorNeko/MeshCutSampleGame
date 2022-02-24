#include "stdafx.h"
#include "PlayerCut.h"

namespace
{
	const char* PATH_CUTPLANEMODEL = "Assets/modelData/CutIndicator.tkm";	//�ؒf�ʂ̃C���W�P�[�^�[�̃��f���p�X
	const Vector3 PLAYER_TO_CUTPOINT = { 0.0f,120.0f,0.0f };				//�v���C���[�̍��W����ؒf�ʒu�ւ̃x�N�g��
	const Vector3 CUT_FORCE_DOWN = Vector3::Down * 30.0f;					//�f�t�H���g�̌���(������)�ւ̐ؒf���̗�
	const float CUT_RANGE = 600.0f;											//�ؒf�̎˒�
	const float START_CUT_ANGLE = 45.0f;									//�Q�[���J�n���̐ؒf�p�x
	const float DEFAULT_CUT_ANGLE = 0.0f;									//�f�t�H���g�̐ؒf���[�h�̐ؒf�p�x
	const Vector3 DEFAULT_CUT_NORMAL = { 1.0f,0.0f,0.0f };					//�f�t�H���g�̐ؒf���[�h�̐ؒf�ʂ̌���
	const Vector2 INPUT_UP = { 0.0f,1.0f };									//�R���g���[���[�̓��͂̊p�x�v�Z�p�̏�x�N�g��
	const float PLAYER_CUTDEG = 0.7f;										//�v���C���[�̐��ʂ̊p�x
	const wchar_t* PATH_CUTSOUND = L"Assets/sound/CutSE.wav";				//�ؒf���̌��ʉ��p�X
}

namespace Game
{
	PlayerCut::~PlayerCut()
	{
		//�ؒf�C���W�P�[�^�[���c���Ă����ꍇ
		if (m_cutPlaneRender != nullptr)
		{
			//�폜
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

		//�ؒf�ʒu�Ɛؒf�ʂ̖@�����擾
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

		//��]��K�p
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
			//��ł��ؒf�������̃`�F�b�N�p�ϐ�
			bool hitCheck = false;

			Game::ModelCutManager::GetInstance()->QueryCut(cutNormal, cutPoint, cutForce, [cutPoint,&hitCheck, playerQRot](const SkinModelRender* cutObject)->bool
				{
					//�ؒf�I�u�W�F�N�g�ւ̋������擾
					Vector3 distance = cutObject->GetPosition() - cutPoint;

					//�v���C���[�̐��ʂ̌������v�Z
					Vector3 front = Vector3::Front;
					playerQRot.Apply(front);

					//�ؒf�I�u�W�F�N�g�ւ̌������v�Z
					Vector3 toCutObject = distance;
					toCutObject.y = 0.0f;
					toCutObject.Normalize();

					//�v���C���[�̐��ʂɐؒf�I�u�W�F�N�g�����邩?
					bool isInRange = false;
					float dot = Dot(toCutObject, front);
					if (dot >= PLAYER_CUTDEG)
					{
						isInRange = true;
					}

					//�ؒf�˒����Ńv���C���[�̐��ʂɐؒf�I�u�W�F�N�g������Ȃ�
					if (distance.LengthSq() < CUT_RANGE * CUT_RANGE && isInRange)
					{
						//��ł��ؒf����
						hitCheck |= true;

						//�ؒf����
						return true;
					}
					return false;
				}
			);
			
			//�������Ă���ΐؒf���𗬂�
			if (hitCheck == true)
			{
				SoundOneShotPlay(PATH_CUTSOUND);
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

		//�܂��ؒf�C���W�P�[�^�[����������Ă��Ȃ��Ȃ�
		if (m_isCutEvent == true && m_cutPlaneRender == nullptr)
		{
			//�ؒf�C���W�P�[�^�[�𐶐�
			m_cutPlaneRender = NewGO<SkinModelRender>(Priority::High);
			m_cutPlaneRender->InitUnlit(PATH_CUTPLANEMODEL);
			m_cutPlaneRender->SetPosition(playerPosition);
			m_cutPlaneQRot = Quaternion::Identity;

			//�Q�[���X�^�[�g���Ȃ�
			if (m_isStart == true)
			{
				//�Q�[���X�^�[�g���p�̊p�x���Z�b�g
				angle = START_CUT_ANGLE;
				m_isStart = false;
			}
			else
			{
				//�f�t�H���g�̊p�x���Z�b�g
				angle = DEFAULT_CUT_ANGLE;
			}
		}

		//�ؒf���[�h���I�����Đؒf�C���W�P�[�^�[���c���Ă����ꍇ
		if (m_isCutEvent == false && m_cutPlaneRender != nullptr)
		{
			//�ؒf�C���W�P�[�^�[���폜
			DeleteGO(m_cutPlaneRender);
			m_cutPlaneRender = nullptr;

			//�ؒf���[�h���I��
			m_isCutMode = false;

			return;
		}
	}
}
