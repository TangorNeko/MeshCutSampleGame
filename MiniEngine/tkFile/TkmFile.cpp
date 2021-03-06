#include "stdafx.h"
#include "tkFile/TkmFile.h"
#include "../GameTemplate/Game/MeshDivider.h"
#include "../GameTemplate/Game/AxisCalculator.h"
#include "../geometry/BSP.h"


//法線スムージング。
class NormalSmoothing {
private:
	struct SSmoothVertex {
		Vector3 newNormal = g_vec3Zero;
		TkmFile::SVertex* vertex = nullptr;
	};
	struct SFace {
		Vector3 normal;
		std::vector<int> vertexNos;
	};
public:
	template <class IndexBuffer>
	void Execute(TkmFile::SMesh& mesh, const IndexBuffer& indexBuffer)
	{

		//ステップ１面法線を計算していく。
		auto numPolygon = indexBuffer.indices.size() / 3;
		std::vector< SFace> faces;
		faces.reserve(numPolygon);
				
		for (auto polyNo = 0; polyNo < numPolygon; polyNo++) {
			auto no = polyNo * 3;
			auto vertNo_0 = indexBuffer.indices[no];
			auto vertNo_1 = indexBuffer.indices[no + 1];
			auto vertNo_2 = indexBuffer.indices[no + 2];

			auto& vert_0 = mesh.vertexBuffer[vertNo_0];
			auto& vert_1 = mesh.vertexBuffer[vertNo_1];
			auto& vert_2 = mesh.vertexBuffer[vertNo_2];

			//法線を計算する。
			Vector3 v0tov1 = vert_1.pos - vert_0.pos;
			Vector3 v0tov2 = vert_2.pos - vert_0.pos;
			Vector3 normal = Cross(v0tov1, v0tov2);
			normal.Normalize();
			SFace face;
			face.normal = normal;
			face.vertexNos.push_back(vertNo_0);
			face.vertexNos.push_back(vertNo_1);
			face.vertexNos.push_back(vertNo_2);
			faces.push_back(face);
		}
		
		//ステップ２　法線の平均化
		for (auto& face : faces) {
			for (auto vertNo : face.vertexNos) {
				auto& vert = mesh.vertexBuffer[vertNo];
				vert.normal += face.normal;
			}
		}
		for (auto& vert : mesh.vertexBuffer) {
			vert.normal.Normalize();
		}
		//ステップ２　座標と向きが同じ頂点の法線を平均化していく。
		if(mesh.isFlatShading == 0)
		{
			//重複している頂点の法線を平均化
			BSP bsp;
			std::vector<SSmoothVertex> smoothVertex;
			smoothVertex.reserve(mesh.vertexBuffer.size());
			for (auto& v : mesh.vertexBuffer) {
				bsp.AddLeaf(v.pos, &v.normal);
				smoothVertex.push_back({ v.normal, &v });
			}
			bsp.Build();
			

			//スムージング処理
			for (auto& va : smoothVertex) {
				bsp.WalkTree(va.vertex->pos, [&](BSP::SLeaf* leaf) {
					if (va.vertex->pos.x == leaf->position.x
						&& va.vertex->pos.y == leaf->position.y
						&& va.vertex->pos.z == leaf->position.z) {
						//同じ座標。
						auto* normal = static_cast<Vector3*>(leaf->extraData);
						if (va.vertex->normal.Dot(*normal) > 0.0f) {
							//同じ向き。
							va.newNormal += *normal;
						}
					}
					});
				va.newNormal.Normalize();
			}
		}
	}
};
/// <summary>
/// TKMファイルフォーマット。
/// </summary>
/// <remarks>
/// 処理系によっては1バイトが8bitでないことがあり、
/// int型やshort型が必ずしも、4バイト、2バイトであるとは限らない。
/// そのため、std::uint16_tやstd::uint32_tを利用している。
/// これらは定義されている処理系であれば、サイズは必ず同じである。
/// </remarks>
namespace tkmFileFormat {
	//現在のTKMファイルのバージョン。
	std::uint16_t VERSION = 100;
	/// <summary>
	/// ヘッダーファイル。
	/// </summary>
	struct SHeader {
		std::uint8_t	version;		//バージョン。
		std::uint8_t	isFlatShading;	//フラットシェーディング？
		std::uint16_t	numMeshParts;	//メッシュパーツの数。
	};
	/// <summary>
	/// メッシュパーツヘッダー。
	/// </summary>
	struct SMeshePartsHeader {
		std::uint32_t numMaterial;		//マテリアルの数。
		std::uint32_t numVertex;		//頂点数。
		std::uint8_t indexSize;			//インデックスのサイズ。2か4。
		std::uint8_t pad[3];			//パディング。
	};
	/// <summary>
	/// 頂点
	/// </summary>
	struct SVertex {
		float pos[3];					//頂点座標。
		float normal[3];				//法線。
		float uv[2];					//UV座標。
		float weights[4];				//スキンウェイト。
		std::int16_t indices[4];		//スキンインデックス。
	};
};
template< class IndexBuffer>
void BuildTangentAndBiNormalImp(TkmFile::SMesh& mesh, const IndexBuffer& indexBuffer)
{
	//頂点スムースは気にしない。
	auto numPolygon = indexBuffer.indices.size()/3;
	for (auto polyNo = 0; polyNo < numPolygon; polyNo++) {
		auto no = polyNo * 3;
		auto vertNo_0 = indexBuffer.indices[no];
		auto vertNo_1 = indexBuffer.indices[no+1];
		auto vertNo_2 = indexBuffer.indices[no+2];

		auto& vert_0 = mesh.vertexBuffer[vertNo_0];
		auto& vert_1 = mesh.vertexBuffer[vertNo_1];
		auto& vert_2 = mesh.vertexBuffer[vertNo_2];

		Vector3 cp0[] = {
			{ vert_0.pos.x, vert_0.uv.x, vert_0.uv.y},
			{ vert_0.pos.y, vert_0.uv.x, vert_0.uv.y},
			{ vert_0.pos.z, vert_0.uv.x, vert_0.uv.y}
		};

		Vector3 cp1[] = {
			{ vert_1.pos.x, vert_1.uv.x, vert_1.uv.y},
			{ vert_1.pos.y, vert_1.uv.x, vert_1.uv.y},
			{ vert_1.pos.z, vert_1.uv.x, vert_1.uv.y}
		};

		Vector3 cp2[] = {
			{ vert_2.pos.x, vert_2.uv.x, vert_2.uv.y},
			{ vert_2.pos.y, vert_2.uv.x, vert_2.uv.y},
			{ vert_2.pos.z, vert_2.uv.x, vert_2.uv.y}
		};

		// 平面パラメータからUV軸座標算出する。
		Vector3 tangent, binormal;
		for (int i = 0; i < 3; ++i) {
			auto V1 = cp1[i] - cp0[i];
			auto V2 = cp2[i] - cp1[i];
			auto ABC = Cross(V1, V2);
	
			if (ABC.x == 0.0f) {
				tangent.v[i] = 0.0f;
				binormal.v[i] = 0.0f;
			}
			else {
				tangent.v[i] = -ABC.y / ABC.x;
				binormal.v[i] = -ABC.z / ABC.x;
			}
		}

		tangent.Normalize();
		binormal.Normalize();

		vert_0.tangent += tangent;
		vert_1.tangent += tangent;
		vert_2.tangent += tangent;

		vert_0.binormal += binormal;
		vert_1.binormal += binormal;
		vert_2.binormal += binormal;
	}
	//法線、接ベクトル、従ベクトルを平均化する。
	for (auto& vert : mesh.vertexBuffer) {
		vert.tangent.Normalize();
		vert.binormal.Normalize();
	}
		
}
std::string TkmFile::LoadTextureFileName(FILE* fp)
{
	std::string fileName;
	std::uint32_t fileNameLen;
	fread(&fileNameLen, sizeof(fileNameLen), 1, fp);
		
	if (fileNameLen > 0) {
		char* localFileName = reinterpret_cast<char*>(malloc(fileNameLen + 1));
		//ヌル文字分も読み込むので＋１
		fread(localFileName, fileNameLen + 1, 1, fp);
		fileName = localFileName;
		free(localFileName);
	}
		
	return fileName;
}
template<class T>
void TkmFile::LoadIndexBuffer(std::vector<T>& indices, int numIndex, FILE* fp)
{
	indices.resize(numIndex);
	for (int indexNo = 0; indexNo < numIndex; indexNo++) {
		T index;
		fread(&index, sizeof(index), 1, fp);
		indices[indexNo] = index - 1;	//todo maxのインデックスは1から開始しているので、-1する。
									//todo エクスポーターで減らすようにしましょう。
	}
}

