#pragma once
#include "UComponent.h"
#include <SFML/System/Vector2.hpp>
#include <vector>

namespace uei
{
	class CPath : public UComponent
	{
	public:

		CPath() : UComponent() 
		{
		}
		CPath(const std::vector<sf::Vector2i>& inPath) : UComponent(),
			path(inPath)
		{
		}

		const std::vector<sf::Vector2i>& Path() const;

		UComponent* Clone() const override
		{
			return new CPath();
		}
		inline virtual std::string ComponentName() const override { return "CPath"; }

		virtual void LoadComponent(UEngine& engine, std::istream& in) override;
		virtual std::string Save() const override;

	protected:
		virtual void OnShowEditor(UEngine& inEngine) override;
		virtual int GetEditorSize(class UEngine& engine) const override;

	private:
		const std::vector<sf::Vector2i> path;

		virtual void OnComponentAdd(class UEntity& entity) override;
		virtual void OnComponentRemove(class UEntity& entity) override;
	};
}