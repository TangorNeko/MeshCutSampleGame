#include "stdafx.h"
#include "RenderingEngine.h"

namespace Graphics
{
	void RenderingEngine::Init()
	{
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
