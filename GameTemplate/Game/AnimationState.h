#pragma once

template <typename AnimationParam,typename AnimationEnum>
class AnimationState
{
public:
	struct RetVal
	{
		AnimationEnum animEnum;
		float animSpeed;
		float animInterpolateTime;
	};

	//ステートの変更を確かめる
	AnimationEnum StateChangeCheck(const AnimationParam& animationParam)
	{
		//変更をチェックする関数のリストを走査
		for (auto func : m_transitionConditionList)
		{
			//変更チェック関数を実行
			AnimationEnum afterEnum = func(animationParam);

			//戻ってきた列挙体が自らのアニメーションと違った場合
			if (afterEnum != myEnum)
			{
				//遷移後のアニメーションの番号の列挙体を返す
				return afterEnum;
			}
		}

		//変更がなかったため自分の番号を返す
		return myEnum;
	}

	void AddCondition(const std::function<AnimationEnum(AnimationParam)>& conditionFunc)
	{
		m_transitionConditionList.push_back(conditionFunc);
	}

	AnimationEnum GetMyState()
	{
		return myEnum;
	}

	void SetState(AnimationEnum animEnum)
	{
		myEnum = animEnum;
	}
private:
	AnimationEnum myEnum;
	std::list<std::function<AnimationEnum(AnimationParam)>> m_transitionConditionList;
};

