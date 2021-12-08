#pragma once
#include "EnemyTask.h"
#include "BossTankCollision.h"
#include "BossTankTasks.h"
#include "BossTankDisplay.h"

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
			m_position = position;
		}

		/**
		 * @brief 座標を取得
		 * @return 座標
		*/
		Vector3 GetPosition()
		{
			return m_position;
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
			m_tankTask[taskNum] = task;
		}

		/**
		 * @brief 砲塔の角度を設定
		 * @param deg 砲塔の角度
		*/
		void SetTurretDeg(float deg)
		{
			m_turretDeg = deg;
		}

		/**
		 * @brief 砲塔の角度を取得
		 * @return 砲塔の角度
		*/
		float GetTurretDeg()
		{
			return m_turretDeg;
		}
		
		/**
		 * @brief 車体の角度を設定
		 * @param deg 車体の角度
		*/
		void SetBaseDeg(float deg)
		{
			m_baseDeg = deg;
		}

		/**
		 * @brief 車体の角度を取得
		 * @return 車体の角度
		*/
		float GetBaseDeg()
		{
			return m_baseDeg;
		}
	private:
		Vector3 m_position = Vector3::Zero;					//ボスの座標
		BossTankCollision m_bossTankCollision;				//ボスの当たり判定クラス
		SkinModelRender* m_baseRender = nullptr;			//車体のモデル
		SkinModelRender* m_turretRender = nullptr;			//砲塔のモデル
		SkinModelRender* m_cannonRender = nullptr;			//砲身のモデル
		BossTankDisplay m_bossTankDisplay;					//ボスの情報表示クラス
		Quaternion m_baseRot = Quaternion::Identity;		//車体のクォータニオン
		Quaternion m_turretRot = Quaternion::Identity;		//砲塔のクォータニオン
		float m_baseDeg = 0.0f;								//車体の角度
		float m_turretDeg = 0.0f;							//砲塔の角度
		int m_hp = 1000;									//体力
		bool m_isCannonBreak = false;						//砲身が壊された?
		bool m_isTurretBreak = false;						//砲塔が壊された?
		bool m_isBaseBreak = false;							//車体が壊された?
		bool m_isSummonMinions = false;						//雑魚敵を召喚した?
		EnemyTask m_tankTask[BossTankTasks::enTaskNum];		//ボスのタスクの配列(原本、ここからキューにコピーされる)
		std::queue<EnemyTask> m_taskQueue;					//ボスのタスクキュー
	};
}

