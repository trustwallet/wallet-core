// SPDX-License-Identifier: Apache-2.0
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWHRP.h>
#include <TrustWalletCore/TWString.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TW_DYNAMIC_LOAD
#include <windows.h>
static TWString* (*string_create)(const char*);
static const char* (*string_bytes)(TWString*);
static void (*string_delete)(TWString*);
static struct TWHDWallet* (*wallet_create)(TWString*, TWString*);
static struct TWHDWallet* (*wallet_random)(int, TWString*);
static TWString* (*wallet_address)(struct TWHDWallet*, enum TWCoinType);
static void (*wallet_delete)(struct TWHDWallet*);
static const char* (*hrp_string)(enum TWHRP);
static enum TWHRP (*hrp_from_string)(const char*);
#define TWStringCreateWithUTF8Bytes string_create
#define TWStringUTF8Bytes string_bytes
#define TWStringDelete string_delete
#define TWHDWalletCreateWithMnemonic wallet_create
#define TWHDWalletCreate wallet_random
#define TWHDWalletGetAddressForCoin wallet_address
#define TWHDWalletDelete wallet_delete
#define stringForHRP hrp_string
#define hrpForString hrp_from_string
#endif

static void require(int ok, const char* message) {
    if (!ok) {
        fprintf(stderr, "%s\n", message);
        exit(1);
    }
}

int main(int argc, char** argv) {
#ifdef TW_DYNAMIC_LOAD
    require(argc == 2, "Pass the relocated DLL path");
    HMODULE library = LoadLibraryExA(argv[1], NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    require(library != NULL, "Cannot load Wallet Core DLL");
#define RESOLVE(name, symbol)                                          \
    do {                                                               \
        FARPROC proc = GetProcAddress(library, symbol);                \
        require(proc != NULL && sizeof(name) == sizeof(proc), symbol); \
        memcpy(&name, &proc, sizeof(name));                            \
    } while (0)
    RESOLVE(string_create, "TWStringCreateWithUTF8Bytes");
    RESOLVE(string_bytes, "TWStringUTF8Bytes");
    RESOLVE(string_delete, "TWStringDelete");
    RESOLVE(wallet_create, "TWHDWalletCreateWithMnemonic");
    RESOLVE(wallet_random, "TWHDWalletCreate");
    RESOLVE(wallet_address, "TWHDWalletGetAddressForCoin");
    RESOLVE(wallet_delete, "TWHDWalletDelete");
    RESOLVE(hrp_string, "stringForHRP");
    RESOLVE(hrp_from_string, "hrpForString");
#else
    (void)argc;
    (void)argv;
#endif
    require(strcmp(stringForHRP(TWHRPBitcoin), "bc") == 0, "Wrong Bitcoin HRP string");
    require(hrpForString("bc") == TWHRPBitcoin, "Wrong Bitcoin HRP enum");
    TWString* mnemonic = TWStringCreateWithUTF8Bytes("abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about");
    TWString* passphrase = TWStringCreateWithUTF8Bytes("");
    require(mnemonic != NULL && passphrase != NULL, "String allocation failed");
    struct TWHDWallet* wallet = TWHDWalletCreateWithMnemonic(mnemonic, passphrase);
    require(wallet != NULL, "Wallet creation failed");
    TWString* address = TWHDWalletGetAddressForCoin(wallet, TWCoinTypeEthereum);
    require(address != NULL, "Address derivation failed");
    require(strcmp(TWStringUTF8Bytes(address), "0x9858EfFD232B4033E47d90003D41EC34EcaEda94") == 0, "Wrong Ethereum address");
    TWStringDelete(address);
    TWHDWalletDelete(wallet);
    wallet = TWHDWalletCreate(128, passphrase);
    require(wallet != NULL, "Windows secure RNG wallet creation failed");
    TWHDWalletDelete(wallet);
    TWStringDelete(passphrase);
    TWStringDelete(mnemonic);
#ifdef TW_DYNAMIC_LOAD
    FreeLibrary(library);
#endif
    puts("Public C ABI consumer passed");
    return 0;
}
