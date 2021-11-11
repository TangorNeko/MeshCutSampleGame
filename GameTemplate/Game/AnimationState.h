#pragma once

/**
 * @brief アニメーションステートクラス
 * @tparam AnimationParam アニメーションの遷移の判定に使用するパラメーター
 * @tparam AnimationEnum アニメーションの番号を表す列挙体
*/
template <typename AnimationParam,typename AnimationEnum>
class AnimationState
{
public:
	/**
	 * @brief ステートの変更を確かめる
	 * @param animationParam アニメーションの遷移に使うパラメーター
	 * @return 変更を確かめた結果のアニメーションの列挙体
	*/
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
				//遷移後のアニメーションの列挙体を返す
				return afterEnum;
			}
		}

		//変更がなかったため自分の列挙体を返す
		return myEnum;
	}

	/**
	 * @brief 遷移条件を追加
	 * @param conditionFunc 遷移条件を記述した関数(引数はアニメーションパラメーター、戻り値はアニメーションの列挙体)
	*/
	void AddCondition(const std::function<AnimationEnum(AnimationParam)>& conditionFunc)
	{
		//条件リストに追加する
		m_transitionConditionList.push_back(conditionFunc);
	}

	/**
	 * @brief アニメーションの列挙体を取得
	 * @return アニメーションの列挙体
	*/
	AnimationEnum GetMyState()
	{
		return myEnum;
	}

	/**
	 * @brief アニメーションの列挙体を登録
	 * @param animEnum アニメーションの列挙体
	*/
	void SetState(AnimationEnum animEnum)
	{
		myEnum = animEnum;
	}
private:
	AnimationEnum myEnum;																//アニメーションの列挙体
	std::list<std::function<AnimationEnum(AnimationParam)>> m_transitionConditionList;	//アニメーションの遷移関数のリスト
};

