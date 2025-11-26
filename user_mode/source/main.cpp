#include <iostream>
#include <cstdint>
#include <map>

#include <Windows.h>
#include <psapi.h>
#include <TlHelp32.h>

#include "../headers/process_helper.h"
#include "../headers/driver_helper.h"
#include "../headers/error_helper.h"
#include "../headers/offsets.h"


int main()
{
	const char* process_name = "Notepad.exe";
	Process process(process_name);
	
	ioctl::Driver driver;

	// Check if process is Valid
	if (process.handle == INVALID_HANDLE_VALUE)
	{
		std::string errorMsg = GetErrorString(GetLastError());

		std::cout << "[-] Failed to create process handle: " << errorMsg;
		return 1;
	}

	// Optional: Print all modules loaded into the process. Remove if not needed.
	{
		std::cout << "\n--------------------------------------\n";

		std::cout << "Modules loaded into " << process_name << ": \n";
		process.PrintAllModules(true);

		std::cout << "--------------------------------------\n\n";
	}
	


	// Check if driver handle is valid
	if (driver.driver_handle == INVALID_HANDLE_VALUE)
	{
		std::string errorMsg = GetErrorString(GetLastError());

		std::cout << "[-] Failed to create driver handle: " << errorMsg << '\n';
		std::cin.get();
		return 1;
	}

	// Your code here
	
	if (driver.attach_to_process(process.pid))
	{

	}

    // End

	std::cout << "End of Usermode Application.\n";
	std::cin.get();

	return 0;

}
