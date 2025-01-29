gcc wmain.c -Wall -DPSAPI_VERSION=1 -lpsapi -o bentest.exe
gcc test.c -DPSAPI_VERSION=1 -lpsapi -o test.exe
