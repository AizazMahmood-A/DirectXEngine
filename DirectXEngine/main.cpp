// Filename : main.cpp

#include "systemclass.h"

void CreateConsole() {
	// Allocate a new console for the application
	AllocConsole();

	// Redirect standard output to the console
	FILE* fptr;
	freopen_s(&fptr, "CONOUT$", "w", stdout);  // Redirect stdout (std::cout)
	freopen_s(&fptr, "CONIN$", "r", stdin);    // Redirect stdin (std::cin) if needed
	freopen_s(&fptr, "CONOUT$", "w", stderr);  // Redirect stderr (std::cerr)

	std::cout << "Console successfully created!" << std::endl;
}

int WINAPI WinMain(HINSTANCE hlInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	// Create Console
	CreateConsole();

	// Create the system object.
	System = new SystemClass;

	// Initialize and run the system object.
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}


