#pragma once
#include "Component.hpp"

class ComponentRegister {
	std::map<ComponentId, std::shared_ptr<Component>> componentContainer;
	ComponentId currId;
	public:
		ComponentRegister()
			: currId(1)
			{}

		ComponentId getNextId(){
			return ++this->currId;
		}
	
		void registerComponent(std::weak_ptr<Component> component){
			this->componentContainer[component.lock()->getId()] = component.lock();
		}
};