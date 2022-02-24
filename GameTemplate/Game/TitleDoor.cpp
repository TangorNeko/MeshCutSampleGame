#include "stdafx.h"
#include "TitleDoor.h"
#include "Player.h"
#include "TitleSprites.h"

namespace
{
	const char* TITLE_DOOR_MODEL_PATH = "Assets/modelData/TitleDoor.tkm";
}

namespace Game
{
	TitleDoor::~TitleDoor()
	{
		//�ؒf����Ă��Ȃ�������폜
		if (m_isCut == false)
		{
			DeleteGO(m_titleDoorRender);
		}
	}

	bool TitleDoor::Start()
	{
		//�^�C�g���p�̃h�A�̃��f���𐶐�
		m_titleDoorRender = NewGO<SkinModelRender>(Priority::High);
		m_titleDoorRender->Init(TITLE_DOOR_MODEL_PATH);
		m_titleDoorRender->SetModelCenterAsOrigin();
		m_titleDoorRender->SetOwner(this);
		m_titleDoorRender->SetDivideFlag(true);
		m_titleDoorRender->SetShadowCasterFlag(false);

		ModelCutManager::GetInstance()->AddCuttable(m_titleDoorRender);

		//�^�C�g���p�̃X�v���C�g��\��
		m_titleSprites = NewGO<TitleSprites>(Priority::High, "titlesprites");

		return true;
	}

	void TitleDoor::OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce)
	{
		//�_�~�[���쐬
		m_titleDoorRender->MakeDummy(cutForce);

		//�J�b�g���ꂽ�t���O���I����
		m_isCut = true;

		//�v���C���[�̐ؒf���[�h���I�t�ɂ���B
		Player* player = FindGO<Player>("player");
		player->EventCutOff();

		//�v���C���[�̗̑͂�\��
		player->InitDisplay();

		//�^�C�g���X�v���C�g������
		m_titleSprites->NoticeGameStart();

		//���f�������_�[���_�~�[�N���X�Ɉ����n�����̂ō폜
		DeleteGO(this);
	}

	void TitleDoor::Update()
	{

	}
}
