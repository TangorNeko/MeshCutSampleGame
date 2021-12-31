#pragma once
#include "BossTankStatus.h"
#include "EnemyTask.h"
#include "BossTankCollision.h"
#include "BossTankTasks.h"
#include "BossTankDisplay.h"
#include "BossTankBehave.h"

namespace Game
{
	class BossTank : public IGameObject
	{
	public:
		~BossTank();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce) override;

		/**
		 * @brief 座標を設定
		 * @param position 座標
		*/
		void SetPosition(const Vector3& position)
		{
			m_bossTankStatus.position = position;
		}

		/**
		 * @brief 座標を取得
		 * @return 座標
		*/
		Vector3 GetPosition()
		{
			return m_bossTankStatus.position;
		}

		/**
		 * @brief ダメージを与える
		 * @param damage ダメージ量
		*/
		void Damage(float damage);

		/**
		 * @brief ボスのタスクをセット
		 * @param taskNum セットするタスクの番号
		 * @param task セットするタスク
		*/
		void SetTask(int taskNum, EnemyTask task)
		{
			m_bossTankBehave.SetTask(taskNum, task);
		}

		/**
		 * @brief 砲塔の角度を設定
		 * @param deg 砲塔の角度
		*/
		void SetTurretDeg(float deg)
		{
			m_bossTankStatus.turretDeg = deg;
		}

		/**
		 * @brief 砲塔の角度を取得
		 * @return 砲塔の角度
		*/
		float GetTurretDeg()
		{
			return m_bossTankStatus.turretDeg;
		}
		
		/**
		 * @brief 車体の角度を設定
		 * @param deg 車体の角度
		*/
		void SetBaseDeg(float deg)
		{
			m_bossTankStatus.baseDeg = deg;
		}

		/**
		 * @brief 車体の角度を取得
		 * @return 車体の角度
		*/
		float GetBaseDeg()
		{
			return m_bossTankStatus.baseDeg;
		}


		Vector3 GetRightCannonPosition();

		Vector3 GetLeftCannonPosition();

		Vector3 GetFrontPosition();
	private:
		SkinModelRender* m_baseRender = nullptr;			//車体のモデル
		SkinModelRender* m_turretRender = nullptr;			//砲塔のモデル
		SkinModelRender* m_rightCannonRender = nullptr;		//右砲身のモデル
		SkinModelRender* m_leftCannonRender = nullptr;		//左砲身のモデル
		BossTankStatus m_bossTankStatus;					//ボスのステータス
		BossTankCollision m_bossTankCollision;				//ボスの当たり判定クラス
		BossTankDisplay m_bossTankDisplay;					//ボスの情報表示クラス
		BossTankBehave m_bossTankBehave;					//ボスの行動分岐
	};
}

