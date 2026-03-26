#include "SPathRequestSystem.h"
#include <components/CAgent.h>
#include <components/CPathRequest.h>
#include <components/CPath.h>
#include <components/CTransform.h>

namespace uei
{
	SPathRequestSystem::SPathRequestSystem()
	{

	}

	SPathRequestSystem::~SPathRequestSystem()
	{

	}

	void uei::SPathRequestSystem::Update(UEngine& engine, std::vector<UEntity*> entities)
	{
		for (auto& e : entities)
		{
			auto* cAgent = e->GetComponent<CAgent>();
			if (cAgent == nullptr) continue;

			auto* cPathRequest = e->GetComponent<CPathRequest>();
			if (cPathRequest != nullptr) continue;

			auto* cPath = e->GetComponent<CPath>();
			if (cPath != nullptr) continue;

			auto* cTransform = e->GetComponent<CTransform>();
			if (cTransform == nullptr) continue;

			CPathRequest* pathRequest = e->AddComponent<CPathRequest>();

			//CPath* path = e->AddComponent<CPath>();
			//path->SetPath(FindPath(engine.CurrentScene()->GetNavGrid(),
			//	PositionToGrid(cTransform->GetPosition(), engine.CurrentScene()->GridSize()),
			//	//PositionToGrid(cPathRequest->TargetPosition(), engine.CurrentScene()->GridColumns(), engine.CurrentScene()->GridRows()),
			//	PositionToGrid(playerTransform->GetPosition(), engine.CurrentScene()->GridSize()),
			//	cMovement->GetValidGridMovement(),
			//	1,
			//	1,
			//	engine.CurrentScene()->GridColumns()));

			/*std::optional<std::type_index> componentToRemove;
			for (auto& [type, value] : e->Components())
			{
				if (value == cPathRequest)
					componentToRemove = type;
			}

			if(componentToRemove)
				e->RemoveComponent(*componentToRemove);*/
		}
	}
	
}