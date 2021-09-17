#pragma once

namespace Util
{
	struct PlaneData
	{
		Vector3 planePoint = Vector3::Zero;		//���ʂ̈�_
		Vector3 planeNormal = Vector3::Right;	//���ʂ̖@��
	};

	struct TriangleData
	{
		Vector3 vertices[3];					//���_
	};

	struct VerticesPack
	{
		std::vector<Vector3> frontVertices;		//���ʂ̕\���ɂ��钸�_�̔z��
		std::vector<Vector3> backVertices;		//���ʂ̗����ɂ��钸�_�̔z��
		std::vector<Vector3> onPlaneVertices;	//���ʏ�ɂ��钸�_�̔z��
	};


	/**
	 * @brief ���ʂ��O�p�`�𕪊����Ă���?
	 * @param[in] planeData ���ʂ̃f�[�^
	 * @param[in] triangleData �O�p�`�̃f�[�^
	 * @param[out] verticesPack ���_�̃f�[�^
	 * @return �������Ă���:true ���Ă��Ȃ�:false
	*/
	bool IsPlaneDivideTriangle(const PlaneData& planeData, const TriangleData& triangleData, VerticesPack& verticesPack);
}