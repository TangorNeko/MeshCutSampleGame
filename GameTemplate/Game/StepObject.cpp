#include "stdafx.h"
#include "StepObject.h"

namespace
{
	const char* STEPMISSILE_MODEL_PATH = "Assets/modelData/Missile_Flat.tkm";	//足場ミサイルのモデルパス
	const float MOVESPEED = 10.0f;												//足場ミサイルの移動速度
	const int MAX_LIFETIME = 400;												//足場ミサイルの生存時間
	const char16_t* EFFECT_MISSILE_PATH = u"Assets/effect/MissileTail.efk";		//足場ミサイルのエフェクトパス
	const Vector3 EFFECT_MISSILE_SCALE = { 25.0f,25.0f,25.0f };					//足場ミサイルのエフェクトの拡大率
	const float EFFECT_MISSILE_ROTATEDEG = -90.0f;								//足場ミサイルエフェクトの回転角度
}

namespace Game
{
	StepObject::~StepObject()
	{
		//モデルを削除
		DeleteGO(m_skinModelRender);

		//ミサイルエフェクトをストップして削除
		m_missileEffect->Stop();
		DeleteGO(m_missileEffect);
	}

	bool StepObject::Start()
	{
		//ミサイルのモデルを作成
		m_skinModelRender = NewGO<SkinModelRender>(Priority::High);
		m_skinModelRender->Init(STEPMISSILE_MODEL_PATH);

		//ミサイルのエフェクトを作成
		m_missileEffect = NewGO<Effect>(Priority::High);
		m_missileEffect->Init(EFFECT_MISSILE_PATH);
		m_missileEffect->SetScale(EFFECT_MISSILE_SCALE);
		m_missileEffect->Play();

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

		//ミサイルの角度をセット
		Quaternion MissileRot;
		MissileRot.SetRotationDegX(EFFECT_MISSILE_ROTATEDEG);
		MissileRot.Multiply(m_qRot);
		m_missileEffect->SetPosition(m_position);
		m_missileEffect->SetRotation(MissileRot);

		//生存時間をインクリメント
		m_lifeTime++;

		//最大生存時間になったら削除
		if (m_lifeTime == MAX_LIFETIME)
		{
			DeleteGO(this);
		}
	}
}
