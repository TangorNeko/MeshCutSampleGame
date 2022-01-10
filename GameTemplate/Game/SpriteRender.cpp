#include "stdafx.h"
#include "SpriteRender.h"

namespace Game
{
	void SpriteRender::Init(const char* spritePath, UINT width, UINT height)
	{
		//�X�v���C�g�̏������f�[�^
		SpriteInitData initData;

		//�X�v���C�g�̃e�N�X�`���p�X
		initData.m_ddsFilePath[0] = spritePath;

		//�X�v���C�g�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
		initData.m_vsEntryPointFunc = "VSMain";

		//�V�F�[�_�[�t�@�C���p�X�̎w��
		initData.m_fxFilePath = "Assets/shader/sprite.fx";

		//�X�v���C�g�̕����w��
		initData.m_width = width;

		//�X�v���C�g�̍������w��
		initData.m_height = height;

		//�X�v���C�g�̃A���t�@�u�����h���[�h�𓧉߂Ɏw��
		initData.m_alphaBlendMode = AlphaBlendMode_Trans;

		//�X�v���C�g�̏�����
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