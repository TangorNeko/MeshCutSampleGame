#include "stdafx.h"
#include "EnemyMissile.h"
#include "Player.h"
#include "HealItem.h"
#include <random>

namespace
{
	const char* PATH_MISSILEMODEL = "Assets/modelData/Missile_Bullet.tkm";	//ミサイルのモデルパス
	const float TRIGGER_HEIGHT = 300.0f;									//ミサイルの当たり判定のカプセルの高さ
	const float TRIGGER_RADIUS = 50.0f;										//ミサイルの当たり判定のカプセルの半径
	const int MISSILE_DAMAGE = 50;											//ミサイルのダメージ
	const char16_t* EFFECT_MISSILE_PATH = u"Assets/effect/MissileTail.efk";	//ミサイルのエフェクトパス
	const Vector3 EFFECT_MISSILE_SCALE = { 25.0f,25.0f,25.0f };				//ミサイルのエフェクトの拡大率
	const float EFFECT_MISSILE_ROTATEDEG = -90.0f;							//ミサイルのエフェクトの回転角度
	const int HEALDROP_MODULO_NUM = 5;										//ミサイルの回復アイテム抽選の余剰値
	const char16_t* EXPLOSION_EFFECT_PATH = u"Assets/effect/Boom.efk";		//爆発のエフェクトパス
	const Vector3 EFFECT_EXPLOSION_SCALE = { 25.0f,25.0f,25.0f };			//爆発のエフェクトの拡大率
	const wchar_t* EXPLOSION_SOUND_PATH = L"Assets/sound/ExplosionSE.wav";	//爆発音のパス
}

namespace Game
{
	EnemyMissile::~EnemyMissile()
	{
		//斬られた時の引き渡しの時以外でデストラクタが呼ばれたらモデルを削除
		if (m_missileRender != nullptr && m_isCut == false)
		{
			DeleteGO(m_missileRender);
		}

		//ミサイルのエフェクトを停止して削除
		m_missileEffect->Stop();
		DeleteGO(m_missileEffect);
	}

	bool EnemyMissile::Start()
	{
		//モデルを作成
		m_missileRender = NewGO<SkinModelRender>(Priority::High);
		m_missileRender->Init(PATH_MISSILEMODEL);

		//OnDivide関数で斬られた時の処理を記述するために所有者を設定
		m_missileRender->SetOwner(this);

		//最初から斬れるようにしておく
		m_missileRender->SetDivideFlag(true);
		Game::ModelCutManager::GetInstance()->AddCuttable(m_missileRender);

		//カプセルトリガーを作成
		m_capsuleTrigger.CreateCapsule(m_position, m_qRot, TRIGGER_RADIUS, TRIGGER_HEIGHT);

		//ターゲットを探す
		m_trackingPlayer = FindGO<Player>("player");

		//ミサイルの移動クラスにターゲットを渡す
		m_missileMove.SetTarget(m_trackingPlayer);

		//ミサイルのエフェクトを作成
		m_missileEffect = NewGO<Effect>(Priority::High);
		m_missileEffect->Init(EFFECT_MISSILE_PATH);
		m_missileEffect->SetScale(EFFECT_MISSILE_SCALE);
		m_missileEffect->Play();
		return true;
	}

	void EnemyMissile::Update()
	{
		//ミサイルの移動処理
		m_missileMove.Update(m_position);

		//ミサイルの回転処理
		m_missileRotation.Update(m_qRot, m_missileMove.GetDirection());

		//モデルの座標と回転をセット
		m_missileRender->SetPosition(m_position);
		m_missileRender->SetRotation(m_qRot);

		//カプセルトリガーの座標と回転をセット
		m_capsuleTrigger.SetPosition(m_position);
		m_capsuleTrigger.SetRotation(m_qRot);

		//ミサイルのエフェクトの回転をセット
		Quaternion MissileRot;
		MissileRot.SetRotationDegX(EFFECT_MISSILE_ROTATEDEG);
		MissileRot.Multiply(m_qRot);
		m_missileEffect->SetPosition(m_position);
		m_missileEffect->SetRotation(MissileRot);

		//プレイヤーとのヒットを確認
		PlayerHitTest();
	}

	void EnemyMissile::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		//ダミーを作成
		m_missileRender->MakeDummy(cutForce);

		//カットされたフラグをオンに
		m_isCut = true;

		//切断された際確率で回復アイテムをドロップする
		std::random_device seed_gen;
		std::mt19937 engine(seed_gen());
		int random = engine();

		if (random % HEALDROP_MODULO_NUM == 0)
		{
			HealItem* healItem = NewGO<HealItem>(Priority::High,"healItem");
			healItem->SetPosition(m_position);
		}
		//モデルレンダーをダミークラスに引き渡したので削除
		DeleteGO(this);
	}

	void EnemyMissile::PlayerHitTest()
	{
		PhysicsWorld::GetInstance()->ContactTest(m_trackingPlayer->GetCharaCon(), [&](const btCollisionObject& contactObject)
			{
				//追いかけているプレイヤーのキャラコンと自分のトリガーが接触していたら
				if (m_capsuleTrigger.IsSelf(contactObject) == true) {
					//ダメージを与える
					m_trackingPlayer->Damage(MISSILE_DAMAGE);

					//爆発エフェクトを作成
					Effect* boomEffect = NewGO<Effect>(Priority::High);
					boomEffect->Init(EXPLOSION_EFFECT_PATH);
					boomEffect->SetScale(EFFECT_EXPLOSION_SCALE);
					boomEffect->SetPosition(m_position);
					boomEffect->Play();

					//爆発サウンドを再生
					SoundOneShotPlay(EXPLOSION_SOUND_PATH);

					//自らを削除
					DeleteGO(this);
				}
			}
		);
	}
}