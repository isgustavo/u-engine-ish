#pragma once

#include "components/UComponent.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>

namespace uei
{
	class UEntity
	{
		friend class UEngine;
		friend class UScene;
		friend class UAsset;

	public:
		UEntity() : iD(-1), name(" "), bIsActive(true) {}
		~UEntity();

		std::string& Name();
		void Name(std::string inName);
		bool IsActive() const;
		void SetActive(bool value);
		std::unordered_map<std::type_index, uei::UComponent*>& Components();

		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<uei::UComponent, T>,
				"T must derive from UComponent");

			components[typeid(T)] = new T(std::forward<Args>(args)...);
				//std::make_unique<T>(std::forward<Args>(args)...);
		}

		void AddComponent(UComponent* component)
		{
			components[typeid(*component)] = component;
		}

		template<typename T>
		T* GetComponent() const
		{
			static_assert(std::is_base_of_v<uei::UComponent, T>,
				"T must derive from UComponent");

			auto component = components.find(typeid(T));
			return component != components.end()
				? static_cast<T*>(component->second)
				: nullptr;
		}
		template<typename T>
		bool HasComponent() const
		{
			static_assert(std::is_base_of_v<uei::UComponent, T>,
				"T must derive from UComponent");

			return components.find(typeid(T)) != components.end();
		}
		template<typename T>
		void RemoveComponent()
		{
			static_assert(std::is_base_of_v<uei::UComponent, T>,
				"T must derive from UComponent");

			auto component = components.find(typeid(T));
			if (component != components.end())
			{
				components.erase(component);
			}
		}

		UEntity* Clone()
		{
			UEntity* entity = new UEntity(0, this->Name());

			for (const auto& [key, value] : components)
			{
				entity->components[key] = value->Clone();
			}
			return entity;
		}

	private:
		UEntity(const size_t inID, std::string& inName) : iD(inID), name(inName), bIsActive(true) {}

		const size_t iD;
		std::string name;
		bool bIsActive;
		std::unordered_map<std::type_index, uei::UComponent*> components;
	};
}