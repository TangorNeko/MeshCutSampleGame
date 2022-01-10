#include "stdafx.h"
#include "SpriteRender.h"

namespace Game
{
	void SpriteRender::Init(const char* spritePath, UINT width, UINT height)
	{
		//スプライトの初期化データ
		SpriteInitData initData;

		//スプライトのテクスチャパス
		initData.m_ddsFilePath[0] = spritePath;

		//スプライトの頂点シェーダーのエントリー関数名の指定
		initData.m_vsEntryPointFunc = "VSMain";

		//シェーダーファイルパスの指定
		initData.m_fxFilePath = "Assets/shader/sprite.fx";

		//スプライトの幅を指定
		initData.m_width = width;

		//スプライトの高さを指定
		initData.m_height = height;

		//スプライトのアルファブレンドモードを透過に指定
		initData.m_alphaBlendMode = AlphaBlendMode_Trans;

		//スプライトの初期化
		m_sprite.Init(initData);
	}

	void SpriteRender::Render(RenderContext& rc)
	{
		m_sprite.Draw(rc);
	}

	void SpriteRender::SetPosition(const Vector3& pos)
	{
		m_position = pos;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	void SpriteRender::SetRotation(const Quaternion& qRot)
	{
		m_qRot = qRot;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	void SpriteRender::SetScale(const Vector3& scale)
	{
		m_scale = scale;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}

	void SpriteRender::SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
		m_sprite.Update(m_position, m_qRot, m_scale, m_pivot);
	}
}