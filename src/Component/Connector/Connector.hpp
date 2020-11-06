#pragma once
#include "../Component.hpp"
#include "../../Boundary/Boundary.hpp"

class Connector: public Component{
		int id;
		std::weak_ptr<Boundary> boundary0;
		std::weak_ptr<Boundary> boundary1;
}