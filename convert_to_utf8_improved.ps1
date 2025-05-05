# 将所有代码文件转换为UTF-8编码（带BOM）
$files = Get-ChildItem -Path . -Recurse -Include *.cpp,*.h,*.hpp

foreach ($file in $files) {
    Write-Host "Processing file: $($file.FullName)"
    
    try {
        # 尝试检测文件编码
        $bytes = [System.IO.File]::ReadAllBytes($file.FullName)
        $hasBOM = $false
        
        # 检查是否有UTF-8 BOM
        if ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) {
            $hasBOM = $true
            Write-Host "  File already has UTF-8 BOM"
        }
        
        # 读取文件内容
        $content = [System.IO.File]::ReadAllText($file.FullName)
        
        # 以UTF-8编码（带BOM）写回文件
        [System.IO.File]::WriteAllText($file.FullName, $content, [System.Text.Encoding]::UTF8)
        
        Write-Host "  Successfully converted to UTF-8 with BOM: $($file.FullName)"
    }
    catch {
        Write-Host "  Error processing file: $_" -ForegroundColor Red
    }
}

Write-Host "All files have been converted to UTF-8 encoding with BOM"
