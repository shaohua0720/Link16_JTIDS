# 将所有代码文件转换为UTF-8编码
$files = Get-ChildItem -Path . -Recurse -Include *.cpp,*.h,*.hpp

foreach ($file in $files) {
    Write-Host "处理文件: $($file.FullName)"
    
    # 读取文件内容
    $content = Get-Content -Path $file.FullName -Raw -Encoding Default
    
    # 将内容以UTF-8编码写回文件
    [System.IO.File]::WriteAllText($file.FullName, $content, [System.Text.Encoding]::UTF8)
    
    Write-Host "已将文件转换为UTF-8编码: $($file.FullName)"
}

Write-Host "所有文件已成功转换为UTF-8编码"
