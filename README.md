The code in this repository is not malicious.

## Compilation
```
cl -O2 /GS- /EHsc /I. /Fe:output.exe main.cpp header.cpp
```
- `-O2` - Maximize speed
- `/GS-` - Disable buffer security check
- `/EHsc` - Standard C++ Exception Handling
- `/I.` - search for header files in `./`