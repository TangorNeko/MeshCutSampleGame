#pragma once
#include "PlayerMove.h"
#include "PlayerCamera.h"
#include "PlayerAnimation.h"
#include "PlayerAttack.h"
#include "PlayerCut.h"
#include "PlayerDisplay.h"

namespace Game
{
	class Player : public IGameObject
	{
		~Player();

		bool Start() override;

		void Update() override;

	public:

		/**
		 * @brief プレイヤーの座標を設定
		 * @param position 座標
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
			m_playerMove.GetCharaCon().SetPosition(m_position);
		}

		/**
		 * @brief プレイヤーの座標を取得
		 * @return 座標 
		*/
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/**
		 * @brief プレイヤーの向きを設定
		 * @param direction 向き
		*/
		void SetPlayerDirection(const Vector3& direction)
		{
			m_playerMove.SetPlayerDirection(direction);
		}

		/**
		 * @brief プレイヤーのキャラコンを取得
		 * @return プレイヤーのキャラコン
		*/
		CharacterController& GetCharaCon()
		{
			return m_playerMove.GetCharaCon();
		}

		/**
		 * @brief プレイヤーのカメラ座標を取得
		 * @return カメラ座標
		*/
		const Vector3& GetCameraPosition()
		{
			return m_playerCamera.GetCameraPosition();
		}
		
		/**
		 * @brief プレイヤーのカメラ注視点を取得
		 * @return カメラ注視点
		*/
		const Vector3& GetCameraTarget()
		{
			return m_playerCamera.GetCameraTarget();
		}

		/**
		 * @brief プレイヤーにダメージを与える
		 * @param damage ダメージ
		*/
		void Damage(int damage);

		/**
		 * @brief プレイヤーを回復する
		 * @param healValue 回復量
		*/
		void Heal(int healValue);

		/**
		 * @brief プレイヤーを吹き飛ばす
		 * @param moveAmount 吹き飛ばす力
		*/
		void KnockDown(const Vector3& moveAmount)
		{
			m_playerMove.KnockDown(moveAmount);
		}

		/**
		 * @brief プレイヤーを後転させる
		 * @param moveAmount 後転の移動力
		*/
		void BackHandSpring(const Vector3& moveAmount)
		{
			m_playerMove.BackHandSpring(moveAmount);
		}

		/**
		 * @brief ミサイルを飛び移る処理のスタートを通知する
		*/
		void NoticeMissileMoveStart();

		/**
		 * @brief ボスの正面に移動する処理のスタートを通知する
		*/
		void NoticeFrontMoveStart();

		/**
		 * @brief ミサイルを飛び移る処理の終わりを通知する
		*/
		void NoticeMissileMoveEnd()
		{
			m_playerMove.NoticeMissileMoveEnd();
			EventCutOff();
		}

		/**
		 * @brief ボスの正面に移動する処理の終わりを通知する
		*/
		void NoticeFrontMoveEnd()
		{
			m_playerMove.NoticeFrontMoveEnd();
			EventCutOff();
		}

		/**
		 * @brief トドメカメラをスタートさせる
		*/
		void StartFinishCamera()
		{
			m_isFinishCamera = true;
		}

		/**
		 * @brief ミサイルを飛び移る処理のコマンドの成功を通知する
		*/
		void MissileMoveSuccess()
		{
			m_playerMove.MissileMoveSuccess();
		}

		/**
		 * @brief イベント用のカットフラグをオン
		*/
		void EventCutOn()
		{
			m_eventCut = true;
		}

		/**
		 * @brief イベント用のカットフラグをオフ
		*/
		void EventCutOff()
		{
			m_eventCut = false;
		}

		/**
		 * @brief プレイヤーの体力表示をスタートさせる
		*/
		void InitDisplay()
		{
			m_playerDisplay.Init(m_hp);
		}

		/**
		 * @brief プレイヤーの体力を取得
		 * @return 体力
		*/
		int GetHp()
		{
			return m_hp;
		}
	private:
		SkinModelRender* m_playerModel = nullptr;	//プレイヤーのモデル
		Vector3 m_position = Vector3::Zero;			//プレイヤーの座標
		PlayerMove m_playerMove;					//プレイヤーの移動クラス
		PlayerCamera m_playerCamera;				//プレイヤーのカメラクラス
		PlayerAnimation m_playerAnimation;			//プレイヤーのアニメーションクラス
		PlayerAnimationParam m_playerAnimationParam;//プレイヤーのアニメーション制御ステータス
		PlayerAttack m_playerAttack;				//プレイヤーの攻撃クラス
		PlayerCut m_playerCut;						//プレイヤーの切断クラス
		PlayerDisplay m_playerDisplay;				//プレイヤーの情報表示クラス
		int m_hp = 1000;							//プレイヤーの体力
		bool m_eventCut = false;					//プレイヤーがイベント切断モード中か?
		bool m_isFinishCamera = false;				//プレイヤーがトドメカメラ中か?
		int m_finishCount = 0;						//トドメカメラがスタートするまでのフレームをカウントする
	};
}

