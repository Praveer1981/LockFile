#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlwapi.h>
#include < strsafe.h>
#pragma comment(lib, "shlwapi")

#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

#define _CREATE_FILE_ 1

int main()
{

    char szExePath[MAX_PATH]{}, szKixFilePath[MAX_PATH]{}, szHandle[64]{}, szCmdLine[512]{};
    GetModuleFileNameA(NULL, szExePath, _countof(szExePath));
    PathRemoveFileSpecA(szExePath); // Removes application name Ex: xyz.exe

    PathAppendA(szExePath, "PraveerTemp.kix"); // Append the file name in the same path
    STARTUPINFOA si{ sizeof si };
    PROCESS_INFORMATION pi{};
#ifdef _CREATE_FILE_
    SECURITY_ATTRIBUTES sa{ sizeof sa, nullptr, TRUE };
    HANDLE hFile = CreateFileA(szExePath,
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                &sa,
                                CREATE_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL, NULL); // Create a file
    if (hFile != INVALID_HANDLE_VALUE)
    {
        char szLines[] = ";--------start PraveerTemp.kix--------\n 'Hello World ' \n;-------- end PraveerTemp.kix--------";
        printf_s("Write the string in the test file:\n%s", szLines);
        DWORD dwWritten{};
        if (WriteFile(hFile,
            szLines,
            static_cast<DWORD>(strlen(szLines) * sizeof(char)),
            &dwWritten,
            NULL)) // Write into the file "PraveerTest.txt"
        {

#endif
            HRESULT result = StringCchCopyA(szKixFilePath, sizeof(szKixFilePath), szExePath);

            if (result != S_OK)
            {
                printf_s("copy string failed, error %d\n", GetLastError());
            }

            PathRemoveFileSpecA(szExePath); // again remove the "PraveerTest.txt" from the path
            PathAppendA(szExePath, "wKiX32.exe"); // I want to wKIX32.exe to run as a child process.
#if _CREATE_FILE_
            UINT_PTR uiHandle = reinterpret_cast<UINT_PTR>(hFile);
            sprintf_s(szCmdLine, "\"%s\" \"%s\" %Iu", szExePath, szKixFilePath, uiHandle);
#else
            sprintf_s(szCmdLine, "\"%s\" \"%s\"", szExePath, szKixFilePath);
#endif
           // sprintf_s(szCmdLine, "\"%s\"", szExePath);
            // Here we are creating the child process - FileReader.exe it will read the dta from "PraveerTest.txt" file

            if (CreateProcessA(nullptr,
                szCmdLine, 
                nullptr,
                nullptr,
                TRUE,
                CREATE_NEW_CONSOLE,
                nullptr, nullptr, &si, &pi))
            {
               CloseHandle(pi.hThread);
               CloseHandle(pi.hProcess);
               // Sleep for 10 seconds to show that FileReader.exe can acess the file while FileCreator holds the file handle
               // also during this time period we can try to open the "Praveertest.txt" file, we can see that it does not allow us to opn the file
               Sleep(10000);
            }
            else
                printf_s("CreateProcess failed, error %d\n", GetLastError());
#ifdef _CREATE_FILE_
        }
        else
            printf_s("Writefile failed, error %d\n", GetLastError());

        CloseHandle(hFile);
    }
    else
        printf_s("CreateFile failed, error %d\n", GetLastError());
#endif
    return 0;
}
