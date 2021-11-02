#pragma once
class CapsuleAxis
{
private:
	/// <summary>
	/// 平面。
	/// </summary>
	struct SPlane {
		Vector3 normal;		// 平面の法線
		float distance;		// 平面までの距離。
	};
public:
	/// <summary>
	/// BSPツリーの要素の種類
	/// </summary>
	enum EnEntityType {
		enEntityType_Node,
		enEntityType_Leaf,
		enEntityType_LeafList
	};
	/// <summary>
	/// BSPツリーの要素
	/// </summary>
	struct SEntity {
		int type = enEntityType_Node;
	};

	using SEntityPtr = std::shared_ptr<SEntity>;
	/// <summary>
	/// ノード
	/// </summary>
	struct SNode : public SEntity {
		SPlane plane;				// 分割平面。
		SEntityPtr rightEntity;		// 右の要素
		SEntityPtr leftEntity;		// 左の要素。
		Vector3 centerPos;			// 中心座標。
		std::vector<SEntityPtr> leafArray;	// ノードが内包しているリーフ。
	};
	/// <summary>
	/// リーフ
	/// </summary>
	struct SLeaf : public SEntity {
		Vector3 position;
		void* extraData;
	};
	/// <summary>
	/// リーフのリスト
	/// </summary>
	struct SLeafList : public SEntity {
		std::vector< SEntityPtr > leafList;
	};
private:

	SEntityPtr m_rootNode = nullptr;		// ルートノード。
	std::vector<SEntityPtr> m_leafArray;	// リーフの配列。
private:
	/// <summary>
	/// 共分散行列から分割平面を計算する。
	/// </summary>
	/// <param name="plane"></param>
	/// <param name="covarianceMatrix"></param>
	void CalcSplitPlaneFromCovarianceMatrix(
		SPlane& plane,
		float covarianceMatrix[3][3],
		const Vector3& centerPos,
		const std::vector<SEntityPtr>& leafArray
	);
	/// <summary>
	/// リーフノードの配列から共分散行列を計算する。
	/// </summary>
	/// <param name="covarianceMatrix">共分散行列の計算先</param>
	/// <param name="leafNodeArray">リーフノードの配列</param>
	/// <param name="centerPos">リーフノードの中心座標</param>
	void CalcCovarianceMatrixFromLeafNodeList(
		float covarianceMatrix[3][3],
		const std::vector<SEntityPtr>& leafArray,
		const Vector3& centerPos
	);
	/// <summary>
	/// リーフのリストから中心座標を計算する。
	/// </summary>
	/// <param name="leafNodeArray"></param>
	/// <returns></returns>
	Vector3 CalcCenterPositionFromLeafList(
		const std::vector<SEntityPtr>& leafArray
	);
	/// <summary>
	/// 新しいBSPツリーの要素を作成する。
	/// </summary>
	/// <param name="leafNodeArray"></param>
	void CreateBSPTreeEntity(const std::vector<SEntityPtr>& leafArray);
};

