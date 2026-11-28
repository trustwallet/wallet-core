# SPDX-License-Identifier: Apache-2.0
$check = Join-Path $PSScriptRoot '..\..\tools\windows-check-exports.ps1'
$exports = @('TWStringCreateWithUTF8Bytes', 'TWDataCreateWithBytes', 'TWMessageSignerSign', 'hrpForString', 'stringForHRP') + @(1..461 | ForEach-Object { "TWExample$_" })
& $check -Exports $exports
foreach ($invalid in @(
    @{ names = $exports[0..464]; error = 'Expected at least 466*' },
    @{ names = @($exports | Where-Object { $_ -ne 'hrpForString' }) + 'TWReplacement'; error = 'Missing public C API export: hrpForString' }
)) {
    $rejected = $false
    try { & $check -Exports $invalid.names } catch {
        if ($_.Exception.Message -notlike $invalid.error) { throw }
        $rejected = $true
    }
    if (!$rejected) { throw 'Invalid export list was accepted' }
}
Write-Output 'Export checks: valid list passed; short list and missing symbol rejected'
