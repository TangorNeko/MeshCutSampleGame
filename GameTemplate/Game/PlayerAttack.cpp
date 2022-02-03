#include "stdafx.h"
#include "PlayerAttack.h"
#include "PlayerAttackSound.h"
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
	const Vector3 PLAYER_CUTPOINT = { 0.0f,120.0f,0.0f };
	const float PLAYER_CUTDEG = 0.7f;
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

			//攻撃の当たり判定を作成する時間になったら
			if (m_attackTime == TIME_ATTACK_COLLISION)
			{

				Vector3 cutPoint = PLAYER_CUTPOINT;
				cutPoint += playerPosition;

				Vector3 cutNormal = ATTACK_CUT_NORMAL[m_comboNum - 1];
				cutNormal.Normalize();

				toMoveRot.Apply(cutNormal);

				Vector3 cutForce = ATTACK_CUT_FORCE[m_comboNum - 1];
				cutForce.Normalize();

				toMoveRot.Apply(cutForce);
				
				Vector3 front = Vector3::Front;
				toMoveRot.Apply(front);

				bool hitCheck = false;

				ModelCutManager::GetInstance()->QueryCut(cutNormal, cutForce * 30,[cutPoint, front, &hitCheck](const SkinModelRender* cutObject)->bool
					{
						Vector3 distance = cutObject->GetPosition() - cutPoint;

						Vector3 toCutObject = distance;
						toCutObject.Normalize();

						bool isInRange = false;
						float dot = Dot(toCutObject, front);
						if (dot >= PLAYER_CUTDEG)
						{
							isInRange = true;
						}

						if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE && cutObject->GetDivideNum() <= MAX_CUT_NUM && isInRange)
						{
							hitCheck |= true;

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
						hitCheck |= true;
						bossTank->Damage(ATTACK_DAMAGE);
					}
				}

				if (hitCheck == false)
				{
					PlayerAttackSound playerAttackSound;
					playerAttackSound.PlayMissSound(m_comboNum - 1);
				}
				else
				{
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