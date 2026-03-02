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

		template<typename T>
		T* AddComponent()
		{
			static_assert(std::is_base_of_v<uei::UComponent, T>, "T must derive from UComponent");

			T* t = new T();
			components[typeid(T)] = t;
			return t;
		}

		void AddComponent(UComponent* component)
		{
			component->OnComponentAdd(*this);
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
		T* GetOrAddComponent()
		{
			static_assert(std::is_base_of_v<uei::UComponent, T>, "T must derive from UComponent");

			auto* component = GetComponent<T>();
			if (component == nullptr)
			{
				component = AddComponent<T>();
			}
			
			return component;
		}

		template<typename T>
		T* SetRequiredByOtherComponent(bool value)
		{
			static_assert(std::is_base_of_v<uei::UComponent, T>,
				"T must derive from UComponent");

			UComponent* component = GetOrAddComponent<T>();
			component->SetRequiredByOtherComponent(value);
			return static_cast<T*>(component);
		}

		void RemoveComponent(std::type_index value)
		{
			auto component = components.find(value);
			if (component != components.end())
			{
				auto* c = component->second;
				c->OnComponentRemove(*this);
				components.erase(value);
			}
		}

		UEntity* Clone()
		{
			UEntity* entity = new UEntity(0, this->Name());

			for (const auto& [key, value] : components)
			{
				entity->components[key] = value->Clone();
			}

			for (const auto& [key, value] : entity->components)
			{
				entity->components[key]->OnComponentAdd(*entity);
			}

			return entity;
		}

	private:
		UEntity(const size_t inID, std::string& inName) : iD(inID), name(inName), bIsActive(true) {}

		const void SetID(const size_t iniD) { iD = iniD; }

		size_t iD;
		std::string name;
		bool bIsActive;
		std::unordered_map<std::type_index, uei::UComponent*> components;
	};
}