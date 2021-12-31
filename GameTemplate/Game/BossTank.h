#pragma once
#include "BossTankStatus.h"
#include "EnemyTask.h"
#include "BossTankCollision.h"
#include "BossTankTasks.h"
#include "BossTankDisplay.h"
#include "BossTankBehave.h"

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
			m_bossTankStatus.position = position;
		}

		/**
		 * @brief ���W���擾
		 * @return ���W
		*/
		Vector3 GetPosition()
		{
			return m_bossTankStatus.position;
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
			m_bossTankBehave.SetTask(taskNum, task);
		}

		/**
		 * @brief �C���̊p�x��ݒ�
		 * @param deg �C���̊p�x
		*/
		void SetTurretDeg(float deg)
		{
			m_bossTankStatus.turretDeg = deg;
		}

		/**
		 * @brief �C���̊p�x���擾
		 * @return �C���̊p�x
		*/
		float GetTurretDeg()
		{
			return m_bossTankStatus.turretDeg;
		}
		
		/**
		 * @brief �ԑ̂̊p�x��ݒ�
		 * @param deg �ԑ̂̊p�x
		*/
		void SetBaseDeg(float deg)
		{
			m_bossTankStatus.baseDeg = deg;
		}

		/**
		 * @brief �ԑ̂̊p�x���擾
		 * @return �ԑ̂̊p�x
		*/
		float GetBaseDeg()
		{
			return m_bossTankStatus.baseDeg;
		}


		Vector3 GetRightCannonPosition();

		Vector3 GetLeftCannonPosition();

		Vector3 GetFrontPosition();
	private:
		SkinModelRender* m_baseRender = nullptr;			//�ԑ̂̃��f��
		SkinModelRender* m_turretRender = nullptr;			//�C���̃��f��
		SkinModelRender* m_rightCannonRender = nullptr;		//�E�C�g�̃��f��
		SkinModelRender* m_leftCannonRender = nullptr;		//���C�g�̃��f��
		BossTankStatus m_bossTankStatus;					//�{�X�̃X�e�[�^�X
		BossTankCollision m_bossTankCollision;				//�{�X�̓����蔻��N���X
		BossTankDisplay m_bossTankDisplay;					//�{�X�̏��\���N���X
		BossTankBehave m_bossTankBehave;					//�{�X�̍s������
	};
}

