#pragma once

namespace Util
{
	class MeshDivider
	{
		/**
		 * @brief ���b�V���̕���(��)
		*/
		void Divide();

	private:
		std::map<std::pair<Vector3, Vector3>, Vector3> m_newVertexContainer;	//�V�����ł������_���i�[����A�z�z��A
	};
}

