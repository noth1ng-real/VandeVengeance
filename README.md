The code in this repository is not malicious.

## Compilation Command
```
cl -O2 /GS- /EHsc /I. /D WIN32_LEAN_AND_MEAN /Fe:output.exe *.cpp
```
- `-O2` - Maximize speed
- `/GS-` - Disable buffer security check
- `/EHsc` - Standard C++ Exception Handling
- `/D WIN32_LEAN_AND_MEAN` - Streamline compilation
- `/I.` - search for header files in `./`
---
Compile:
```
compile.bat
```
 Or specify `.exe` output name:
```
compile.bat output.exe
```