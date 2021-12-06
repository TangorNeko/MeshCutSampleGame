#include "stdafx.h"
#include "EnemyMissile.h"
#include "Player.h"

namespace
{
	const char* PATH_MISSILEMODEL = "Assets/modelData/TankMissile.tkm";
	const float TRIGGER_HEIGHT = 300.0f;
	const float TRIGGER_RADIUS = 50.0f;
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
		m_missileMove.Update(m_position);
		m_missileRotation.Update(m_qRot, m_missileMove.GetDirection());

		//モデルの座標と回転をセット
		m_missileRender->SetPosition(m_position);
		m_missileRender->SetRotation(m_qRot);

		//カプセルトリガーの座標と回転をセット
		m_capsuleTrigger.SetPosition(m_position);
		m_capsuleTrigger.SetRotation(m_qRot);

		PlayerHitTest();
	}

	void EnemyMissile::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		//ダミーを作成
		m_missileRender->MakeDummy(cutForce);

		//カットされたフラグをオンに
		m_isCut = true;

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
					m_trackingPlayer->Damage(50);

					//自らを削除
					DeleteGO(this);
				}
			}
		);
	}
}