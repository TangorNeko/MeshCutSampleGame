#pragma once

namespace Game
{
	class Player;
	class Door : public IGameObject
	{
	public:
		~Door();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce) override;

	private:
		SkinModelRender* m_doorModel = nullptr;					//ドアのモデル
		PhysicsStaticObject* m_physicsStaticObject = nullptr;	//ドアのモデルの当たり判定
		Player* m_player = nullptr;								//プレイヤークラスのポインタ
		bool m_isDisplayAttackTutorial = false;					//攻撃のチュートリアルを表示した?
	};
}