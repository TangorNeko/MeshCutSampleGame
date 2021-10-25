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
			return m_testCutPlane;
		}
	private:
		SkinModelRender* m_testCutPlane = nullptr;
		bool m_isCutMode = false;
		Quaternion m_cutPlaneQRot = Quaternion::Identity;
		float m_cutDeg = 0.0f;
	};
}

