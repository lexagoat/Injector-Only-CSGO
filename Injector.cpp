#include <windows.h>
#include <iostream>
#include <TlHelp32.h>

void LoadDll(HANDLE process, const wchar_t* dllPath)
{
	int namelen = wcslen(dllPath) + 1;
	LPVOID remoteString = VirtualAllocEx(process, nullptr, namelen * 2, MEM_COMMIT, PAGE_EXECUTE);
	WriteProcessMemory(process, remoteString, dllPath, namelen * 2, nullptr);

	HMODULE kernel32 = GetModuleHandle("kernel32.dll");
	std::cout << "Kernal Module Handle: " << kernel32 << '\n';

	LPVOID funcAdr = GetProcAddress(kernel32, "LoadLibraryW");
	std::cout << "LoadLibraryW function address: " << funcAdr << '\n';

	HANDLE thread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)funcAdr, remoteString, NULL, NULL);

	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
}

int main()
{
	HWND myWindow = FindWindowA(nullptr, "Counter-Strike: Global Offensive");
	if (myWindow == NULL)
	{
		MessageBox(nullptr, "Couldn't found game Window", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	
	DWORD PID;
	GetWindowThreadProcessId(myWindow, &PID);

	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	LoadDll(proc, L"c::\\CSGO-Cheat-Internal.dll"); // <--- change DLL Path to whatever u want example

	return 0;
}
