#include "stdafx.h"
#include "PlayerAttack.h"
#include "PlayerAttackSound.h"
#include "EnemyMissile.h"
#include "BossTank.h"

namespace
{
	const int TIME_ATTACK_END = 60;							//攻撃の終了フレーム
	const int TIME_COMBO_ACCEPTABLE = 20;					//次コンボの受付フレーム
	const int TIME_ATTACK_COLLISION = 14;					//攻撃の当たり判定の発生フレーム
	const int MAX_CUT_NUM = 2;								//最大切断回数
	const float ATTACK_RANGE = 600.0f;						//攻撃の射程
	const int ATTACK_DAMAGE = 25.0f;						//攻撃のダメージ
	const Vector3 PLAYER_CUTPOINT = { 0.0f,120.0f,0.0f };	//通常攻撃の切断の起点
	const float PLAYER_CUTDEG = 0.7f;						//プレイヤーの正面の角度
	const Vector3 ATTACK_CUT_NORMAL[3] = {					//各コンボの切断面の法線
		{0.1f,0.9f,-0.1f},
		{0.5f,0.5f,0.2f},
		{0.3f,0.9f,0.1f}
	};

	const Vector3 ATTACK_CUT_FORCE[3] = {					//各コンボの切断時の力
		{0.9f,0.1f,-0.1f},
		{-0.5f,0.5f,0.2f},
		{0.9f,0.3f,0.1f}
	};
	const EnButton BUTTON_ATTACK = enButtonX;				//攻撃ボタン
}

namespace Game
{
	void PlayerAttack::Update(const Vector3& playerPosition, PlayerAnimationParam& animParam, const Quaternion& toMoveRot)
	{
		//攻撃していない状態で攻撃ボタンを押すと
		if (g_pad[0]->IsTrigger(BUTTON_ATTACK) && m_comboNum == 0)
		{
			//コンボ段数をインクリメント
			m_comboNum++;
		}

		//コンボ1以降を出しているなら
		if (m_comboNum >= 1)
		{
			//攻撃のカウントをインクリメント
			m_attackTime++;

			//攻撃の当たり判定を作成する時間になったら
			if (m_attackTime == TIME_ATTACK_COLLISION)
			{
				//切断の起点を計算
				Vector3 cutPoint = PLAYER_CUTPOINT;
				cutPoint += playerPosition;

				//コンボ段数から切断面の法線を取得
				Vector3 cutNormal = ATTACK_CUT_NORMAL[m_comboNum - 1];
				cutNormal.Normalize();

				//法線にプレイヤーの回転を適用
				toMoveRot.Apply(cutNormal);

				//コンボ段数から切断時の力を取得
				Vector3 cutForce = ATTACK_CUT_FORCE[m_comboNum - 1];
				cutForce.Normalize();

				//切断時の力にプレイヤーの回転を適用
				toMoveRot.Apply(cutForce);
				
				//プレイヤーの正面を取得
				Vector3 front = Vector3::Front;
				toMoveRot.Apply(front);

				//攻撃がヒットしたか
				bool hitCheck = false;

				//切断クエリ
				ModelCutManager::GetInstance()->QueryCut(cutNormal, cutForce * 30,[cutPoint, front, &hitCheck](const SkinModelRender* cutObject)->bool
					{
						//プレイヤーの切断オブジェクトとの距離を計算
						Vector3 distance = cutObject->GetPosition() - cutPoint;

						Vector3 toCutObject = distance;
						toCutObject.Normalize();

						//プレイヤーの正面に切断オブジェクトがあるか
						bool isInRange = false;
						float dot = Dot(toCutObject, front);
						if (dot >= PLAYER_CUTDEG)
						{
							isInRange = true;
						}

						//切断範囲内で、プレイヤーの正面にあり、切断回数が少ないなら
						if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE && cutObject->GetDivideNum() <= MAX_CUT_NUM && isInRange)
						{
							//攻撃がヒットした
							hitCheck |= true;

							//切断
							return true;
						}

						//切断しない
						return false;
					}
				);

				//ボスのインスタンスを取得
				BossTank* bossTank = FindGO<BossTank>("bosstank");

				//ボスが存在した場合
				if (bossTank != nullptr)
				{
					Vector3 distance = playerPosition - bossTank->GetPosition();

					//ボスに近ければダメージを与える
					if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE)
					{
						//攻撃がヒットした
						hitCheck |= true;
						bossTank->Damage(ATTACK_DAMAGE);
					}
				}

				//攻撃がどこにもヒットしていない場合
				if (hitCheck == false)
				{
					//空振り効果音を再生
					PlayerAttackSound playerAttackSound;
					playerAttackSound.PlayMissSound(m_comboNum - 1);
				}
				//ボスまたは切断オブジェクトにヒットした
				else
				{
					//ヒット効果音を再生
					PlayerAttackSound playerAttackSound;
					playerAttackSound.PlayHitSound(m_comboNum - 1);
				}
			}

			//1,2段目までの攻撃開始コンボ受付時間以内で、攻撃ボタンを押すと
			if (g_pad[0]->IsTrigger(BUTTON_ATTACK) && m_attackTime > TIME_COMBO_ACCEPTABLE && m_comboNum <= 2)
			{
				//コンボ段数をインクリメント
				m_comboNum++;

				//攻撃時間もリセット
				m_attackTime = 0;
			}

			//攻撃終了フレームを超えると完全に攻撃終了
			if (m_attackTime > TIME_ATTACK_END)
			{
				//コンボ段数と攻撃時間をリセット
				m_comboNum = 0;
				m_attackTime = 0;
			}
		}

		//アニメーション用
		animParam.comboNum = m_comboNum;
		animParam.attackingTime = m_attackTime;
	}
}