#include "stdafx.h"
#include "BossTank.h"
#include "Player.h"
#include "Explosion.h"

namespace
{
	const char* MODEL_PATH_BASE = "Assets/modelData/TankBase.tkm";					//車体のモデルパス
	const char* MODEL_PATH_TURRET = "Assets/modelData/TankTurret.tkm";				//砲塔のモデルパス
	const char* MODEL_PATH_CANNONRIGHT = "Assets/modelData/TankCannonRight.tkm";	//右砲身のモデルパス
	const char* MODEL_PATH_CANNONLEFT = "Assets/modelData/TankCannonLeft.tkm";		//左砲身のモデルパス
	const int MAX_HP = 400;															//最大HP
	const char16_t* WARPEFFECT_PATH = u"Assets/effect/Teleport.efk";				//出現エフェクトのパス
	const Vector3 CANNON_VECTOR_TOCORE = { 0.0f,175.0f,-100.0f };					//コアへのベクトル
	const Vector3 TO_RIGHTCANNON_VECTOR = { 160.0f,250.0f,0.0f };					//右砲身へのベクトル
	const Vector3 TO_LEFTCANNON_VECTOR = { -160.0f,250.0f,0.0f };					//左砲身へのベクトル
	const Vector3 TO_FRONT_VECTOR = { 0.0f, 190.0f, 300.0f };						//ボス正面へのベクトル
	const float PLAYER_BACKHANDSPRING_POWER = 5.0f;									//切断された際プレイヤーへ後転させる強さ
}

namespace Game
{
	BossTank::~BossTank()
	{
		if (m_bossTankStatus.isTurretBreak == false && m_bossTankStatus.isBaseBreak == false)
		{
			//車体と砲塔が切断されていない場合削除(切断されている場合はダミーにモデルが移譲されているため削除しない)
			DeleteGO(m_baseRender);
			DeleteGO(m_turretRender);
			DeleteGO(m_rightCannonRender);
			DeleteGO(m_leftCannonRender);
		}
	}

	bool BossTank::Start()
	{
		//車体、砲塔、砲身を別々のモデルとして生成
		m_baseRender = NewGO<SkinModelRender>(Priority::High);
		m_turretRender = NewGO<SkinModelRender>(Priority::High);
		m_rightCannonRender = NewGO<SkinModelRender>(Priority::High);
		m_leftCannonRender = NewGO<SkinModelRender>(Priority::High);

		//初期化
		m_baseRender->Init(MODEL_PATH_BASE);
		m_turretRender->Init(MODEL_PATH_TURRET);
		m_rightCannonRender->Init(MODEL_PATH_CANNONRIGHT);
		m_leftCannonRender->Init(MODEL_PATH_CANNONLEFT);

		//砲身のコア(切断された場合に残したい場所の座標)を設定
		m_rightCannonRender->SetToCoreVector(CANNON_VECTOR_TOCORE);
		m_leftCannonRender->SetToCoreVector(CANNON_VECTOR_TOCORE);

		//モデルの所有者を設定
		m_baseRender->SetOwner(this);
		m_turretRender->SetOwner(this);
		m_rightCannonRender->SetOwner(this);
		m_leftCannonRender->SetOwner(this);

		//当たり判定の初期化
		m_bossTankCollision.Init(m_bossTankStatus.position, m_bossTankStatus.baseRot);

		//体力等の表示の初期化
		m_bossTankDisplay.Init(m_bossTankStatus.hp);

		//タスクの登録
		m_bossTankBehave.SubmitTo(this);

		//出現エフェクトを再生
		Game::Effect* spawnEffect = NewGO<Game::Effect>(Priority::High);
		spawnEffect->SetPosition(m_bossTankStatus.position);
		spawnEffect->Init(WARPEFFECT_PATH);
		spawnEffect->Play();

		return true;
	}

	void BossTank::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		Player* player = FindGO<Player>("player");

		//斬られたのが砲身なら
		if ((skinModelRender == m_rightCannonRender || skinModelRender == m_leftCannonRender) && m_bossTankStatus.isCannonBreak == false)
		{
			//砲身が壊れたフラグをオン、行動を分岐させるのに使う
			m_bossTankStatus.isCannonBreak = true;

			m_bossTankBehave.TerminateTask();

			player->NoticeMissileMoveEnd();
		}

		//NOTE:baseRenderとturretRenderはおそらく同じくフレームで切断されるが、
		//どちらも切断された時のみDeleteする。

		//斬られたのが車体なら
		if (skinModelRender == m_baseRender)
		{
			//元々の車体のモデルをダミーとして生成
			m_baseRender->MakeDummy(cutForce);
			player->NoticeFrontMoveEnd();
			m_bossTankStatus.isBaseBreak = true;
		}

		//斬られたのが砲塔なら
		if (skinModelRender == m_turretRender)
		{
			//元々の砲塔のモデルをダミーとして作成
			m_turretRender->MakeDummy(cutForce);
			player->NoticeFrontMoveEnd();
			m_bossTankStatus.isTurretBreak = true;
		}

