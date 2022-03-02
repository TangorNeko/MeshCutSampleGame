#include "stdafx.h"
#include "Phase0.h"
#include "Player.h"
#include "DashTutorial.h"

namespace
{
	const Vector3 DASHTUTORIAL_TO_TRIGGER = { 0.0f, 0.0f, 1400.0f };	//����̃`���[�g���A���̃g���K�[�ւ̃x�N�g��
	const float DASHTUTORIAL_RANGE = 300.0f;							//����̃`���[�g���A���̕\�����J�n�����͈�
}

namespace Game
{
	void Phase0::Init()
	{
		//�v���C���[���擾
		m_player = FindGO<Player>("player");
	}

	bool Phase0::PhaseUpdate()
	{
		//�v���C���[����g���K�[�܂ł̋������v�Z
		Vector3 toTrigger = m_player->GetPosition();
		toTrigger -= DASHTUTORIAL_TO_TRIGGER;

		//�v���C���[���`���[�g���A���\���͈͓��Ȃ�
		if (toTrigger.LengthSq() < DASHTUTORIAL_RANGE * DASHTUTORIAL_RANGE)
		{
			//�_�b�V���̃`���[�g���A����\��
			NewGO<DashTutorial>(Priority::High, "dashtutorial");

			//�t�F�[�Y���I��
			return true;
		}

		return false;
	}
}