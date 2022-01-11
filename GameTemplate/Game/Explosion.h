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
		 * @brief 座標を設定
		 * @param position 座標
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
	private:
		Vector3 m_position = Vector3::Zero;	//爆発エフェクトの座標
		int m_explodeFrame = 120;			//爆発が起こるフレーム
		int m_currentFrame = 0;				//現在のフレーム
	};
}
