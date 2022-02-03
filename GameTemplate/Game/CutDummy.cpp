#include "stdafx.h"
#include "CutDummy.h"

namespace
{
	const int TIME_DITHER_START = 50;
	const int FRAME_DELETE = 360;
	const float DUMMY_CAPSULE_COLLISION_DIVIDENUM = 3.0f;
	const float DUMMY_RIGIDBODY_MASS = 100.0f;
	const float DUMMY_RIGIDBODY_LOCALINTERIA = 0.1f;
	const float DUMMY_RIGIDBODY_FRICTION = 10.0f;
	const float DUMMY_RIGIDBODY_LINEARFACTOR = 1.0f;
}

namespace Game
{
	CutDummy::~CutDummy()
	{
		//カットされていなかったらモデルを削除
		if (m_isCut == false)
		{
			DeleteGO(m_dummyModel);
		}

		//剛体は常に削除
		delete m_rigidBody;
	}

	bool CutDummy::Start()
	{
		m_rigidBody = new RigidBody;

		//カプセル状のコライダーを作成
		m_capsuleCollider.Init(m_dummyRadius/ DUMMY_CAPSULE_COLLISION_DIVIDENUM,m_dummyHeight/ DUMMY_CAPSULE_COLLISION_DIVIDENUM);
		
		//球状のコライダーを作成
		RigidBodyInitData rbInitData;

		//重量をセット(サンプルのまま)
		rbInitData.mass = DUMMY_RIGIDBODY_MASS;

		//コライダーをセット
		rbInitData.collider = &m_capsuleCollider;

		//モデルの回転を保存しておく
		m_modelRot = m_dummyModel->GetRotation();

		//初期座標と回転をセット
		rbInitData.pos = m_dummyModel->GetPosition();
		rbInitData.rot = m_capsuleRot;

		//回転のしやすさを設定する。(0～1、サンプルのまま)
		rbInitData.localInteria.Set(
			DUMMY_RIGIDBODY_LOCALINTERIA,
			DUMMY_RIGIDBODY_LOCALINTERIA,
			DUMMY_RIGIDBODY_LOCALINTERIA
		);

		m_rigidBody->Init(rbInitData);
		//摩擦力を設定する。(0～10、サンプルのまま)
		m_rigidBody->SetFriction(DUMMY_RIGIDBODY_FRICTION);
		//線形移動する要素を設定する。
		//0を指定した軸は移動しない。
		m_rigidBody->SetLinearFactor(DUMMY_RIGIDBODY_LINEARFACTOR, DUMMY_RIGIDBODY_LINEARFACTOR, DUMMY_RIGIDBODY_LINEARFACTOR);

		m_rigidBody->AddForce(cutForce, m_dummyModel->GetPosition());
		//所有者をセット
		m_dummyModel->SetOwner(this);
		return true;
	}

	void CutDummy::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		//分割後の元のモデルをダミーとして作成
		m_dummyModel->MakeDummy(cutForce);

		//カットされたフラグをオン
		m_isCut = true;

		//ダミーとして新しく作成されたので、自身のクラスは不要
		DeleteGO(this);
	}

	void CutDummy::Update()
	{
		//時間のカウントを増加
		m_timer++;

		if (m_timer < FRAME_DELETE - TIME_DITHER_START)
		{
			//剛体の座標と回転を受け取り
			m_rigidBody->GetPositionAndRotation(m_position, m_rigidBodyRot);

			//座標はそのままモデルに適用
			m_dummyModel->SetPosition(m_position);

			//rotは上方向からカプセル方向への回転
			//m_toModelRotはその回転を逆にした回転なので、
			//m_toModelRotを乗算する事によって常にモデルは上向きになる。
			m_rigidBodyRot.Multiply(m_toModelRot, m_rigidBodyRot);

			//そこに元のモデルの回転を乗算することでカプセルとモデルの回転が整合する
			m_rigidBodyRot.Multiply(m_modelRot, m_rigidBodyRot);

			//回転を適用
			m_dummyModel->SetRotation(m_rigidBodyRot);
		}

		if (m_timer == FRAME_DELETE - TIME_DITHER_START)
		{
			//ディザリングが始まったら剛体は削除する
			delete m_rigidBody;
			m_rigidBody = nullptr;

			//切断不可に
			m_dummyModel->SetDivideFlag(false);
		}

		//少しずつディザリング
		if (m_timer >= FRAME_DELETE - TIME_DITHER_START)
		{
			m_dummyModel->DitherProgress();
		}

		//削除フレームになれば削除
		if (m_timer == FRAME_DELETE)
		{
			DeleteGO(this);
		}
	}
}