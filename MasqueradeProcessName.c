#include <Windows.h>
#include <winternl.h>
#include <stdio.h>

typedef NTSTATUS(*MYPROC) (HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

int main()
{
	HANDLE h = GetCurrentProcess();
	PROCESS_BASIC_INFORMATION ProcessInformation;
	ULONG length = 0;
	HINSTANCE ntdll;
	MYPROC GetProcessInformation;
	wchar_t commandline[] = L"C:\\Windows\\System32\\notepad.exe";
	ntdll = LoadLibrary(TEXT("Ntdll.dll"));

	//resolve address of NtQueryInformationProcess in ntdll.dll
	GetProcessInformation = (MYPROC)GetProcAddress(ntdll, "NtQueryInformationProcess");

	//get _PEB object
	(GetProcessInformation)(h, ProcessBasicInformation, &ProcessInformation, sizeof(ProcessInformation), &length);

	//replace commandline and imagepathname
	ProcessInformation.PebBaseAddress->ProcessParameters->CommandLine.Length = sizeof(commandline);
	ProcessInformation.PebBaseAddress->ProcessParameters->CommandLine.Buffer = commandline;
	ProcessInformation.PebBaseAddress->ProcessParameters->ImagePathName.Length = sizeof(commandline);
	ProcessInformation.PebBaseAddress->ProcessParameters->ImagePathName.Buffer = commandline;

	DWORD PID = GetCurrentProcessId();
	printf("%d", PID);

	getch();

	return 0;
}