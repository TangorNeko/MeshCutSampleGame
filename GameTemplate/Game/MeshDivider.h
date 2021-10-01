#pragma once

namespace Util
{
	class MeshDivider
	{
		/**
		 * @brief メッシュの分割(仮)
		*/
		void Divide();

	private:
		std::map<std::pair<Vector3, Vector3>, Vector3> m_newVertexContainer;	//新しくできた頂点を格納する連想配列、
	};
}

