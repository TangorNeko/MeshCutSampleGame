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
		m_capsuleCollider.Init(m_dummyRadius/1.5f,m_dummyHeight/1.5f);
		
		//球状のコライダーを作成
		RigidBodyInitData rbInitData;

		//重量をセット(サンプルのまま)
		rbInitData.mass = 0.1f;

		//コライダーをセット
		rbInitData.collider = &m_capsuleCollider;

		//モデルの回転を保存しておく
		m_modelRot = m_dummyModel->GetRotation();

		//初期座標と回転をセット
		rbInitData.pos = m_dummyModel->GetPosition();
		rbInitData.rot = m_capsuleRot;

		//回転のしやすさを設定する。(0～1、サンプルのまま)
		rbInitData.localInteria.Set(
			0.1f,
			0.1f,
			0.1f
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
		
		//剛体の座標と回転を受け取り
		m_rigidBody.GetPositionAndRotation(pos, rot);

		//座標はそのままモデルに適用
		m_dummyModel->SetPosition(pos);

		//rotは上方向からカプセル方向への回転
		//m_toModelRotはその回転を逆にした回転なので、
		//m_toModelRotを乗算する事によって常にモデルは上向きになる。
		rot.Multiply(m_toModelRot,rot);

		//そこに元のモデルの回転を乗算することでカプセルとモデルの回転が整合する
		rot.Multiply(m_modelRot, rot);
		
		//回転を適用
		m_dummyModel->SetRotation(rot);
	}
}