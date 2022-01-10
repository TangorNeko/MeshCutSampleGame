#include "stdafx.h"
#include "PlayerAttack.h"
#include "EnemyMissile.h"
#include "BossTank.h"

namespace
{
	const char* PATH_HITBOXMODEL = "Assets/modelData/CutIndicator.tkm";
	const int TIME_ATTACK_END = 60;
	const int TIME_COMBO_ACCEPTABLE = 20;
	const int TIME_ATTACK_COLLISION = 14;
	const int MAX_CUT_NUM = 2;
	const float ATTACK_RANGE = 600.0f;
	const float ATTACK_DAMAGE = 25.0f;
	const Vector3 ATTACK_CUT_NORMAL[3] = {
		{0.1f,0.9f,-0.1f},
		{0.5f,0.5f,0.2f},
		{0.3f,0.9f,0.1f}
	};

	const Vector3 ATTACK_CUT_FORCE[3] = {
		{0.9f,0.1f,-0.1f},
		{-0.5f,0.5f,0.2f},
		{0.9f,0.3f,0.1f}
	};
	const EnButton BUTTON_ATTACK = enButtonX;
}

namespace Game
{
	PlayerAttack::~PlayerAttack()
	{
		/*
		if (m_testHitBox != nullptr)
		{
			DeleteGO(m_testHitBox);
		}
		*/
		
	}

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

			//当たり判定があれば
			
			/*
			if (m_testHitBox != nullptr)
			{
				Vector3 cutPos = { 0.0f,120.0f,0.0f };
				cutPos += playerPosition;
				//当たり判定の座標をプレイヤーの位置にセット

				Quaternion QRot;
				QRot.SetRotation(Vector3::Right, ATTACK_CUT_NORMAL[m_comboNum-1]);
				QRot.Multiply(toMoveRot);
				m_testHitBox->SetRotation(QRot);
				m_testHitBox->SetPosition(cutPos);
			}
			*/

			//攻撃の当たり判定を作成する時間になったら
			if (m_attackTime == TIME_ATTACK_COLLISION)
			{
				//TODO:トリガーの当たり判定を作成
				/*
				m_testHitBox = NewGO<SkinModelRender>(0);
				m_testHitBox->Init(PATH_HITBOXMODEL);
				m_testHitBox->SetPosition(playerPosition);
				*/

				

				Vector3 cutPoint = { 0.0f,120.0f,0.0f };
				cutPoint += playerPosition;

				Vector3 cutNormal = ATTACK_CUT_NORMAL[m_comboNum - 1];
				cutNormal.Normalize();

				toMoveRot.Apply(cutNormal);

				Vector3 cutForce = ATTACK_CUT_FORCE[m_comboNum - 1];
				cutForce.Normalize();

				toMoveRot.Apply(cutForce);
				
				Vector3 front = Vector3::Front;
				toMoveRot.Apply(front);

				ModelCutManager::GetInstance()->QueryCut(cutNormal, cutForce * 30,[cutPoint, front](const SkinModelRender* cutObject)->bool
					{
						Vector3 distance = cutObject->GetPosition() - cutPoint;

						Vector3 toCutObject = distance;
						toCutObject.Normalize();

						bool isInRange = false;
						float dot = Dot(toCutObject, front);
						if (dot >= 0.7f)
						{
							isInRange = true;
						}

						if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE && cutObject->GetDivideNum() <= MAX_CUT_NUM && isInRange)
						{
							return true;
						}
						return false;
					}
				);

				//ボスに近ければダメージを与える
				//TODO:ボスの種類が増えることが予想されるので基底クラスを用意する?
				//TODO:敵によって大きさが違うのでATTACK_RANGEにボス自身の大きさに合わせて距離を追加する
				//TODO:ミサイル等の重要でない敵オブジェクトは通常攻撃でも斬れるようにする?
				BossTank* bossTank = FindGO<BossTank>("bosstank");

				if (bossTank != nullptr)
				{
					Vector3 distance = playerPosition - bossTank->GetPosition();

					if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE)
					{
						bossTank->Damage(ATTACK_DAMAGE);
					}
				}
			}

			//1,2段目までの攻撃開始コンボ受付時間以内で、攻撃ボタンを押すと
			if (g_pad[0]->IsTrigger(BUTTON_ATTACK) && m_attackTime > TIME_COMBO_ACCEPTABLE && m_comboNum <= 2)
			{
				//次のコンボに移るため当たり判定は削除
				
				/*
				DeleteGO(m_testHitBox);
				m_testHitBox = nullptr;
				*/

				//コンボ段数をインクリメント
				m_comboNum++;

				//攻撃時間もリセット
				m_attackTime = 0;
			}

			//攻撃終了フレームを超えると完全に攻撃終了
			if (m_attackTime > TIME_ATTACK_END)
			{
				//当たり判定を削除
				/*
				DeleteGO(m_testHitBox);
				m_testHitBox = nullptr;
				*/

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