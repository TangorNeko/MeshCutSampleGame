#include "stdafx.h"
#include "ShadowRender.h"

namespace Graphics
{
	void ShadowRender::Init()
	{
		//�V���h�E�}�b�v�̍쐬�B
		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		m_shadowMap.Create(
			2048,
			2048,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}


	void ShadowRender::Draw(RenderContext rc)
	{
		//�V���h�E�}�b�v���g�p�ł���܂őҋ@
		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
		rc.SetRenderTargetAndViewport(m_shadowMap);
		rc.ClearRenderTargetView(m_shadowMap.GetRTVCpuDescriptorHandle(), m_shadowMap.GetRTVClearColor());
		rc.ClearDepthStencilView(m_shadowMap.GetDSVCpuDescriptorHandle(), m_shadowMap.GetDSVClearValue());

		//�V�U�����O��`���e�p�ɕύX
		D3D12_RECT shadowRect;
		shadowRect.left = 0;
		shadowRect.top = 0;
		shadowRect.right = 2048;
		shadowRect.bottom = 2048;
		rc.SetScissorRect(shadowRect);

		rc.SetRenderStep(RenderContext::enShadow);

		//�V���h�E�}�b�v�֏�������
		GameObjectManager::GetInstance()->ExecuteRender(rc,Light::LightManager::GetInstance()->GetLightCamera());

		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

		//�V�U�����O��`��߂�
		D3D12_RECT normalRect;
		normalRect.left = 0;
		normalRect.top = 0;
		normalRect.right = 1280;
		normalRect.bottom = 720;
		rc.SetScissorRect(normalRect);

		//�����_�[�^�[�Q�b�g���t���[���o�b�t�@�ɖ߂�
		g_graphicsEngine->ChangeRenderTargetToFrameBuffer(rc);
	}
}
