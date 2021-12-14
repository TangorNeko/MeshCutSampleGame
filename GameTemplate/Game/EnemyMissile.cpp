#include "stdafx.h"
#include "EnemyMissile.h"
#include "Player.h"
#include "HealItem.h"
#include <random>

namespace
{
	const char* PATH_MISSILEMODEL = "Assets/modelData/TankMissile.tkm";
	const float TRIGGER_HEIGHT = 300.0f;
	const float TRIGGER_RADIUS = 50.0f;
	const int MISSILE_DAMAGE = 50;
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
		m_trackingPlayer = FindGO<Player>("player");

		//�~�T�C���̈ړ��N���X�Ƀ^�[�Q�b�g��n��
		m_missileMove.SetTarget(m_trackingPlayer);
		return true;
	}

	void EnemyMissile::Update()
	{
		//�~�T�C���̈ړ�����
		m_missileMove.Update(m_position);

		//�~�T�C���̉�]����
		m_missileRotation.Update(m_qRot, m_missileMove.GetDirection());

		//���f���̍��W�Ɖ�]���Z�b�g
		m_missileRender->SetPosition(m_position);
		m_missileRender->SetRotation(m_qRot);

		//�J�v�Z���g���K�[�̍��W�Ɖ�]���Z�b�g
		m_capsuleTrigger.SetPosition(m_position);
		m_capsuleTrigger.SetRotation(m_qRot);

		//�v���C���[�Ƃ̃q�b�g���m�F
		PlayerHitTest();
	}

	void EnemyMissile::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		//�_�~�[���쐬
		m_missileRender->MakeDummy(cutForce);

		//�J�b�g���ꂽ�t���O���I����
		m_isCut = true;

		std::random_device seed_gen;
		std::mt19937 engine(seed_gen());
		int random = engine();

		if (random % 5 == 0)
		{
			HealItem* healItem = NewGO<HealItem>(0);
			healItem->SetPosition(m_position);
		}
		//���f�������_�[���_�~�[�N���X�Ɉ����n�����̂ō폜
		DeleteGO(this);
	}

	void EnemyMissile::PlayerHitTest()
	{
		PhysicsWorld::GetInstance()->ContactTest(m_trackingPlayer->GetCharaCon(), [&](const btCollisionObject& contactObject)
			{
				//�ǂ������Ă���v���C���[�̃L�����R���Ǝ����̃g���K�[���ڐG���Ă�����
				if (m_capsuleTrigger.IsSelf(contactObject) == true) {
					//�_���[�W��^����
					m_trackingPlayer->Damage(MISSILE_DAMAGE);

					//������폜
					DeleteGO(this);
				}
			}
		);
	}
}