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

	////NOTE:�e�X�g�R�[�h
	//Util::PlaneData plane;
	//Util::TriangleData triangle;

	//plane.SetPoint({ 0.0f,1.0f,0.0f });
	//plane.SetNormal({ 2.0f,1.0f,0.0f });

	//triangle.vertexIndexes[0] = 0;
	//triangle.vertexIndexes[1] = 1;
	//triangle.vertexIndexes[2] = 2;

	//std::array<Vector3, 2> newpointArray;
	//
	//Util::TriangleDivider divider;
	//std::map<std::pair<Vector3, Vector3>, Vector3> newvertexContainer;
	//divider.Init(plane,&newvertexContainer);
	//divider.Divide(triangle);
	//divider.GetCrossPoint({ 7.0f,-5.0f,0.0f }, { -2.0f,-5.0f,0.0f });
	//if (divider.IsPlaneDivideTriangle() == Util::Divided_1OnPlane)
	//{
	//	int newpoint = divider.GetDividedPoint(newpointArray);
	//	OutputDebugStringA("Divided");
	//}

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

		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}

