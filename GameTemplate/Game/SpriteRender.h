#pragma once

namespace Game
{
	class SpriteRender : public IGameObject
	{
	public:

		/**
		 * @brief スプライトの描画
		 * @param rc レンダーコンテキスト
		*/
		void PostRender(RenderContext& rc) override;

		/**
		 * @brief スプライトの初期化
		 * @param spritePath スプライトのパス
		 * @param width スプライトの横幅
		 * @param height スプライトの高さ
		*/
		void Init(const char* spritePath, UINT width, UINT height);

		/**
		 * @brief スプライトの座標を設定
		 * @param pos スプライトの座標
		*/
		void SetPosition(const Vector3& pos);

		/**
		 * @brief スプライトの回転を設定
		 * @param qRot スプライトの回転
		*/
		void SetRotation(const Quaternion& qRot);

		/**
		 * @brief スプライトの拡大率を設定
		 * @param 拡大率
		*/
		void SetScale(const Vector3& scale);

		/**
		 * @brief スプライトのピボットを設定
		 * @param ピボット
		*/
		void SetPivot(const Vector2& pivot);

		/**
		 * @brief スプライトの座標を取得
		 * @return 座標
		*/
		const Vector3& GetPosition() const { return m_position; }

		/**
		 * @brief スプライトの回転を取得
		 * @return 回転
		*/
		const Quaternion& GetRotation() const { return m_qRot; }

		/**
		 * @brief スプライトの拡大率を取得
		 * @return 拡大率
		*/
		const Vector3& GetScale() const { return m_scale; }

		/**
		 * @brief スプライトのピボットを取得
		 * @return ピボット
		*/
		const Vector2& GetPivot() const { return m_pivot; }

		/**
		 * @brief スプライトの乗算カラーを設定
		 * @param mulColor 乗算カラー
		*/
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}

	private:
		Sprite m_sprite;							//スプライト
		Vector3 m_position = Vector3::Zero;			//スプライトの座標
		Quaternion m_qRot = Quaternion::Identity;	//スプライトの回転
		Vector3 m_scale = Vector3::One;				//スプライトの拡大率
		Vector2 m_pivot = { 0.5f,0.5f };			//スプライトのピボット
	};
}

