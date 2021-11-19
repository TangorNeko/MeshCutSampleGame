#include "stdafx.h"
#include "EnemyMissile.h"

namespace
{
	const char* PATH_MISSILEMODEL = "Assets/modelData/TankMissile.tkm";
	const float TRIGGER_HEIGHT = 350.0f;
	const float TRIGGER_RADIUS = 50.0f;
}

namespace Game
{
	EnemyMissile::~EnemyMissile()
	{
		//�a��ꂽ���̈����n���̎��ȊO�Ńf�X�g���N�^���Ă΂ꂽ�烂�f�����폜
		if (m_missileRender != nullptr && m_isCut == false)
		{
			DeleteGO(m_missileRender);
		}
	}

	bool EnemyMissile::Start()
	{
		//���f�����쐬
		m_missileRender = NewGO<SkinModelRender>(0);
		m_missileRender->Init(PATH_MISSILEMODEL);

		//OnDivide�֐��Ŏa��ꂽ���̏������L�q���邽�߂ɏ��L�҂�ݒ�
		m_missileRender->SetOwner(this);

		//�ŏ�����a���悤�ɂ��Ă���
		m_missileRender->SetDivideFlag(true);
		Game::ModelCutManager::GetInstance()->AddCuttable(m_missileRender);

		//�J�v�Z���g���K�[���쐬
		m_capsuleTrigger.CreateCapsule(m_position, m_qRot, TRIGGER_RADIUS, TRIGGER_HEIGHT);

		//�^�[�Q�b�g��T��
		m_missileMove.FindTarget();
		return true;
	}

	void EnemyMissile::Update()
	{
		m_missileMove.Update(m_position);

		//���f���̍��W�Ɖ�]���Z�b�g
		m_missileRender->SetPosition(m_position);
		m_missileRender->SetRotation(m_qRot);

		//�J�v�Z���g���K�[�̍��W�Ɖ�]���Z�b�g
		m_capsuleTrigger.SetPosition(m_position);
		m_capsuleTrigger.SetRotation(m_qRot);
	}

	void EnemyMissile::OnDivide(const SkinModelRender* skinModelRender)
	{
		//�_�~�[���쐬
		m_missileRender->MakeDummy();

		//�J�b�g���ꂽ�t���O���I����
		m_isCut = true;

		//���f�������_�[���_�~�[�N���X�Ɉ����n�����̂ō폜
		DeleteGO(this);
	}
}