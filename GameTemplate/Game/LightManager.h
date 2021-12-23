#pragma once
#include <map>
#include <utility>
#include "stdafx.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace
{
	//�e���C�g�̍ő吔�@���̐���model.fx�̃��C�g�z��̐��ƈꏏ�ɂ��Ȃ���΂Ȃ�Ȃ�
	const int DIRECTIONLIGHT_NUMBER_MAX = 5;
	const int POINTLIGHT_NUMBER_MAX = 20;
	const int SPOTLIGHT_NUMBER_MAX = 20;
}

//TODO:���C�g�����ۍő吔�ȏ�̎�throw����̂ł͂Ȃ���ԌÂ����C�g�������悤�ȏ����ɂ�������悢?
//����ɏ���(��:��ԌÂ����C�g�^�O�ɑΉ�����������-1�ɂ��AUpdateLight��-1�Ȃ�Light.cpp��Delete(this)������)
//�Ɩ����ȃA�h���X�ɒl�����邱�ƂɂȂ肻������
//�f�[�^���g��Ȃ������ŏ����Ȃ��悤�ɂ���(��:�Â����C�g�^�O�ɑΉ�����������-1�ɂ��邾��)
//�Ƃ���͂���Ń������̖��ʌ����ɂȂ肻��

namespace Light
{
	class LightManager
	{
	private:
		LightManager() = default;
		~LightManager() = default;

		static LightManager* m_instance;

		//���C�g�̏��ƃJ�����̈ʒu�ƃ��C�g�̐����W�߂��f�[�^(�萔�o�b�t�@�Ƃ���HLSL�ɑ�����\����)
		struct LigDatas
		{
			Light::DirLigData directionLightArray[DIRECTIONLIGHT_NUMBER_MAX];	//�f�B���N�V�������C�g�̃f�[�^�̔z��
			Light::PointLigData pointLightArray[POINTLIGHT_NUMBER_MAX];		//�|�C���g���C�g�̃f�[�^�̔z��
			Light::SpotLigData spotLightArray[SPOTLIGHT_NUMBER_MAX];			//�X�|�b�g���C�g�̃f�[�^�̔z��
			Vector3 eyePos = Vector3::Zero;										//�J�����̈ʒu
			int directionLightNum = 0;											//�f�B���N�V�������C�g�̐�
			int pointLightNum = 0;												//�|�C���g���C�g�̐�
			int spotLightNum = 0;												//�X�|�b�g���C�g�̐�
		};

		struct LigCameraDatas
		{
			Matrix lightCameraProjectionMatrix;				//���C�g�r���[�v���W�F�N�V�����s��
			Vector3 lightCameraPosition = Vector3::Zero;	//���C�g�J�����̈ʒu
			float pad = 0;									//�p�f�B���O
			Vector3 lightCameraDirection = Vector3::Zero;	//���C�g�J�����̌���
		};

		LigDatas m_ligData;									//���C�g�̃f�[�^
		int m_size = sizeof(m_ligData);						//���C�g�̃f�[�^�̃T�C�Y

		LigCameraDatas m_ligCameraData;						//���C�g���_�̃J�����̃f�[�^
		int m_ligCameraDataSize = sizeof(m_ligCameraData);	//���C�g���_�̃J�����̃f�[�^�̃T�C�Y

		int m_dirLigNum = 0;								//���ɍ����f�B���N�V�������C�g�ɕt�^����^�O�ԍ�(������܂łɉ��f�B���N�V�������C�g�����ꂽ��)
		int m_pointLigNum = 0;								//���ɍ����|�C���g���C�g�ɕt�^����^�O�ԍ�(������܂łɉ��|�C���g���C�g�����ꂽ��)
		int m_spotLigNum = 0;								//���ɍ����X�|�b�g���C�g�ɕt�^����^�O�ԍ�(������܂łɉ��X�|�b�g���C�g�����ꂽ��)
		std::map<int, int> m_dirLigMap;						//�f�B���N�V�������C�g�̃^�O���猻�݂̃f�B���N�V�������C�g�̈ʒu��Ԃ��Ă����Map
		std::map<int, int> m_pointLigMap;					//�|�C���g���C�g�̃^�O���猻�݂̃|�C���g���C�g�̈ʒu��Ԃ��Ă����Map
		std::map<int, int> m_spotLigMap;					//�X�|�b�g���C�g�̃^�O���猻�݂̃X�|�b�g���C�g�̈ʒu��Ԃ��Ă����Map

