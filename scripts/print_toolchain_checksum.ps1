$BaseDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$tools = Get-Content "$BaseDir\requirements-fixed.txt" -Raw
$pyreq = Get-Content "$BaseDir\tools-versions-win10.yml" -Raw
$Content = $tools + $pyreq
$Normalized = $Content -replace "(\r\n?|\n)", "`n"
$Bytes = [System.Text.Encoding]::UTF8.GetBytes($Normalized)
$HashBytes = [System.Security.Cryptography.SHA256]::Create().ComputeHash($Bytes)
$ToolchainVersion = (($HashBytes | ForEach-Object { $_.ToString("x2") }) -join "").Substring(0,10)
Write-Output $ToolchainVersion

