/*
* main.c - app_crashe.exe
*
* 	Spawns a remote thread in target process to start
* execution at memory location 0x00000000.
* Simulates a null pointer dereference for the purpose
* of testing the monitoring software used by fuzzers.	
*
*/

#include "stdio.h"
#include "stdlib.h"
#include "Windows.h"

int main(int argc, char **argv)
{
	HANDLE app = NULL;
	HANDLE thread = NULL;
	int error = 0;
	int PID = 0;

	if ( argc < 2)
	{
		printf("Please provide an application to crash!!\n");
		printf("Usage: app_crasher.exe [PID]\n");
		return 1;
	}
	
	PID = atoi(argv[1]);
	printf("Opening Process with PID=%d\n", PID);

	app = OpenProcess((PROCESS_CREATE_THREAD | 
		PROCESS_QUERY_INFORMATION | 
		PROCESS_VM_OPERATION | 
		PROCESS_VM_WRITE | 
		PROCESS_VM_READ ),
		FALSE, PID);
	
	error = GetLastError();
	if ( app == NULL )
	{
		printf("ERROR:0x%0.8X\n", error);
		return error;
	}
	
	thread = CreateRemoteThread(app, NULL, NULL, NULL, NULL, NULL, NULL);

	error = GetLastError();
	if ( thread == NULL)
	{
		printf("ERROR:0x%0.8X\n", error);
		return error;
	}

	CloseHandle(thread);
	CloseHandle(app);
	printf("Done!");
	return 0;
}
