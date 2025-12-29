#pragma once
#include <string>

namespace uei
{
	//using Components = std::tuple<class CTransform>;

	class UEntity
	{
	
	friend class EntityManager;

	public:
		bool IsActive() const;
		const std::string& GetTag() const;
		//get<T>: T&
		//add<T>: void
		//has<T>: bool

		~UEntity() = default;

	private:

		UEntity(const size_t inID, const std::string& inTag)
			:iD(inID), bIsActive(true), tag(inTag) {
		}

		const size_t iD;
		bool bIsActive;
		const std::string& tag;
		//Components components;
	};
}