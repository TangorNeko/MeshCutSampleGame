#pragma once
#include <map>
#include <utility>
#include "stdafx.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace
{
	//各ライトの最大数　この数はmodel.fxのライト配列の数と一緒にしなければならない
	const int DIRECTIONLIGHT_NUMBER_MAX = 5;
	const int POINTLIGHT_NUMBER_MAX = 20;
	const int SPOTLIGHT_NUMBER_MAX = 20;
}

//TODO:ライトを作る際最大数以上の時throwするのではなく一番古いライトを消すような処理にする方がよい?
//勝手に消す(例:一番古いライトタグに対応した数字を-1にし、UpdateLightで-1ならLight.cppでDelete(this)させる)
//と無効なアドレスに値を入れることになりそうだが
//データを使わないだけで消さないようにする(例:古いライトタグに対応した数字を-1にするだけ)
//とそれはそれでメモリの無駄遣いになりそう

namespace Light
{
	class LightManager
	{
	private:
		LightManager() = default;
		~LightManager() = default;

		static LightManager* m_instance;

		//ライトの情報とカメラの位置とライトの数を集めたデータ(定数バッファとしてHLSLに送られる構造体)
		struct LigDatas
		{
			Light::DirLigData directionLightArray[DIRECTIONLIGHT_NUMBER_MAX];	//ディレクションライトのデータの配列
			Light::PointLigData pointLightArray[POINTLIGHT_NUMBER_MAX];		//ポイントライトのデータの配列
			Light::SpotLigData spotLightArray[SPOTLIGHT_NUMBER_MAX];			//スポットライトのデータの配列
			Vector3 eyePos = Vector3::Zero;										//カメラの位置
			int directionLightNum = 0;											//ディレクションライトの数
			int pointLightNum = 0;												//ポイントライトの数
			int spotLightNum = 0;												//スポットライトの数
		};

		struct LigCameraDatas
		{
			Matrix lightCameraProjectionMatrix;				//ライトビュープロジェクション行列
			Vector3 lightCameraPosition = Vector3::Zero;	//ライトカメラの位置
			float pad = 0;									//パディング
			Vector3 lightCameraDirection = Vector3::Zero;	//ライトカメラの向き
		};

		LigDatas m_ligData;									//ライトのデータ
		int m_size = sizeof(m_ligData);						//ライトのデータのサイズ

		LigCameraDatas m_ligCameraData;						//ライト視点のカメラのデータ
		int m_ligCameraDataSize = sizeof(m_ligCameraData);	//ライト視点のカメラのデータのサイズ

		int m_dirLigNum = 0;								//次に作られるディレクションライトに付与するタグ番号(≒これまでに何個ディレクションライトが作られたか)
		int m_pointLigNum = 0;								//次に作られるポイントライトに付与するタグ番号(≒これまでに何個ポイントライトが作られたか)
		int m_spotLigNum = 0;								//次に作られるスポットライトに付与するタグ番号(≒これまでに何個スポットライトが作られたか)
		std::map<int, int> m_dirLigMap;						//ディレクションライトのタグから現在のディレクションライトの位置を返してくれるMap
		std::map<int, int> m_pointLigMap;					//ポイントライトのタグから現在のポイントライトの位置を返してくれるMap
		std::map<int, int> m_spotLigMap;					//スポットライトのタグから現在のスポットライトの位置を返してくれるMap

		Camera m_lightCamera;								//シャドウマップ用のライトの位置のカメラ。
	public:

		/**
		 * @brief ライトマネージャーのインスタンスを作成する
		*/
		static void CreateInstance()
		{
			if (!m_instance)
			{
				m_instance = new LightManager;
			}
		}

		/**
		 * @brief ライトマネージャーのインスタンスを削除する
		*/
		static void DeleteInstance()
		{
			delete m_instance;
			m_instance = nullptr;
		}

		/**
		 * @brief ライトマネージャーのインスタンスを取得する
		*/
		static LightManager* GetInstance() { return m_instance; }

		/**
		 * @brief ライトのデータの塊を取得する(定数バッファに渡す用)
		 * @return 全ライトのデータ
		*/
		LigDatas* GetLigDatas() { return &m_ligData; }

		/**
		 * @brief ライトのデータの塊のサイズを取得する(定数バッファに渡す用)
		 * @return 全ライトのデータのサイズ
		*/
		int GetLigDataSize() const { return m_size; }

