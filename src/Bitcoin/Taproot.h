#include "Data.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"

using namespace TW;

class TransactionBuilder {
public:
    Rust::TWTransactionBuilder* builder;

    TransactionBuilder();
    void AddP2PKHInput(const uint8_t *txid, uint32_t vout, const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis);
    void AddP2WPKHInput(const uint8_t *txid, uint32_t vout, const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis);
    void AddP2TRKeyPathInput(const uint8_t *txid, uint32_t vout, const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis);
    void AddP2PKHOutput(const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis);
    void AddP2WPKHOutput(const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis);
    void AddP2TRKeyPathOutput(const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis);
    Rust::CByteArrayResult Sign(const uint8_t *secret_key, size_t secret_key_len);
};
