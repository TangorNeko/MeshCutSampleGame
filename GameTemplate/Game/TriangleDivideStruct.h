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
		const Vector3& GetNormal()const { return planeNormal; }

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
		uint32_t vertexIndexes[3];					//頂点のインデックス
	};

	/**
	 * @brief 頂点のデータ
	*/
	struct VertexIndexesPack
	{
		std::vector<uint32_t> frontVertexIndexes;	//平面の表側にある頂点のインデックスの配列
		std::vector<uint32_t> backVertexIndexes;		//平面の裏側にある頂点のインデックス配列
		std::vector<uint32_t> onPlaneVertexIndexes;	//平面上にある頂点のインデックスの配列
	};
}
