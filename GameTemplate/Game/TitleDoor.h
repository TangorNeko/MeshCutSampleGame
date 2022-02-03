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

		void Update() override;

	private:
		SkinModelRender* m_titleDoorRender = nullptr;
		bool m_isCut = false;
		TitleSprites* m_titleSprites = nullptr;
	};
}

