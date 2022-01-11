#pragma once

namespace Game
{
	class Explosion : public IGameObject
	{
	public:
		~Explosion();

		bool Start() override;

		void Update() override;

		/**
		 * @brief ���W��ݒ�
		 * @param position ���W
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
	private:
		Vector3 m_position = Vector3::Zero;	//�����G�t�F�N�g�̍��W
		int m_explodeFrame = 120;			//�������N����t���[��
		int m_currentFrame = 0;				//���݂̃t���[��
	};
}
