
# Run tests

$ErrorActionPreference = "Stop"

.\build\static\trezor-crypto\crypto\tests\Release\TrezorCryptoTests.exe
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

.\build\static\tests\Release\tests.exe tests
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
