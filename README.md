# LockFile
The purpose of this sample application is to demonstrate that wkix32.exe can not run the script from a .kix file which is created using win32 API

1. Build the code using (VS2019 +). Make sure `File Creator` is set as active project.
2. Now we can debug the code. we can see that it throws error.

    `---------------------------`
    `>>> KiXtart <<<`
    `---------------------------`

    `ERROR : failed to find/open script [F:\Praveer_Practice\vs2012\FileLock_V2\LockFile\x64\Debug\PraveerTemp.kix]!`

    `---------------------------`
    `OK`
    `---------------------------`
    ![ScreenShot](/image/Error.png)

    I think in this case wkix32.exe tries to open the file which was locked by the parent process. Eventhough, we pass the handle of the file to `wkix32.exe` , internally it does not use that file handle to read the file and hence it throws error. I believe that `wkix32.exe` uses `c` functions to open the file which does not need the file handle.

3. comment `#define _CREATE_FILE_ 1` in `FileCreator.cpp` and build the code.

4. Debug the code from `File Creator` project. you will not see this error.
