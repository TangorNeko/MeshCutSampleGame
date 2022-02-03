#pragma once

namespace Game
{
	class CommandInput : public IGameObject
	{
	public:
		~CommandInput();

		bool Start() override;

		void Update() override;

		void Pause() override;

		/**
		 * @brief �R�}���h�\���̃X�v���C�g���t�F�[�Y�A�E�g������
		*/
		void FadeOut();

		/**
		 * @brief �R�}���h���͂�����������?
		 * @return �R�}���h���͂�����������
		*/
		bool isSuccess()
		{
			return m_isSuccess;
		}
	private:
		SpriteRender* m_commandSprite = nullptr;	//�R�}���h�̃X�v���C�g
		bool m_isSuccess = false;					//�R�}���h���͂���������?
		int m_fadeFrame = 0;						//�t�F�[�h�A�E�g�̐i�x
	};
}

