#include "stdafx.h"
#include "BossTank.h"
#include "EnemyMissile.h"
#include "Player.h"

namespace
{
	const char* MODEL_PATH_BASE = "Assets/modelData/TankBase.tkm";
	const char* MODEL_PATH_TURRET = "Assets/modelData/TankTurret.tkm";
	const char* MODEL_PATH_CANNON = "Assets/modelData/TankCannon.tkm";
	const wchar_t* CANNONCUT_TEXT = L"Can Cut Cannon";
	const wchar_t* ALLCUT_TEXT = L"Can Cut All";
	const Vector4 SHADOWCOLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };
}

namespace Game
{
	BossTank::~BossTank()
	{
		if (m_isTurretBreak == false && m_isBaseBreak == false)
		{
			DeleteGO(m_baseRender);
			DeleteGO(m_turretRender);
			DeleteGO(m_cannonRender);
		}
	}

	bool BossTank::Start()
	{
		m_baseRender = NewGO<SkinModelRender>(0);
		m_turretRender = NewGO<SkinModelRender>(0);
		m_cannonRender = NewGO<SkinModelRender>(0);


		m_baseRender->Init(MODEL_PATH_BASE);
		m_turretRender->Init(MODEL_PATH_TURRET);
		m_cannonRender->Init(MODEL_PATH_CANNON);
		m_cannonRender->SetToCoreVector({ 0.0f,175.0f,0.0f });
		m_baseRender->SetOwner(this);
		m_turretRender->SetOwner(this);
		m_cannonRender->SetOwner(this);

		//�����蔻��̏�����
		m_bossTankCollision.Init(m_position, m_baseRot);

		//�̗͓��̕\���̏�����
		m_bossTankDisplay.Init(m_hp);

		//�^�X�N�̓o�^
		BossTankTasks bossTankTasks;
		bossTankTasks.SubmitTo(this);

		//�o�����̑ҋ@�^�X�N
		m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);

		return true;
	}

	void BossTank::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		if (skinModelRender == m_cannonRender)
		{
			//�C�g����ꂽ�t���O���I���A�s���𕪊򂳂���̂Ɏg��
			m_isCannonBreak = true;
		}

		//NOTE:baseRender��turretRender�͂����炭�������t���[���Őؒf����邪�A
		//�ǂ�����ؒf���ꂽ���̂�Delete����B
		if (skinModelRender == m_baseRender)
		{
			m_baseRender->MakeDummy(cutForce);
			m_isBaseBreak = true;
		}

		if (skinModelRender == m_turretRender)
		{
			m_turretRender->MakeDummy(cutForce);
			m_isTurretBreak = true;
		}

		//�C���Ɩ{�̂��ǂ�����a��ꂽ�犮�S�Ƀ_�~�[�Ɉڍs����
		if (m_isTurretBreak && m_isBaseBreak)
		{
			m_cannonRender->MakeDummy(Vector3::Zero);
			DeleteGO(this);
		}
	}

	void BossTank::Update()
	{
		//�v���C���[�̍s���p�^�[���A�ʂɕ���������
		if (m_taskQueue.size() == 0)
		{
			Player* player = FindGO<Player>("player");

			Vector3 distance = player->GetPosition() - m_position;

			if (m_hp <= 500 && m_isSummonMinions == false && m_isCannonBreak == true)
			{
				m_taskQueue.push(m_tankTask[BossTankTasks::enSummon]);

				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
				m_isSummonMinions = true;
			}
			else if (distance.LengthSq() <= 1000.0f * 1000.0f)
			{
				m_taskQueue.push(m_tankTask[BossTankTasks::enRolling]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
			else
			{
				m_taskQueue.push(m_tankTask[BossTankTasks::enMissile]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
		}

		if (m_taskQueue.size() > 0)
		{
			bool isEnd = m_taskQueue.front().Execute();

			if (isEnd)
			{
				m_taskQueue.pop();
			}
		}

		if (g_pad[0]->IsTrigger(enButtonLB2))
		{
			while (m_taskQueue.size() != 0)
			{
				m_taskQueue.front().Terminate();
				m_taskQueue.pop();
			}
		}

		m_turretRot.SetRotationDegY(m_turretDeg);
		m_baseRot.SetRotationDegY(m_baseDeg);

		m_turretRot.Multiply(m_baseRot, m_turretRot);

		m_baseRender->SetRotation(m_baseRot);
		m_turretRender->SetRotation(m_turretRot);
		m_cannonRender->SetRotation(m_turretRot);

		m_baseRender->SetPosition(m_position);
		m_turretRender->SetPosition(m_position);
		m_cannonRender->SetPosition(m_position);

		//�����蔻��̍X�V
		m_bossTankCollision.Update(m_position, m_baseRot);

		//�̗͓��̏��̍X�V
		m_bossTankDisplay.Update(m_hp, m_position);
	}

	void BossTank::Damage(float damage)
	{
		m_hp -= damage;

		//�̗͔����ŖC�g�ؒf�\��
		if (m_hp <= 500.0f)
		{
			m_cannonRender->SetDivideFlag(true);
			ModelCutManager::GetInstance()->AddCuttable(m_cannonRender);
		}
	
		//�̗�0�Ȃ�C���Ɩ{�̂��ؒf�\��
		if (m_hp <= 0)
		{
			m_hp = 0;
			m_baseRender->SetDivideFlag(true);
			m_turretRender->SetDivideFlag(true);
			ModelCutManager::GetInstance()->AddCuttable(m_baseRender);
			ModelCutManager::GetInstance()->AddCuttable(m_turretRender);
		}
	}
}