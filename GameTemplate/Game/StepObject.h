#pragma once

namespace Game
{
	class StepObject : public IGameObject
	{
	public:
		~StepObject();

		bool Start() override;

		void Update() override;

		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		const Vector3& GetPosition()
		{
			return m_position;
		}
	private:
		Vector3 m_position =  Vector3::Zero;
		SkinModelRender* m_skinModelRender = nullptr;
	};
}