		/**
		 * @brief カメラのポジションを更新する
		 * @param カメラの番号
		*/
		void UpdateEyePos() { m_ligData.eyePos = g_camera3D->GetPosition(); }

		/**
		 * @brief 影の描画に使用するライトカメラのデータの塊を取得する(定数バッファに渡す用)
		 * @return ライトカメラのデータ
		*/
		LigCameraDatas* GetLigCameraDatas() { return &m_ligCameraData; }

		/**
		 * @brief 影の描画に使用するライトカメラのデータの塊のサイズを取得する(定数バッファに渡す用)
		 * @return ライトカメラのデータのサイズ
		*/
		int GetLigCameraDataSize() const { return m_ligCameraDataSize; }

		/**
		 * @brief 影の描画に使用するライトカメラを取得
		 * @return ライトカメラ
		*/
		Camera* GetLightCamera()
		{
			return &m_lightCamera;
		}

		/**
		 * @brief 影の描画に使用するライトカメラの座標を指定する
		 * @param pos ライトカメラの座標
		*/
		void SetLightCameraPosition(const Vector3& pos)
		{
			//座標をセット
			m_lightCamera.SetPosition(pos);
			m_lightCamera.Update();

			//ライトカメラのデータを更新後のものに設定
			m_ligCameraData.lightCameraPosition = pos;
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();

			m_ligCameraData.lightCameraDirection = m_lightCamera.GetTarget() - m_lightCamera.GetPosition();
			m_ligCameraData.lightCameraDirection.Normalize();
		}

		/**
		 * @brief 影の描画に使用するライトカメラのターゲットの座標を指定する
		 * @param targetPos ライトカメラのターゲットの座標
		*/
		void SetLightCameraTarget(const Vector3& targetPos)
		{
			//ターゲットの座標をセット
			m_lightCamera.SetTarget(targetPos);
			m_lightCamera.Update();

			//ライトカメラのデータを更新後のものに設定
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();

			m_ligCameraData.lightCameraDirection = m_lightCamera.GetTarget() - m_lightCamera.GetPosition();
			m_ligCameraData.lightCameraDirection.Normalize();
		}

		/**
		 * @brief 影の描画に使用するライトカメラの上方向を指定する
		 * @param up カメラの上方向のベクトル
		*/
		void SetLightCameraUp(const Vector3& up)
		{
			//上方向のベクトルをセット
			m_lightCamera.SetUp(up);
			m_lightCamera.Update();

			//ライトカメラのデータを更新後のものに設定
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();
		}

		/**
		 * @brief 影の描画に使用するライトカメラの射影行列の更新方法を設定する
		 * @param func 射影行列の更新方法
		*/
		void SetLightCameraUpdateProjMatrixFunc(Camera::EnUpdateProjMatrixFunc func)
		{
			//射影行列の更新方法をセット
			m_lightCamera.SetUpdateProjMatrixFunc(func);
			m_lightCamera.Update();

			//ライトカメラのデータを更新後のものに設定
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();
		}

		/**
		 * @brief 影の描画に使用するライトカメラの横幅を指定する(平行投影限定)
		 * @param width ライトカメラの横幅
		*/
		void SetLightCameraWidth(const float& width)
		{
			//横幅をセット
			m_lightCamera.SetWidth(width);
			m_lightCamera.Update();

			//ライトカメラのデータを更新後のものに設定
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();
		}

		/**
		 * @brief 影の描画に使用するライトカメラの高さを指定する(平行投影限定)
		 * @param height ライトカメラの高さ
		*/
		void SetLightCameraHeight(const float& height)
		{
			//高さをセット
			m_lightCamera.SetHeight(height);
			m_lightCamera.Update();

			//ライトカメラのデータを更新後のものに設定
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();
		}

		//ディレクションライト用////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief ディレクションライトの数のカウントをプラスする
		*/
		void DirectionLightPlus() { m_ligData.directionLightNum++; }

		/**
		 * @brief ディレクションライトの数のカウントをマイナスする
		*/
		void DirectionLightMinus() {
			m_ligData.directionLightNum--;

			//ライトの数が0以下になる時はおかしくなっているのでthrowする(起こり得ないと信じたい)
			if (m_ligData.directionLightNum < 0)
			{
				MessageBoxA(nullptr, "ディレクションライトの数がマイナスになっています。\n", "エラー", MB_OK);
			}
		}

