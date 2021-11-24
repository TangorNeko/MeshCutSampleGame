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

		//移動
		//NOTE:仮。LB1押している時は移動させない
		if (g_pad[0]->IsPress(enButtonLB1) == false)
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

		//移動方向にモデルを向ける
		m_playerMove.TurnModelToMoveDirection(m_playerModel);

		//NOTE:仮で攻撃モデルと切断モデルを移動に追従させている　後から使わないようにする
		m_playerMove.TurnModelToMoveDirection(m_playerAttack.GetModel());
		m_playerMove.TurnModelToMoveDirection(m_playerCut.GetModel());


		//攻撃のアップデート
		m_playerAttack.Update(m_position, m_playerAnimationParam);
		
		//切断のアップデート
		m_playerCut.Update(m_position, m_playerMove.CalcToModelDirectionQRot());

		//NOTE:仮。LB1押している時はカメラを動かせない
		if (g_pad[0]->IsPress(enButtonLB1) == false)
		{
			//カメラの移動
			m_playerCamera.Update(m_position);
		}
		else
		{
			m_playerCamera.UpdateCutMode(m_position);
		}
		//TODO:LB1を押した時はプレイヤーの向きにカメラを移動

		//アニメーションのアップデート
		m_playerAnimation.Update(m_playerModel, m_playerAnimationParam);

		//移動の結果をモデルに反映
		m_playerModel->SetPosition(m_position);
	}

	void Player::Damage(int damage)
	{
		m_hp -= damage;
	}
}
