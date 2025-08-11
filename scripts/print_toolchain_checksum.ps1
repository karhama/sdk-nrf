$BaseDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# Combine files and remove carriage returns
$Content = (Get-Content "$BaseDir\requirements-fixed.txt","$BaseDir\tools-versions-win10.yml" -Raw) -replace "`r",""

# Compute SHA256 hash from string
$Bytes = [System.Text.Encoding]::UTF8.GetBytes($Content)
$HashBytes = [System.Security.Cryptography.SHA256]::Create().ComputeHash($Bytes)
$ToolchainVersion = ($HashBytes | ForEach-Object { $_.ToString("x2") }) -join "" | Select-Object -First 10

Write-Output $ToolchainVersion

