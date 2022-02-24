#include "stdafx.h"
#include "MiniEnemy.h"
#include "Player.h"

namespace
{
	const char* MODEL_PATH = "Assets/modelData/Drone.tkm";						//雑魚敵のモデルパス
	const int KNOCKDOWN_SPEED = 10;												//雑魚敵の吹き飛ばし速度
	const int ATTACK_RANGE = 200;												//雑魚敵の攻撃範囲
	const int CHARGE_SPEED = 25;												//雑魚敵の移動速度
	const int FRAME_ACTION_START = 0;											//行動開始フレーム
	const int FRMAE_MOVE_START = 60;											//移動開始フレーム
	const int FRAME_ATTACK_START = 80;											//攻撃開始フレーム
	const int FRAME_ATTACK_HIT = 105;											//攻撃の当たり判定を作成するフレーム
	const int FRAME_ACTION_END = 300;											//行動終了フレーム
	const int ATTACK_DAMAGE = 25;												//攻撃のダメージ
	const Vector3 FLOATING_HEIGHT = { 0.0f,100.0f,0.0f };						//浮遊している高さへのベクトル
	const Vector3 ATTACK_HEIGHT = { 0.0f,50.0f,0.0f };							//攻撃の中心地点へのベクトル
	const char16_t* WARPEFFECT_PATH = u"Assets/effect/Teleport.efk";			//出現エフェクトパス
	const char16_t* LIGHTNINGEFFECT_PATH = u"Assets/effect/ThunderAttack.efk";	//電流攻撃エフェクトパス
	const wchar_t* LIGHTNING_SOUND_PATH = L"Assets/sound/LightningSE.wav";		//電流攻撃の効果音パス
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
		m_baseRender = NewGO<SkinModelRender>(Priority::High);

		//初期化
		m_baseRender->Init(MODEL_PATH);

		//モデルの所有者を設定
		m_baseRender->SetOwner(this);

		//斬りやすいようにモデルのセンターをずらす
		m_baseRender->SetModelCenterAsOrigin();

		//最初から斬れるようにしておく
		m_baseRender->SetDivideFlag(true);
		ModelCutManager::GetInstance()->AddCuttable(m_baseRender);

		//出現エフェクトを再生
		Game::Effect* spawnEffect = NewGO<Game::Effect>(Priority::High);
		spawnEffect->SetPosition(m_position);
		spawnEffect->Init(WARPEFFECT_PATH);
		spawnEffect->Play();

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
		//移動中なら
		else if (m_actionFrame > FRMAE_MOVE_START && m_actionFrame < FRAME_ATTACK_START)
		{
			//プレイヤーに向かって移動するる
			m_position += m_moveDirection * CHARGE_SPEED;
		}
		//攻撃の当たり判定フレームなら
		else if (m_actionFrame == FRAME_ATTACK_HIT)
		{
			//プレイヤーに攻撃し、軽く吹き飛ばす
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - m_position;

			//電撃エフェクトを再生
			Game::Effect* attackEffect = NewGO<Game::Effect>(Priority::High);
			attackEffect->Init(LIGHTNINGEFFECT_PATH);
			attackEffect->SetPosition(m_position + ATTACK_HEIGHT);
			attackEffect->Play();

			//電撃効果音を再生
			SoundOneShotPlay(LIGHTNING_SOUND_PATH);

			//攻撃範囲内ならプレイヤーにダメージを与えて吹き飛ばす
			if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE)
			{
				distance.Normalize();
				player->KnockDown(distance * KNOCKDOWN_SPEED);
				player->Damage(ATTACK_DAMAGE);
			}
		}
		//行動終了フレームなら
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