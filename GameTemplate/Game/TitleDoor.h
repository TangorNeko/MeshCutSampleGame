#pragma once

namespace Game
{
	class TitleSprites;

	class TitleDoor : public IGameObject
	{
	public:
		~TitleDoor();

		bool Start() override;

		void OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce) override;
	private:
		SkinModelRender* m_titleDoorRender = nullptr;	//�^�C�g���p�̃h�A�̃��f��
		bool m_isCut = false;							//�ؒf���ꂽ?
		TitleSprites* m_titleSprites = nullptr;			//�^�C�g���̃X�v���C�g�N���X
	};
}

