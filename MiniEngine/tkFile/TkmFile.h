/// <summary>
/// tkmファイル。
/// </summary>
/// <remarks>
/// tkmファイルは3Dモデルフォーマットです。
/// このクラスを利用することでtkmファイルを扱うことができます。。
/// </remarks>

#pragma once

	
/// <summary>
/// tkmファイルクラス。
/// </summary>
class  TkmFile {
public:
	/// <summary>
	/// マテリアル
	/// </summary>
	struct SMaterial {
		std::string albedoMapFileName;			//アルベドマップのファイル名。
		std::string normalMapFileName;			//法線マップのファイル名。
		std::string specularMapFileName;		//スペキュラマップのファイル名。
		std::string reflectionMapFileName;		//リフレクションマップのファイル名。
		std::string refractionMapFileName;		//屈折マップのファイル名。
		std::unique_ptr<char[]>	albedoMap = nullptr;		//ロードされたアルベドマップ。(ddsファイル)
		unsigned int albedoMapSize;				//アルベドマップのサイズ。(ddsファイル)
		std::unique_ptr<char[]>	normalMap = nullptr;		//ロードされた法線マップ。(ddsファイル)
		unsigned int normalMapSize;				//法線マップのサイズ。
		std::unique_ptr<char[]>	specularMap = nullptr;	//ロードされたスペキュラマップ。(ddsファイル)
		unsigned int specularMapSize;			//スペキュラマップのサイズ。(ddsファイル)
		std::unique_ptr<char[]>	reflectionMap = nullptr;	//ロードされたリフレクションマップ。(ddsファイル)
		unsigned int reflectionMapSize;			//リフレクションマップのサイズ。(ddsファイル)
		std::unique_ptr<char[]>	refractionMap = nullptr;	//ロードされた屈折マップ。(ddsファイル)
		unsigned int refractionMapSize;			//屈折マップのサイズ。(ddsファイル)

		//NOTE:ここより後はメッシュの分割時マテリアルをコピーできるように書いてみた物です。
		//動作が全く保証されていないので注意してください。

		//デフォルトコンストラクタ
		SMaterial() {}

		//コピーコンストラクタ
		SMaterial(const SMaterial& rhs)
		{
			albedoMapFileName = rhs.albedoMapFileName;
			normalMapFileName = rhs.normalMapFileName;
			specularMapFileName = rhs.specularMapFileName;
			reflectionMapFileName = rhs.reflectionMapFileName;
			refractionMapFileName = rhs.refractionMapFileName;
			albedoMap = std::make_unique<char[]>(*(rhs.albedoMap.get()));
			albedoMapSize = rhs.albedoMapSize;
			normalMap = std::make_unique<char[]>(*(rhs.normalMap.get()));
			normalMapSize = rhs.normalMapSize;
			specularMap = std::make_unique<char[]>(*(rhs.specularMap.get()));
			specularMapSize = specularMapSize;
			reflectionMap = std::make_unique<char[]>(*(rhs.reflectionMap.get()));
			reflectionMapSize = reflectionMapSize;
			refractionMap = std::make_unique<char[]>(*(rhs.refractionMap.get()));
			refractionMapSize = refractionMapSize;
		}

		//コピー代入演算子
		void operator=(const SMaterial& rhs)
		{
			albedoMapFileName = rhs.albedoMapFileName;
			normalMapFileName = rhs.normalMapFileName;
			specularMapFileName = rhs.specularMapFileName;
			reflectionMapFileName = rhs.reflectionMapFileName;
			refractionMapFileName = rhs.refractionMapFileName;
			albedoMap = std::make_unique<char[]>(*(rhs.albedoMap.get()));
			albedoMapSize = rhs.albedoMapSize;
			normalMap = std::make_unique<char[]>(*(rhs.normalMap.get()));
			normalMapSize = rhs.normalMapSize;
			specularMap = std::make_unique<char[]>(*(rhs.specularMap.get()));
			specularMapSize = specularMapSize;
			reflectionMap = std::make_unique<char[]>(*(rhs.reflectionMap.get()));
			reflectionMapSize = reflectionMapSize;
			refractionMap = std::make_unique<char[]>(*(rhs.refractionMap.get()));
			refractionMapSize = refractionMapSize;
		}
	};
	/// <summary>
	/// 頂点。
	/// </summary>
	/// <remarks>
	/// ここを編集したら、レイトレのシェーダーで定義されている頂点構造体も変更する必要がある。
	/// </remarks>
	struct SVertex {
		Vector3 pos;			//座標。
		Vector3 normal;		//法線。
		Vector3 tangent;		//接ベクトル。
		Vector3 binormal;		//従ベクトル。
		Vector2 uv;			//UV座標。
		int indices[4];			//スキンインデックス。
		Vector4 skinWeights;	//スキンウェイト。
	};

