@echo off
echo Checking UTF-8 BOM for file: %1
powershell -Command "$bytes = [System.IO.File]::ReadAllBytes('%1'); if ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) { Write-Host 'File has UTF-8 BOM' } else { Write-Host 'File does NOT have UTF-8 BOM' }"
