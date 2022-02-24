#include "stdafx.h"
#include "ShadowRender.h"

namespace Graphics
{
	void ShadowRender::Init()
	{
		//シャドウマップの作成。
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
		//シャドウマップが使用できるまで待機
		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
		rc.SetRenderTargetAndViewport(m_shadowMap);
		rc.ClearRenderTargetView(m_shadowMap.GetRTVCpuDescriptorHandle(), m_shadowMap.GetRTVClearColor());
		rc.ClearDepthStencilView(m_shadowMap.GetDSVCpuDescriptorHandle(), m_shadowMap.GetDSVClearValue());

		//シザリング矩形を影用に変更
		D3D12_RECT shadowRect;
		shadowRect.left = 0;
		shadowRect.top = 0;
		shadowRect.right = 2048;
		shadowRect.bottom = 2048;
		rc.SetScissorRect(shadowRect);

		rc.SetRenderStep(RenderContext::enShadow);

		//シャドウマップへ書き込み
		GameObjectManager::GetInstance()->ExecuteRender(rc,Light::LightManager::GetInstance()->GetLightCamera());

		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

		//シザリング矩形を戻す
		D3D12_RECT normalRect;
		normalRect.left = 0;
		normalRect.top = 0;
		normalRect.right = 1280;
		normalRect.bottom = 720;
		rc.SetScissorRect(normalRect);

		//レンダーターゲットをフレームバッファに戻す
		g_graphicsEngine->ChangeRenderTargetToFrameBuffer(rc);
	}
}
