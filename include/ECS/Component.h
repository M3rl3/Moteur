#pragma once

#include <string>

class Component {
public:
	Component() {}

	virtual ~Component() {}

	std::string GetType() {
		return componentType;
	};

private:

	std::string componentType = "";
	
};