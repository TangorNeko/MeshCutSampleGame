#include "stdafx.h"
#include "system/system.h"
#include "TriangleDivider.h"


///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//NOTE:�e�X�g�R�[�h

	ModelInitData modelInitData;
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	modelInitData.m_tkmFilePath = "Assets/modelData/teapot.tkm";
	modelInitData.m_modelUpAxis = enModelUpAxisZ;

	Model testModel;
	testModel.Init(modelInitData);
	g_camera3D->SetFar(100000.0f);

	float deg = 0.0f;
	float x = 0.0f;
	Quaternion testQRot = g_quatIdentity;

	Vector3 cutNormal = { 1.0f,0.0f,0.0f };
	cutNormal.Normalize();
	Vector3 cutPoint = { 0.0f, 25.0f, 0.0f };

	g_camera3D->SetPosition(0.0f, 0.0f, -1000.0f);
	g_camera3D->SetTarget(0.0f, 0.0f, 0.0f);

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		g_engine->BeginFrame();


		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////

		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		testModel.Draw(renderContext);

		deg += g_pad[0]->GetRStickXF();
		x += g_pad[0]->GetLStickXF();

		testQRot.SetRotationDegY(deg);

		testModel.UpdateWorldMatrix({ x,0.0f,0.0f }, testQRot, { 1.0f,1.0f,1.0f });

		if (g_pad[0]->IsTrigger(enButtonA))
		{
			testModel.Divide(modelInitData, cutNormal, cutPoint);
		}
		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}

