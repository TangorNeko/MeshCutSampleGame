#pragma once

namespace Game
{
	class FontRender : public IGameObject
	{
	public:
		/**
		 * @brief フォントの描画
		 * @param rc レンダーコンテキスト
		*/
		void Render(RenderContext& rc) override;

		/**
		 * @brief フォントのテキストをセット
		 * @param text テキスト
		*/
		void SetText(const std::wstring& text) { m_text = text; }

		/**
		 * @brief フォントの座標をセット
		 * @param position 座標
		*/
		void SetPosition(const Vector2& position) { m_position = position; }

		/**
		 * @brief フォントの色をセット
		 * @param 色
		*/
		void SetColor(const Vector4& color) { m_color = color; }

		/**
		 * @brief フォントの回転角度をセット
		 * @param 回転角度
		*/
		void SetRotation(const float& rotation) { m_rotation = rotation;; }

		/**
		 * @brief フォントの拡大率をセット
		 * @param scale 拡大率
		*/
		void SetScale(float scale) { m_scale = scale; }

		/**
		 * @brief フォントのピボットをセット(動作していません)
		 * @param pivot ピボット
		*/
		void SetPivot(const Vector2& pivot) { m_pivot = pivot; }

		/**
		 * @brief フォントのアウトラインの描画フラグをセット
		 * @param flag 描画する:true 描画しない:false
		*/
		void SetShadowFlag(bool flag)
		{
			m_drawShadowFlag = flag;
			m_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}

		/**
		 * @brief フォントのアウトラインの太さをセット
		 * @param offset アウトラインの太さ
		*/
		void SetShadowOffset(float offset)
		{
			m_shadowOffset = offset;
			m_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}

		/**
		 * @brief フォントのアウトラインの色をセット
		 * @param color アウトラインの色
		*/
		void SetShadowColor(const Vector4& color)
		{
			m_shadowColor = color;
			m_font.SetShadowParam(m_drawShadowFlag, m_shadowOffset, m_shadowColor);
		}

	private:
		Font m_font;

		std::wstring m_text = L"";							//表示するテキスト
		Vector2 m_position = { 0.0f,0.0f };					//表示位置
		Vector4 m_color = { 1.0f,1.0f,1.0f,1.0f };			//フォントの色
		float m_rotation = 0.0f;							//フォントの回転角度
		float m_scale = 1.0f;								//フォントの拡大率
		Vector2 m_pivot = { 0.5f,0.5f };					//フォントのピボット(動いていない?)
		bool m_drawShadowFlag = false;						//フォントのアウトラインを描くかのフラグ
		float m_shadowOffset = 1.0f;						//フォントのアウトラインの太さ
		Vector4 m_shadowColor = { 1.0f,1.0f,1.0f,1.0f };	//フォントのアウトラインの色
	};
}
