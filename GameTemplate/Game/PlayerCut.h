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
		SkinModelRender* m_cutPlaneRender = nullptr;			//Ø’f–Ê‚ğ¦‚·ƒ‚ƒfƒ‹
		bool m_isCutMode = false;							//Ø’fƒ‚[ƒh’†?
		Quaternion m_cutPlaneQRot = Quaternion::Identity;	//Ø’f–Ê‚ğ•\‚·ƒ‚ƒfƒ‹‚Ì‰ñ“]
		float angle = 0.0f;
	};
}

