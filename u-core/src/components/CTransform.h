#pragma once
#include "UComponent.h"
#include <SFML/System/Vector2.hpp>
#include <istream>

namespace uei
{
	class CTransform : public UComponent
	{
	public:
		CTransform();
		CTransform(const sf::Vector2f& inPosition/*, sf::Vector2f& inInitialVelocity*/);

		sf::Vector2f& Position();
		sf::Vector2f& PositionLastUpdate();
		void InitPosition(const sf::Vector2f& inPosition);
		void SetPosition(const sf::Vector2f& inPosition);
		bool IsMoving(const float threshold = 0.001f);
		//void SetVelocity(const sf::Vector2f& inVelocity);
		bool ShouldUpdate() const;

		UComponent* Clone() const override
		{
			return new CTransform(sf::Vector2f(this->position.x, this->position.y));
		}
		inline std::string ComponentName() const override { return "CTransform"; }

		void LoadComponent(UEngine& engine, std::istream& in) override;
		std::string Save() const override;

	protected: 
		void OnShowEditor(UEngine& engine) override;
		int GetEditorSize(UEngine& engine) const override;

	private:
		
		sf::Vector2f position;
		sf::Vector2f positionLastUpdate;
		bool bUpdate;

		void OnComponentAdd(class UEntity& entity) override;
		void OnComponentRemove(class UEntity& entity) override;

		// Inherited via UComponent

		//sf::Vector2f velocity;
		//Vec2f scale;
		//float angle;
	};


}


