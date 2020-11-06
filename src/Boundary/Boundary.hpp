#pragma once

class Boundary{
	public:
		virtual int getId() const = 0;
		virtual int getParameterCount() const = 0;
		virtual float getParameter(int i) const = 0;
};