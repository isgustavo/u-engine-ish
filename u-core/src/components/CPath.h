#pragma once
#include "UComponent.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <iostream>

namespace uei
{
	class CPath : public UComponent
	{
	public:

		CPath();
		~CPath();

		int GetCurrentPathNodeIndex() const { return currentPathNodeIndex; }
		void UpdateCurrentPathNodeIndex();
		sf::Vector2i& GetCurrentPathNode() { return path[currentPathNodeIndex]; }
		std::vector<sf::Vector2i>& GetPath() { return path; }
		void SetPath(std::vector<sf::Vector2i> inPath);

		UComponent* Clone() override
		{
			return new CPath(currentPathNodeIndex, GetPath());
		}

		inline virtual std::string ComponentName() const override { return "CPath"; }

	private:
		CPath(int inCurrentPathNodeIndex, std::vector<sf::Vector2i>& inPath);
		
		int currentPathNodeIndex = 0;
		std::vector<sf::Vector2i> path;
	};
}