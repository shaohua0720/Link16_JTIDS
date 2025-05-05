@echo off
echo Checking all code files for UTF-8 BOM...
echo.

set total=0
set utf8=0
set non_utf8=0

for /r %%f in (*.cpp *.h *.hpp) do (
    set /a total+=1
    powershell -Command "$bytes = [System.IO.File]::ReadAllBytes('%%f'); if ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) { exit 0 } else { exit 1 }" && (
        set /a utf8+=1
    ) || (
        set /a non_utf8+=1
        echo File does NOT have UTF-8 BOM: %%f
    )
)

echo.
echo Total files: %total%
echo UTF-8 (with BOM) files: %utf8%
echo Non-UTF-8 (with BOM) files: %non_utf8%

if %non_utf8% equ 0 (
    echo All files have been successfully converted to UTF-8 (with BOM) encoding
)
