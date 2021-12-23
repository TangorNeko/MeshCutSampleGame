#include "stdafx.h"
#include "system/system.h"
#include "TriangleDivider.h"
#include "GameScene.h"

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
	Light::LightManager::CreateInstance();
	Game::ModelCutManager::CreateInstance();
	//���\�[�X�o���N�}�l�[�W���[�̃C���X�^���X���쐬
	Engine::ResourceBankManager::CreateInstance();

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	NewGO<Game::GameScene>(0);

	//FPS�v���p�X�g�b�v�E�H�b�`
	Stopwatch stopwatch;
	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//FPS�v��(�Œ�)�p�X�g�b�v�E�H�b�`�̌v���J�n
		stopwatch.Start();

		//�����_�����O�J�n�B
		g_engine->BeginFrame();


		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////

		GameObjectManager::GetInstance()->ExecuteUpdate();
		PhysicsWorld::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());

#ifdef _DEBUGWIREFRAME
		PhysicsWorld::GetInstance()->DebugDrawWorld(renderContext);
#endif
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		
		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();

		//�X�s�����b�N���s���B
		int restTime = 0;
		do {
			stopwatch.Stop();
			restTime = 16 - (int)stopwatch.GetElapsedMillisecond();
		} while (restTime > 0);


		//�X�g�b�v�E�H�b�`�̌v���I��
		stopwatch.Stop();
		//�f���^�^�C�����X�g�b�v�E�H�b�`�̌v�����Ԃ���A�v�Z����
		g_gameTime->PushFrameDeltaTime((float)stopwatch.GetElapsed());
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();

	//���\�[�X�o���N�}�l�[�W���[���폜
	Engine::ResourceBankManager::DeleteInstance();
	return 0;
}

