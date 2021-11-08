#include "stdafx.h"
#include "CutDummy.h"

namespace Game
{
	CutDummy::~CutDummy()
	{

	}

	bool CutDummy::Start()
	{
		//カプセル状のコライダーを作成
		//m_capsuleCollider.Init(m_dummyRadius,m_dummyHeight * 2);
		
		//球状のコライダーを作成
		m_sphereCollider.Create(m_dummyRadius);
		RigidBodyInitData rbInitData;

		//重量をセット(サンプルのまま)
		rbInitData.mass = 0.1f;

		//コライダーをセット
		//rbInitData.collider = &m_capsuleCollider;
		rbInitData.collider = &m_sphereCollider;

		Quaternion colliderRot;
		colliderRot.Multiply(m_dummyModel->GetRotation(), colliderRot);
		colliderRot.Multiply(m_capsuleRot, colliderRot);
		//初期座標と回転をセット
		rbInitData.pos = m_dummyModel->GetPosition();
		rbInitData.rot = colliderRot;

		//回転のしやすさを設定する。(0～1、サンプルのまま)
		rbInitData.localInteria.Set(
			0.5f,
			0.5f,
			0.5f
		);

		m_rigidBody.Init(rbInitData);
		//摩擦力を設定する。(0～10、サンプルのまま)
		m_rigidBody.SetFriction(10.0f);
		//線形移動する要素を設定する。
		//0を指定した軸は移動しない。
		m_rigidBody.SetLinearFactor(1.0f, 1.0f, 1.0f);
		return true;
	}

	void CutDummy::Update()
	{
		Vector3 pos;
		Quaternion rot;
		m_rigidBody.GetPositionAndRotation(pos, rot);

		m_dummyModel->SetPosition(pos);
		rot.Multiply(m_toModelRot);
		m_dummyModel->SetRotation(rot);
	}
}