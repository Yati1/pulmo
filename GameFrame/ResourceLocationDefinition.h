#ifndef RESOURCELOCATIONDEFINITION_H_
#define RESOURCELOCATIONDEFINITION_H_

#include "string"
#include "vector"

struct ResourceLocationDefinition
{
	ResourceLocationDefinition(std::string type, std::string location, std::string section) :
		type(type),
		location(location),
		section(section)
	{

	}

	~ResourceLocationDefinition() {}
	
	std::string type;
	std::string location;
	std::string section;
};

typedef std::vector<ResourceLocationDefinition> ResourceLocationDefinitionVector;

#endif
