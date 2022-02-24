#include "stdafx.h"
#include "TitleDoor.h"
#include "Player.h"
#include "TitleSprites.h"

namespace
{
	const char* TITLE_DOOR_MODEL_PATH = "Assets/modelData/TitleDoor.tkm";
}

namespace Game
{
	TitleDoor::~TitleDoor()
	{
		//切断されていなかったら削除
		if (m_isCut == false)
		{
			DeleteGO(m_titleDoorRender);
		}
	}

	bool TitleDoor::Start()
	{
		//タイトル用のドアのモデルを生成
		m_titleDoorRender = NewGO<SkinModelRender>(Priority::High);
		m_titleDoorRender->Init(TITLE_DOOR_MODEL_PATH);
		m_titleDoorRender->SetModelCenterAsOrigin();
		m_titleDoorRender->SetOwner(this);
		m_titleDoorRender->SetDivideFlag(true);
		m_titleDoorRender->SetShadowCasterFlag(false);

		ModelCutManager::GetInstance()->AddCuttable(m_titleDoorRender);

		//タイトル用のスプライトを表示
		m_titleSprites = NewGO<TitleSprites>(Priority::High, "titlesprites");

		return true;
	}

	void TitleDoor::OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce)
	{
		//ダミーを作成
		m_titleDoorRender->MakeDummy(cutForce);

		//カットされたフラグをオンに
		m_isCut = true;

		//プレイヤーの切断モードをオフにする。
		Player* player = FindGO<Player>("player");
		player->EventCutOff();

		//プレイヤーの体力を表示
		player->InitDisplay();

		//タイトルスプライトを消す
		m_titleSprites->NoticeGameStart();

		//モデルレンダーをダミークラスに引き渡したので削除
		DeleteGO(this);
	}

	void TitleDoor::Update()
	{

	}
}