		/**
		 * @brief ライトマネージャーにディレクションライトを登録する
		 * @param dirLigData ディレクションライトのデータ
		 * @return 登録したディレクションライトのタグ番号(ライトの照合に使う)
		*/
		int AddDirectionLight(Light::DirLigData* dirLigData)
		{
			//ライトの数が最初に決めた数以上ならthrowする(いっぱい作るとふつうに起こる)
			if (m_ligData.directionLightNum >= DIRECTIONLIGHT_NUMBER_MAX)
			{
				MessageBoxA(nullptr, "ディレクションライトの数が最大数を超えています。\n", "エラー", MB_OK);
			}

			//空きの中で一番先頭位置にデータを格納する
			m_ligData.directionLightArray[m_ligData.directionLightNum] = *dirLigData;

			//作ったライトのタグ番号とデータの格納位置を関連付ける
			m_dirLigMap.insert(std::make_pair(m_dirLigNum, m_ligData.directionLightNum));

			//次にライトが入る位置をずらす
			DirectionLightPlus();

			//作ったライトのタグ番号を返し、1増やして次のライトの作成に備える
			return m_dirLigNum++;
		}

		/**
		 * @brief ディレクションライトを削除する
		 * @param directionLightTag 削除したいディレクションライトのタグ番号
		*/
		void RemoveDirectionLight(int directionLightTag)
		{
			//タグから削除する位置を取得し、削除するライトをソートで一番終端に持っていく
			for (int i = m_dirLigMap.at(directionLightTag); i < m_ligData.directionLightNum - 1; i++)
			{
				std::swap(m_ligData.directionLightArray[i], m_ligData.directionLightArray[i + 1]);
			}

			//ライトの数を減らすことで空いた位置に次のライトを入れられるようになる
			DirectionLightMinus();

			//削除したライト以降のライトの位置番号が一つずつずれたのでタグに対応した位置の番号を1ずつ減らす
			for (auto i = m_dirLigMap.upper_bound(directionLightTag); i != m_dirLigMap.end(); i++)
			{
				i->second--;
			}
		}

		/**
		 * @brief ディレクションライトの情報を更新する
		 * @param directionLightTag 更新したいライトのタグ番号
		 * @param dirLigData 更新したいライトの新しいライトデータ
		*/
		void UpdateDirectionLight(int directionLightTag, Light::DirLigData* dirLigData)
		{
			//タグから取得したライトの位置のデータを更新する。
			m_ligData.directionLightArray[m_dirLigMap.at(directionLightTag)] = *dirLigData;
		}

		//ポイントライト用///////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief ポイントライトの数のカウントをプラスする
		*/
		void PointLightPlus() { m_ligData.pointLightNum++; }

		/**
		 * @brief ポイントライトの数のカウントをマイナスする
		*/
		void PointLightMinus()
		{
			m_ligData.pointLightNum--;

			//ライトの数が0以下になる時はおかしくなっているのでthrowする(起こり得ないと信じたい)
			if (m_ligData.pointLightNum < 0)
			{
				MessageBoxA(nullptr, "ポイントライトの数がマイナスになっています。\n", "エラー", MB_OK);
			}
		}

		/**
		 * @brief ライトマネージャーにポイントライトを登録する
		 * @param pointLigData ポイントライトのデータ
		 * @return 登録したポイントライトのタグ番号(ライトの照合に使う)
		*/
		int AddPointLight(Light::PointLigData* pointLigData)
		{
			//ライトの数が最初に決めた数以上ならthrowする(いっぱい作るとふつうに起こる)
			if (m_ligData.pointLightNum >= POINTLIGHT_NUMBER_MAX)
			{
				MessageBoxA(nullptr, "ポイントライトの数が最大数を超えています。\n", "エラー", MB_OK);
			}

			//空きの中で一番先頭位置にデータを格納する
			m_ligData.pointLightArray[m_ligData.pointLightNum] = *pointLigData;

			//作ったライトのタグ番号とデータの格納位置を関連付ける
			m_pointLigMap.insert(std::make_pair(m_pointLigNum, m_ligData.pointLightNum));

			//次にライトが入る位置をずらす
			PointLightPlus();

			//作ったライトのタグ番号を返し、1増やして次のライトの作成に備える
			return m_pointLigNum++;
		}

