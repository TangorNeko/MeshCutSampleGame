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
		SkinModelRender* m_titleDoorRender = nullptr;	//タイトル用のドアのモデル
		bool m_isCut = false;							//切断された?
		TitleSprites* m_titleSprites = nullptr;			//タイトルのスプライトクラス
	};
}

