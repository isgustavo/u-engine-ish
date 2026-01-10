#pragma once
#include "UComponent.h"
#include <SFML/System/Vector2.hpp>
#include <vector>

namespace uei
{
	class CPath : public UComponent
	{
	public:

		CPath(const std::vector<sf::Vector2i>& inPath) : UComponent(),
			path(inPath)
		{
		}

		const std::vector<sf::Vector2i>& Path() const;

	private:
		const std::vector<sf::Vector2i> path;
	};
}