		/**
		 * @brief ポイントライトを削除する
		 * @param pointLightTag 削除したいポイントライトのタグ番号
		*/
		void RemovePointLight(int pointLightTag)
		{
			//タグから削除する位置を取得し、削除するライトをソートで一番終端に持っていく
			for (int i = m_pointLigMap.at(pointLightTag); i < m_ligData.pointLightNum - 1; i++)
			{
				std::swap(m_ligData.pointLightArray[i], m_ligData.pointLightArray[i + 1]);
			}

			//ライトの数を減らすことで空いた位置に次のライトを入れられるようになる
			PointLightMinus();

			//削除したライト以降のライトの位置番号が一つずつずれたのでタグに対応した位置の番号を1ずつ減らす
			for (auto i = m_pointLigMap.upper_bound(pointLightTag); i != m_pointLigMap.end(); i++)
			{
				i->second--;
			}
		}

		/**
		 * @brief ポイントライトの情報を更新する
		 * @param pointLightTag 更新したいライトのタグ番号
		 * @param pointLigData 更新したいライトの新しいライトデータ
		*/
		void UpdatePointLight(int pointLightTag, Light::PointLigData* pointLigData)
		{
			m_ligData.pointLightArray[m_pointLigMap.at(pointLightTag)] = *pointLigData;
		}

		//スポットライト用/////////////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * @brief スポットライトの数のカウントをプラスする
		*/
		void SpotLightPlus() { m_ligData.spotLightNum++; }

		/**
		 * @brief スポットライトの数のカウントをマイナスする
		*/
		void SpotLightMinus()
		{
			m_ligData.spotLightNum--;

			//ライトの数が0以下になる時はおかしくなっているのでthrowする(起こり得ないと信じたい)
			if (m_ligData.spotLightNum < 0)
			{
				MessageBoxA(nullptr, "スポットライトの数がマイナスになっています。\n", "エラー", MB_OK);
			}
		}

		/**
		 * @brief ライトマネージャーにスポットライトを登録する
		 * @param spotLigData スポットライトのデータ
		 * @return 登録したスポットライトのタグ番号(ライトの照合に使う)
		*/
		int AddSpotLight(Light::SpotLigData* spotLigData)
		{
			//ライトの数が最初に決めた数以上ならthrowする(いっぱい作るとふつうに起こる)
			if (m_ligData.spotLightNum >= SPOTLIGHT_NUMBER_MAX)
			{
				MessageBoxA(nullptr, "スポットライトの数が最大数を超えています。\n", "エラー", MB_OK);
			}

			//空きの中で一番先頭位置にデータを格納する
			m_ligData.spotLightArray[m_ligData.spotLightNum] = *spotLigData;

			//作ったライトのタグ番号とデータの格納位置を関連付ける
			m_spotLigMap.insert(std::make_pair(m_spotLigNum, m_ligData.spotLightNum));

			//次にライトが入る位置をずらす
			SpotLightPlus();

			//作ったライトのタグ番号を返し、1増やして次のライト作成に備える
			return m_spotLigNum++;
		}

		/**
		 * @brief スポットライトを削除する
		 * @param spotLightTag 削除したいスポットライトのタグ番号
		*/
		void RemoveSpotLight(int spotLightTag)
		{
			//タグから削除する位置を取得し、削除するライトをソートで一番終端に持っていく
			for (int i = m_spotLigMap.at(spotLightTag); i < m_ligData.spotLightNum - 1; i++)
			{
				std::swap(m_ligData.spotLightArray[i], m_ligData.spotLightArray[i + 1]);
			}

			//ライトの数を減らすことで空いた位置に次のライトを入れられるようになる
			SpotLightMinus();

			//削除したライト以降のライトの位置番号が一つずつずれたのでタグに対応した位置の番号を1ずつ減らす
			for (auto i = m_spotLigMap.upper_bound(spotLightTag); i != m_spotLigMap.end(); i++)
			{
				i->second--;
			}
		}

		/**
		 * @brief スポットライトの情報を更新する
		 * @param spotLightTag 更新したいライトのタグ番号
		 * @param spotLigData 更新したいライトの新しいライトデータ
		*/
		void UpdateSpotLight(int spotLightTag, Light::SpotLigData* spotLigData)
		{
			m_ligData.spotLightArray[m_spotLigMap.at(spotLightTag)] = *spotLigData;
		}

	};
}

