#include "stdafx.h"
#include "MiniEnemy.h"
#include "Player.h"

namespace
{
	const char* MODEL_PATH = "Assets/modelData/Drone.tkm";
	const int KNOCKDOWN_SPEED = 10;
	const int ATTACK_RANGE = 200;
	const int CHARGE_SPEED = 25;
	const float ROTATE_DEG = 15.0f;
	const int FRAME_ACTION_START = 0;
	const int FRMAE_MOVE_START = 60;
	const int FRAME_ATTACK_START = 80;
	const int FRAME_ATTACK_HIT = 105;
	const int FRAME_ACTION_END = 300;
	const int ATTACK_DAMAGE = 25;
	const Vector3 FLOATING_HEIGHT = { 0.0f,100.0f,0.0f };
}

namespace Game
{
	MiniEnemy::~MiniEnemy()
	{
		//どこも斬られていなかった場合のみモデルを削除
		if (m_isDead == false)
		{
			DeleteGO(m_baseRender);
		}
	}

	void MiniEnemy::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		//斬られたので死亡
		m_isDead = true;

		//モデルからダミー生成
		m_baseRender->MakeDummy(cutForce);

		//もう処理の必要がないので削除
		DeleteGO(this);
	}

	bool MiniEnemy::Start()
	{
		//モデルの生成
		m_baseRender = NewGO<SkinModelRender>(0);

		//初期化
		m_baseRender->Init(MODEL_PATH);

		//モデルの所有者を設定
		m_baseRender->SetOwner(this);

		//斬りやすいようにモデルのセンターをずらす
		m_baseRender->SetModelCenterAsOrigin();

		//最初から斬れるようにしておく
		m_baseRender->SetDivideFlag(true);
		ModelCutManager::GetInstance()->AddCuttable(m_baseRender);

		return true;
	}

	void MiniEnemy::Update()
	{
		//死んでいた場合何もしない
		if (m_isDead == true)
		{
			return;
		}

		//移動開始フレームなら
		if (m_actionFrame == FRMAE_MOVE_START)
		{
			//プレイヤーへの向きを計算
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - m_position;
			distance.Normalize();
			
			//移動方向はプレイヤーへの向き
			m_moveDirection = distance;

			//プレイヤーに向く
			m_baseRot.SetRotation(Vector3::Front, distance);
		}
		else if (m_actionFrame > FRMAE_MOVE_START && m_actionFrame < FRAME_ATTACK_START)
		{
			//プレイヤーに向かって移動するる
			m_position += m_moveDirection * CHARGE_SPEED;
		}
		else if (m_actionFrame == FRAME_ATTACK_HIT)
		{
			//プレイヤーに攻撃し、軽く吹き飛ばす
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - m_position;

			if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE)
			{
				distance.Normalize();
				player->KnockDown(distance * KNOCKDOWN_SPEED);
				player->Damage(ATTACK_DAMAGE);
			}
		}
		else if (m_actionFrame == FRAME_ACTION_END)
		{
			//開始フレームにリセット
			m_actionFrame = FRAME_ACTION_START;
		}

		//座標をセット
		m_baseRender->SetPosition(m_position + FLOATING_HEIGHT);

		//計算した回転をセット
		m_baseRender->SetRotation(m_baseRot);

		//経過フレームを増加させる
		m_actionFrame++;
	}
}