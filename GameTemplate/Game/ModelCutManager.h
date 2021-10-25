#pragma once

namespace Game
{
	class ModelCutManager
	{
	private:
		//NOTE:�V���O���g���p�^�[���B�@�V���O���g���p�^�[���̐���͌�قǌ�������
		//�Q�[���V�[���ȊO�Őؒf���s��Ȃ��Ȃ�GameScene�Ƃ��ɕR�t�����ق�������?
		ModelCutManager() = default;
		~ModelCutManager() = default;
		static ModelCutManager* m_instance;

	public:
		/**
		 * @brief �C���X�^���X�̐���
		*/
		static void CreateInstance()
		{
			if (!m_instance)
			{
				m_instance = new ModelCutManager;
			}
		}

		/**
		 * @brief �C���X�^���X�̍폜
		*/
		static void DeleteInstance()
		{
			delete m_instance;
			m_instance = nullptr;
		}

		/**
		 * @brief 
		 * @return CubObjectManager�C���X�^���X 
		*/
		static ModelCutManager* GetInstance()
		{
			return m_instance;
		}

		/**
		 * @brief 
		 * @param cuttableModel �ؒf���郂�f��
		*/
		void AddCuttable(SkinModelRender* cuttableModel)
		{
			//�ؒf�t���O��true�Ȃ�
			if (cuttableModel->GetDivideFlag() == true)
			{
				//�J�b�g���郂�f���̃R���e�i�ɑ}��
				m_cutModelPtrSet.insert(cuttableModel);
			}
		}

		/**
		 * @brief ���݂̐ؒf���I��������ǉ�����郂�f��(�ؒf��̐V�K���f��)���Z�b�g
		 * @param cuttableModel �ǉ�����郂�f��
		*/
		void AddNextCuttable(SkinModelRender* cuttableModel)
		{
			//NOTE:���̂܂�m_cutModelPtrSet�ɑ}������ƁA�ؒf�̌��ʂł������f����������x�ؒf���Ă��܂�
			//���̐ؒf�łł������f����������x�ؒf��...�̃��[�v���N����̂ŕʂɃZ�b�g���Čォ��ǉ�����
			m_nextCutModelPtrList.insert(cuttableModel);
		}

		/**
		 * @brief m_cutModelPtrSet�ɓo�^����Ă��郂�f���̂����AcutPoint�����苗���ȓ��̂��̂�ؒf����
		 * @param cutNormal �ؒf�ʂ̈�_����̖@���̌���
		 * @param cutPoint �ؒf�ʂ̈�_�̍��W
		*/
		void QueryCut(const Vector3& cutNormal, const Vector3& cutPoint, float cutDistance);

	private:
		std::set<SkinModelRender*> m_cutModelPtrSet;		//�ؒf�\�ȃ��f���̃R���e�i
		std::set<SkinModelRender*> m_nextCutModelPtrList;	//�ؒf���I��������ǉ�����郂�f���̃R���e�i
	};
}