		//砲塔と本体がどちらも斬られたら完全にダミーに移行する
		if (m_bossTankStatus.isTurretBreak && m_bossTankStatus.isBaseBreak)
		{
			//残っていた砲身もダミーに
			m_rightCannonRender->MakeDummy(cutForce);
			m_leftCannonRender->MakeDummy(cutForce);

			//プレイヤーに後転させる
			Vector3 toResPos = player->GetPosition() - m_bossTankStatus.position;
			toResPos.y = 0.0f;
			toResPos.Normalize();
			player->BackHandSpring(toResPos * PLAYER_BACKHANDSPRING_POWER);
			player->StartFinishCamera();

			//爆発エフェクトの作成
			Explosion* explosion = NewGO<Explosion>(Priority::High);
			explosion->SetPosition(m_bossTankStatus.position);

			//すべてダミーになったのでボスとしての挙動は不要なので削除
			DeleteGO(this);
		}
	}

	void BossTank::Update()
	{
		//トドメ用のコマンドを表示するかのチェック
		m_bossTankFinishCommand.Execute(m_bossTankStatus.position, m_bossTankStatus.hp);

		//行動分岐
		m_bossTankBehave.Execute(m_bossTankStatus);

		//砲塔の回転角度から回転を作成
		m_bossTankStatus.turretRot.SetRotationDegY(m_bossTankStatus.turretDeg);
		//車体の回転角度から回転を作成
		m_bossTankStatus.baseRot.SetRotationDegY(m_bossTankStatus.baseDeg);

		//砲塔の回転に車体の回転をかけ合わせる(車体の回転に砲塔が追従するため)
		m_bossTankStatus.turretRot.Multiply(m_bossTankStatus.baseRot, m_bossTankStatus.turretRot);

		//計算した回転をセット
		m_baseRender->SetRotation(m_bossTankStatus.baseRot);
		m_turretRender->SetRotation(m_bossTankStatus.turretRot);
		m_rightCannonRender->SetRotation(m_bossTankStatus.turretRot);
		m_leftCannonRender->SetRotation(m_bossTankStatus.turretRot);

		//座標をセット
		m_baseRender->SetPosition(m_bossTankStatus.position);
		m_turretRender->SetPosition(m_bossTankStatus.position);
		m_rightCannonRender->SetPosition(m_bossTankStatus.position);
		m_leftCannonRender->SetPosition(m_bossTankStatus.position);

		//当たり判定の更新
		m_bossTankCollision.Update(m_bossTankStatus.position, m_bossTankStatus.baseRot);

		//体力等の情報の更新
		m_bossTankDisplay.Update(m_bossTankStatus.hp, m_bossTankStatus.position);

		//斬られたのが車体なら
		if (m_bossTankStatus.isBaseBreak == true && m_bossTankStatus.isTurretBreak == false)
		{
			//元々の砲塔のモデルをダミーとして作成
			m_turretRender->MakeDummy(Vector3::Zero);
			m_bossTankStatus.isTurretBreak = true;

			//残っていた砲身もダミーに
			m_rightCannonRender->MakeDummy(Vector3::Zero);
			m_leftCannonRender->MakeDummy(Vector3::Zero);

			//すべてダミーになったのでボスとしての挙動は不要なので削除
			DeleteGO(this);
		}

		//斬られたのが砲塔なら
		if (m_bossTankStatus.isTurretBreak == true && m_bossTankStatus.isBaseBreak == false)
		{
			//元々の車体のモデルをダミーとして生成
			m_baseRender->MakeDummy(Vector3::Zero);
			m_bossTankStatus.isBaseBreak = true;

			//残っていた砲身もダミーに
			m_rightCannonRender->MakeDummy(Vector3::Zero);
			m_leftCannonRender->MakeDummy(Vector3::Zero);

			//すべてダミーになったのでボスとしての挙動は不要なので削除
			DeleteGO(this);
		}
	}

	void BossTank::Damage(int damage)
	{
		//体力を減らす
		m_bossTankStatus.hp -= damage;

		//体力半分で砲身切断可能に
		if (m_bossTankStatus.hp <= MAX_HP/2)
		{
			m_rightCannonRender->SetDivideFlag(true);
			m_leftCannonRender->SetDivideFlag(true);
		}
	
		//体力0なら砲塔と本体も切断可能に
		if (m_bossTankStatus.hp <= 0)
		{
			m_bossTankStatus.hp = 0;
			m_baseRender->SetDivideFlag(true);
			m_turretRender->SetDivideFlag(true);
			m_bossTankBehave.TerminateTask();
		}
	}

	Vector3 BossTank::GetRightCannonPosition()
	{
		Vector3 toRightCannonVec = TO_RIGHTCANNON_VECTOR;

		//元々の右砲身へのベクトルに現在の回転を適用
		m_bossTankStatus.turretRot.Apply(toRightCannonVec);

		//ボスの座標にベクトルを加算して戻す
		return m_bossTankStatus.position + toRightCannonVec;
	}

	Vector3 BossTank::GetLeftCannonPosition()
	{
		Vector3 toLeftCannonVec = TO_LEFTCANNON_VECTOR;

		//元々の左砲身へのベクトルに現在の回転を適用
		m_bossTankStatus.turretRot.Apply(toLeftCannonVec);

		//ボスの座標にベクトルを加算して戻す
		return m_bossTankStatus.position + toLeftCannonVec;
	}

	Vector3 BossTank::GetFrontPosition()
	{
		Vector3 toFrontVec = TO_FRONT_VECTOR;

		//元々のボス正面へのベクトルに現在の回転を適用
		m_bossTankStatus.baseRot.Apply(toFrontVec);

		//ボスの座標にベクトルを加算して戻す
		return m_bossTankStatus.position + toFrontVec;
	}

	void BossTank::AllowCannonCut()
	{
		//左右砲身を切断可能モデルリストに追加
		ModelCutManager::GetInstance()->AddCuttable(m_rightCannonRender);
		ModelCutManager::GetInstance()->AddCuttable(m_leftCannonRender);
	}

	void BossTank::AllowBodyCut()
	{
		//車体と砲塔を切断可能モデルリストに追加
		ModelCutManager::GetInstance()->AddCuttable(m_baseRender);
		ModelCutManager::GetInstance()->AddCuttable(m_turretRender);
	}
}