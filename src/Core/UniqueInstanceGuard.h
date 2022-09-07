#pragma once

class UniqueInstanceGuard
{
public:
	UniqueInstanceGuard();
	~UniqueInstanceGuard();

private:
#ifdef GDN_WINDOWS
	HANDLE m_Mutex;
#endif
};
