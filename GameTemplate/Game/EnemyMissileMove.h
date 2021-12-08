#pragma once

namespace Game
{
	class Player;

	class EnemyMissileMove
	{
	public:
		~EnemyMissileMove();

		/**
		 * @brief �ǔ�����v���C���[��ݒ肷��
		 * @param targetPlayer 
		*/
		void SetTarget(const Player* targetPlayer)
		{
			m_trackingPlayer = targetPlayer;
		}

		/**
		 * @brief �ړ������̃A�b�v�f�[�g
		 * @param position �����O�̃~�T�C���̍��W
		*/
		void Update(Vector3& position);

		/**
		 * @brief �ړ��������擾
		 * @return �ړ�����
		*/
		const Vector3& GetDirection()
		{
			return m_moveDirection;
		}
	private:
		/**
		 * @brief �ŋ߂̃t���[���Ƃ̈ړ������̕��ς��v�Z����
		 * @param direction ���t���[���̈ړ�����
		*/
		void CalcAvg(Vector3& direction);
	private:
		const Player* m_trackingPlayer = nullptr;	//�ǔ�����v���C���[
		Vector3 m_moveDirection = Vector3::Zero;	//�ړ�����
		std::list<Vector3> m_pastDirectionList;		//�ߋ��̈ړ������̃��X�g
		int m_moveTime = 0;							//�ړ�����
	};
}

