#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <iostream>


int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    std::cout << " value of file handle is  " << argv[1] << '\n';
    Sleep(5000);
    char* endptr{};
    HANDLE hFile = reinterpret_cast<HANDLE>(static_cast<UINT_PTR>(strtoul(argv[1], &endptr, 10)));

    char Buffer[1024]{};
    DWORD dwRead{};
    LARGE_INTEGER li{};

//  Reposition file pointer to beginning of the file
    if (SetFilePointerEx(hFile, li, nullptr, FILE_BEGIN))
    {
        //Assumes that file does not contain UNICODE (UTF16-LE) strings
        if (ReadFile(hFile, Buffer, 512, &dwRead, NULL))
            printf_s("%s", Buffer);
        else
            printf("ReadFile failed - %d\n", GetLastError());
    }
    else
        printf("SetFilePointerEx failed - %d\n", GetLastError());

    if (!CloseHandle(hFile))
        printf_s("CloseHandle failed - %d\n", GetLastError());

    puts("Hit a key to exit");

    _getch();

    return 0;
}
