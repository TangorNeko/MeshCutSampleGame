#include "stdafx.h"
#include "CommandInput.h"
#include "Player.h"

namespace
{
	const char* COMMAND_SPRITE_PATH = "Assets/Image/BButtonSprite.dds";		//コマンド入力のスプライトのパス
	const UINT COMMAND_SPRITE_SIDE = 100;									//コマンド入力スプライトの一辺の長さ
	const Vector4 COLOR_TRANSPARENT = {1.0f,1.0f,1.0f,0.0f};				//透過色
	const Vector4 COLOR_OPAQUE = { 1.0f,1.0f,1.0f,1.0f };					//不透明色
	const int FADE_FRAME_MAX = 15;											//フェードの最大値
	const float FADE_RATE_PROGRESS = 1.0f;									//フェードの進行度
	const Vector3 COMMAND_POSITION = {0.0f,-200.0f,0.0f};					//コマンドスプライトの表示座標
	const wchar_t* APPEAR_SOUND_PATH = L"Assets/sound/ButtonAppearSE.wav";	//コマンド入力の出現音のファイルパス
	const wchar_t* SUCCESS_SOUND_PATH = L"Assets/sound/ButtonSuccessSE.wav";//コマンド入力成功音のファイルパス
	const float COMMAND_SCALE_Z = 1.0f;										//Z方向の拡大率
}

namespace Game
{
	CommandInput::~CommandInput()
	{
		//スプライトを削除
		DeleteGO(m_commandSprite);
	}

	bool CommandInput::Start()
	{
		//コマンド入力のスプライトを作成
		m_commandSprite = NewGO<SpriteRender>(Priority::High);
		m_commandSprite->Init(COMMAND_SPRITE_PATH, COMMAND_SPRITE_SIDE, COMMAND_SPRITE_SIDE);
		m_commandSprite->SetPosition(COMMAND_POSITION);

		//出現時のサウンドを再生
		SoundOneShotPlay(APPEAR_SOUND_PATH);
		return true;
	}

	void CommandInput::Update()
	{
		if (g_pad[0]->IsTrigger(enButtonB) && m_isSuccess == false)
		{
			//コマンド入力に成功したら成功音を鳴らす
			m_isSuccess = true;
			SoundOneShotPlay(SUCCESS_SOUND_PATH);
		}

		if (m_isSuccess)
		{
			//コマンド入力に成功していたら徐々にフェードアウト
			FadeOut();
		}
	}

	void CommandInput::Pause()
	{
		if (g_pad[0]->IsTrigger(enButtonB) && m_isSuccess == false)
		{
			//コマンド入力に成功したら成功音を鳴らす
			m_isSuccess = true;
			SoundOneShotPlay(SUCCESS_SOUND_PATH);

			//ポーズを解除
			GameObjectManager::GetInstance()->SetPauseFlag(false);

			//プレイヤーにミサイル移動の成功を通知
			Player* player = FindGO<Player>("player");
			player->MissileMoveSuccess();
		}
	}

	void CommandInput::FadeOut()
	{
		//経過フレーム数を増加
		m_fadeFrame++;

		//最大値を設定
		m_fadeFrame = min(m_fadeFrame, FADE_FRAME_MAX);

		//現在のフェード率を計算
		float fadeRate = static_cast<float>(m_fadeFrame) / FADE_FRAME_MAX;

		//フェード率に合わせてスプライトを拡大
		float spriteScale = fadeRate + FADE_RATE_PROGRESS;
		m_commandSprite->SetScale({ spriteScale ,spriteScale,COMMAND_SCALE_Z });

		//フェード率に合わせて透過させる
		Vector4 mulColor;
		mulColor.Lerp(fadeRate, COLOR_OPAQUE, COLOR_TRANSPARENT);
		m_commandSprite->SetMulColor(mulColor);
	}
}
