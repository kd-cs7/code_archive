## About
This script splits a CSV file into several files. It can be executed on Windows PowerShell.

## How to Use
1. Open Windows PowerShell.
2. Run the following command in PowerShell:
```powershell
powershell -ExecutionPolicy Bypass -File "C:\data\splitcsv.ps1" -f "C:\data\testfile.csv" -n 2 -h 4

## Command Options
-File: Location of the script.
-f: Location of the file to be split. The split files will be output to the same directory.
-n: Split the file into n parts.
-h: Copy the first h lines of the CSV file to each split file.