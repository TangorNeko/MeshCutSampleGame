#pragma once

namespace Game
{
	class MiniEnemy : public IGameObject
	{
	public:
		~MiniEnemy();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce) override;

		/**
		 * @brief À•W‚ğİ’è
		 * @param position 
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
	private:
		SkinModelRender* m_baseRender = nullptr;		//Ô‘Ì‚Ìƒ‚ƒfƒ‹
		SkinModelRender* m_turretRender = nullptr;		//–C“ƒ‚Ìƒ‚ƒfƒ‹
		SkinModelRender* m_cannonRender = nullptr;		//–Cg‚Ìƒ‚ƒfƒ‹
		Vector3 m_position = Vector3::Zero;				//À•W
		Quaternion m_baseRot = Quaternion::Identity;	//Ô‘Ì‚Ì‰ñ“]
		Quaternion m_turretRot = Quaternion::Identity;	//–C“ƒ‚Ì‰ñ“]
		Vector3 m_moveDirection = Vector3::Zero;		//ˆÚ“®•ûŒü
		int m_actionFrame = 0;							//s“®‚ÌŒo‰ßƒtƒŒ[ƒ€
		float m_turretDeg = 0.0f;						//–C“ƒ‚ÌŠp“x
		bool m_isDead = false;							//€–S‚µ‚½?
		bool m_isBaseBreak = false;						//Ô‘Ì‚ª”j‰ó‚³‚ê‚½?
		bool m_isTurretBreak = false;					//–C“ƒ‚ª”j‰ó‚³‚ê‚½?
		bool m_isCannonBreak = false;					//–Cg‚ª”j‰ó‚³‚ê‚½?
	};
}

