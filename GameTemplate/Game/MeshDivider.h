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
		//WARNING:map�Ɏg�p���邽�߂�����Vector3���m�̔�r���Z�q��K���ɒ�`���Ă���̂ŁA��r�̌��ʂ��������Ȃ��\��������B
		//m_newVertexContainer����ŃG���[���N�����ۂ͂܂������������邱�ƁB
		std::map<std::pair<Vector3, Vector3>, Vector3> m_newVertexContainer;	//�V�����ł������_���i�[����A�z�z��
	};
}

