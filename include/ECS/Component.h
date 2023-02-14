#pragma once

#include <string>

// Interface for all components
class Component {
public:

	// Constructor
	Component() {}

	// virtual destructor to prevent memory leaks
	virtual ~Component() {}

	// returns the type of the component;
	std::string GetType() {
		return componentType;
	};
	
	// Sets the component type
	void SetType(std::string type) {
		componentType = type;
	};

private:

	// string value holding the name of the component
	std::string componentType;
};