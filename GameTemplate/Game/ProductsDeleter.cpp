#include "stdafx.h"
#include "ProductsDeleter.h"

#include "EnemyMissile.h"
#include "HealItem.h"
#include "EnemyRock.h"
#include "StepObject.h"
#include "CutDummy.h"
#include "MiniEnemy.h"
#include "AttackTutorial.h"
#include "DashTutorial.h"

namespace Game
{
	void ProductsDeleter::DeleteProducts()
	{
		//ÉQÅ[ÉÄÉVÅ[ÉìÇ≈ê∂ê¨Ç≥ÇÍÇÈï®ÇÇ‹Ç∆ÇﬂÇƒçÌèúÇ∑ÇÈ

		QueryGOs<EnemyMissile>("missile", [](EnemyMissile* enemyMissile)->bool
			{
				DeleteGO(enemyMissile);
				return true;
			}
		);

		QueryGOs<HealItem>("healItem", [](HealItem* healItem)->bool
			{
				DeleteGO(healItem);
				return true;
			}
		);

		QueryGOs<EnemyRock>("rock", [](EnemyRock* enemyRock)->bool
			{
				DeleteGO(enemyRock);
				return true;
			}
		);

		QueryGOs<StepObject>("stepObject", [](StepObject* stepObject)->bool
			{
				DeleteGO(stepObject);
				return true;
			}
		);

		QueryGOs<CutDummy>("cutDummy", [](CutDummy* cutDummy)->bool
			{
				DeleteGO(cutDummy);
				return true;
			}
		);
		
		QueryGOs<MiniEnemy>("enemy", [](MiniEnemy* miniEnemy)->bool
			{
				DeleteGO(miniEnemy);
				return true;
			}
		);

		DeleteGO(FindGO<DashTutorial>("dashtutorial"));

		DeleteGO(FindGO<AttackTutorial>("attacktutorial"));

		EffectEngine::GetInstance()->StopAllEffects();
	}
}