	static SVertex lerpVertex(float lerpRate,const SVertex& v0, const SVertex& v1)
	{
		SVertex vertex;
		vertex.pos.Lerp(lerpRate, v0.pos, v1.pos);
		vertex.normal.Lerp(lerpRate, v0.normal, v1.normal);
		vertex.tangent.Lerp(lerpRate, v0.tangent, v1.tangent);
		vertex.binormal.Lerp(lerpRate, v0.binormal, v1.binormal);
		vertex.uv.Lerp(lerpRate, v0.uv, v1.uv);

		//WARNING:スキンインデックスより下の補完の仕方が分からない(そもそも補完していいの?)
		//のでスキンインデックスはv0のインデックスを、スキンウェイトは作成したLerp関数を使ってみる
		vertex.indices[0] = v0.indices[0];
		vertex.indices[1] = v0.indices[1];
		vertex.indices[2] = v0.indices[2];
		vertex.indices[3] = v0.indices[3];
		vertex.skinWeights.Lerp(lerpRate, v0.skinWeights, v1.skinWeights);

		return vertex;
	}
	/// <summary>
	/// 32ビットのインデックスバッファ。
	/// </summary>
	struct SIndexBuffer32 {
		std::vector< uint32_t > indices;	//インデックス。
	};
	/// <summary>
	/// 16ビットのインデックスバッファ。
	/// </summary>
	struct SIndexbuffer16 {
		std::vector< uint16_t > indices;	//インデックス。
	};
	/// <summary>
	/// メッシュパーツ。
	/// </summary>
	struct SMesh {
		bool isFlatShading;							//フラットシェーディング？
		std::vector< SMaterial > materials;				//マテリアルの配列。
		std::vector< SVertex >	vertexBuffer;			//頂点バッファ。
		std::vector<SIndexBuffer32> indexBuffer32Array;	//インデックスバッファの配列。マテリアルの数分だけインデックスバッファはあるよ。
		std::vector< SIndexbuffer16> indexBuffer16Array;
	};
		
	/// <summary>
	/// 3Dモデルをロード。
	/// </summary>
	/// <param name="filePath">ファイルパス。</param>
	void Load(const char* filePath);

	/**
	 * @brief メッシュを追加
	 * @param mesh 追加するメッシュ
	*/
	void AddMesh(SMesh mesh)
	{
		m_meshParts.push_back(mesh);
	}
		
	/// <summary>
	/// メッシュパーツに対してクエリを行う。
	/// </summary>
	/// <param name="func">クエリ関数</param>
	void QueryMeshParts(std::function<void(const SMesh& mesh)> func) const
	{
		for (auto& mesh : m_meshParts) {
			func(mesh);
		}
	}
	/// <summary>
	/// メッシュパーツを取得。
	/// </summary>
	/// <returns></returns>
	const std::vector< SMesh>& GetMeshParts() const
	{
		return m_meshParts;
	}
	/// <summary>
	/// メッシュの数を取得。
	/// </summary>
	/// <returns></returns>
	int GetNumMesh() const
	{
		return (int)(m_meshParts.size());
	}
private:
	/// <summary>
	/// テクスチャ名をロード。
	/// </summary>
	/// <param name="fp"></param>
	/// <returns></returns>
	std::string LoadTextureFileName(FILE* fp);
	/// <summary>
	/// インデックスバッファをロード。
	/// </summary>
	template<class T>
	void LoadIndexBuffer(std::vector<T>& indexBuffer, int numIndex, FILE* fp);
	/// <summary>
	/// マテリアルを構築。
	/// </summary>
	/// <param name="tkmMat"></param>
	void BuildMaterial(SMaterial& tkmMat, FILE* fp, const char* filePath);
	/// <summary>
	/// 接ベクトルと従ベクトルを計算する。
	/// </summary>
	/// <remarks>
	/// 3dsMaxScriptでやるべきなんだろうけど、デバッグしたいので今はこちらでやる。
	/// </remarks>
	void BuildTangentAndBiNormal();
private:
	std::vector< SMesh>	m_meshParts;		//メッシュパーツ。
};
