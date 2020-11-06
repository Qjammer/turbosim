#pragma once
#include "../Boundary/Boundary.hpp"

class Component{
	public:
		virtual int getId() const = 0;
		virtual int getBoundaries() const = 0;
		virtual const Boundary& getBoundary(int i) const = 0;
		virtual int getInvariantCount() const = 0;
		virtual float getInvariant(int i) const = 0;
		virtual float getInvariantDerivative(int invariant, int parameter) const = 0;
};