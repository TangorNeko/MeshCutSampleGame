#include "stdafx.h"
#include "Door.h"

namespace
{
	const char* PATH_DOORMODEL = "Assets/modelData/Door.tkm";
}

namespace Game
{
	Door::~Door()
	{
		DeleteGO(m_doorModel);
		delete m_physicsStaticObject;
	}

	bool Door::Start()
	{
		m_doorModel = NewGO<SkinModelRender>(0);
		m_doorModel->Init(PATH_DOORMODEL);
		m_doorModel->SetModelCenterAsOrigin();
		m_doorModel->SetOwner(this);
		m_doorModel->SetDivideFlag(true);
		ModelCutManager::GetInstance()->AddCuttable(m_doorModel);

		//ドアのモデルが斬られた場合当たり判定をそのままにはできないので任意のタイミングで削除できるポインタにしている。
		m_physicsStaticObject = new PhysicsStaticObject;
		m_physicsStaticObject->CreateFromModel(m_doorModel->GetModel(), m_doorModel->GetModel()->GetWorldMatrix());

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
	}
}
