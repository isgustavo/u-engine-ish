#include "CPathRequest.h"
#include <SFML/System/Vector2.hpp>

namespace uei
{
	CPathRequest::CPathRequest() : UComponent()
	{
		targetPosition = sf::Vector2f(0.0f, 0.0f);
	}

	CPathRequest::CPathRequest(sf::Vector2f& inTargetPosition) : CPathRequest()
	{
		targetPosition = inTargetPosition;
	}

	CPathRequest::~CPathRequest()
	{

	}
}