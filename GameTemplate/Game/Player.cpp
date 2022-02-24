#include "stdafx.h"
#include "Player.h"
#include "GameOverNotifier.h"
#include "BossTank.h"

namespace
{
	const char* PATH_PLAYER_MODEL = "Assets/modelData/Player.tkm";		//プレイヤーのモデルパス
	const char* PATH_PLAYER_SKELETON = "Assets/modelData/Player.tks";	//プレイヤーのモデルのスケルトンパス
	const int FINISH_CAMERA_START = 50;									//トドメカメラの演出開始フレーム
	const int PLAYER_HP_MAX = 1000;										//プレイヤーの最大HP
}

namespace Game
{
	Player::~Player()
	{
		//モデルを削除
		DeleteGO(m_playerModel);
	}

	bool Player::Start()
	{
		//モデルを生成
		m_playerModel = NewGO<SkinModelRender>(Priority::High);

		//アニメーションを初期化
		m_playerAnimation.InitAnim();

		//モデルの初期化
		m_playerModel->Init(PATH_PLAYER_MODEL, PATH_PLAYER_SKELETON,m_playerAnimation.GetAnimationClip(),m_playerAnimation.GetAnimClipNum(),enModelUpAxisZ);
		m_playerModel->SetPosition(m_position);

		//プレイヤーの移動クラスの初期化
		m_playerMove.Init(m_position);

		//初期の向きをセット
		m_playerMove.SetPlayerDirection(Vector3::Back);
		return true;
	}

	void Player::Update()
	{
		//プレイヤーのHP表示の更新
		m_playerDisplay.Update(m_hp);

		//切断モード中でない時は移動可能
		if (m_eventCut == false)
		{
			m_eventCut = m_playerMove.Move(m_position,m_playerAnimationParam);
		}
		else
		{
			m_playerAnimationParam.playerState = PlayerAnimationParam::enCutMode;
		}

		//切断イベントのアップデート
		m_playerCut.SetCutEvent(m_eventCut,m_position);

		//トドメカメラ中ならトドメカウントを増加
		if (m_isFinishCamera == true)
		{
			m_finishCount++;
		}

		//トドメカメラの演出開始フレーム中なら
		if (m_finishCount >= FINISH_CAMERA_START)
		{
			//トドメカメラ
			m_playerCamera.UpdateFinishCamera(m_playerAnimationParam);
		}
		//切断モードでないなら
		else if (m_eventCut == false)
		{
			//通常のカメラの移動
			m_playerCamera.Update(m_position);
		}
		//切断モードなら
		else
		{
			//切断モード用カメラの移動
			Vector3 newDirection = m_playerCamera.UpdateCutMode(m_position, m_playerMove.GetPlayerDirection());

			//切断モードカメラで視点を変更するとプレイヤーの向きも変更されるので新しい向きを格納
			m_playerMove.SetPlayerDirection(newDirection);
		}
		
		//プレイヤーの向きから回転を計算
		m_playerMove.CalcToModelDirectionQRot();

		//プレイヤーの向きにモデルを向ける
		m_playerModel->SetRotation(m_playerMove.GetPlayerDirectionRot());

		//切断モード中でないなら
		if (m_eventCut == false)
		{
			//攻撃のアップデート
			m_playerAttack.Update(m_position, m_playerAnimationParam, m_playerMove.GetPlayerDirectionRot());
		}

		//切断のアップデート
		m_playerCut.Update(m_position, m_playerMove.GetPlayerDirectionRot());

		//アニメーションのアップデート
		m_playerAnimation.Update(m_playerModel, m_playerAnimationParam);

		//移動の結果をモデルに反映
		m_playerModel->SetPosition(m_position);
	}

	void Player::Damage(int damage)
	{
		//HPからダメージ量分減らす
		m_hp -= damage;

		//体力が0以下になったら
		if (m_hp <= 0)
		{
			//死亡処理
			m_hp = 0;

			//プレイヤーの移動を禁止
			m_playerMove.DisableMove();
			//プレイヤーのアニメーションの状態を死亡状態にする。
			m_playerAnimationParam.playerState = PlayerAnimationParam::enDead;

			//ゲームオーバーを通知
			GameOverNotifier gameOverNotifer;
			gameOverNotifer.NotifyGameOver();
		}
	}

	void Player::Heal(int healValue)
	{
		//体力を回復
		m_hp += healValue;

		//体力の最大値以上なら
		if (m_hp > PLAYER_HP_MAX)
		{
			//体力を最大値に
			m_hp = PLAYER_HP_MAX;
		}
	}

	void Player::NoticeMissileMoveStart()
	{
		//ミサイル移動処理の開始を通知
		m_playerMove.NoticeMissileMoveStart();

		//ボスの砲身の切断を許可
		BossTank* bossTank = FindGO<BossTank>("bosstank");
		bossTank->AllowCannonCut();
	}

	void Player::NoticeFrontMoveStart()
	{
		//ボスの正面への移動処理の開始を通知
		m_playerMove.NoticeFrontMoveStart();

		//ボスの車体と砲塔の切断を許可
		BossTank* bossTank = FindGO<BossTank>("bosstank");
		bossTank->AllowBodyCut();
	}
}
