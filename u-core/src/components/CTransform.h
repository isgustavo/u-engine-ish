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
		void SetPosition(const sf::Vector2f& inPosition);
		//void SetVelocity(const sf::Vector2f& inVelocity);
		bool ShouldUpdate() const;

		UComponent* Clone() const override
		{
			return new CTransform(sf::Vector2f(this->position.x, this->position.y));
		}
		inline virtual std::string ComponentName() const override { return "CTransform"; }

		virtual void LoadComponent(UEngine& engine, std::istream& in) override;
		virtual std::string Save() const override;

		//void Update(const float deltaTime);
	protected: 
		virtual void OnShowEditor(UEngine& engine, std::function<void()> onRemove) override;
		virtual int GetEditorSize(UEngine& engine) const override;

	private:
		
		sf::Vector2f position;
		sf::Vector2f positionLastUpdate;
		bool bUpdate;

		virtual void OnComponentAdd(class UEntity& entity) override;
		virtual void OnComponentRemove(class UEntity& entity) override;

		// Inherited via UComponent

		//sf::Vector2f velocity;
		//Vec2f scale;
		//float angle;
	};


}


