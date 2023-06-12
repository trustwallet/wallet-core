#include "Data.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Result.h"
#include "rust/Wrapper.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"

using namespace TW;

enum TaprootBuilderError {
    InvalidInput,
    InvalidOutput,
    FailedToSign,
};

using Error = TaprootBuilderError;

class TaprootBuilder {
public:
    Rust::TWTransactionBuilder* builder;

    TaprootBuilder();
    Result<void, Error> AddP2PKHInput(const uint8_t* txid, uint32_t vout, const PublicKey& pubkey, uint64_t satoshis);
    Result<void, Error> AddP2WPKHInput(const uint8_t* txid, uint32_t vout, const PublicKey& pubkey, uint64_t satoshis);
    Result<void, Error> AddP2TRKeyPathInput(const uint8_t* txid, uint32_t vout, const PublicKey& pubkey, uint64_t satoshis);
    Result<void, Error> AddP2PKHOutput(const PublicKey& pubkey, uint64_t satoshis);
    Result<void, Error> AddP2WPKHOutput(const PublicKey& pubkey, uint64_t satoshis);
    Result<void, Error> AddP2TRKeyPathOutput(const PublicKey& pubkey, uint64_t satoshis);
    Result<Data, Error> Sign(const PrivateKey& privkey);
};
