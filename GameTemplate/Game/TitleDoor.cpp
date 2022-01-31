#include "stdafx.h"
#include "TitleDoor.h"
#include "Player.h"

namespace
{
	const char* TITLE_DOOR_MODEL_PATH = "Assets/modelData/TitleDoor.tkm";
}

namespace Game
{
	TitleDoor::~TitleDoor()
	{
		if (m_isCut == false)
		{
			DeleteGO(m_titleDoorRender);
		}
	}

	bool TitleDoor::Start()
	{
		m_titleDoorRender = NewGO<SkinModelRender>(0);
		m_titleDoorRender->Init(TITLE_DOOR_MODEL_PATH);
		m_titleDoorRender->SetModelCenterAsOrigin();
		m_titleDoorRender->SetOwner(this);
		m_titleDoorRender->SetDivideFlag(true);
		m_titleDoorRender->SetShadowCasterFlag(false);

		ModelCutManager::GetInstance()->AddCuttable(m_titleDoorRender);

		return true;
	}

	void TitleDoor::OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce)
	{
		//ダミーを作成
		m_titleDoorRender->MakeDummy(cutForce);

		//カットされたフラグをオンに
		m_isCut = true;

		Player* player = FindGO<Player>("player");
		player->EventCutOff();
		player->InitDisplay();

		//モデルレンダーをダミークラスに引き渡したので削除
		DeleteGO(this);
	}

	void TitleDoor::Update()
	{

	}
}
