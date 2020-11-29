#pragma once
#include "../Math/Parameter/Parameter.hpp"

class Boundary {
		int id;
	public:
		Boundary(int id): id(id) {}
		virtual int getId() const {return this->id;};
};