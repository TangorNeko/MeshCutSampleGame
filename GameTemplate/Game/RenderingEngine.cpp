#include "stdafx.h"
#include "RenderingEngine.h"

namespace Graphics
{
	RenderingEngine* g_renderingEngine = nullptr;	//�����_�����O

	void RenderingEngine::Init()
	{
		g_renderingEngine = this;

		m_shadowRender.Init();
	}

	void RenderingEngine::Render(RenderContext& rc)
	{
		//�V���h�E�}�b�v�̕`��
		m_shadowRender.Draw(rc);

		//TODO:���݂̓t�H���[�h�����_�����O�A�ύX����Ȃ�N���X����
		SetDefaultViewPort(rc);
		rc.SetRenderStep(RenderContext::enModel);
		GameObjectManager::GetInstance()->ExecuteRender(rc,g_camera3D);
	}

	void RenderingEngine::SetDefaultViewPort(RenderContext& rc)
	{
		//�r���[�|�[�g���f�t�H���g�̂��̂ɖ߂�
		D3D12_VIEWPORT defaultViewPort;
		defaultViewPort.TopLeftX = 0;
		defaultViewPort.TopLeftY = 0;
		defaultViewPort.Width = static_cast<FLOAT>(FRAME_BUFFER_W);
		defaultViewPort.Height = static_cast<FLOAT>(FRAME_BUFFER_H);
		defaultViewPort.MinDepth = D3D12_MIN_DEPTH;
		defaultViewPort.MaxDepth = D3D12_MAX_DEPTH;

		rc.SetViewport(defaultViewPort);
	}
}
