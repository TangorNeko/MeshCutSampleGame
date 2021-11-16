#pragma once

namespace Game
{
	class EnemyMissile : public IGameObject
	{
		~EnemyMissile();

		bool Start() override;

		void Update() override;

		void OnDivide() override;

	public:
		/**
		 * @brief 座標をセット
		 * @param position 座標
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/**
		 * @brief 座標を取得
		 * @return 座標
		*/
		const Vector3& GetPosition()
		{
			return m_position;
		}
		
		/**
		 * @brief 回転をセット
		 * @param rot 回転
		*/
		void SetRotation(const Quaternion rot)
		{
			m_qRot = rot;
		}

		/**
		 * @brief ダメージを与える(後に削除予定)
		 * @param damage ダメージ
		*/
		void Damage(float damage);
	private:
		SkinModelRender* m_missileRender = nullptr;		//スキンモデルレンダー
		Vector3 m_position = Vector3::Zero;				//座標
		Quaternion m_qRot = Quaternion::Identity;		//回転
		float m_hp = 25.0f;								//体力(後に削除予定)
		FontRender* m_canCutTextRender = nullptr;		//カット可能テキスト(後に削除予定)
		bool m_isCut = false;							//カットされた?
	};
}