void TkmFile::BuildMaterial(SMaterial& tkmMat, FILE* fp, const char* filePath)
{
	//アルベドのファイル名をロード。
	tkmMat.albedoMapFileName = LoadTextureFileName(fp);
	//法線マップのファイル名をロード。
	tkmMat.normalMapFileName = LoadTextureFileName(fp);
	//スペキュラマップのファイル名をロード。
	tkmMat.specularMapFileName = LoadTextureFileName(fp);
	//リフレクションマップのファイル名をロード。
	tkmMat.reflectionMapFileName = LoadTextureFileName(fp);
	//屈折マップのファイル名をロード。
	tkmMat.refractionMapFileName = LoadTextureFileName(fp);

	std::string texFilePath = filePath;
	auto loadTexture = [&](
		std::string& texFileName, 
		std::unique_ptr<char[]>& ddsFileMemory, 
		unsigned int& fileSize
	) {
		int filePathLength = static_cast<int>(texFilePath.length());
		if (texFileName.length() > 0) {
			//モデルのファイルパスからラストのフォルダ区切りを探す。
			auto replaseStartPos = texFilePath.find_last_of('/');
			if (replaseStartPos == std::string::npos) {
				replaseStartPos = texFilePath.find_last_of('\\');
			}
			replaseStartPos += 1;
			auto replaceLen = filePathLength - replaseStartPos;
			texFilePath.replace(replaseStartPos, replaceLen, texFileName);
			//拡張子をddsに変更する。
			replaseStartPos = texFilePath.find_last_of('.') + 1;
			replaceLen = texFilePath.length() - replaseStartPos;
			texFilePath.replace(replaseStartPos, replaceLen, "dds");
				
			//テクスチャをロード。
			FILE* texFileFp = fopen(texFilePath.c_str(), "rb");
			if (texFileFp != nullptr) {
				//ファイルサイズを取得。
				fseek(texFileFp, 0L, SEEK_END);		
				fileSize = ftell(texFileFp);
				fseek(texFileFp, 0L, SEEK_SET);

				ddsFileMemory = std::make_unique<char[]>(fileSize);
				fread(ddsFileMemory.get(), fileSize, 1, texFileFp);
				fclose(texFileFp);
			}
			else {
				MessageBoxA(nullptr, "テクスチャのロードに失敗しました。", "エラー", MB_OK);
				std::abort();
			}
		}
	};
	//テクスチャをロード。
	loadTexture( tkmMat.albedoMapFileName, tkmMat.albedoMap, tkmMat.albedoMapSize );
	loadTexture( tkmMat.normalMapFileName, tkmMat.normalMap, tkmMat.normalMapSize );
	loadTexture( tkmMat.specularMapFileName, tkmMat.specularMap, tkmMat.specularMapSize );
	loadTexture( tkmMat.reflectionMapFileName, tkmMat.reflectionMap, tkmMat.reflectionMapSize );
	loadTexture( tkmMat.refractionMapFileName, tkmMat.refractionMap, tkmMat.refractionMapSize) ;
}
void TkmFile::BuildTangentAndBiNormal()
{
	NormalSmoothing normalSmoothing;
	for (auto& mesh : m_meshParts) {
		for (auto& indexBuffer : mesh.indexBuffer16Array) {
			normalSmoothing.Execute(mesh, indexBuffer);
			BuildTangentAndBiNormalImp(mesh, indexBuffer);
		}
		for (auto& indexBuffer : mesh.indexBuffer32Array) {
			normalSmoothing.Execute(mesh, indexBuffer);
			BuildTangentAndBiNormalImp(mesh, indexBuffer);
		}
	}
}
void TkmFile::Load(const char* filePath)
{
	FILE* fp = fopen(filePath, "rb");
	if (fp == nullptr) {
		MessageBoxA(nullptr, "tkmファイルが開けません。", "エラー", MB_OK);
		return ;
	}
	//tkmファイルのヘッダーを読み込み。
	tkmFileFormat::SHeader header;
	fread(&header, sizeof(header), 1, fp);
	if (header.version != tkmFileFormat::VERSION) {
		//tkmファイルのバージョンが違う。
		MessageBoxA(nullptr, "tkmファイルのバージョンが異なっています。", "エラー", MB_OK);
	}
	//メッシュ情報をロードしていく。
	m_meshParts.resize(header.numMeshParts);
	for (int meshPartsNo = 0; meshPartsNo < header.numMeshParts; meshPartsNo++) {
			
		auto& meshParts = m_meshParts[meshPartsNo];
		meshParts.isFlatShading = header.isFlatShading;
		tkmFileFormat::SMeshePartsHeader meshPartsHeader;
		fread(&meshPartsHeader, sizeof(meshPartsHeader), 1, fp);
		//マテリアル情報を記録できる領域を確保。
		meshParts.materials.resize(meshPartsHeader.numMaterial);
		//マテリアル情報を構築していく。
		for (unsigned int materialNo = 0; materialNo < meshPartsHeader.numMaterial; materialNo++) {
			auto& material = meshParts.materials[materialNo];
			BuildMaterial(material, fp, filePath);
		}
			
		//続いて頂点バッファ。
		meshParts.vertexBuffer.resize(meshPartsHeader.numVertex);
		for (unsigned int vertNo = 0; vertNo < meshPartsHeader.numVertex; vertNo++) {
			tkmFileFormat::SVertex vertexTmp;
			fread(&vertexTmp, sizeof(vertexTmp), 1, fp);
			auto& vertex = meshParts.vertexBuffer[vertNo];
			vertex.pos.Set(vertexTmp.pos[0], vertexTmp.pos[1], vertexTmp.pos[2]);
		//	vertex.normal.Set(vertexTmp.normal[0], vertexTmp.normal[1], vertexTmp.normal[2]);
			vertex.normal = g_vec3Zero;
			vertex.tangent = g_vec3Zero;
			vertex.binormal = g_vec3Zero;
			vertex.uv.Set(vertexTmp.uv[0], vertexTmp.uv[1]);
			vertex.skinWeights.Set(vertexTmp.weights[0], vertexTmp.weights[1], vertexTmp.weights[2], vertexTmp.weights[3]);
			vertex.indices[0] = vertexTmp.indices[0] != -1 ? vertexTmp.indices[0] : 0;
			vertex.indices[1] = vertexTmp.indices[1] != -1 ? vertexTmp.indices[1] : 0;
			vertex.indices[2] = vertexTmp.indices[2] != -1 ? vertexTmp.indices[2] : 0;
			vertex.indices[3] = vertexTmp.indices[3] != -1 ? vertexTmp.indices[3] : 0;
		}
		
		//続いてインデックスバッファ。
		//インデックスバッファはマテリアルの数分だけ存在するんじゃよ。
		if (meshPartsHeader.indexSize == 2) {
			//16bitのインデックスバッファ。
			meshParts.indexBuffer16Array.resize(meshPartsHeader.numMaterial);
		}
		else {
			//32bitのインデックスバッファ。
			meshParts.indexBuffer32Array.resize(meshPartsHeader.numMaterial);
		}
			
		for (unsigned int materialNo = 0; materialNo < meshPartsHeader.numMaterial; materialNo++) {
			//ポリゴン数をロード。
			int numPolygon;
			fread(&numPolygon, sizeof(numPolygon), 1, fp);
			//トポロジーはトライアングルリストオンリーなので、3を乗算するとインデックスの数になる。
			int numIndex = numPolygon * 3;
			if (meshPartsHeader.indexSize == 2) {
				LoadIndexBuffer(
					meshParts.indexBuffer16Array[materialNo].indices,
					numIndex,
					fp 
				);
			}
			else {
				LoadIndexBuffer(
					meshParts.indexBuffer32Array[materialNo].indices,
					numIndex,
					fp
				);
			}

		}
	}
	//接ベクトルと従ベクトルを構築する。
	BuildTangentAndBiNormal();

	fclose(fp);

}

