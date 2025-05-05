# 验证所有代码文件是否为UTF-8编码
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
            Write-Output "UTF-8 with BOM: $filePath"
        }
        else {
            $nonUtf8Files += $filePath
            Write-Output "Not UTF-8 with BOM: $filePath"
        }
    }
    catch {
        Write-Output "Error reading file: $filePath - $_"
        $nonUtf8Files += $filePath
    }
}

Write-Output ""
Write-Output "Summary:"
Write-Output "Total files: $totalFiles"
Write-Output "UTF-8 (with BOM) files: $utf8Files"
Write-Output "Non-UTF-8 (with BOM) files: $($nonUtf8Files.Count)"

if ($nonUtf8Files.Count -gt 0) {
    Write-Output ""
    Write-Output "Files that may not be UTF-8 (with BOM) encoded:"
    foreach ($file in $nonUtf8Files) {
        Write-Output "  $file"
    }
}
else {
    Write-Output ""
    Write-Output "All files have been successfully converted to UTF-8 (with BOM) encoding"
}
