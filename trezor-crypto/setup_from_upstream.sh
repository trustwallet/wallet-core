# Steps to update TrezorCrypto lib to a newer version:
# - start with a clean wallet-core repo
# - clone fresh version of https://github.com/trezor/trezor-firmware
# - Copy contents of trezor-firmware/crypto to trezor-crypto/crypto
# - Run this script (or below commands) from trezor-crypto folder, to move over header files to include folder
# - Try compiling TrezorCrypto lib, wallet-core lib, TrezorCrypto tests, wallet-core tests
# - Execute tests
# - Do fixes as needed
# - Review changes

TARGET=include/TrezorCrypto

cp crypto/*.h $TARGET
cp crypto/aes/aes.h cpcrypto/ed25519-donna/*.h crypto/chacha20poly1305/chacha20poly1305.h $TARGET
rm crypto/*.h
