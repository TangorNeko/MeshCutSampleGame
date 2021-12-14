#include "stdafx.h"
#include "StepObject.h"

namespace
{
	const float MOVESPEED = 10.0f;
	const int MAX_LIFETIME = 400;
}

namespace Game
{
	StepObject::~StepObject()
	{
		DeleteGO(m_skinModelRender);
	}

	bool StepObject::Start()
	{
		m_skinModelRender = NewGO<SkinModelRender>(0);
		m_skinModelRender->Init("Assets/modelData/TankMissile.tkm");

		return true;
	}

	void StepObject::Update()
	{
		//移動方向への回転を計算
		m_qRot.SetRotation(Vector3::Up, m_moveDirection);
		
		//移動方向へ移動
		m_position += m_moveDirection * MOVESPEED;

		//座標と回転をセット
		m_skinModelRender->SetPosition(m_position);
		m_skinModelRender->SetRotation(m_qRot);

		//生存時間をインクリメント
		m_lifeTime++;

		//最大生存時間になったら削除
		if (m_lifeTime == MAX_LIFETIME)
		{
			DeleteGO(this);
		}
	}
}
