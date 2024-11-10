/* */

#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <tlhelp32.h>

/*
  - thread for monitoring processes
  - if notepad, snippingtool or calcluator have been run then kill the processes
  - terminate and clean memory under key press
*/

using namespace std;

/* thread related */
uint8_t* pDataArray;

/* input related */
uint8_t stop_and_exit_event = 1;

/* processes related */
DWORD aProcesses[1024], cbNeeded, cProcesses;
const char notepad[]    = "Notepad.exe";
const char snipping[]   = "SnippingTool.exe";
const char calc[]       = "CalculatorApp.exe";


DWORD WINAPI MyThreadFunction(LPVOID lpParam) {
    HANDLE hndl;
    bool in_the_list = false;

    while (stop_and_exit_event) {
        cout << "checking" << endl;
        hndl = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPMODULE, 0);
        if (hndl)
        {
            PROCESSENTRY32 process = { sizeof(PROCESSENTRY32) };
            Process32First(hndl, &process);
            do {
                //cout << process.szExeFile << endl;
                //wprintf(L"Name =  %s\n", process.szExeFile);
                in_the_list = false;
                switch (process.szExeFile[0]) {
                case 'N':
                    for (uint8_t i = 0; i < strlen(notepad); i++) {
                        if ( process.szExeFile[i] != notepad[i] ) {
                            in_the_list = false;
                        }
                        else {
                            in_the_list = true;
                        }
                    }
                    if ( in_the_list == true ) {
                        //kill notepad
                        cout << "I see the notepad" << endl;
                        const auto explorer = OpenProcess(PROCESS_TERMINATE, false, process.th32ProcessID);
                        TerminateProcess(explorer, 1);
                        CloseHandle(explorer);
                    }
                    break;

                case 'S':
                    for (uint8_t i = 0; i < strlen(snipping); i++) {
                        if (process.szExeFile[i] != snipping[i]) {
                            in_the_list = false;
                        }
                        else {
                            in_the_list = true;
                        }
                    }

                    /*
                     * It was observed that unrelated processes with SnippingTool name
                     * have ParentProcessIDs less than 2000
                     */
                    if ( (in_the_list == true) && (process.th32ParentProcessID > 2000) ) {
                        //kill snip
                        cout << "I see the snipTool" << endl;
                        const auto explorer = OpenProcess(PROCESS_TERMINATE, false, process.th32ProcessID);
                        TerminateProcess(explorer, 1);
                        CloseHandle(explorer);
                    }
                    break;

                case 'C':
                    for (uint8_t i = 0; i < strlen(calc); i++) {
                        if (process.szExeFile[i] != calc[i]) {
                            in_the_list = false;
                        }
                        else {
                            in_the_list = true;
                        }
                    }
                    if (in_the_list) {
                        //kill notepad
                        cout << "I see the calc" << endl;
                        const auto explorer = OpenProcess(PROCESS_TERMINATE, false, process.th32ProcessID);
                        TerminateProcess(explorer, 1);
                        CloseHandle(explorer);
                    }
                    break;

                default:
                    break;
                }
            } while (Process32Next(hndl, &process));
        }

        Sleep(1000);
    }
    return 0;
}

int main()
{
    /* Thread related */
    HANDLE  hThreadArray;
    DWORD   dwThreadIdArray;

    /* input related */
    DWORD cc;
    INPUT_RECORD irec;
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);

    pDataArray = (uint8_t*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(uint8_t));

    hThreadArray = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size  
        MyThreadFunction,       // thread function name
        pDataArray,          // argument to thread function 
        0,                      // use default creation flags 
        &dwThreadIdArray);

    irec.EventType = 0;

    /* Detect a keypress and notify the thread */
    while (irec.EventType != KEY_EVENT) {
        ReadConsoleInput(h, &irec, 1, &cc);
    }
    stop_and_exit_event = 0;

    if (hThreadArray != 0){
        WaitForSingleObject(hThreadArray, INFINITE);
        cout << "Monitor thread is finished" << endl;
        CloseHandle(hThreadArray);
        HeapFree(GetProcessHeap(), 0, pDataArray);
        pDataArray = NULL;    // Ensure address is not reused.
    } else {
        cout << "Thread is Null" << endl;
        ExitProcess(3);
    }

    return 0;
}

