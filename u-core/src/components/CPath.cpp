#include "CPath.h"

namespace uei
{
	CPath::CPath() : UComponent()
	{

	}

	CPath::CPath(int inCurrentPathNodeIndex, std::vector<sf::Vector2i>& inPath) : CPath()
	{
		currentPathNodeIndex = currentPathNodeIndex;
		path = inPath;
	}

	CPath::~CPath()
	{
		path.clear();
	}

	void CPath::SetPath(std::vector<sf::Vector2i> inPath)
	{
		path.clear();
		path = inPath;
		currentPathNodeIndex = path.size() - 1;
	}

	void CPath::UpdateCurrentPathNodeIndex()
	{ 
		currentPathNodeIndex--;
	}
}
