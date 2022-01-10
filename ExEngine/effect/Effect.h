/*!
* @brief	エフェクト
*/

#pragma once

/*!
* @brief	エフェクト。
*/
namespace Game
{
	class Effect : public IGameObject
	{
	public:
		Effect();
		~Effect()override;

		/**
		 * @brief エフェクトの初期化
		 * @param filePath エフェクトのファイルパス
		*/
		void Init(const char16_t* filePath);

		/**
		 * @brief エフェクトの再生
		*/
		void Play();

		/**
		 * @brief エフェクトの再生を終了
		*/
		void Stop();

		/**
		 * @brief エフェクトの座標を設定
		 * @param pos 座標
		*/
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		/**
		 * @brief エフェクトの座標を取得
		 * @return 座標
		*/
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/**
		 * @brief エフェクトの回転を設定
		 * @param rot 回転
		*/
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}

		/**
		 * @brief エフェクトの回転を取得
		 * @return 回転
		*/
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}

		/**
		 * @brief エフェクトの拡大率を設定
		 * @param scale 拡大率
		*/
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/**
		 * @brief エフェクトの拡大率を取得
		 * @return 拡大率
		*/
		const Vector3& GetScale() const
		{
			return m_scale;
		}

		/**
		 * @brief エフェクトが再生中か
		 * @return 再生中:true 再生していない:false
		*/
		bool IsPlay() const
		{
			return EffectEngine::GetInstance()->IsPlay(m_handle);
		}

		/**
		 * @brief エフェクトの更新
		*/
		void Update();

		/**
		 * @brief 事前にエフェクトをロードしておく
		 * @param filePath ロードしたいエフェクトのファイルパス
		*/
		static void PreLoadEffect(const char16_t* filePath)
		{
			EffectEngine::GetInstance()->LoadEffect(filePath);
		}
	private:
		Effekseer::EffectRef m_effect;					//エフェクト
		int	m_handle = -1;								//再生中のエフェクトのハンドル。
		Vector3 m_position = Vector3::Zero;				//座標。
		Quaternion m_rotation = Quaternion::Identity;	//回転。
		Vector3 m_scale = g_vec3One;					//拡大率。
	};
}
