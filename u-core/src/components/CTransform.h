#pragma once
#include "UComponent.h"
#include <SFML/System/Vector2.hpp>

namespace uei
{
	class CTransform : public UComponent
	{
	public:
		CTransform();
		~CTransform();

		sf::Vector2f& GetPosition();
		sf::Vector2f& GetPositionLastUpdate();
		void SetPosition(const sf::Vector2f& inPosition);
		bool IsMoving(const float threshold = 0.001f) const;

		UComponent* Clone() override
		{
			return new CTransform(sf::Vector2f(this->position.x, this->position.y));
		}

		inline std::string ComponentName() const override { return "CTransform"; }
		void LoadComponent(std::istream & in) override;
		std::string SaveComponent() const override;

	protected: 
		void OnShowEditor(UEngine& engine) override;

	private:
		CTransform(const sf::Vector2f& inPosition);

		sf::Vector2f position;
		sf::Vector2f positionLastUpdate;
	};


}


