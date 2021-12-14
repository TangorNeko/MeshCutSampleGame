#include "stdafx.h"
#include "EnemyRock.h"
#include "Player.h"
#include "HealItem.h"
#include <random>

namespace
{
	const char* PATH_ROCKMODEL = "Assets/modelData/stone.tkm";
	const Vector3 ROCK_SCALE = { 5.0f,5.0f,5.0f };
	const float ROCK_COLLISION_RADIUS = 75.0f;
	const float ROCK_MOVESPEED = 30.0f;
	const float ROCK_GRAVITY = 0.5f;
	const int ROCK_DAMAGE = 50;
}

namespace Game
{
	EnemyRock::~EnemyRock()
	{
		//�a��ꂽ���̈����n���̎��ȊO�Ńf�X�g���N�^���Ă΂ꂽ�烂�f�����폜
		if (m_rockModel != nullptr && m_isCut == false)
		{
			DeleteGO(m_rockModel);
		}
	}

	bool EnemyRock::Start()
	{
		//��̃��f����������
		m_rockModel = NewGO<SkinModelRender>(0);
		m_rockModel->Init(PATH_ROCKMODEL);
		m_rockModel->SetScale(ROCK_SCALE);

		//���f���̒��S�����_�ɂ���
		m_rockModel->SetModelCenterAsOrigin();

		//���f���̏��L�҂�ݒ�
		m_rockModel->SetOwner(this);

		//�ŏ�����ؒf�\�ɂ��Ă���
		m_rockModel->SetDivideFlag(true);
		Game::ModelCutManager::GetInstance()->AddCuttable(m_rockModel);

		//����̓����蔻����쐬
		m_sphereTrigger.CreateSphere(m_position, Quaternion::Identity, ROCK_COLLISION_RADIUS);

		//�_���[�W��^����v���C���[��T��
		m_targetPlayer = FindGO<Player>("player");

		return true;
	}

	void EnemyRock::OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce)
	{
		//�_�~�[���쐬
		m_rockModel->MakeDummy(cutForce);

		//�J�b�g���ꂽ�t���O���I����
		m_isCut = true;

		std::random_device seed_gen;
		std::mt19937 engine(seed_gen());
		int random = engine();

		if (random % 10 == 0)
		{
			HealItem* healItem = NewGO<HealItem>(0);
			healItem->SetPosition(m_position);
		}
		//���f�������_�[���_�~�[�N���X�Ɉ����n�����̂ō폜
		DeleteGO(this);
	}

	void EnemyRock::Update()
	{
		//�ړ�����
		m_position += m_moveDirection * ROCK_MOVESPEED;
		m_position.y -= ROCK_GRAVITY;

		//���f���Ɠ����蔻��ɍ��W���Z�b�g
		m_rockModel->SetPosition(m_position);
		m_sphereTrigger.SetPosition(m_position);

		//�v���C���[�Ƃ̃q�b�g���m�F
		PlayerHitTest();
	}

	void EnemyRock::PlayerHitTest()
	{
		PhysicsWorld::GetInstance()->ContactTest(m_targetPlayer->GetCharaCon(), [&](const btCollisionObject& contactObject)
			{
				//�v���C���[�̃L�����R���Ǝ����̃g���K�[���ڐG���Ă�����
				if (m_sphereTrigger.IsSelf(contactObject) == true) {
					//�_���[�W��^����
					m_targetPlayer->Damage(ROCK_DAMAGE);

					//������폜
					DeleteGO(this);
				}
			}
		);
	}
}
