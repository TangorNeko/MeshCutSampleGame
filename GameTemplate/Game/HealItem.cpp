#include "stdafx.h"
#include "HealItem.h"
#include "Player.h"

namespace
{
	const char* PATH_MODEL = "Assets/modelData/MediKit.tkm";
	const Vector3 SCALE_COLLIDER = { 10.0f,10.0f,10.0f };
	const float PICKUP_RANGE = 200.0f;
	const int HEAL_HP = 25;
	const wchar_t* HEAL_SOUND_PATH = L"Assets/sound/HealSE.wav";
}

namespace Game
{
	HealItem::~HealItem()
	{
		DeleteGO(m_skinModelRender);
	}

	bool HealItem::Start()
	{
		m_skinModelRender = NewGO<SkinModelRender>(0);
		m_skinModelRender->Init(PATH_MODEL);

		//�J�v�Z����̃R���C�_�[���쐬
		m_boxCollider.Create(SCALE_COLLIDER);

		//����̃R���C�_�[���쐬
		RigidBodyInitData rbInitData;

		//�d�ʂ��Z�b�g(�T���v���̂܂�)
		rbInitData.mass = 500.0f;

		//�R���C�_�[���Z�b�g
		rbInitData.collider = &m_boxCollider;

		//�������W�Ɖ�]���Z�b�g
		rbInitData.pos = m_position;

		//��]�̂��₷����ݒ肷��
		rbInitData.localInteria.Set(
			0.0f,
			0.0f,
			0.0f
		);

		m_rigidBody.Init(rbInitData);
		//���C�͂�ݒ肷��B(0�`10�A�T���v���̂܂�)
		m_rigidBody.SetFriction(10.0f);
		//���`�ړ�����v�f��ݒ肷��B
		//0���w�肵�����͈ړ����Ȃ��B
		m_rigidBody.SetLinearFactor(0.0f, 1.0f, 0.0f);

		m_player = FindGO<Player>("player");

		return true;
	}
	
	void HealItem::Update()
	{
		//��]�p�x�𑝉�
		m_rotateDegY++;

		//����������̍��W���擾
		m_rigidBody.GetPositionAndRotation(m_position, m_qRot);

		//��]�p�x��K�p
		m_qRot.SetRotationDegY(m_rotateDegY);

		//���f���ɍ��W�Ɖ�]���Z�b�g
		m_skinModelRender->SetPosition(m_position);
		m_skinModelRender->SetRotation(m_qRot);

		PickedUp();
	}

	void HealItem::PickedUp()
	{
		//�v���C���[�Ƃ̋����𑪒�
		Vector3 distance = m_player->GetPosition() - m_position;

		//�E���鋗���Ȃ�HP���񕜂��A�������폜
		if (distance.LengthSq() < PICKUP_RANGE * PICKUP_RANGE)
		{
			m_player->Heal(HEAL_HP);
			SoundOneShotPlay(HEAL_SOUND_PATH);
			DeleteGO(this);
		}
	}
}
