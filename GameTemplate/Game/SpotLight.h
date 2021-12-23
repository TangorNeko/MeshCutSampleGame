#pragma once

namespace Light
{
	struct SpotLigData //スポットライトのデータ
	{
		Vector3 ligPos = { 0.0f,0.0f,0.0f };	//ライトの座標
		float pad = 0.0f;						//パディング
		Vector3 ligColor = { 1.0f,1.0f,1.0f };	//ライトの色
		float ligRange = 0.0f;					//ライトの影響範囲
		Vector3 ligDir = { 1.0f,0.0f,0.0f };	//ライトの向き
		float ligAngle = 0.0f;					//ライトの影響角度(ラジアン単位)
	};

	class SpotLight : public IGameObject
	{
		~SpotLight()override;
		bool Start()override;
		void Update()override;

	public:
		/**
		 * @brief スポットライトのデータを取得
		 * @return スポットライトのデータ
		*/
		SpotLigData* GetLigData() { return &m_spotLigData; }

		/**
		 * @brief スポットライトのデータのサイズを取得
		 * @return スポットライトのデータのサイズ
		*/
		int GetLigDataSize() const { return sizeof(SpotLigData); }

		/**
		 * @brief スポットライトの座標を設定
		 * @param pos スポットライトの座標
		*/
		void SetPosition(const Vector3& pos) { m_spotLigData.ligPos = pos; }

		/**
		 * @brief スポットライトの座標を取得
		 * @return スポットライトの座標
		*/
		const Vector3& GetPosition() const { return m_spotLigData.ligPos; }

		/**
		 * @brief スポットライトの色を設定
		 * @param color スポットライトの色
		*/
		void SetColor(const Vector3& color) { m_spotLigData.ligColor = color; }

		/**
		 * @brief スポットライトの色を取得
		 * @return スポットライトの色
		*/
		const Vector3& GetColor() const { return m_spotLigData.ligColor; }

		/**
		 * @brief スポットライトの影響範囲を設定
		 * @param range スポットライトの影響範囲
		*/
		void SetRange(float range) { m_spotLigData.ligRange = range; }

		/**
		 * @brief スポットライトの影響範囲を取得
		 * @return スポットライトの影響範囲
		*/
		float GetRange() const { return m_spotLigData.ligRange; }

		/**
		 * @brief スポットライトの向きを設定
		 * @param dir スポットライトの向き
		*/
		void SetDirection(const Vector3& dir) { m_spotLigData.ligDir = dir; }

		/**
		 * @brief スポットライトの向きを取得
		 * @return スポットライトの向き
		*/
		const Vector3& GetDirection() const { return m_spotLigData.ligDir; }

		/**
		 * @brief スポットライトの影響角度を設定(ラジアン単位)
		 * @param angle_rad スポットライトの影響角度
		*/
		void SetAngle(float angle_rad) { m_spotLigData.ligAngle = angle_rad; }

		/**
		 * @brief スポットライトの影響角度を取得(ラジアン単位)
		 * @return スポットライトの影響角度
		*/
		float GetAngle() const { return m_spotLigData.ligAngle; }

		/**
		 * @brief スポットライトの影響角度を設定(デグリー単位)
		 * @param angle_deg スポットライトの影響角度
		*/
		void SetAngleDeg(float angle_deg) { m_spotLigData.ligAngle = Math::DegToRad(angle_deg); }

		/**
		 * @brief スポットライトの影響角度を取得(デグリー単位)
		 * @return スポットライトの影響角度
		*/
		float GetAngleDeg() const { return Math::RadToDeg(m_spotLigData.ligAngle); }

	private:
		SpotLigData m_spotLigData;	//スポットライトのデータ
		int m_spotLigTag = 0;			//スポットライトのタグ(何番目に作られたライト?)
	};
}

