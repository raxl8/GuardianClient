#pragma once

template<typename T>
class Instance
{
public:
	Instance()
	{
		s_Instance = (T*)this;
	}

	virtual ~Instance() = default;

	static T* Get() { return s_Instance; }

private:
	static T* s_Instance;
};

#define DEFINE_INSTANCE_TYPE(type) type* Instance<type>::s_Instance = nullptr
