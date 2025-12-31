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
		friend class UScene;

	public:
		bool IsActive() const;
		const std::string& GetTag() const;

		template<typename T, typename... Args>
		void AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<uei::UComponent, T>,
				"T must derive from UComponent");

			components[typeid(T)] =
				std::make_unique<T>(std::forward<Args>(args)...);
		}

		template<typename T>
		T* GetComponent()
		{
			static_assert(std::is_base_of_v<uei::UComponent, T>,
				"T must derive from UComponent");

			auto it = components.find(typeid(T));
			return it != components.end()
				? static_cast<T*>(it->second.get())
				: nullptr;
		}

		template<typename T>
		bool HasComponent() const
		{
			static_assert(std::is_base_of_v<uei::UComponent, T>,
				"T must derive from UComponent");

			return components.find(typeid(T)) != components.end();
		}

	private:

		UEntity(const size_t inID, const std::string& inTag)
			:iD(inID), bIsActive(true), tag(inTag)
		{
		}

		const size_t iD;
		bool bIsActive;
		const std::string tag;
		std::unordered_map<std::type_index, std::unique_ptr<uei::UComponent>> components;
	};
}