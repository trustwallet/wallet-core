#include <Data.h>
#include <Taproot.h>

using namespace TW;

TransactionBuilder::TransactionBuilder() {
    builder = TW::Rust::tw_transaction_builder_create();
}

void TransactionBuilder::AddP2PKHInput(const uint8_t *txid, uint32_t vout, const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis) {
    auto input = TW::Rust::tw_tx_input_p2pkh_create(txid, vout, pubkey, pubkey_len, satoshis);
    TW::Rust::tw_transaction_add_p2pkh_input(builder, input);
}

void TransactionBuilder::AddP2WPKHInput(const uint8_t *txid, uint32_t vout, const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis) {
    auto input = TW::Rust::tw_tx_input_p2wpkh_create(txid, vout, pubkey, pubkey_len, satoshis);
    TW::Rust::tw_transaction_add_p2wpkh_input(builder, input);
}

void TransactionBuilder::AddP2TRKeyPathInput(const uint8_t *txid, uint32_t vout, const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis) {
    auto input = TW::Rust::tw_tx_input_p2tr_key_path_create(txid, vout, pubkey, pubkey_len, satoshis);
    TW::Rust::tw_transaction_add_p2tr_key_path_input(builder, input);
}

void TransactionBuilder::AddP2PKHOutput(const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis) {
    auto output = TW::Rust::tw_tx_output_p2pkh_create(pubkey, pubkey_len, satoshis);
    TW::Rust::tw_transaction_add_p2pkh_output(builder, output);
}

void TransactionBuilder::AddP2WPKHOutput(const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis) {
    auto output = TW::Rust::tw_tx_output_p2wpkh_create(pubkey, pubkey_len, satoshis);
    TW::Rust::tw_transaction_add_p2wpkh_output(builder, output);
}

void TransactionBuilder::AddP2TRKeyPathOutput(const uint8_t *pubkey, size_t pubkey_len, uint64_t satoshis) {
    auto output = TW::Rust::tw_tx_output_p2tr_key_path_create(pubkey, pubkey_len, satoshis);
    TW::Rust::tw_transaction_add_p2tr_key_path_output(builder, output);
}

TW::Rust::CByteArrayResult TransactionBuilder::Sign(const uint8_t *secret_key, size_t secret_key_len) {
    return TW::Rust::tw_transaction_sign(builder, secret_key, secret_key_len);
}
