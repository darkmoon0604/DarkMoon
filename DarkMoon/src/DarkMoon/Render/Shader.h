#pragma once

#include "DarkMoon/Core/Core.h"

#include <string>
#include <unordered_map>

namespace DarkMoon
{
	class DARKMOON_API Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Use() const = 0;
		virtual void UnUse() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filePath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
	};

	class DARKMOON_API ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);

		Ref<Shader> Load(const std::string& filePath);
		Ref<Shader> Load(const std::string& name, const std::string& filePath);
		
		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}