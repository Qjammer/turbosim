#pragma once

class ComponentRegister {
	std::map<ComponentId, std::weak_ptr<Component>> componentContainer;
	ComponentId currId;
	public:
		ComponentRegister()
			: currId(1)
			{}

		ComponentId getNextId(){
			return ++this->currId;
		}
	
		void registerComponent(std::weak_ptr<Component> component){
			this->componentContainer[component->getId()] = component;
		}
}