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
		SkinModelRender* m_cutPlaneRender = nullptr;			//切断面を示すモデル
		bool m_isCutMode = false;							//切断モード中?
		Quaternion m_cutPlaneQRot = Quaternion::Identity;	//切断面を表すモデルの回転
		float angle = 0.0f;
	};
}

