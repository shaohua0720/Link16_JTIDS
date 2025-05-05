@echo off
echo Converting all code files to UTF-8 with BOM...

for /r %%f in (*.cpp *.h *.hpp) do (
    echo Processing: %%f
    powershell -Command "[System.IO.File]::WriteAllText('%%f', [System.IO.File]::ReadAllText('%%f'), [System.Text.Encoding]::UTF8)"
)

echo All files have been converted to UTF-8 with BOM encoding.
