#pragma once

namespace Game
{
	class PlayerCut
	{
	public:
		~PlayerCut();

		void Update(const Vector3& playerPosition,const Quaternion& playerQRot);

		void CutUpdate(const Vector3& playerPosition,const Quaternion& playerQRot);

		void NormalUpdate(const Vector3& playerPosition,const Quaternion& playerQRot);

		SkinModelRender* GetModel()
		{
			return m_cutPlaneRender;
		}

		void SetCutEvent(bool flag, const Vector3& playerPosition);
	private:
		SkinModelRender* m_cutPlaneRender = nullptr;			//Ø’f–Ê‚ğ¦‚·ƒ‚ƒfƒ‹
		bool m_isCutMode = false;							//Ø’fƒ‚[ƒh’†?
		bool m_isCutEvent = false;
		Quaternion m_cutPlaneQRot = Quaternion::Identity;	//Ø’f–Ê‚ğ•\‚·ƒ‚ƒfƒ‹‚Ì‰ñ“]
		float angle = 0.0f;
	};
}

