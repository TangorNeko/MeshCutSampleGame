#include "stdafx.h"
#include "PlayerCut.h"

namespace
{
	const char* PATH_CUTPLANEMODEL = "Assets/modelData/CutIndicator.tkm";	//切断面のインジケーターのモデルパス
	const Vector3 PLAYER_TO_CUTPOINT = { 0.0f,120.0f,0.0f };				//プレイヤーの座標から切断位置へのベクトル
	const Vector3 CUT_FORCE_DOWN = Vector3::Down * 30.0f;					//デフォルトの向き(下方向)への切断時の力
	const float CUT_RANGE = 600.0f;											//切断の射程
	const float START_CUT_ANGLE = 45.0f;									//ゲーム開始時の切断角度
	const float DEFAULT_CUT_ANGLE = 0.0f;									//デフォルトの切断モードの切断角度
	const Vector3 DEFAULT_CUT_NORMAL = { 1.0f,0.0f,0.0f };					//デフォルトの切断モードの切断面の向き
	const Vector2 INPUT_UP = { 0.0f,1.0f };									//コントローラーの入力の角度計算用の上ベクトル
	const float PLAYER_CUTDEG = 0.7f;										//プレイヤーの正面の角度
	const wchar_t* PATH_CUTSOUND = L"Assets/sound/CutSE.wav";				//切断時の効果音パス
}

namespace Game
{
	PlayerCut::~PlayerCut()
	{
		//切断インジケーターが残っていた場合
		if (m_cutPlaneRender != nullptr)
		{
			//削除
			DeleteGO(m_cutPlaneRender);
		}
	}

	void PlayerCut::Update(const Vector3& playerPosition, const Quaternion& playerQRot)
	{
		//切断モードがオンかどうかで分岐
		if (m_isCutMode == true || m_isCutEvent == true)
		{
			CutUpdate(playerPosition, playerQRot);
		}
		else
		{
			NormalUpdate(playerPosition, playerQRot);
		}
	}

	void PlayerCut::CutUpdate(const Vector3& playerPosition, const Quaternion& playerQRot)
	{
		//切断モード中にLB1の入力がなくなると切断モード終了
		if (g_pad[0]->IsPress(enButtonLB1) == false && m_isCutEvent == false)
		{
			DeleteGO(m_cutPlaneRender);
			m_cutPlaneRender = nullptr;
			m_isCutMode = false;

			return;
		}

		//切断位置と切断面の法線を取得
		Vector3 cutPoint = playerPosition + PLAYER_TO_CUTPOINT;
		m_cutPlaneRender->SetPosition(cutPoint);
		Vector3 cutNormal = DEFAULT_CUT_NORMAL;

		//切断モデルをプレイヤーの操作通りに回転させる
		Vector2 input = { g_pad[0]->GetRStickXF(),g_pad[0]->GetRStickYF() };
		if (input.LengthSq() > 0.0f)
		{
			input.Normalize();
			angle = Dot(input, INPUT_UP);
			angle = acos(angle);

			if (input.x > 0.0f)
			{
				angle *= -1;
			}
		}

		//回転を適用
		m_cutPlaneQRot.SetRotationZ(angle);
		m_cutPlaneQRot.Multiply(playerQRot);
		m_cutPlaneQRot.Apply(cutNormal);
		m_cutPlaneRender->SetRotation(m_cutPlaneQRot);

		//斬った際の力の向き
		//何も向きを変えていない時は下向き
		Vector3 cutForce = CUT_FORCE_DOWN;
		//切断面の回転に合わせて力の向きも回転させる
		m_cutPlaneQRot.Apply(cutForce);

		//切断モード中にRB1ボタンで切断
		if (g_pad[0]->IsTrigger(enButtonRB1))
		{
			//一つでも切断したかのチェック用変数
			bool hitCheck = false;

			Game::ModelCutManager::GetInstance()->QueryCut(cutNormal, cutPoint, cutForce, [cutPoint,&hitCheck, playerQRot](const SkinModelRender* cutObject)->bool
				{
					//切断オブジェクトへの距離を取得
					Vector3 distance = cutObject->GetPosition() - cutPoint;

					//プレイヤーの正面の向きを計算
					Vector3 front = Vector3::Front;
					playerQRot.Apply(front);

					//切断オブジェクトへの向きを計算
					Vector3 toCutObject = distance;
					toCutObject.y = 0.0f;
					toCutObject.Normalize();

					//プレイヤーの正面に切断オブジェクトがあるか?
					bool isInRange = false;
					float dot = Dot(toCutObject, front);
					if (dot >= PLAYER_CUTDEG)
					{
						isInRange = true;
					}

					//切断射程内でプレイヤーの正面に切断オブジェクトがあるなら
					if (distance.LengthSq() < CUT_RANGE * CUT_RANGE && isInRange)
					{
						//一つでも切断した
						hitCheck |= true;

						//切断する
						return true;
					}
					return false;
				}
			);
			
			//当たっていれば切断音を流す
			if (hitCheck == true)
			{
				SoundOneShotPlay(PATH_CUTSOUND);
			}
		}
	}

	void PlayerCut::NormalUpdate(const Vector3& playerPosition, const Quaternion& playerQRot)
	{
		//切断モードでない時はLB1を押すと切断モードに移行
		if (g_pad[0]->IsPress(enButtonLB1))
		{
			m_cutPlaneRender = NewGO<SkinModelRender>(Priority::High);
			m_cutPlaneRender->InitUnlit(PATH_CUTPLANEMODEL);
			m_cutPlaneRender->SetPosition(playerPosition);


			m_cutPlaneQRot = Quaternion::Identity;
			angle = DEFAULT_CUT_ANGLE;

			m_isCutMode = true;
		}
	}

	void PlayerCut::SetCutEvent(bool flag,const Vector3& playerPosition)
	{
		m_isCutEvent = flag;

		//まだ切断インジケーターが生成されていないなら
		if (m_isCutEvent == true && m_cutPlaneRender == nullptr)
		{
			//切断インジケーターを生成
			m_cutPlaneRender = NewGO<SkinModelRender>(Priority::High);
			m_cutPlaneRender->InitUnlit(PATH_CUTPLANEMODEL);
			m_cutPlaneRender->SetPosition(playerPosition);
			m_cutPlaneQRot = Quaternion::Identity;

			//ゲームスタート時なら
			if (m_isStart == true)
			{
				//ゲームスタート時用の角度をセット
				angle = START_CUT_ANGLE;
				m_isStart = false;
			}
			else
			{
				//デフォルトの角度をセット
				angle = DEFAULT_CUT_ANGLE;
			}
		}

		//切断モードが終了して切断インジケーターが残っていた場合
		if (m_isCutEvent == false && m_cutPlaneRender != nullptr)
		{
			//切断インジケーターを削除
			DeleteGO(m_cutPlaneRender);
			m_cutPlaneRender = nullptr;

			//切断モードを終了
			m_isCutMode = false;

			return;
		}
	}
}