		Camera m_lightCamera;								//�V���h�E�}�b�v�p�̃��C�g�̈ʒu�̃J�����B
	public:

		/**
		 * @brief ���C�g�}�l�[�W���[�̃C���X�^���X���쐬����
		*/
		static void CreateInstance()
		{
			if (!m_instance)
			{
				m_instance = new LightManager;
			}
		}

		/**
		 * @brief ���C�g�}�l�[�W���[�̃C���X�^���X���폜����
		*/
		static void DeleteInstance()
		{
			delete m_instance;
			m_instance = nullptr;
		}

		/**
		 * @brief ���C�g�}�l�[�W���[�̃C���X�^���X���擾����
		*/
		static LightManager* GetInstance() { return m_instance; }

		/**
		 * @brief ���C�g�̃f�[�^�̉���擾����(�萔�o�b�t�@�ɓn���p)
		 * @return �S���C�g�̃f�[�^
		*/
		LigDatas* GetLigDatas() { return &m_ligData; }

		/**
		 * @brief ���C�g�̃f�[�^�̉�̃T�C�Y���擾����(�萔�o�b�t�@�ɓn���p)
		 * @return �S���C�g�̃f�[�^�̃T�C�Y
		*/
		int GetLigDataSize() const { return m_size; }

		/**
		 * @brief �J�����̃|�W�V�������X�V����
		 * @param �J�����̔ԍ�
		*/
		void UpdateEyePos() { m_ligData.eyePos = g_camera3D->GetPosition(); }

		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̃f�[�^�̉���擾����(�萔�o�b�t�@�ɓn���p)
		 * @return ���C�g�J�����̃f�[�^
		*/
		LigCameraDatas* GetLigCameraDatas() { return &m_ligCameraData; }

		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̃f�[�^�̉�̃T�C�Y���擾����(�萔�o�b�t�@�ɓn���p)
		 * @return ���C�g�J�����̃f�[�^�̃T�C�Y
		*/
		int GetLigCameraDataSize() const { return m_ligCameraDataSize; }

		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�������擾
		 * @return ���C�g�J����
		*/
		Camera* GetLightCamera()
		{
			return &m_lightCamera;
		}

		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̍��W���w�肷��
		 * @param pos ���C�g�J�����̍��W
		*/
		void SetLightCameraPosition(const Vector3& pos)
		{
			//���W���Z�b�g
			m_lightCamera.SetPosition(pos);
			m_lightCamera.Update();

			//���C�g�J�����̃f�[�^���X�V��̂��̂ɐݒ�
			m_ligCameraData.lightCameraPosition = pos;
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();

			m_ligCameraData.lightCameraDirection = m_lightCamera.GetTarget() - m_lightCamera.GetPosition();
			m_ligCameraData.lightCameraDirection.Normalize();
		}

		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̃^�[�Q�b�g�̍��W���w�肷��
		 * @param targetPos ���C�g�J�����̃^�[�Q�b�g�̍��W
		*/
		void SetLightCameraTarget(const Vector3& targetPos)
		{
			//�^�[�Q�b�g�̍��W���Z�b�g
			m_lightCamera.SetTarget(targetPos);
			m_lightCamera.Update();

			//���C�g�J�����̃f�[�^���X�V��̂��̂ɐݒ�
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();

			m_ligCameraData.lightCameraDirection = m_lightCamera.GetTarget() - m_lightCamera.GetPosition();
			m_ligCameraData.lightCameraDirection.Normalize();
		}

		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̏�������w�肷��
		 * @param up �J�����̏�����̃x�N�g��
		*/
		void SetLightCameraUp(const Vector3& up)
		{
			//������̃x�N�g�����Z�b�g
			m_lightCamera.SetUp(up);
			m_lightCamera.Update();

			//���C�g�J�����̃f�[�^���X�V��̂��̂ɐݒ�
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();
		}

		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̎ˉe�s��̍X�V���@��ݒ肷��
		 * @param func �ˉe�s��̍X�V���@
		*/
		void SetLightCameraUpdateProjMatrixFunc(Camera::EnUpdateProjMatrixFunc func)
		{
			//�ˉe�s��̍X�V���@���Z�b�g
			m_lightCamera.SetUpdateProjMatrixFunc(func);
			m_lightCamera.Update();

			//���C�g�J�����̃f�[�^���X�V��̂��̂ɐݒ�
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();
		}

		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̉������w�肷��(���s���e����)
		 * @param width ���C�g�J�����̉���
		*/
		void SetLightCameraWidth(const float& width)
		{
			//�������Z�b�g
			m_lightCamera.SetWidth(width);
			m_lightCamera.Update();

			//���C�g�J�����̃f�[�^���X�V��̂��̂ɐݒ�
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();
		}

		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̍������w�肷��(���s���e����)
		 * @param height ���C�g�J�����̍���
		*/
		void SetLightCameraHeight(const float& height)
		{
			//�������Z�b�g
			m_lightCamera.SetHeight(height);
			m_lightCamera.Update();

			//���C�g�J�����̃f�[�^���X�V��̂��̂ɐݒ�
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();
		}

		//�f�B���N�V�������C�g�p////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief �f�B���N�V�������C�g�̐��̃J�E���g���v���X����
		*/
		void DirectionLightPlus() { m_ligData.directionLightNum++; }

		/**
		 * @brief �f�B���N�V�������C�g�̐��̃J�E���g���}�C�i�X����
		*/
		void DirectionLightMinus() {
			m_ligData.directionLightNum--;

			//���C�g�̐���0�ȉ��ɂȂ鎞�͂��������Ȃ��Ă���̂�throw����(�N���蓾�Ȃ��ƐM������)
			if (m_ligData.directionLightNum < 0)
			{
				MessageBoxA(nullptr, "�f�B���N�V�������C�g�̐����}�C�i�X�ɂȂ��Ă��܂��B\n", "�G���[", MB_OK);
			}
		}

		/**
		 * @brief ���C�g�}�l�[�W���[�Ƀf�B���N�V�������C�g��o�^����
		 * @param dirLigData �f�B���N�V�������C�g�̃f�[�^
		 * @return �o�^�����f�B���N�V�������C�g�̃^�O�ԍ�(���C�g�̏ƍ��Ɏg��)
		*/
		int AddDirectionLight(Light::DirLigData* dirLigData)
		{
			//���C�g�̐����ŏ��Ɍ��߂����ȏ�Ȃ�throw����(�����ς����Ƃӂ��ɋN����)
			if (m_ligData.directionLightNum >= DIRECTIONLIGHT_NUMBER_MAX)
			{
				MessageBoxA(nullptr, "�f�B���N�V�������C�g�̐����ő吔�𒴂��Ă��܂��B\n", "�G���[", MB_OK);
			}

			//�󂫂̒��ň�Ԑ擪�ʒu�Ƀf�[�^���i�[����
			m_ligData.directionLightArray[m_ligData.directionLightNum] = *dirLigData;

			//��������C�g�̃^�O�ԍ��ƃf�[�^�̊i�[�ʒu���֘A�t����
			m_dirLigMap.insert(std::make_pair(m_dirLigNum, m_ligData.directionLightNum));

			//���Ƀ��C�g������ʒu�����炷
			DirectionLightPlus();

			//��������C�g�̃^�O�ԍ���Ԃ��A1���₵�Ď��̃��C�g�̍쐬�ɔ�����
			return m_dirLigNum++;
		}

		/**
		 * @brief �f�B���N�V�������C�g���폜����
		 * @param directionLightTag �폜�������f�B���N�V�������C�g�̃^�O�ԍ�
		*/
		void RemoveDirectionLight(int directionLightTag)
		{
			//�^�O����폜����ʒu���擾���A�폜���郉�C�g���\�[�g�ň�ԏI�[�Ɏ����Ă���
			for (int i = m_dirLigMap.at(directionLightTag); i < m_ligData.directionLightNum - 1; i++)
			{
				std::swap(m_ligData.directionLightArray[i], m_ligData.directionLightArray[i + 1]);
			}

			//���C�g�̐������炷���Ƃŋ󂢂��ʒu�Ɏ��̃��C�g��������悤�ɂȂ�
			DirectionLightMinus();

			//�폜�������C�g�ȍ~�̃��C�g�̈ʒu�ԍ���������ꂽ�̂Ń^�O�ɑΉ������ʒu�̔ԍ���1�����炷
			for (auto i = m_dirLigMap.upper_bound(directionLightTag); i != m_dirLigMap.end(); i++)
			{
				i->second--;
			}
		}

		/**
		 * @brief �f�B���N�V�������C�g�̏����X�V����
		 * @param directionLightTag �X�V���������C�g�̃^�O�ԍ�
		 * @param dirLigData �X�V���������C�g�̐V�������C�g�f�[�^
		*/
		void UpdateDirectionLight(int directionLightTag, Light::DirLigData* dirLigData)
		{
			//�^�O����擾�������C�g�̈ʒu�̃f�[�^���X�V����B
			m_ligData.directionLightArray[m_dirLigMap.at(directionLightTag)] = *dirLigData;
		}

		//�|�C���g���C�g�p///////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief �|�C���g���C�g�̐��̃J�E���g���v���X����
		*/
		void PointLightPlus() { m_ligData.pointLightNum++; }

		/**
		 * @brief �|�C���g���C�g�̐��̃J�E���g���}�C�i�X����
		*/
		void PointLightMinus()
		{
			m_ligData.pointLightNum--;

			//���C�g�̐���0�ȉ��ɂȂ鎞�͂��������Ȃ��Ă���̂�throw����(�N���蓾�Ȃ��ƐM������)
			if (m_ligData.pointLightNum < 0)
			{
				MessageBoxA(nullptr, "�|�C���g���C�g�̐����}�C�i�X�ɂȂ��Ă��܂��B\n", "�G���[", MB_OK);
			}
		}

		/**
		 * @brief ���C�g�}�l�[�W���[�Ƀ|�C���g���C�g��o�^����
		 * @param pointLigData �|�C���g���C�g�̃f�[�^
		 * @return �o�^�����|�C���g���C�g�̃^�O�ԍ�(���C�g�̏ƍ��Ɏg��)
		*/
		int AddPointLight(Light::PointLigData* pointLigData)
		{
			//���C�g�̐����ŏ��Ɍ��߂����ȏ�Ȃ�throw����(�����ς����Ƃӂ��ɋN����)
			if (m_ligData.pointLightNum >= POINTLIGHT_NUMBER_MAX)
			{
				MessageBoxA(nullptr, "�|�C���g���C�g�̐����ő吔�𒴂��Ă��܂��B\n", "�G���[", MB_OK);
			}

			//�󂫂̒��ň�Ԑ擪�ʒu�Ƀf�[�^���i�[����
			m_ligData.pointLightArray[m_ligData.pointLightNum] = *pointLigData;

			//��������C�g�̃^�O�ԍ��ƃf�[�^�̊i�[�ʒu���֘A�t����
			m_pointLigMap.insert(std::make_pair(m_pointLigNum, m_ligData.pointLightNum));

			//���Ƀ��C�g������ʒu�����炷
			PointLightPlus();

			//��������C�g�̃^�O�ԍ���Ԃ��A1���₵�Ď��̃��C�g�̍쐬�ɔ�����
			return m_pointLigNum++;
		}

		/**
		 * @brief �|�C���g���C�g���폜����
		 * @param pointLightTag �폜�������|�C���g���C�g�̃^�O�ԍ�
		*/
		void RemovePointLight(int pointLightTag)
		{
			//�^�O����폜����ʒu���擾���A�폜���郉�C�g���\�[�g�ň�ԏI�[�Ɏ����Ă���
			for (int i = m_pointLigMap.at(pointLightTag); i < m_ligData.pointLightNum - 1; i++)
			{
				std::swap(m_ligData.pointLightArray[i], m_ligData.pointLightArray[i + 1]);
			}

			//���C�g�̐������炷���Ƃŋ󂢂��ʒu�Ɏ��̃��C�g��������悤�ɂȂ�
			PointLightMinus();

			//�폜�������C�g�ȍ~�̃��C�g�̈ʒu�ԍ���������ꂽ�̂Ń^�O�ɑΉ������ʒu�̔ԍ���1�����炷
			for (auto i = m_pointLigMap.upper_bound(pointLightTag); i != m_pointLigMap.end(); i++)
			{
				i->second--;
			}
		}

		/**
		 * @brief �|�C���g���C�g�̏����X�V����
		 * @param pointLightTag �X�V���������C�g�̃^�O�ԍ�
		 * @param pointLigData �X�V���������C�g�̐V�������C�g�f�[�^
		*/
		void UpdatePointLight(int pointLightTag, Light::PointLigData* pointLigData)
		{
			m_ligData.pointLightArray[m_pointLigMap.at(pointLightTag)] = *pointLigData;
		}

		//�X�|�b�g���C�g�p/////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief �X�|�b�g���C�g�̐��̃J�E���g���v���X����
		*/
		void SpotLightPlus() { m_ligData.spotLightNum++; }

		/**
		 * @brief �X�|�b�g���C�g�̐��̃J�E���g���}�C�i�X����
		*/
		void SpotLightMinus()
		{
			m_ligData.spotLightNum--;

			//���C�g�̐���0�ȉ��ɂȂ鎞�͂��������Ȃ��Ă���̂�throw����(�N���蓾�Ȃ��ƐM������)
			if (m_ligData.spotLightNum < 0)
			{
				MessageBoxA(nullptr, "�X�|�b�g���C�g�̐����}�C�i�X�ɂȂ��Ă��܂��B\n", "�G���[", MB_OK);
			}
		}

		/**
		 * @brief ���C�g�}�l�[�W���[�ɃX�|�b�g���C�g��o�^����
		 * @param spotLigData �X�|�b�g���C�g�̃f�[�^
		 * @return �o�^�����X�|�b�g���C�g�̃^�O�ԍ�(���C�g�̏ƍ��Ɏg��)
		*/
		int AddSpotLight(Light::SpotLigData* spotLigData)
		{
			//���C�g�̐����ŏ��Ɍ��߂����ȏ�Ȃ�throw����(�����ς����Ƃӂ��ɋN����)
			if (m_ligData.spotLightNum >= SPOTLIGHT_NUMBER_MAX)
			{
				MessageBoxA(nullptr, "�X�|�b�g���C�g�̐����ő吔�𒴂��Ă��܂��B\n", "�G���[", MB_OK);
			}

			//�󂫂̒��ň�Ԑ擪�ʒu�Ƀf�[�^���i�[����
			m_ligData.spotLightArray[m_ligData.spotLightNum] = *spotLigData;

			//��������C�g�̃^�O�ԍ��ƃf�[�^�̊i�[�ʒu���֘A�t����
			m_spotLigMap.insert(std::make_pair(m_spotLigNum, m_ligData.spotLightNum));

			//���Ƀ��C�g������ʒu�����炷
			SpotLightPlus();

			//��������C�g�̃^�O�ԍ���Ԃ��A1���₵�Ď��̃��C�g�쐬�ɔ�����
			return m_spotLigNum++;
		}

		/**
		 * @brief �X�|�b�g���C�g���폜����
		 * @param spotLightTag �폜�������X�|�b�g���C�g�̃^�O�ԍ�
		*/
		void RemoveSpotLight(int spotLightTag)
		{
			//�^�O����폜����ʒu���擾���A�폜���郉�C�g���\�[�g�ň�ԏI�[�Ɏ����Ă���
			for (int i = m_spotLigMap.at(spotLightTag); i < m_ligData.spotLightNum - 1; i++)
			{
				std::swap(m_ligData.spotLightArray[i], m_ligData.spotLightArray[i + 1]);
			}

			//���C�g�̐������炷���Ƃŋ󂢂��ʒu�Ɏ��̃��C�g��������悤�ɂȂ�
			SpotLightMinus();

			//�폜�������C�g�ȍ~�̃��C�g�̈ʒu�ԍ���������ꂽ�̂Ń^�O�ɑΉ������ʒu�̔ԍ���1�����炷
			for (auto i = m_spotLigMap.upper_bound(spotLightTag); i != m_spotLigMap.end(); i++)
			{
				i->second--;
			}
		}

		/**
		 * @brief �X�|�b�g���C�g�̏����X�V����
		 * @param spotLightTag �X�V���������C�g�̃^�O�ԍ�
		 * @param spotLigData �X�V���������C�g�̐V�������C�g�f�[�^
		*/
		void UpdateSpotLight(int spotLightTag, Light::SpotLigData* spotLigData)
		{
			m_ligData.spotLightArray[m_spotLigMap.at(spotLightTag)] = *spotLigData;
		}

	};
}

