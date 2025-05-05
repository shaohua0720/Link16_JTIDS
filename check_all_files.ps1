# 检查所有代码文件是否有UTF-8 BOM
$files = Get-ChildItem -Path . -Recurse -Include *.cpp,*.h,*.hpp

$totalFiles = 0
$utf8Files = 0
$nonUtf8Files = @()

foreach ($file in $files) {
    $totalFiles++
    $filePath = $file.FullName
    
    # 读取文件前几个字节
    try {
        $fileStream = [System.IO.File]::OpenRead($filePath)
        $bytes = New-Object byte[] 3
        $bytesRead = $fileStream.Read($bytes, 0, 3)
        $fileStream.Close()
        
        # 检查是否有UTF-8 BOM
        if ($bytesRead -eq 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) {
            $utf8Files++
        }
        else {
            $nonUtf8Files += $filePath
        }
    }
    catch {
        Write-Host "Error reading file: $filePath - $_"
        $nonUtf8Files += $filePath
    }
}

Write-Host "Total files: $totalFiles"
Write-Host "UTF-8 (with BOM) files: $utf8Files"
Write-Host "Non-UTF-8 (with BOM) files: $($nonUtf8Files.Count)"

if ($nonUtf8Files.Count -gt 0) {
    Write-Host "Files that may not be UTF-8 (with BOM) encoded:"
    foreach ($file in $nonUtf8Files) {
        Write-Host "  $file"
    }
}
else {
    Write-Host "All files have been successfully converted to UTF-8 (with BOM) encoding"
}
