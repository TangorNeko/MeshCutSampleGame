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
	ModelInitData cutPlanelInitData;
	cutPlanelInitData.m_fxFilePath = "Assets/shader/model.fx";
	cutPlanelInitData.m_tkmFilePath = "Assets/modelData/testCutPlane.tkm";
	cutPlanelInitData.m_modelUpAxis = enModelUpAxisZ;

	Model cutPlane;
	cutPlane.Init(cutPlanelInitData);

	ModelInitData modelInitData;
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	modelInitData.m_tkmFilePath = "Assets/modelData/teapot.tkm";
	modelInitData.m_modelUpAxis = enModelUpAxisZ;

	Model testModel;
	testModel.Init(modelInitData);
	g_camera3D->SetFar(100000.0f);

	float modelDeg = 0.0f;
	float modelX = 0.0f;
	Quaternion modelQRot = g_quatIdentity;


	Quaternion cutPlaneQRot = g_quatIdentity;
	float cutDeg = 0.0f;
	Vector3 cutNormal = { 1.0f,0.0f,0.0f };
	cutNormal.Normalize();
	Vector3 cutPoint = { 0.0f, 0.0f, 0.0f };

	g_camera3D->SetPosition(0.0f, 0.0f, -1000.0f);
	g_camera3D->SetTarget(0.0f, 0.0f, 0.0f);
	g_camera3D->SetUpdateProjMatrixFunc(g_camera3D->enUpdateProjMatrixFunc_Ortho);

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

		cutDeg += g_pad[0]->GetRStickXF();
		cutPoint.x += g_pad[0]->GetRStickYF();

		cutNormal = { 1.0f,0.0f,0.0f };
		cutPlaneQRot.SetRotationDegZ(cutDeg);
		cutPlaneQRot.Apply(cutNormal);

		cutPlane.UpdateWorldMatrix(cutPoint, cutPlaneQRot, { 1.0f,5.0f,1.0f });

		modelDeg += g_pad[0]->GetLStickYF();
		modelX += g_pad[0]->GetLStickXF();

		modelQRot.SetRotationDegY(modelDeg);
		testModel.UpdateWorldMatrix({ 0.0f,modelX,0.0f }, modelQRot, { 1.0f,1.0f,1.0f });

		//����A���b�V���p�[�c�A�}�e���A�����Ƃɐؒf��̒��_�C���f�b�N�X�o�b�t�@�̃T�C�Y��0�Ȃ�G���[��f���B
		//0�T�C�Y�̃C���f�b�N�X�o�b�t�@��GPU�ɃR�s�[���悤�Ƃ��邽��?
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			testModel.Divide(modelInitData, cutNormal, cutPoint);
		}
		
		cutPlane.Draw(renderContext);
		testModel.Draw(renderContext);
		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}

