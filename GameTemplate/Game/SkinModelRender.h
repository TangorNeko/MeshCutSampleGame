#pragma once

namespace Game
{
	class SkinModelRender : public IGameObject
	{
	private:
		~SkinModelRender()
		{
			//���f�������݂��Ă�����폜
			if (m_model != nullptr)
			{
				delete m_model;
			}
		}

		/**
		 * @brief ���f���̍��W�A��]�A�g��̃A�b�v�f�[�g
		*/
		void UpdateModel()
		{
			m_model->UpdateWorldMatrix(m_position, m_qRot, m_scale);
			m_skeleton.Update(m_model->GetWorldMatrix());
		}

	public:
		/**
		 * @brief ���f���̕`��
		 * @param rc �����_�[�R���e�L�X�g
		*/
		void Render(RenderContext& rc)override;

		/**
		 * @brief ���f���̏������֐��@�A�j���[�V������
		 * @param modelPath ���f���t�@�C���̃p�X(.tkm)
		 * @param skeletonPath �X�P���g���t�@�C���̃p�X(.tks)
		 * @param animClips �A�j���[�V�����N���b�v�̔z��
		 * @param animClipNum �A�j���[�V�����N���b�v�̐�
		*/
		void Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum, EnModelUpAxis modelUpAxis = enModelUpAxisY);

		/**
		 * @brief ���f���̏������֐�
		 * @param modelPath ���f���t�@�C���̃p�X(.tkm)
		 * @param skeletonPath �X�P���g���t�@�C���̃p�X(.tks)
		*/
		void Init(const char* modelPath, const char* skeletonPath, EnModelUpAxis modelUpAxis = enModelUpAxisZ);

		/**
		 * @brief ���f���̏������֐��@���f���p�X������
		 * @param modelPath ���f���t�@�C���̃p�X(.tkm)
		*/
		void Init(const char* modelPath, EnModelUpAxis modelUpAxis = enModelUpAxisZ);

		/**
		 * @brief ���f���N���X�̃|�C���^����̏�����
		 * @param model 
		*/
		void InitFromModel(Model* model);

		/**
		 * @brief �A�j���[�V�����̍Đ�
		 * @param animationNo �A�j���[�V�����ԍ�
		 * @param interpolateTime �A�j���[�V�����̕⊮����
		*/
		void PlayAnimation(int animationNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animationNo, interpolateTime);
			m_animation.Progress(m_animationSpeed / 60.0f);
			UpdateModel();
		}

		/**
		 * @brief �A�j���[�V�����̍Đ����x��ݒ肷��
		 * @param animationSpeed �Đ����x
		*/
		void SetAnimationSpeed(float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		/**
		 * @brief �A�j���[�V�������Đ����Ă��邩?
		 * @return �Đ����Ă���:true �Đ����Ă��Ȃ�:false
		*/
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/**
		 * @brief ���f���̍��W��ݒ肷��
		 * @param pos ���W
		*/
		void SetPosition(Vector3 pos)
		{
			m_position = pos;
			UpdateModel();
		}

		/**
		 * @brief ���f���̉�]��ݒ肷��
		 * @param qRot ��]
		*/
		void SetRotation(Quaternion qRot)
		{
			m_qRot = qRot;
			UpdateModel();
		}

		/**
		 * @brief ���f���̊g�嗦��ݒ肷��
		 * @param scale �g�嗦
		*/
		void SetScale(Vector3 scale)
		{
			m_scale = scale;
			UpdateModel();
		}

		/**
		 * @brief ���f���̃��[���h�s��𒼐ڃZ�b�g����
		 * @param world ���[���h�s��
		*/
		void SetMatrix(Matrix world)
		{
			m_model->SetWorldMatrix(world);
			m_skeleton.Update(world);
		}

		/**
		 * @brief �ؒf�\���̃t���O���Z�b�g
		 * @param isDividable �ؒf�\��?
		*/
		void SetDivideFlag(bool isDividable);

		/**
		 * @brief ���f���̏������f�[�^���Z�b�g
		 * @param modelInitData ���f���̏������f�[�^
		*/
		void SetModelInitData(const ModelInitData& modelInitData)
		{
			//TODO:modelInitData���g��Ȃ����@��T��
			m_modelInitData = modelInitData;
		}

		/**
		 * @brief ���f���̍��W���擾
		 * @return ���W
		*/
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/**
		 * @brief ���f���̉�]���擾
		 * @return ��]
		*/
		const Quaternion& GetRotation() const
		{
			return m_qRot;
		}

		/**
		 * @brief ���f���̊g�嗦���擾
		 * @return �g�嗦
		*/
		const Vector3& GetScale() const
		{
			return m_scale;
		}

		/**
		 * @brief �ؒf�\���̃t���O���擾
		 * @return �ؒf�\��?
		*/
		bool GetDivideFlag() const
		{
			return m_isDividable;
		}

		Model* GetModel()
		{
			return m_model;
		}

		/**
		 * @brief ���f���̐ؒf
		 * @param cutNormal �ؒf�ʂ̈�_����̖@���̌���
		 * @param cutPoint �ؒf�ʂ̈�_�̍��W
		*/
		void Divide(const Vector3& cutNormal, const Vector3& cutPoint);
	private:
		Model* m_model = nullptr;					//���f��
		Vector3 m_position = Vector3::Zero;			//���W
		Quaternion m_qRot = Quaternion::Identity;	//��]
		Vector3 m_scale = Vector3::One;				//�g��
		Skeleton m_skeleton;						//�X�P���g��
		AnimationClip* m_animationClips = nullptr;	//�A�j���[�V�����N���b�v
		int m_animationClipNum = 0;					//�A�j���[�V�����N���b�v�̐�
		Animation m_animation;						//�A�j���[�V����
		float m_animationSpeed = 1.0f;				//�A�j���[�V�����̑��x
		bool m_isDividable = false;					//�ؒf�\?

		//NOTE:�ؒf���ĐV�����ł������f�������������邽�߂ɂ͌��̃��f����ModelInitData��
		//�K�v�Ȃ̂ŁASkinModelRender�̃����o�ϐ��Ƃ��ē���Ă���
		//TODO:�Ȃ�Ƃ����ĕʂ̕��@��T��������...
		ModelInitData m_modelInitData;				//���f���̏������f�[�^
	};
}
