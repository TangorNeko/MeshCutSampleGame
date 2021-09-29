#include "stdafx.h"
#include "system/system.h"
#include "HitTest.h"


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
	Util::PlaneData plane;
	Util::TriangleData triangle;

	plane.SetPoint({ 0.0f,1.0f,0.0f });
	plane.SetNormal({ 2.0f,1.0f,0.0f });

	triangle.vertices[0] = { -2.0f,-5.0f,0.0f };
	triangle.vertices[1] = { 7.0f,-5.0f,0.0f };
	triangle.vertices[2] = { 0.0f,1.0f,0.0f };

	std::array<Vector3, 2> newpointArray;
	
	Util::TriangleDivider divider;
	divider.SetPlane(plane);
	divider.Divide(triangle);
	if (divider.IsPlaneDivideTriangle() == true)
	{
		int newpoint = divider.GetDividedPoint(newpointArray);
		OutputDebugStringA("Divided");
	}

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

