#pragma once

template<typename T>
class Instance
{
public:
	Instance() = default;
	~Instance() = default;

	static void Set(T* instance) { s_Instance = instance; }
	static T* Get() { return s_Instance; }

private:
	static T* s_Instance;
};

#define DEFINE_INSTANCE_TYPE(type) type* Instance<type>::s_Instance = nullptr
