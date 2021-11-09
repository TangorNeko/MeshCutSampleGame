#pragma once

/**
 * @brief ���p�`�����̃N���X
* �ȉ��̃y�[�W�̃v���O�������𗘗p���Ă��܂��B
* http://www.prefield.com/algorithm/geometry/triangulate.html
*/
class TwoDTriangulate
{
	/**
	 * @brief �_a,b,c�̏��Ԃɐi�ގ�
	 * @param a �_a�̍��W
	 * @param b �_b�̍��W
	 * @param c �_c�̍��W
	 * @return 
		+1:a-b�̌㔽���v��������b-c
		-1:a-b�̌㎞�v��������b-c
		+2:a-b�̌�t��������a���܂����`��b-c�@	(c-a-b)
		-2:a-b�̌セ�̂܂ܒ��i����b-c			(a-b-c)
		 0:a-b�̌�t��������b-c					(a-c-b)
	*/
	int CCW(Vector2 a, Vector2 b, Vector2 c);

	/**
	 * @brief �O�_����O�p�`���쐬
	 * @param a �_a�̍��W
	 * @param b �_b�̍��W
	 * @param c �_c�̍��W
	 * @return �_1,2,3����̃R���e�i�ɂ܂Ƃ߂���
	*/
	std::vector<Vector2> MakeTriangle(const Vector2& a, const Vector2& b, const Vector2& c);

	/**
	 * @brief �C���f�b�N�X�o�b�t�@�ɎO�p�`���\������_��ǉ�����
	 * @param a �_a�̍��W
	 * @param b �_b�̍��W
	 * @param c �_c�̍��W
	*/
	void PushIndex(const Vector2& a, const Vector2& b, const Vector2 c);

	/**
	 * @brief �_P���O�p�`�̒��Ɋ܂܂�邩?
	 * @param tri �O�p�`�̃f�[�^
	 * @param p �_P�̍��W
	 * @return 
	*/
	bool TriangleContains(const std::vector<Vector2>& tri, const Vector2& p);

	/**
	 * @brief ������?
	*/
	bool EarQ(int i, int j, int k, const std::vector<Vector2>& P);

public:
	/**
	 * @brief �_�̍��W�ƃC���f�b�N�X�Ƃ̊֌W���������A�z�z����i�[����
	 * @param TDMap �A�z�z��
	*/
	void SetMap(std::map<Vector2, int>* TDMap)
	{
		m_2DMap = TDMap;
	}

	/**
	 * @brief ���p�`����
	 * @param P ��������_�Q
	*/
	void Triangulate(const std::vector<Vector2>& P);

	/**
	 * @brief �ؒf�ʂ̃C���f�b�N�X�o�b�t�@���擾
	 * @return �C���f�b�N�X�o�b�t�@
	*/
	std::vector<int> GetBuffer()
	{
		return m_indexBuffer;
	}
private:
	std::map<Vector2, int>* m_2DMap = nullptr;	//���f���̒��_��2D���W�Ƃ��̃C���f�b�N�X���֘A�t�����A�z�z��
	std::vector<int> m_indexBuffer;				//�ؒf�ʂ̃C���f�b�N�X�o�b�t�@
};

