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
		m_playerMove.Move(m_position);

		//移動方向にモデルを向ける
		m_playerMove.TurnModelToMoveDirection(m_playerModel);

		//カメラの移動
		m_playerCamera.Update(m_position);

		//アニメーションのアップデート
		m_playerAnimation.Update(m_playerModel);

		//移動の結果をモデルに反映
		m_playerModel->SetPosition(m_position);
	}
}
