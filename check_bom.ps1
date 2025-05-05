# 检查文件是否有UTF-8 BOM
param(
    [string]$filePath
)

$fileStream = [System.IO.File]::OpenRead($filePath)
$bytes = New-Object byte[] 3
$bytesRead = $fileStream.Read($bytes, 0, 3)
$fileStream.Close()

if ($bytesRead -eq 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) {
    Write-Host "File has UTF-8 BOM: $filePath"
    exit 0
} else {
    Write-Host "File does NOT have UTF-8 BOM: $filePath"
    exit 1
}
