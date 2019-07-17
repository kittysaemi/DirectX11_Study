
#include "SystemClass.h"

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	CSystemClass* System = new CSystemClass;

	if(!System)
		return 0;

	if(System->Initialize())
	{
		System->Run();
	}


	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}