#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <vector>

using namespace std;

int main() {

    char lpszComLine[1000];
    char readFlag[] = "readFlag";
    HANDLE startRead;
    HANDLE writer, reader, inheriter;
    STARTUPINFO startInfo;
    PROCESS_INFORMATION procInfo;
    SECURITY_ATTRIBUTES secAttr;

    startRead = CreateEvent(NULL, FALSE, FALSE, readFlag);
    secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    secAttr.lpSecurityDescriptor = NULL;
    secAttr.bInheritHandle = TRUE;
    ZeroMemory(&startInfo, sizeof(STARTUPINFO));

    int size;
    cout << "Enter size of array: ";
    cin >> size;

    vector<int> first(size);
    for (int i = 0; i < size; i++)
    {
        cout << "Enter element[" << i+1 << "]\n";
        cin >> first[i];
    }

    cout << endl;



    if (!CreatePipe(&reader,&writer,&secAttr,0)) {
        return GetLastError();
    }
    if (!DuplicateHandle(GetCurrentProcess(), writer, GetCurrentProcess(),
                         &inheriter, 0, TRUE, DUPLICATE_SAME_ACCESS))
    {
        return GetLastError();
    }
    CloseHandle(writer);
    startInfo.cb = sizeof(STARTUPINFO);

    wsprintf(lpszComLine, "..\\Client\\a.exe %d %d",
             (intptr_t) inheriter, (intptr_t) reader);
    if (!CreateProcess(
            NULL,
            lpszComLine,
            NULL,
            NULL,
            TRUE,
            CREATE_NEW_CONSOLE,
            NULL,
            NULL,
            &startInfo,
            &procInfo
    )) {
        return GetLastError();
    }

    CloseHandle(procInfo.hProcess);
    CloseHandle(procInfo.hThread);
    DWORD dwBytesWritten;
    WriteFile(inheriter, &size ,sizeof(size), &dwBytesWritten ,NULL);

    for (int j = 0; j < first.size(); j++) {
        if (!WriteFile(
                inheriter,
                &first[j],
                sizeof(first[j]),
                &dwBytesWritten,
                NULL)) {
            return GetLastError();
        }
    }
    WaitForSingleObject(startRead, INFINITE);
    DWORD dwBytesRead;
    int result_size;

    ReadFile(reader,&result_size,sizeof(result_size),&dwBytesRead,NULL);
    vector<int> result;
    for (int j = 0; j < result_size; j++) {
        int nData;
        if (!ReadFile(
                reader,
                &nData,
                sizeof(nData),
                &dwBytesRead,
                NULL)) {
            return GetLastError();
        }
        result.push_back(nData);
    }

    for (int i = 0; i < result.size(); i++){
        cout << result[i] << " ";
    }
    CloseHandle(reader);
    CloseHandle(inheriter);
    CloseHandle(startRead);
    return 0;
}
