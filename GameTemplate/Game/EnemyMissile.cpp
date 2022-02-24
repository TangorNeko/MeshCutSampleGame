#include "stdafx.h"
#include "EnemyMissile.h"
#include "Player.h"
#include "HealItem.h"
#include <random>

namespace
{
	const char* PATH_MISSILEMODEL = "Assets/modelData/Missile_Bullet.tkm";
	const float TRIGGER_HEIGHT = 300.0f;
	const float TRIGGER_RADIUS = 50.0f;
	const int MISSILE_DAMAGE = 50;
	const char16_t* EFFECT_MISSILE_PATH = u"Assets/effect/MissileTail.efk";
	const Vector3 EFFECT_MISSILE_SCALE = { 25.0f,25.0f,25.0f };
	const float EFFECT_MISSILE_ROTATEDEG = -90.0f;
	const wchar_t* EXPLOSION_SOUND_PATH = L"Assets/sound/ExplosionSE.wav";
	const Vector3 EFFECT_EXPLOSION_SCALE = { 25.0f,25.0f,25.0f };
	const int HEALDROP_MODULO_NUM = 5;
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

		m_missileEffect->Stop();
		DeleteGO(m_missileEffect);
	}

	bool EnemyMissile::Start()
	{
		//���f�����쐬
		m_missileRender = NewGO<SkinModelRender>(Priority::High);
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

		m_missileEffect = NewGO<Effect>(Priority::High);
		m_missileEffect->Init(EFFECT_MISSILE_PATH);
		m_missileEffect->SetScale(EFFECT_MISSILE_SCALE);
		m_missileEffect->Play();
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

		Quaternion MissileRot;
		MissileRot.SetRotationDegX(EFFECT_MISSILE_ROTATEDEG);
		MissileRot.Multiply(m_qRot);
		m_missileEffect->SetPosition(m_position);
		m_missileEffect->SetRotation(MissileRot);

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

		if (random % HEALDROP_MODULO_NUM == 0)
		{
			HealItem* healItem = NewGO<HealItem>(Priority::High,"healItem");
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

					Effect* boomEffect = NewGO<Effect>(Priority::High);
					boomEffect->Init(u"Assets/effect/Boom.efk");
					boomEffect->SetScale(EFFECT_EXPLOSION_SCALE);
					boomEffect->SetPosition(m_position);
					boomEffect->Play();

					SoundOneShotPlay(EXPLOSION_SOUND_PATH);

					//������폜
					DeleteGO(this);
				}
			}
		);
	}
}