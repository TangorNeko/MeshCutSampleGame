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
		//WARNING:mapに使用するためだけにVector3同士の比較演算子を適当に定義しているので、比較の結果が正しくない可能性がある。
		//m_newVertexContainer周りでエラーが起きた際はまずそこをあたること。
		std::map<std::pair<Vector3, Vector3>, Vector3> m_newVertexContainer;	//新しくできた頂点を格納する連想配列
	};
}

