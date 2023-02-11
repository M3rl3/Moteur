#pragma once

#include <string>

class Component {
public:
	Component() {}

	virtual ~Component() {}

	std::string GetType() {
		return componentType;
	};
	
	void SetType(std::string type) {
		componentType = type;
	};

private:

	std::string componentType;
	
};