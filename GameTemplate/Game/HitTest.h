#pragma once

namespace Util
{
	/**
	 * @brief 平面のデータ
	*/
	struct PlaneData
	{
		/**
		 * @brief 平面上の一点を取得
		 * @return 平面上の一点の座標
		*/
		const Vector3& GetPoint() const { return planePoint; }

		/**
		 * @brief 平面上の一点を設定
		 * @param point 平面上の一点の座標
		*/
		void SetPoint(const Vector3& point)
		{
			planePoint = point;
		}

		/**
		 * @brief 法線を取得
		 * @return 法線のベクトル
		*/
		const Vector3& GetNormal()const  { return planeNormal; }

		/**
		 * @brief 法線を設定し正規化
		 * @param normal 法線のベクトル
		*/
		void SetNormal(const Vector3& normal)
		{
			if (normal.LengthSq() == 0)
			{
				MessageBoxA(nullptr, "不正な法線をセットしています。\n", "エラー", MB_OK);
				std::abort();
			}
			planeNormal = normal;
			planeNormal.Normalize();
		}
	private:
		Vector3 planePoint = Vector3::Zero;		//平面の一点
		Vector3 planeNormal = Vector3::Right;	//平面の法線
	};

	/**
	 * @brief 三角形のデータ
	*/
	struct TriangleData
	{
		Vector3 vertices[3];					//頂点
	};

	/**
	 * @brief 頂点のデータ
	*/
	struct VertexIndexesPack
	{
		std::vector<int> frontVertexIndexes;	//平面の表側にある頂点のインデックスの配列
		std::vector<int> backVertexIndexes;		//平面の裏側にある頂点のインデックス配列
		std::vector<int> onPlaneVertexIndexes;	//平面上にある頂点のインデックスの配列
	};

	class TriangleDivider
	{
	public:

		/**
		 * @brief 分割平面のセット
		 * @param planeData 分割平面のデータ
		*/
		void SetPlane(const PlaneData& planeData)
		{
			m_planeData = planeData;
			m_isPlaneDataRegisterd = true;
		}

		/**
		 * @brief 分割の実行(仮)　必ず分割平面をセットしてから呼ぶ事
		 * @param triangleData[in] 分割を実行する三角形のデータ
		*/
		void Divide(const TriangleData& TriangleData);

		//private:

		/**
		 * @brief データのリセット
		*/
		void DataReset()
		{
			//分割頂点を取得したかのフラグをリセット
			m_alreadyGetAnyDividePoint = false;

			//面の裏・表・上に存在する頂点のインデックスデータをリセット
			m_vertexIndexesPack.frontVertexIndexes.clear();
			m_vertexIndexesPack.backVertexIndexes.clear();
			m_vertexIndexesPack.onPlaneVertexIndexes.clear();
		}

		/**
		 * @brief 平面が三角形を分割している?
		 * @return 分割している:true していない:false
		*/
		bool IsPlaneDivideTriangle();

		/**
		 * @brief 平面と線分との交差地点を求める(ヒットしている前提)
		 * @param startPoint[in] 線分の開始位置
		 * @param endPoint[in] 線分の終了位置
		 * @return 交差地点
		*/
		Vector3 GetCrossPoint(const Vector3& startPoint, const Vector3& endPoint);

		/**
		 * @brief 0,1,2のインデックス番号のうち含まれていない物を返す
		 * @param first 1つ目のインデックスの数字
		 * @param second 2つ目のインデックスの数字
		 * @return 最後に残ったインデックスの数字
		*/
		int GetLeftoverOfThree(int first, int second)
		{
			return 3 - first - second;
		}

		/**
		 * @brief 平面とグループ分けされた頂点から分割された部分の頂点を求める
		 * @param verticesPack[in] 頂点のデータ(グループ分け済)
		 * @param points[out] 分割部分の頂点の配列(要素数2)
		 * @return 分割された際新しくできた頂点の数
		*/
		int GetDividedPoint(std::array<Vector3, 2>& points);

	private:
		bool m_isPlaneDataRegisterd = false;		//分割平面データが登録されている?
		bool m_alreadyGetAnyDividePoint = false;	//すでに分割頂点を1つでも取得した?
		PlaneData m_planeData;						//分割平面データ
		TriangleData m_triangleData;				//分割される三角形のデータ
		VertexIndexesPack m_vertexIndexesPack;		//面の裏・表・上に存在する頂点のインデックスデータ
	};
}