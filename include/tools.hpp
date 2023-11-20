#pragma once

#include <string>

class Tool {
private:
	int toolID;
	std::string toolName;
public:
	Tool() = delete;
	Tool(const std::string& _name);
	
	void setToolID( int ID );
	int getToolID();

	void setToolName( std::string Name );
	std::string getToolName();

};

Tool::Tool(const std::string& _name) :
	toolName(_name) {}

void Tool::setToolID(int ID) { toolID = ID; }
int Tool::getToolID() { return toolID; }

void Tool::setToolName(std::string Name) { toolName = Name; }
std::string Tool::getToolName() { return toolName; }