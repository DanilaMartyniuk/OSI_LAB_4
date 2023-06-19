#include <windows.h>
#include <conio.h>
#include<vector>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {

    HANDLE writer, reader;
    HANDLE startRead;
    char readFlag[] = "readFlag";
    startRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, readFlag);
    writer = (HANDLE) atoi(argv[1]);
    reader = (HANDLE) atoi(argv[2]);
    DWORD dwBytesRead;
    int size = 0;

    ReadFile(reader,&size,sizeof(size),&dwBytesRead,NULL);
    vector<int> first;
    for (int j = 0; j < size; j++) {
        int nData;
        if (!ReadFile(
                reader,
                &nData,
                sizeof(nData),
                &dwBytesRead,
                NULL)) {
            return GetLastError();
        }
        cout << nData << endl;
        first.push_back(nData);
    }

    vector<int> second;
    for (int i = 0; i < 10; i++)
    {
        second.push_back(i);
    }

    vector<int> third;
    for (int i = 0; i < first.size(); i++){
        third.push_back(first[i]);
    }


    for (int i = 0; i < second.size(); i++){
            third.push_back(second[i]);
    }

    DWORD dwBytesWritten;
    int result_size = third.size();

    WriteFile(writer, &result_size ,sizeof(result_size), &dwBytesWritten ,NULL);
    for (int j = 0; j < third.size(); j++) {
        if (!WriteFile(
                writer,
                &third[j],
                sizeof(third[j]),
                &dwBytesWritten,
                NULL)) {
            return GetLastError();
        }
    }
    SetEvent(startRead);
    CloseHandle(writer);
    CloseHandle(reader);
    CloseHandle(startRead);
    return 0;
}