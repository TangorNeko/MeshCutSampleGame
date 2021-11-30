#include "stdafx.h"
#include "Player.h"

namespace
{
	const char* PATH_PLAYER_MODEL = "Assets/modelData/Samurai.tkm";
	const char* PATH_PLAYER_SKELETON = "Assets/modelData/Samurai.tks";
	const Vector4 SHADOWCOLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };
}

namespace Game
{
	Player::~Player()
	{

	}

	bool Player::Start()
	{
		//モデルを生成
		m_playerModel = NewGO<SkinModelRender>(0);

		//アニメーションを初期化
		m_playerAnimation.InitAnim();

		//モデルの初期化
		m_playerModel->Init(PATH_PLAYER_MODEL, PATH_PLAYER_SKELETON,m_playerAnimation.GetAnimationClip(),m_playerAnimation.GetAnimClipNum(),enModelUpAxisZ);
		m_playerMove.Init(m_position);
		m_playerModel->SetPosition(m_position);

		//プレイヤーのHP表示
		m_hpRender = NewGO<FontRender>(1);
		wchar_t buffer[256];
		swprintf_s(buffer, L"HP:%d",m_hp);
		m_hpRender->SetText(buffer);
		m_hpRender->SetShadowFlag(true);
		m_hpRender->SetShadowColor(SHADOWCOLOR_BLACK);
		return true;
	}

	void Player::Update()
	{
		//プレイヤーのHP表示
		//TODO:クラス分離
		wchar_t buffer[256];
		swprintf_s(buffer, L"HP:%d", m_hp);
		m_hpRender->SetText(buffer);
		m_hpRender->SetPosition({ -600.0f,330.0f });

		bool isCutMode = g_pad[0]->IsPress(enButtonLB1);

		//移動
		//NOTE:仮。LB1押している時は移動させない
		if (isCutMode == false)
		{
			m_playerMove.Move(m_position,m_playerAnimationParam);
			m_playerAnimationParam.isCutMode = false;;
		}
		else
		{
			m_playerAnimationParam.isCutMode = true;
		}

		//TODO:仮のガードアニメ　後からクラス分離
		if (g_pad[0]->IsPress(enButtonRB1))
		{
			m_playerAnimationParam.isGuarding = true;
		}
		else
		{
			m_playerAnimationParam.isGuarding = false;
		}

		if (isCutMode == false)
		{
			//カメラの移動
			m_playerCamera.Update(m_position);
		}
		else
		{
			//切断モード用カメラの移動
			Vector3 newDirection = m_playerCamera.UpdateCutMode(m_position,m_playerMove.GetPlayerDirection());

			//切断モードカメラで視点を変更するとプレイヤーの向きも変更されるので新しい向きを格納
			m_playerMove.SetPlayerDirection(newDirection);
		}

		//プレイヤーの向きにモデルを向ける
		m_playerMove.TurnModelToPlayerDirection(m_playerModel);

		//NOTE:仮で攻撃モデルと切断モデルを向きに追従させている　後から使わないようにする
		if (m_playerAttack.GetModel() != nullptr)
		{
			m_playerMove.TurnModelToPlayerDirection(m_playerAttack.GetModel());
		}

		if (m_playerCut.GetModel() != nullptr)
		{
			m_playerMove.TurnModelToPlayerDirection(m_playerCut.GetModel());
		}


		//攻撃のアップデート
		m_playerAttack.Update(m_position, m_playerAnimationParam);
		
		//切断のアップデート
		m_playerCut.Update(m_position, m_playerMove.CalcToModelDirectionQRot());


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
		}
	}
}