std::vector< TkmFile::SMesh> TkmFile::Divide(const Vector3& cutNormal,const Vector3& cutPoint)
{
	Util::MeshDivider meshDivider;

	std::vector< SMesh>	frontMesh;
	std::vector< SMesh>	backMesh;
	for (auto& mesh : m_meshParts)
	{
		meshDivider.Init(&mesh);
		auto newMeshPair = meshDivider.Divide(cutNormal, cutPoint);

		//1メッシュパーツ分すべてが分割の結果無くなった時は追加しない
		if (newMeshPair.first.indexBuffer16Array.size() > 0)
		{
			frontMesh.push_back(newMeshPair.first);
		}

		if (newMeshPair.second.indexBuffer16Array.size() > 0)
		{
			backMesh.push_back(newMeshPair.second);
		}
	}

	//表側のメッシュはそのまま自らのTkmFileとして使用
	m_meshParts = frontMesh;

	//裏側のメッシュは新規モデルとして使用
	return backMesh;
}

Vector3 TkmFile::GetOriginToCenter()
{
	//座標値の合計
	Vector3 sumOfPos = Vector3::Zero;

	//座標値の数
	int numOfPos = 0;

	for (auto& mesh : m_meshParts)
	{
		for (auto& indexBuffer : mesh.indexBuffer16Array)
		{
			for (auto vertexIndex : indexBuffer.indices)
			{
				//頂点バッファに格納されている座標を合計に加算していく
				Vector3 vertPos = mesh.vertexBuffer.at(vertexIndex).pos;
				sumOfPos += vertPos;
				numOfPos++;
			}
		}
	}

	//座標値の合計を座標値の数で割って中心を求める
	Vector3 centerPos = sumOfPos / static_cast<float>(numOfPos);
	return centerPos;
}

