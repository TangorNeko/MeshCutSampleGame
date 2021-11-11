#pragma once

/**
 * @brief �A�j���[�V�����X�e�[�g�N���X
 * @tparam AnimationParam �A�j���[�V�����̑J�ڂ̔���Ɏg�p����p�����[�^�[
 * @tparam AnimationEnum �A�j���[�V�����̔ԍ���\���񋓑�
*/
template <typename AnimationParam,typename AnimationEnum>
class AnimationState
{
public:
	/**
	 * @brief �X�e�[�g�̕ύX���m���߂�
	 * @param animationParam �A�j���[�V�����̑J�ڂɎg���p�����[�^�[
	 * @return �ύX���m���߂����ʂ̃A�j���[�V�����̗񋓑�
	*/
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
				//�J�ڌ�̃A�j���[�V�����̗񋓑̂�Ԃ�
				return afterEnum;
			}
		}

		//�ύX���Ȃ��������ߎ����̗񋓑̂�Ԃ�
		return myEnum;
	}

	/**
	 * @brief �J�ڏ�����ǉ�
	 * @param conditionFunc �J�ڏ������L�q�����֐�(�����̓A�j���[�V�����p�����[�^�[�A�߂�l�̓A�j���[�V�����̗񋓑�)
	*/
	void AddCondition(const std::function<AnimationEnum(AnimationParam)>& conditionFunc)
	{
		//�������X�g�ɒǉ�����
		m_transitionConditionList.push_back(conditionFunc);
	}

	/**
	 * @brief �A�j���[�V�����̗񋓑̂��擾
	 * @return �A�j���[�V�����̗񋓑�
	*/
	AnimationEnum GetMyState()
	{
		return myEnum;
	}

	/**
	 * @brief �A�j���[�V�����̗񋓑̂�o�^
	 * @param animEnum �A�j���[�V�����̗񋓑�
	*/
	void SetState(AnimationEnum animEnum)
	{
		myEnum = animEnum;
	}
private:
	AnimationEnum myEnum;																//�A�j���[�V�����̗񋓑�
	std::list<std::function<AnimationEnum(AnimationParam)>> m_transitionConditionList;	//�A�j���[�V�����̑J�ڊ֐��̃��X�g
};

