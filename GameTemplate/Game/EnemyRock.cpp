#include "stdafx.h"
#include "EnemyRock.h"
#include "Player.h"

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
		m_rockModel = NewGO<SkinModelRender>(0);
		m_rockModel->Init("Assets/modelData/stone.tkm");
		m_rockModel->SetOwner(this);
		m_rockModel->SetDivideFlag(true);
		m_rockModel->SetScale({ 5.0f,5.0f,5.0f });
		m_rockModel->SetModelCenterAsOrigin();
		Game::ModelCutManager::GetInstance()->AddCuttable(m_rockModel);

		m_sphereTrigger.CreateSphere(m_position, Quaternion::Identity, 75.0f);

		m_targetPlayer = FindGO<Player>("player");

		return true;
	}

	void EnemyRock::OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce)
	{
		//�_�~�[���쐬
		m_rockModel->MakeDummy(cutForce);

		//�J�b�g���ꂽ�t���O���I����
		m_isCut = true;

		//���f�������_�[���_�~�[�N���X�Ɉ����n�����̂ō폜
		DeleteGO(this);
	}

	void EnemyRock::Update()
	{
		m_position += m_moveDirection * 30;
		m_position.y -= 0.5f;

		m_rockModel->SetPosition(m_position);

		m_sphereTrigger.SetPosition(m_position);

		PlayerHitTest();
	}

	void EnemyRock::PlayerHitTest()
	{
		PhysicsWorld::GetInstance()->ContactTest(m_targetPlayer->GetCharaCon(), [&](const btCollisionObject& contactObject)
			{
				//�ǂ������Ă���v���C���[�̃L�����R���Ǝ����̃g���K�[���ڐG���Ă�����
				if (m_sphereTrigger.IsSelf(contactObject) == true) {
					//�_���[�W��^����
					m_targetPlayer->Damage(50);

					//������폜
					DeleteGO(this);
				}
			}
		);
	}
}
