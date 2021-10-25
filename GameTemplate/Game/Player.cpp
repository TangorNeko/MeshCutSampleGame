#include "stdafx.h"
#include "Player.h"

namespace
{
	const char* PATH_PLAYER_MODEL = "Assets/modelData/unityChan.tkm";
	const char* PATH_PLAYER_SKELETON = "Assets/modelData/unityChan.tks";
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
		m_playerModel->Init(PATH_PLAYER_MODEL, PATH_PLAYER_SKELETON,m_playerAnimation.GetAnimationClip(),m_playerAnimation.GetAnimClipNum());
		m_playerMove.Init(m_position);
		m_playerModel->SetPosition(m_position);

		return true;
	}

	void Player::Update()
	{
		//移動
		//NOTE:仮。LB1押している時は移動させない
		if (g_pad[0]->IsPress(enButtonLB1) == false)
		{
			m_playerMove.Move(m_position);
		}

		//移動方向にモデルを向ける
		m_playerMove.TurnModelToMoveDirection(m_playerModel);

		//NOTE:仮で攻撃モデルと切断モデルを移動に追従させている　後から使わないようにする
		m_playerMove.TurnModelToMoveDirection(m_playerAttack.GetModel());
		m_playerMove.TurnModelToMoveDirection(m_playerCut.GetModel());


		//攻撃のアップデート
		m_playerAttack.Update(m_position);
		
		//切断のアップデート
		m_playerCut.Update(m_position, m_playerMove.CalcToModelDirectionQRot());

		//NOTE:仮。LB1押している時はカメラを動かせない
		if (g_pad[0]->IsPress(enButtonLB1) == false)
		{
			//カメラの移動
			m_playerCamera.Update(m_position);
		}
		//TODO:LB1を押した時はプレイヤーの向きにカメラを移動

		//アニメーションのアップデート
		m_playerAnimation.Update(m_playerModel);

		//移動の結果をモデルに反映
		m_playerModel->SetPosition(m_position);
	}
}
