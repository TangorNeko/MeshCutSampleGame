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
		 * @brief 線分がステージのモデルにヒット(交差)しているか?
		 * @param start 線分の開始地点
		 * @param end 線分の終了地点
		 * @param crossPoint ヒットした地点(参照受け取り)
		 * @return ヒットした?
		*/
		bool isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint) const;
	private:
		SkinModelRender* m_stageModel = nullptr;		//ステージのモデル
		Door* m_stageDoor = nullptr;					//ステージのドア
		PhysicsStaticObject m_physicsStaticObject;		//ステージのモデルの当たり判定
		Light::DirectionLight* m_stageLight = nullptr;	//ステージの並行光源
	};
}

