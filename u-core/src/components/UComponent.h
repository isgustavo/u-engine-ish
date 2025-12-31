#pragma once

namespace uei
{
	class UComponent
	{
	public:
		UComponent() : bExists(false) {}
		UComponent(bool InExists) : bExists(InExists) {}

		bool GetExists() const;
	protected:
		bool bExists;
	};
}