#pragma once

namespace Game
{
	class PlayerCut
	{
	public:
		~PlayerCut();

		/**
		 * @brief �v���C���[�̐ؒf�����̍X�V
		 * @param playerPosition �v���C���[�̍��W
		 * @param playerQRot �v���C���[�̉�]
		*/
		void Update(const Vector3& playerPosition,const Quaternion& playerQRot);

		/**
		 * @brief �ؒf���[�h���̍X�V
		 * @param playerPosition �v���C���[�̍��W
		 * @param playerQRot �v���C���[�̉�]
		*/
		void CutUpdate(const Vector3& playerPosition,const Quaternion& playerQRot);

		/**
		 * @brief ��ؒf���[�h���̍X�V
		 * @param playerPosition �v���C���[�̍��W
		 * @param playerQRot �v���C���[�̉�]
		*/
		void NormalUpdate(const Vector3& playerPosition,const Quaternion& playerQRot);

		/**
		 * @brief �ؒf�ʂ̃��f�����擾
		 * @return 
		*/
		SkinModelRender* GetModel()
		{
			return m_cutPlaneRender;
		}

		void SetCutEvent(bool flag, const Vector3& playerPosition);
	private:
		SkinModelRender* m_cutPlaneRender = nullptr;		//�ؒf�ʂ��������f��
		bool m_isCutMode = false;							//�ؒf���[�h��?
		bool m_isCutEvent = false;							//�C�x���g�ؒf���[�h��?
		bool m_isStart = true;								//�ŏ��̐ؒf���[�h?
		Quaternion m_cutPlaneQRot = Quaternion::Identity;	//�ؒf�ʂ�\�����f���̉�]
		float angle = 0.0f;									//�ؒf�̊p�x
	};
}

