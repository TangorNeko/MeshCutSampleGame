#include "stdafx.h"
#include "EnemyMissile.h"
#include "Player.h"
#include "HealItem.h"
#include <random>

namespace
{
	const char* PATH_MISSILEMODEL = "Assets/modelData/TankMissile.tkm";
	const float TRIGGER_HEIGHT = 300.0f;
	const float TRIGGER_RADIUS = 50.0f;
	const int MISSILE_DAMAGE = 50;
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
	}

	bool EnemyMissile::Start()
	{
		//モデルを作成
		m_missileRender = NewGO<SkinModelRender>(0);
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

		//プレイヤーとのヒットを確認
		PlayerHitTest();
	}

	void EnemyMissile::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		//ダミーを作成
		m_missileRender->MakeDummy(cutForce);

		//カットされたフラグをオンに
		m_isCut = true;

		std::random_device seed_gen;
		std::mt19937 engine(seed_gen());
		int random = engine();

		if (random % 5 == 0)
		{
			HealItem* healItem = NewGO<HealItem>(0);
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

					//自らを削除
					DeleteGO(this);
				}
			}
		);
	}
}