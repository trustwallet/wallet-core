#include "Taproot.h"
#include "Data.h"

using namespace TW;

TaprootBuilder::TaprootBuilder() {
    builder = Rust::tw_transaction_builder_create();
}

Result<void, Error> TaprootBuilder::AddP2PKHInput(const uint8_t* txid, uint32_t vout, const PublicKey& pubkey, uint64_t satoshis) {
    auto input = Rust::tw_tx_input_p2pkh_create(txid, vout, pubkey.bytes.data(), pubkey.bytes.size(), satoshis);
    if (input == nullptr) {
        return Result<void, TaprootBuilderError>::failure(TaprootBuilderError::InvalidInput);
    }

    auto updatedBuilder = Rust::tw_transaction_add_p2pkh_input(builder, input);
    if (updatedBuilder == nullptr) {
        return Result<void, TaprootBuilderError>::failure(TaprootBuilderError::InvalidOutput);
    }

    builder = updatedBuilder;
    return Result<void, TaprootBuilderError>::success();
}

Result<void, Error> TaprootBuilder::AddP2WPKHInput(const uint8_t* txid, uint32_t vout, const PublicKey& pubkey, uint64_t satoshis) {
    auto input = Rust::tw_tx_input_p2wpkh_create(txid, vout, pubkey.bytes.data(), pubkey.bytes.size(), satoshis);
    if (input == nullptr) {
        return Result<void, TaprootBuilderError>::failure(TaprootBuilderError::InvalidInput);
    }

    auto updatedBuilder = Rust::tw_transaction_add_p2wpkh_input(builder, input);
    if (updatedBuilder == nullptr) {
        return Result<void, TaprootBuilderError>::failure(TaprootBuilderError::InvalidOutput);
    }

    builder = updatedBuilder;
    return Result<void, TaprootBuilderError>::success();
}

Result<void, Error> TaprootBuilder::AddP2TRKeyPathInput(const uint8_t* txid, uint32_t vout, const PublicKey& pubkey, uint64_t satoshis) {
    auto input = Rust::tw_tx_input_p2tr_key_path_create(txid, vout, pubkey.bytes.data(), pubkey.bytes.size(), satoshis);
    if (input == nullptr) {
        return Result<void, TaprootBuilderError>::failure(TaprootBuilderError::InvalidInput);
    }

    auto updatedBuilder = Rust::tw_transaction_add_p2tr_key_path_input(builder, input);
    if (updatedBuilder == nullptr) {
        return Result<void, TaprootBuilderError>::failure(TaprootBuilderError::InvalidInput);
    }

    builder = updatedBuilder;
    return Result<void, TaprootBuilderError>::success();
}

Result<void, Error> TaprootBuilder::AddP2PKHOutput(const PublicKey& pubkey, uint64_t satoshis) {
    auto output = Rust::tw_tx_output_p2pkh_create(pubkey.bytes.data(), pubkey.bytes.size(), satoshis);
    if (output == nullptr) {
        return Result<void, TaprootBuilderError>::failure(TaprootBuilderError::InvalidOutput);
    }

    auto updatedBuilder = Rust::tw_transaction_add_p2pkh_output(builder, output);
    if (updatedBuilder == nullptr) {
        return Result<void, TaprootBuilderError>::failure(TaprootBuilderError::InvalidOutput);
    }

    builder = updatedBuilder;
    return Result<void, TaprootBuilderError>::success();
}

Result<void, Error> TaprootBuilder::AddP2WPKHOutput(const PublicKey& pubkey, uint64_t satoshis) {
    auto output = Rust::tw_tx_output_p2wpkh_create(pubkey.bytes.data(), pubkey.bytes.size(), satoshis);
    if (output == nullptr) {
        return Result<void, TaprootBuilderError>::failure(TaprootBuilderError::InvalidOutput);
    }

    auto updatedBuilder = Rust::tw_transaction_add_p2wpkh_output(builder, output);
    if (updatedBuilder == nullptr) {
        return Result<void, TaprootBuilderError>::failure(TaprootBuilderError::InvalidOutput);
    }

    builder = updatedBuilder;
    return Result<void, TaprootBuilderError>::success();
}

Result<void, Error> TaprootBuilder::AddP2TRKeyPathOutput(const PublicKey& pubkey, uint64_t satoshis) {
    auto output = Rust::tw_tx_output_p2tr_key_path_create(pubkey.bytes.data(), pubkey.bytes.size(), satoshis);
    if (output == nullptr) {
        return Result<void, TaprootBuilderError>::failure(TaprootBuilderError::InvalidOutput);
    }

    auto updatedBuilder = Rust::tw_transaction_add_p2tr_key_path_output(builder, output);
    if (updatedBuilder == nullptr) {
        return Result<void, TaprootBuilderError>::failure(TaprootBuilderError::InvalidOutput);
    }

    builder = updatedBuilder;
    return Result<void, TaprootBuilderError>::success();
}

Result<Data, Error> TaprootBuilder::Sign(const PrivateKey& privkey) {
    Rust::CByteArrayResultWrapper res = Rust::tw_transaction_sign(builder, privkey.bytes.data(), privkey.bytes.size());
    if (res.isErr()) {
        return Result<Data, TaprootBuilderError>::failure(TaprootBuilderError::FailedToSign);
    }

    return Result<Data, TaprootBuilderError>::success(res.unwrap().data);
}
