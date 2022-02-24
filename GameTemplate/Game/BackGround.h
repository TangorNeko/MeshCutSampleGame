#pragma once

namespace Game
{
	class Door;
	class BackGround : public IGameObject
	{
	public:
		~BackGround();

		bool Start() override;

		/**
		 * @brief �������X�e�[�W�̃��f���Ƀq�b�g(����)���Ă��邩?
		 * @param start �����̊J�n�n�_
		 * @param end �����̏I���n�_
		 * @param crossPoint �q�b�g�����n�_(�Q�Ǝ󂯎��)
		 * @return �q�b�g����?
		*/
		bool isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint) const;
	private:
		SkinModelRender* m_stageModel = nullptr;		//�X�e�[�W�̃��f��
		Door* m_stageDoor = nullptr;					//�X�e�[�W�̃h�A
		PhysicsStaticObject m_physicsStaticObject;		//�X�e�[�W�̃��f���̓����蔻��
		Light::DirectionLight* m_stageLight = nullptr;	//�X�e�[�W�̕��s����
	};
}

