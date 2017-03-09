#pragma once

class Module
{
protected:
	virtual void Load() = 0;
	virtual void Unload() = 0;
};
