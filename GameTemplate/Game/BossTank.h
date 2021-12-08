#pragma once
#include "EnemyTask.h"
#include "BossTankCollision.h"
#include "BossTankTasks.h"
#include "BossTankDisplay.h"

namespace Game
{
	class BossTank : public IGameObject
	{
	public:
		~BossTank();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce) override;

		/**
		 * @brief ���W��ݒ�
		 * @param position ���W
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/**
		 * @brief ���W���擾
		 * @return ���W
		*/
		Vector3 GetPosition()
		{
			return m_position;
		}

		/**
		 * @brief �_���[�W��^����
		 * @param damage �_���[�W��
		*/
		void Damage(float damage);

		/**
		 * @brief �{�X�̃^�X�N���Z�b�g
		 * @param taskNum �Z�b�g����^�X�N�̔ԍ�
		 * @param task �Z�b�g����^�X�N
		*/
		void SetTask(int taskNum, EnemyTask task)
		{
			m_tankTask[taskNum] = task;
		}

		/**
		 * @brief �C���̊p�x��ݒ�
		 * @param deg �C���̊p�x
		*/
		void SetTurretDeg(float deg)
		{
			m_turretDeg = deg;
		}

		/**
		 * @brief �C���̊p�x���擾
		 * @return �C���̊p�x
		*/
		float GetTurretDeg()
		{
			return m_turretDeg;
		}
		
		/**
		 * @brief �ԑ̂̊p�x��ݒ�
		 * @param deg �ԑ̂̊p�x
		*/
		void SetBaseDeg(float deg)
		{
			m_baseDeg = deg;
		}

		/**
		 * @brief �ԑ̂̊p�x���擾
		 * @return �ԑ̂̊p�x
		*/
		float GetBaseDeg()
		{
			return m_baseDeg;
		}
	private:
		Vector3 m_position = Vector3::Zero;					//�{�X�̍��W
		BossTankCollision m_bossTankCollision;				//�{�X�̓����蔻��N���X
		SkinModelRender* m_baseRender = nullptr;			//�ԑ̂̃��f��
		SkinModelRender* m_turretRender = nullptr;			//�C���̃��f��
		SkinModelRender* m_cannonRender = nullptr;			//�C�g�̃��f��
		BossTankDisplay m_bossTankDisplay;					//�{�X�̏��\���N���X
		Quaternion m_baseRot = Quaternion::Identity;		//�ԑ̂̃N�H�[�^�j�I��
		Quaternion m_turretRot = Quaternion::Identity;		//�C���̃N�H�[�^�j�I��
		float m_baseDeg = 0.0f;								//�ԑ̂̊p�x
		float m_turretDeg = 0.0f;							//�C���̊p�x
		int m_hp = 1000;									//�̗�
		bool m_isCannonBreak = false;						//�C�g���󂳂ꂽ?
		bool m_isTurretBreak = false;						//�C�����󂳂ꂽ?
		bool m_isBaseBreak = false;							//�ԑ̂��󂳂ꂽ?
		bool m_isSummonMinions = false;						//�G���G����������?
		EnemyTask m_tankTask[BossTankTasks::enTaskNum];		//�{�X�̃^�X�N�̔z��(���{�A��������L���[�ɃR�s�[�����)
		std::queue<EnemyTask> m_taskQueue;					//�{�X�̃^�X�N�L���[
	};
}

