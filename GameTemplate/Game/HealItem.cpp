#include "stdafx.h"
#include "HealItem.h"
#include "Player.h"

namespace
{
	const char* PATH_MODEL = "Assets/modelData/MediKit.tkm";		//回復アイテムのモデルパス
	const Vector3 SCALE_COLLIDER = { 10.0f,10.0f,10.0f };			//回復アイテムの当たり判定の拡大率
	const float PICKUP_RANGE = 200.0f;								//回復アイテムを拾える範囲
	const int HEAL_HP = 25;											//回復するHP量
	const wchar_t* HEAL_SOUND_PATH = L"Assets/sound/HealSE.wav";	//回復の効果音パス
	const float HEAL_RIGIDBODY_MASS = 500.0f;						//剛体の質量
	const float HEAL_RIGIDBODY_LOCALINTERIA = 0.0f;					//剛体の回転のしやすさ
	const float HEAL_RIGIDBODY_FRICTION = 10.0f;					//剛体の摩擦力
	const float HEAL_RIGIDBODY_LINEARFACTOR_X = 0.0f;				//剛体のX軸方向の移動可能量
	const float HEAL_RIGIDBODY_LINEARFACTOR_Y = 1.0f;				//剛体のY軸方向の移動可能量
	const float HEAL_RIGIDBODY_LINEARFACTOR_Z = 0.0f;				//剛体のZ軸方向の移動可能量
}

namespace Game
{
	HealItem::~HealItem()
	{
		DeleteGO(m_skinModelRender);
	}

	bool HealItem::Start()
	{
		m_skinModelRender = NewGO<SkinModelRender>(Priority::High);
		m_skinModelRender->Init(PATH_MODEL);

		//カプセル状のコライダーを作成
		m_boxCollider.Create(SCALE_COLLIDER);

		//球状のコライダーを作成
		RigidBodyInitData rbInitData;

		//重量をセット(サンプルのまま)
		rbInitData.mass = HEAL_RIGIDBODY_MASS;

		//コライダーをセット
		rbInitData.collider = &m_boxCollider;

		//初期座標と回転をセット
		rbInitData.pos = m_position;

		//回転のしやすさを設定する
		rbInitData.localInteria.Set(
			HEAL_RIGIDBODY_LOCALINTERIA,
			HEAL_RIGIDBODY_LOCALINTERIA,
			HEAL_RIGIDBODY_LOCALINTERIA
		);

		m_rigidBody.Init(rbInitData);
		//摩擦力を設定する。(0～10、サンプルのまま)
		m_rigidBody.SetFriction(HEAL_RIGIDBODY_FRICTION);
		//線形移動する要素を設定する。
		//0を指定した軸は移動しない。
		m_rigidBody.SetLinearFactor(HEAL_RIGIDBODY_LINEARFACTOR_X, HEAL_RIGIDBODY_LINEARFACTOR_Y, HEAL_RIGIDBODY_LINEARFACTOR_Z);

		m_player = FindGO<Player>("player");

		return true;
	}
	
	void HealItem::Update()
	{
		//回転角度を増加
		m_rotateDegY++;

		//物理挙動後の座標を取得
		m_rigidBody.GetPositionAndRotation(m_position, m_qRot);

		//回転角度を適用
		m_qRot.SetRotationDegY(m_rotateDegY);

		//モデルに座標と回転をセット
		m_skinModelRender->SetPosition(m_position);
		m_skinModelRender->SetRotation(m_qRot);

		PickedUp();
	}

	void HealItem::PickedUp()
	{
		//プレイヤーとの距離を測定
		Vector3 distance = m_player->GetPosition() - m_position;

		//拾える距離ならHPを回復し、自分を削除
		if (distance.LengthSq() < PICKUP_RANGE * PICKUP_RANGE)
		{
			m_player->Heal(HEAL_HP);
			SoundOneShotPlay(HEAL_SOUND_PATH);
			DeleteGO(this);
		}
	}
}
