#pragma once

namespace Game
{
	class PlayerCut
	{
	public:
		~PlayerCut();

		void Update(const Vector3& playerPosition,const Quaternion& playerQRot);

		SkinModelRender* GetModel()
		{
			return m_cutPlaneRender;
		}
	private:
		SkinModelRender* m_cutPlaneRender = nullptr;			//�ؒf�ʂ��������f��
		bool m_isCutMode = false;							//�ؒf���[�h��?
		Quaternion m_cutPlaneQRot = Quaternion::Identity;	//�ؒf�ʂ�\�����f���̉�]
		float angle = 0.0f;
	};
}

