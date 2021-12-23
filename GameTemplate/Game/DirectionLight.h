#pragma once

namespace Light
{
	struct DirLigData
	{
		Vector3 ligDir = { 1.0f,0.0f,0.0f };	//ライトの向き
		float pad0 = 0.0f;						//パディング
		Vector3 ligColor = { 1.0f,1.0f,1.0f };	//ライトの色
		float pad1 = 0.0f;						//パディング(単体だと必要ないがこれがないとDirLigDataを配列にした際HLSLで4バイトずつずれていくことになる?)
	};

	class DirectionLight : public IGameObject
	{
		~DirectionLight() override;
		bool Start() override;
		void Update() override;

	public:
		/**
		 * @brief ディレクションライトのデータを取得
		 * @return ディレクションライトのデータ
		*/
		DirLigData* GetLigData() { return &m_dirLigData; }

		/**
		 * @brief ディレクションライトのデータのサイズを取得
		 * @return ディレクションライトのデータのサイズ
		*/
		int GetLigDataSize() const { return sizeof(m_dirLigData); }

		/**
		 * @brief ディレクションライトの向きを設定
		 * @param dir ディレクションライトの向き
		*/
		void SetDirection(const Vector3& dir) {
			m_dirLigData.ligDir = dir;
			m_dirLigData.ligDir.Normalize();
		}

		/**
		 * @brief ディレクションライトの向きを取得
		 * @return ディレクションライトの向き
		*/
		const Vector3& GetDirecion() const { return m_dirLigData.ligDir; }

		/**
		 * @brief ディレクションライトの色を設定
		 * @param color ディレクションライトの色
		*/
		void SetColor(const Vector3& color) { m_dirLigData.ligColor = color; }

		/**
		 * @brief ディレクションのライトの色を取得
		 * @return ディレクションライトの色
		*/
		const Vector3& GetColor() const { return m_dirLigData.ligColor; }

	private:
		DirLigData m_dirLigData;	//ディレクションライトのデータ
		int m_dirLigTag = 0;			//ディレクションライトの番号(何番目に作られたライト?)
	};
}

