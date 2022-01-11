#include "stdafx.h"
#include "Player.h"
#include "GameOverNotifier.h"

namespace
{
	const char* PATH_PLAYER_MODEL = "Assets/modelData/Player.tkm";
	const char* PATH_PLAYER_SKELETON = "Assets/modelData/Player.tks";
}

namespace Game
{
	Player::~Player()
	{
		DeleteGO(m_playerModel);
	}

	bool Player::Start()
	{
		//モデルを生成
		m_playerModel = NewGO<SkinModelRender>(0);

		//アニメーションを初期化
		m_playerAnimation.InitAnim();

		//モデルの初期化
		m_playerModel->Init(PATH_PLAYER_MODEL, PATH_PLAYER_SKELETON,m_playerAnimation.GetAnimationClip(),m_playerAnimation.GetAnimClipNum(),enModelUpAxisZ);
		m_playerModel->SetPosition(m_position);

		//プレイヤーの移動クラスの初期化
		m_playerMove.Init(m_position);

		//プレイヤーのHP表示
		m_playerDisplay.Init(m_hp);

		//初期の向きをセット
		m_playerMove.SetPlayerDirection(Vector3::Back);
		return true;
	}

	void Player::Update()
	{
		//プレイヤーのHP表示の更新
		m_playerDisplay.Update(m_hp);

		bool isCutMode = g_pad[0]->IsPress(enButtonLB1);

		//移動
		//NOTE:仮。LB1押している時は移動させない
		if (isCutMode == false && m_eventCut == false)
		{
			m_eventCut = m_playerMove.Move(m_position,m_playerAnimationParam);
			m_playerAnimationParam.isCutMode = false;
		}
		else
		{
			m_playerAnimationParam.isCutMode = true;
		}

		m_playerCut.SetCutEvent(m_eventCut,m_position);

		//TODO:仮のガードアニメ　後からクラス分離
		if (g_pad[0]->IsPress(enButtonRB1))
		{
			m_playerAnimationParam.isGuarding = true;
		}
		else
		{
			m_playerAnimationParam.isGuarding = false;
		}

		if (g_pad[0]->IsPress(enButtonA))
		{
			m_playerAnimationParam.isUnequip = true;

			g_camera3D->SetPosition({ 0.0f,250.0f,-700.0f });

			g_camera3D->SetTarget({ 0.0f,0.0f,-1300.0f });


		}
		else {
			if (isCutMode == false && m_eventCut == false)
			{
				//カメラの移動
				m_playerCamera.Update(m_position);
			}
			else
			{
				//切断モード用カメラの移動
				Vector3 newDirection = m_playerCamera.UpdateCutMode(m_position, m_playerMove.GetPlayerDirection());

				//切断モードカメラで視点を変更するとプレイヤーの向きも変更されるので新しい向きを格納
				m_playerMove.SetPlayerDirection(newDirection);
			}
		}
		
		//プレイヤーの向きから回転を計算
		m_playerMove.CalcToModelDirectionQRot();

		//プレイヤーの向きにモデルを向ける
		m_playerModel->SetRotation(m_playerMove.GetPlayerDirectionRot());

		//攻撃のアップデート
		m_playerAttack.Update(m_position, m_playerAnimationParam,m_playerMove.GetPlayerDirectionRot());
		
		//切断のアップデート
		m_playerCut.Update(m_position, m_playerMove.GetPlayerDirectionRot());


		//アニメーションのアップデート
		m_playerAnimation.Update(m_playerModel, m_playerAnimationParam);

		//移動の結果をモデルに反映
		m_playerModel->SetPosition(m_position);
	}

	void Player::Damage(int damage)
	{
		m_hp -= damage;

		if (m_hp < 0)
		{
			//TODO:死亡処理
			m_hp = 0;

			//ゲームオーバーを通知
			GameOverNotifier gameOverNotifer;
			gameOverNotifer.NotifyGameOver();
		}
	}

	void Player::Heal(int healValue)
	{
		m_hp += healValue;

		if (m_hp > 1000)
		{
			m_hp = 1000;
		}
	}
}
