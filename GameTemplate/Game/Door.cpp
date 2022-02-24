#include "stdafx.h"
#include "Door.h"
#include "AttackTutorial.h"
#include "Player.h"

namespace
{
	const char* PATH_DOORMODEL = "Assets/modelData/Door.tkm";	//ドアのモデルのパス
	const float ATTACK_TUTORIAL_DISTANCE = 500.0f;				//攻撃チュートリアルの出現距離
}

namespace Game
{
	Door::~Door()
	{
		//ドアのモデルと当たり判定を削除
		DeleteGO(m_doorModel);
		delete m_physicsStaticObject;
	}

	bool Door::Start()
	{
		//ドアのモデルを作成
		m_doorModel = NewGO<SkinModelRender>(Priority::High);
		m_doorModel->Init(PATH_DOORMODEL);
		m_doorModel->SetModelCenterAsOrigin();
		m_doorModel->SetOwner(this);
		m_doorModel->SetDivideFlag(true);
		m_doorModel->SetShadowCasterFlag(false);
		ModelCutManager::GetInstance()->AddCuttable(m_doorModel);

		//ドアのモデルが斬られた場合当たり判定をそのままにはできないので任意のタイミングで削除できるポインタにしている。
		m_physicsStaticObject = new PhysicsStaticObject;
		m_physicsStaticObject->CreateFromModel(m_doorModel->GetModel(), m_doorModel->GetModel()->GetWorldMatrix());
		
		//プレイヤーを検索
		m_player = FindGO<Player>("player");
		return true;
	}

	void Door::OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce)
	{
		//ドアが斬られたので当たり判定を削除
		delete m_physicsStaticObject;
		m_physicsStaticObject = nullptr;
	}

	void Door::Update()
	{
		Vector3 distance = m_player->GetPosition() - m_doorModel->GetPosition();
		//ドアの近くにプレイヤーが来ると攻撃のチュートリアルを表示
		if (distance.LengthSq() < ATTACK_TUTORIAL_DISTANCE * ATTACK_TUTORIAL_DISTANCE && m_isDisplayAttackTutorial == false)
		{
			NewGO<AttackTutorial>(Priority::High, "attacktutorial");
			m_isDisplayAttackTutorial = true;
		}
	}
}
