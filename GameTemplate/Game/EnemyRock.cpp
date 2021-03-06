#include "stdafx.h"
#include "EnemyRock.h"
#include "Player.h"
#include "HealItem.h"
#include <random>

namespace
{
	const char* PATH_ROCKMODEL = "Assets/modelData/stone.tkm";	//岩のモデルパス
	const Vector3 ROCK_SCALE = { 5.0f,5.0f,5.0f };				//岩のの拡大率
	const float ROCK_COLLISION_RADIUS = 75.0f;					//岩の当たり判定の半径
	const float ROCK_MOVESPEED = 30.0f;							//岩の移動速度
	const float ROCK_GRAVITY = 0.5f;							//岩にかかる重力の大きさ
	const int ROCK_DAMAGE = 50;									//岩のダメージ
	const int HEALDROP_MODULO_NUM = 10;							//岩の回復アイテム抽選の余剰値
}

namespace Game
{
	EnemyRock::~EnemyRock()
	{
		//斬られた時の引き渡しの時以外でデストラクタが呼ばれたらモデルを削除
		if (m_rockModel != nullptr && m_isCut == false)
		{
			DeleteGO(m_rockModel);
		}
	}

	bool EnemyRock::Start()
	{
		//岩のモデルを初期化
		m_rockModel = NewGO<SkinModelRender>(Priority::High);
		m_rockModel->Init(PATH_ROCKMODEL);
		m_rockModel->SetScale(ROCK_SCALE);

		//モデルの中心を原点にする
		m_rockModel->SetModelCenterAsOrigin();

		//モデルの所有者を設定
		m_rockModel->SetOwner(this);

		//最初から切断可能にしておく
		m_rockModel->SetDivideFlag(true);
		Game::ModelCutManager::GetInstance()->AddCuttable(m_rockModel);

		//球状の当たり判定を作成
		m_sphereTrigger.CreateSphere(m_position, Quaternion::Identity, ROCK_COLLISION_RADIUS);

		//ダメージを与えるプレイヤーを探す
		m_targetPlayer = FindGO<Player>("player");

		return true;
	}

	void EnemyRock::OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce)
	{
		//ダミーを作成
		m_rockModel->MakeDummy(cutForce);

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

	void EnemyRock::Update()
	{
		//移動処理
		m_position += m_moveDirection * ROCK_MOVESPEED;
		m_position.y -= ROCK_GRAVITY;

		//モデルと当たり判定に座標をセット
		m_rockModel->SetPosition(m_position);
		m_sphereTrigger.SetPosition(m_position);

		//プレイヤーとのヒットを確認
		PlayerHitTest();
	}

	void EnemyRock::PlayerHitTest()
	{
		PhysicsWorld::GetInstance()->ContactTest(m_targetPlayer->GetCharaCon(), [&](const btCollisionObject& contactObject)
			{
				//プレイヤーのキャラコンと自分のトリガーが接触していたら
				if (m_sphereTrigger.IsSelf(contactObject) == true) {
					//ダメージを与える
					m_targetPlayer->Damage(ROCK_DAMAGE);

					//自らを削除
					DeleteGO(this);
				}
			}
		);
	}
}
