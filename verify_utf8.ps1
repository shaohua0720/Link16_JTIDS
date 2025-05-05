# 验证所有代码文件是否为UTF-8编码
$files = Get-ChildItem -Path . -Recurse -Include *.cpp,*.h,*.hpp

$totalFiles = 0
$utf8Files = 0
$nonUtf8Files = @()

foreach ($file in $files) {
    $totalFiles++
    
    # 读取文件前几个字节
    $bytes = [System.IO.File]::ReadAllBytes($file.FullName)
    
    # 检查是否有UTF-8 BOM
    if ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) {
        $utf8Files++
    }
    else {
        $nonUtf8Files += $file.FullName
    }
}

Write-Host "检查结果:"
Write-Host "总文件数: $totalFiles"
Write-Host "UTF-8 (带BOM)文件数: $utf8Files"

if ($nonUtf8Files.Count -gt 0) {
    Write-Host "以下文件可能不是UTF-8 (带BOM)编码:"
    foreach ($file in $nonUtf8Files) {
        Write-Host "  $file"
    }
}
else {
    Write-Host "所有文件都已成功转换为UTF-8 (带BOM)编码"
}