void TkmFile::SetOriginOffset(const Vector3& offset)
{
	for (auto& mesh : m_meshParts)
	{
		for (auto& vertex : mesh.vertexBuffer)
		{
			//引数で受け取ったオフセット分座標をずらす
			vertex.pos -= offset;
		}
	}
}

Vector3 TkmFile::CalcCapsuleAxis(Vector2& heightAndRadius)
{
	std::vector<Vector3> posArray;

	//軸の判定に使用する頂点の座標を配列に格納
	for (auto& mesh : m_meshParts)
	{
		for (auto& indexBuffer : mesh.indexBuffer16Array)
		{
			for (auto vertexIndex : indexBuffer.indices)
			{
				posArray.push_back(mesh.vertexBuffer.at(vertexIndex).pos);
			}
		}
	}

	//カプセルの軸の判定をする
	AxisCalculator axisCalculator;
	Vector3 capsuleAxis = axisCalculator.CalcAxis(posArray);

	float maxHeight = 0.0f;
	float maxRadius = 0.0f;

	//すべての頂点を走査
	for (auto& pos : posArray)
	{	//カプセルの軸に合わせて射影を求め、高さと半径を求める
		float height = fabsf(Dot(capsuleAxis, pos));

		float rad = acos(height/pos.Length());

		float radius = height * tan(rad);

		//求めた値の方が大きかったら最大値として格納
		maxHeight = max(maxHeight, height);
		maxRadius = max(maxRadius, radius);
	}

	//戻り値用のVector2クラスに高さと半径を格納する
	heightAndRadius.x = maxHeight;
	heightAndRadius.y = maxRadius;

	return capsuleAxis;
}

bool TkmFile::DivideCheck(const Vector3& cutNormal, const Vector3& cutPoint)
{
	Util::MeshDivider meshDivider;

	//メッシュパーツを走査
	for (auto& mesh : m_meshParts)
	{
		meshDivider.Init(&mesh);

		//メッシュパーツを平面が切断するか判定
		bool isDivided = meshDivider.DivideCheck(cutNormal, cutPoint);

		//一箇所でも切断の必要があれば真を返す
		if (isDivided == true)
		{
			return true;
		}
	}

	//すべてのメッシュパーツが切断の必要がなかったので偽を返す
	return false;
}

void TkmFile::CopyTo(TkmFile* newTkmFile)
{
	newTkmFile->m_meshParts = m_meshParts;
}