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

	//�X�e�[�g�̕ύX���m���߂�
	AnimationEnum StateChangeCheck(const AnimationParam& animationParam)
	{
		//�ύX���`�F�b�N����֐��̃��X�g�𑖍�
		for (auto func : m_transitionConditionList)
		{
			//�ύX�`�F�b�N�֐������s
			AnimationEnum afterEnum = func(animationParam);

			//�߂��Ă����񋓑̂�����̃A�j���[�V�����ƈ�����ꍇ
			if (afterEnum != myEnum)
			{
				//�J�ڌ�̃A�j���[�V�����̔ԍ��̗񋓑̂�Ԃ�
				return afterEnum;
			}
		}

		//�ύX���Ȃ��������ߎ����̔ԍ���Ԃ�
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

