#pragma once
#include"preinclude.h"
class ControlBase
{
private:
	static int _resource_id;
protected:
	void IncrementResourceID();
public:
	int resource_id;
	std::string unique_name;

	ControlBase();	
	virtual ~ControlBase() {}
};
