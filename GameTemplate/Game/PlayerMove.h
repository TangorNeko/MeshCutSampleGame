#pragma once
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerMove
	{
	public:

		/**
		 * @brief 初期化
		 * @param playerPosition プレイヤーの座標
		*/
		void Init(Vector3& playerPosition);

		/**
		 * @brief 移動処理
		 * @param playerPosition プレイヤーの座標
		 * @param animParam プレイヤーのアニメーションパラメーター
		 * @return 
		*/
		bool Move(Vector3& playerPosition,PlayerAnimationParam& animParam);

		/**
		 * @brief モデルの向きのクォータニオンを計算する
		*/
		void CalcToModelDirectionQRot();

		/**
		 * @brief プレイヤーの向きを取得
		 * @return プレイヤーの向き
		*/
		const Vector3& GetPlayerDirection()
		{
			return m_playerDirection;
		}

		/**
		 * @brief プレイヤーの向きをセット
		 * @param direction プレイヤーの向き
		*/
		void SetPlayerDirection(const Vector3& direction)
		{
			m_playerDirection = direction;
		}

		/**
		 * @brief プレイヤーのキャラコンを取得
		 * @return プレイヤーのキャラコン
		*/
		CharacterController& GetCharaCon()
		{
			return m_charaCon;
		}

		/**
		 * @brief プレイヤーの向きへのクォータニオンを取得
		 * @return 
		*/
		const Quaternion& GetPlayerDirectionRot()
		{
			return m_toMoveDirectionRot;
		}

		/**
		 * @brief プレイヤーを吹き飛ばす
		 * @param moveAmount 吹き飛ばす力
		*/
		void KnockDown(const Vector3& moveAmount);

		/**
		 * @brief プレイヤーに後転させる
		 * @param moveAmount 後転の移動力
		*/
		void BackHandSpring(const Vector3& moveAmount);

		/**
		 * @brief ミサイルを飛び移る処理のスタートを通知する
		*/
		void NoticeMissileMoveStart()
		{
			m_playerMoveEvent = enMissileMove;
		}

		/**
		 * @brief ボスの正面に移動する処理のスタートを通知する
		*/
		void NoticeFrontMoveStart()
		{
			m_playerMoveEvent = enFrontMove;

			//ボスの方に向かせる
			SetPlayerDirection(Vector3::Back);
		}

		/**
		 * @brief ミサイルを飛び移る処理の終わりを通知する
		*/
		void NoticeMissileMoveEnd()
		{
			m_isMissileMove = false;

			m_playerMoveEvent = enNormal;
		}

		/**
		 * @brief ボスの正面に移動する処理の終わりを通知する
		*/
		void NoticeFrontMoveEnd()
		{
			m_isFrontMove = false;

			m_playerMoveEvent = enNormal;
		}

		//プレイヤーの移動を不可にする
		void DisableMove()
		{
			m_playerMoveEvent = enDead;
		}

		/**
		 * @brief 通常の移動処理
		 * @param playerPosition プレイヤーの座標
		 * @param animParam プレイヤーのアニメーションパラメーター
		 * @return 
		*/
		bool NormalMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		/**
		 * @brief ミサイルを飛び移る移動処理
		 * @param playerPosition プレイヤーの座標
		 * @param animParam プレイヤーのアニメーションパラメーター
		 * @return
		*/
		bool MissileMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		/**
		 * @brief ボスの正面に移動する処理
		 * @param playerPosition プレイヤーの座標
		 * @param animParam プレイヤーのアニメーションパラメーター
		 * @return
		*/
		bool FrontMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		/**
		 * @brief 後転の移動処理
		 * @param playerPosition プレイヤーの座標
		 * @param animParam プレイヤーのアニメーションパラメーター
		 * @return
		*/
		bool BackHandspringMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		/**
		 * @brief 吹き飛ばしの移動処理
		 * @param playerPosition プレイヤーの座標
		 * @param animParam プレイヤーのアニメーションパラメーター
		 * @return
		*/
		bool KnockDownMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		/**
		 * @brief 死亡時の移動処理(何もしない)
		 * @return 
		*/
		bool DeadMove()
		{
			return false;
		}

		/**
		 * @brief ミサイルを飛び移る処理のコマンドの成功を通知する
		*/
		void MissileMoveSuccess()
		{
			m_isMissileMove = true;
		}

	private:
		CharacterController m_charaCon;							//プレイヤーのキャラコン
		Vector3 m_moveAmount = Vector3::Zero;					//プレイヤーの移動量
		Vector3 m_prevPosition = Vector3::Zero;					//1フレーム前のプレイヤーの座標
		Vector3 m_playerDirection = Vector3::Front;				//プレイヤーの向き
		Quaternion m_toMoveDirectionRot = Quaternion::Identity;	//プレイヤーの移動方向へのクォータニオン
		int m_aerialFrame = 0;									//空中にいるフレーム数
		//吹き飛ばし移動用変数
		int m_knockDownFrame = 0;								//吹き飛んでいるフレーム数
		Vector3 m_knockDownAmount = Vector3::Zero;				//吹き飛ばし力
		//ミサイルジャンプ用変数
		bool m_isMissileMove = false;							//ミサイル移動処理中?
		Vector3 m_targetPos[5] =								//ミサイル移動のターゲットの座標
		{ Vector3::Zero,Vector3::Zero,Vector3::Zero ,Vector3::Zero,Vector3::Zero };
		int m_moveState = 0;									//ミサイル移動の状態	
		int m_targetCount = 0;									//何番目のミサイルをターゲットしているか
		int m_jumpFrameCount = 0;								//ジャンプしているフレーム数
		int m_distanceCount = 0;								//距離のカウント
		bool m_isMovingUp = true;								//上向きに移動中?
		bool m_isMoveStartFrame = true;							//移動の開始フレーム?
		//ボス前ジャンプ用変数
		bool m_isFrontMove = false;								//ボス前ジャンプ中?
		bool m_isJumpStartFrame = true;							//移動の開始フレーム?
		Vector3 m_frontMoveAmount = Vector3::Zero;				//ボス前ジャンプの移動量
		//後転用変数
		int backHandspringFrame = 0;							//後転のフレーム数
		Vector3 backHandSpringAmount = Vector3::Zero;			//後転の移動力

		//移動状態
		enum MoveEvent
		{
			enNormal,			//通常移動
			enMissileMove,		//ミサイルジャンプ
			enFrontMove,		//ボス前ジャンプ
			enBackHandspring,	//後転
			enKnockDown,		//吹き飛ばし
			enDead,				//死亡中
		};

		MoveEvent m_playerMoveEvent = enNormal;					//現在の移動状態
	};
}

