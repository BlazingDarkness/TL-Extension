#pragma once

class IManagedResource
{
public:
	virtual void Update(float delta) = 0;
	virtual bool IsFinished() = 0;
	virtual void Hide() = 0;
	virtual void Reset() = 0;
};