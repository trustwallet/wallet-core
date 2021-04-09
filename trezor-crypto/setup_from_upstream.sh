# Steps to update TrezorCrypto lib to a newer version:
# - start with a clean wallet-core repo
# - clone fresh version of https://github.com/trezor/trezor-firmware
# - Copy contents of trezor-firmware/crypto to trezor-crypto/crypto
# - Run this script (or below commands) from trezor-crypto folder, to move over header files to include folder
# - Try compiling TrezorCrypto lib, wallet-core lib, TrezorCrypto tests, wallet-core tests
# - Execute tests
# - Do fixes as needed
# - Review changes (changed files, new files, removed files)
# - Things to watch out for:
#  -- changes in #include's due to different location of header files
#  -- wallet-core-specific changes, marked [wallet-core]
#  -- static variables

TARGET=include/TrezorCrypto

mkdir -p $TARGET
mv crypto/*.h $TARGET
mv crypto/aes/aes.h $TARGET
mkdir -p $TARGET/aes
mv crypto/aes/*.h $TARGET/aes
mv crypto/ed25519-donna/ed25519.h $TARGET
mkdir -p $TARGET/ed25519-donna
mv crypto/ed25519-donna/*.h $TARGET/ed25519-donna
mkdir -p $TARGET/chacha20poly1305
mv crypto/chacha20poly1305/*.h $TARGET/chacha20poly1305
