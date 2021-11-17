#pragma once
#include "TResourceBank.h"

namespace Engine
{
	class ResourceBankManager
	{
	private:
		ResourceBankManager() = default;
		~ResourceBankManager() = default;

		static ResourceBankManager* m_instance;

		TResourceBank<Shader> m_shaderBank;				//シェーダーバンク

	public:
		static void CreateInstance()
		{
			if (!m_instance)
			{
				m_instance = new ResourceBankManager;
			}
		}

		static void DeleteInstance()
		{
			delete m_instance;
			m_instance = nullptr;
		}

		static ResourceBankManager* GetInstance()
		{
			return m_instance;
		}

		/// <summary>
		/// シェーダーファイルバンクからシェーダーを取得。
		/// </summary>
		/// <param name="filePath"></param>
		/// <returns></returns>
		Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
		{
			std::string programName = filePath;
			programName += entryPointFuncName;
			return m_shaderBank.Get(programName.c_str());
		}
		/// <summary>
		/// シェーダーファイルをバンクに登録。
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="shader"></param>
		void RegistShaderToBank(const char* filePath, const char* entryPointFuncName, Shader* shader)
		{
			std::string programName = filePath;
			programName += entryPointFuncName;
			m_shaderBank.Regist(programName.c_str(), shader);
		}
	};
}