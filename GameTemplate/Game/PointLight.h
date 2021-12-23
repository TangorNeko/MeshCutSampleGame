#pragma once

namespace Light
{
	struct PointLigData
	{
		Vector3 ligPos = { 0.0f,0.0f,0.0f };	//ライトの座標
		float pad = 0.0f;						//パディング
		Vector3 ligColor = { 1.0f,1.0f,1.0f };	//ライトの色
		float ligRange = 0.0f;					//ライトの影響範囲
	};

	class PointLight : public IGameObject
	{
		~PointLight()override;
		bool Start()override;
		void Update()override;

	public:
		/**
		 * @brief ポイントライトのデータを取得
		 * @return ポイントライトのデータ
		*/
		PointLigData* GetLigData() { return &m_pointLigData; }

		/**
		 * @brief ポイントライトのデータのサイズを取得
		 * @return ポイントライトのデータのサイズ
		*/
		int GetLigDataSize() const { return sizeof(PointLigData); }

		/**
		 * @brief ポイントライトの座標を設定
		 * @param pos ポイントライトの座標
		*/
		void SetPosition(const Vector3& pos) { m_pointLigData.ligPos = pos; }

		/**
		 * @brief ポイントライトの座標を取得
		 * @return ポイントライトの座標
		*/
		const Vector3& GetPosition() const { return m_pointLigData.ligPos; }

		/**
		 * @brief ポイントライトの色を設定
		 * @param color ポイントライトの色
		*/
		void SetColor(const Vector3& color) { m_pointLigData.ligColor = color; }

		/**
		 * @brief ポイントライトの色を取得
		 * @return ポイントライトの色
		*/
		const Vector3& GetColor() const { return m_pointLigData.ligColor; }

		/**
		 * @brief ポイントライトの影響範囲を設定
		 * @param range ポイントライトの影響範囲
		*/
		void SetRange(float range) { m_pointLigData.ligRange = range; }

		/**
		 * @brief ポイントライトの影響範囲を取得
		 * @return ポイントライトの影響範囲
		*/
		float GetRange() const { return m_pointLigData.ligRange; }

	private:
		PointLigData m_pointLigData;	//ポイントライトのデータ
		int m_pointLigTag = 0;				//ポイントライトのタグ(何番目に作られたライト?)
	};
}
