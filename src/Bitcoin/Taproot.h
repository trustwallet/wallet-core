#include "Data.h"
#include "Result.h"
#include "rust/Wrapper.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"

using namespace TW;

enum TransactionBuilderError {
	InvalidInput,
	InvalidOutput,
	FailedToSign,
};

using Error = TransactionBuilderError;

class TransactionBuilder {
public:
    Rust::TWTransactionBuilder* builder;

    TransactionBuilder();
    Result<void, Error> AddP2PKHInput(const uint8_t *txid, uint32_t vout, const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis);
    Result<void, Error> AddP2WPKHInput(const uint8_t *txid, uint32_t vout, const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis);
    Result<void, Error> AddP2TRKeyPathInput(const uint8_t *txid, uint32_t vout, const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis);
    Result<void, Error> AddP2PKHOutput(const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis);
    Result<void, Error> AddP2WPKHOutput(const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis);
    Result<void, Error> AddP2TRKeyPathOutput(const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis);
    Result<Data, Error> Sign(const uint8_t *secret_key, size_t secret_key_len);
};
