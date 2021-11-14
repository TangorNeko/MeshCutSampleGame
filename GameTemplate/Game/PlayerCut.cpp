#include "stdafx.h"
#include "PlayerCut.h"

namespace
{
	const char* PATH_CUTPLANEMODEL = "Assets/modelData/testCutPlane.tkm";
	const Vector3 SCALE_CUTPLANE = { 0.25f,1.0f,1.0f };
	const Vector3 PLAYER_TO_CUTPOINT = { 0.0f,120.0f,0.0f };
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
		//�ؒf���[�h���I���Ȃ�ؒf���f���𐳂�����]������
		if (m_isCutMode == true)
		{
			Vector3 CutPoint = playerPosition + PLAYER_TO_CUTPOINT;
			m_testCutPlane->SetPosition(CutPoint);
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
			m_testCutPlane->SetRotation(m_cutPlaneQRot);

			//NOTE:�v���g�p�@�ؒf�����c�肪��ɉ����ɂȂ�悤�ɂ���
			Vector3 up = Vector3::Up;
			float deg = up.Dot(cutNormal);
			if (deg > 0)
			{
				cutNormal *= -1.0f;
			}

			//�ؒf���[�h����RB1�{�^���Őؒf
			if (g_pad[0]->IsTrigger(enButtonRB1))
			{
				Game::ModelCutManager::GetInstance()->QueryCut(cutNormal, CutPoint, CUT_RANGE);
			}
		}

		//�ؒf���[�h�łȂ�����LB1�������Ɛؒf���[�h�Ɉڍs
		if (g_pad[0]->IsPress(enButtonLB1) && m_isCutMode == false)
		{
			m_testCutPlane = NewGO<SkinModelRender>(0);
			m_testCutPlane->Init(PATH_CUTPLANEMODEL);
			m_testCutPlane->SetScale(SCALE_CUTPLANE);
			m_testCutPlane->SetPosition(playerPosition);


			m_cutPlaneQRot = Quaternion::Identity;
			angle = 0.0f;

			m_isCutMode = true;
		}

		//�ؒf���[�h����LB1�̓��͂��Ȃ��Ȃ�Ɛؒf���[�h�I��
		if (g_pad[0]->IsPress(enButtonLB1) == false && m_isCutMode == true)
		{
			DeleteGO(m_testCutPlane);
			m_testCutPlane = nullptr;
			m_isCutMode = false;
		}
	}
}
