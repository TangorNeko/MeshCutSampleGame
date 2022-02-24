#include "stdafx.h"
#include "StepObject.h"

namespace
{
	const char* STEPMISSILE_MODEL_PATH = "Assets/modelData/Missile_Flat.tkm";	//����~�T�C���̃��f���p�X
	const float MOVESPEED = 10.0f;												//����~�T�C���̈ړ����x
	const int MAX_LIFETIME = 400;												//����~�T�C���̐�������
	const char16_t* EFFECT_MISSILE_PATH = u"Assets/effect/MissileTail.efk";		//����~�T�C���̃G�t�F�N�g�p�X
	const Vector3 EFFECT_MISSILE_SCALE = { 25.0f,25.0f,25.0f };					//����~�T�C���̃G�t�F�N�g�̊g�嗦
	const float EFFECT_MISSILE_ROTATEDEG = -90.0f;								//����~�T�C���G�t�F�N�g�̉�]�p�x
}

namespace Game
{
	StepObject::~StepObject()
	{
		//���f�����폜
		DeleteGO(m_skinModelRender);

		//�~�T�C���G�t�F�N�g���X�g�b�v���č폜
		m_missileEffect->Stop();
		DeleteGO(m_missileEffect);
	}

	bool StepObject::Start()
	{
		//�~�T�C���̃��f�����쐬
		m_skinModelRender = NewGO<SkinModelRender>(Priority::High);
		m_skinModelRender->Init(STEPMISSILE_MODEL_PATH);

		//�~�T�C���̃G�t�F�N�g���쐬
		m_missileEffect = NewGO<Effect>(Priority::High);
		m_missileEffect->Init(EFFECT_MISSILE_PATH);
		m_missileEffect->SetScale(EFFECT_MISSILE_SCALE);
		m_missileEffect->Play();

		return true;
	}

	void StepObject::Update()
	{
		//�ړ������ւ̉�]���v�Z
		m_qRot.SetRotation(Vector3::Up, m_moveDirection);
		
		//�ړ������ֈړ�
		m_position += m_moveDirection * MOVESPEED;

		//���W�Ɖ�]���Z�b�g
		m_skinModelRender->SetPosition(m_position);
		m_skinModelRender->SetRotation(m_qRot);

		//�~�T�C���̊p�x���Z�b�g
		Quaternion MissileRot;
		MissileRot.SetRotationDegX(EFFECT_MISSILE_ROTATEDEG);
		MissileRot.Multiply(m_qRot);
		m_missileEffect->SetPosition(m_position);
		m_missileEffect->SetRotation(MissileRot);

		//�������Ԃ��C���N�������g
		m_lifeTime++;

		//�ő吶�����ԂɂȂ�����폜
		if (m_lifeTime == MAX_LIFETIME)
		{
			DeleteGO(this);
		}
	}
}
