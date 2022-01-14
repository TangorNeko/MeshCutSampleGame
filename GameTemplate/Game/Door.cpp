#include "stdafx.h"
#include "Door.h"
#include "AttackTutorial.h"
#include "Player.h"

namespace
{
	const char* PATH_DOORMODEL = "Assets/modelData/Door.tkm";

	const float ATTACK_TUTORIAL_DISTANCE = 300.0f;
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

		//�h�A�̃��f�����a��ꂽ�ꍇ�����蔻������̂܂܂ɂ͂ł��Ȃ��̂ŔC�ӂ̃^�C�~���O�ō폜�ł���|�C���^�ɂ��Ă���B
		m_physicsStaticObject = new PhysicsStaticObject;
		m_physicsStaticObject->CreateFromModel(m_doorModel->GetModel(), m_doorModel->GetModel()->GetWorldMatrix());
		
		m_player = FindGO<Player>("player");
		return true;
	}

	void Door::OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce)
	{
		//�h�A���a��ꂽ�̂œ����蔻����폜
		delete m_physicsStaticObject;
		m_physicsStaticObject = nullptr;
	}

	void Door::Update()
	{
		Vector3 distance = m_player->GetPosition() - m_doorModel->GetPosition();
		//�h�A�̋߂��Ƀv���C���[������ƍU���̃`���[�g���A����\��
		if (distance.LengthSq() < ATTACK_TUTORIAL_DISTANCE * ATTACK_TUTORIAL_DISTANCE && m_isDisplayedTutorial == false)
		{
			NewGO<AttackTutorial>(0, "attacktutorial");
			m_isDisplayedTutorial = true;
		}
	}
}
