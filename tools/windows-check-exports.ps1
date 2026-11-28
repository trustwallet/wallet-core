# SPDX-License-Identifier: Apache-2.0
param([Parameter(Mandatory = $true)][string[]]$Exports)

# Regression floor, not a proof that every public declaration was parsed.
if (@($Exports | Select-Object -Unique).Count -lt 466) { throw 'Expected at least 466 public C API exports' }
foreach ($name in @('TWStringCreateWithUTF8Bytes', 'TWDataCreateWithBytes', 'TWMessageSignerSign', 'hrpForString', 'stringForHRP')) {
    if ($Exports -notcontains $name) { throw "Missing public C API export: $name" }
}
