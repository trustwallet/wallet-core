// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "../Cbor.h"
#include "../Data.h"
#include "../HexCoding.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"

using namespace TW;
using namespace TW::Cardano;
using namespace TW::Cbor;
using namespace std;


Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    Proto::SigningOutput output;
    try {
        output = buildTransaction(input);
        Data unisgnedEncodedCborData = prepareUnsignedTx(input, output);
        prepareSignedTx(input, unisgnedEncodedCborData, output);
    } catch (...) {
        // just return empty output
    }
    return output;
}

Proto::SigningOutput Signer::buildTransaction(const Proto::SigningInput& input) {
    if (input.fee() == 0) {
        throw logic_error("Zero fee");
    }

    Proto::SigningOutput output;
    // inputs
    uint64_t sum_utxo = 0;
    for (int i = 0; i < input.utxo_size(); ++i) {
        auto txInput = output.mutable_transaction()->add_inputs();
        txInput->mutable_previousoutput()->set_txid(input.utxo(i).out_point().txid());
        txInput->mutable_previousoutput()->set_index(input.utxo(i).out_point().index());
        sum_utxo += input.utxo(i).amount();
    }

    // compute amount, fee
    uint64_t amount = input.amount();
    if (amount > sum_utxo) {
        throw logic_error("Insufficent balance");
    }
    uint64_t fee = input.fee();
    // compute change, check if enough
    if (amount + fee > sum_utxo) {
        throw logic_error("Insufficent balance");
    }
    uint64_t changeAmount = sum_utxo - (amount + fee);
    output.set_fee(fee);

    // outputs array
    auto txOutput = output.mutable_transaction()->add_outputs();
    txOutput->set_to_address(input.to_address());
    txOutput->set_value(amount);
    if (changeAmount != 0) {
        auto txChangeOutput = output.mutable_transaction()->add_outputs();
        txChangeOutput->set_to_address(input.change_address());
        txChangeOutput->set_value(changeAmount);
    }
    return output;
}

Data Signer::prepareUnsignedTx(const Proto::SigningInput& input, const Proto::SigningOutput& output) {
    // inputs from inputs.utxo
    uint64_t sum_utxo = 0;
    auto inputsArray = Encode::indefArray();
    for (int i = 0; i < input.utxo_size(); ++i) {
        Data outPointData = Encode::array({
            Encode::bytes(parse_hex(input.utxo(i).out_point().txid())),
            Encode::uint(input.utxo(i).out_point().index())
        }).encoded();
        inputsArray.addIndefArrayElem(
            Encode::array({
                Encode::uint(0), // type
                Encode::tag(Address::PayloadTag, Encode::bytes(outPointData))
            })
        );
        sum_utxo += input.utxo(i).amount();
    }
    inputsArray.closeIndefArray();

    // outputs array from output.transaction.outputs
    const Proto::Transaction& transaction = output.transaction();
    auto outputsArray = Encode::indefArray();
    for (int i = 0; i < transaction.outputs_size(); ++i) {
        Address addr = Address(transaction.outputs(i).to_address());
        outputsArray.addIndefArrayElem(
            Encode::array({
                Encode::fromRaw(addr.getCborData()),
                Encode::uint(transaction.outputs(i).value())
            })
        );
    }
    outputsArray.closeIndefArray();

    Data enc = Encode::array({
        inputsArray,
        outputsArray,
        // attributes
        Encode::map({})
    }).encoded();
    return enc;
}

void Signer::prepareSignedTx(const Proto::SigningInput& input, const Data& unisgnedEncodedCborData, Proto::SigningOutput& output) {
    Data txId = Hash::blake2b(unisgnedEncodedCborData, 32);

    // array with signatures
    vector<Encode> signatures;
    for (int i = 0; i < input.private_key_size(); ++i) {
        PrivateKey fromPri = PrivateKey(input.private_key(i));
        PublicKey fromPub = fromPri.getPublicKey(TWPublicKeyTypeED25519Extended);
        // sign; msg is txId with prefix
        Data txToSign = parse_hex("01"); // transaction prefix
        TW::append(txToSign, Encode::uint(Network_Mainnet_Protocol_Magic).encoded());
        TW::append(txToSign, Encode::bytes(txId).encoded());
        Data signature = fromPri.sign(txToSign, TWCurveED25519Extended);
        Data signatureCbor = Encode::array({
            Encode::bytes(fromPub.bytes),
            Encode::bytes(signature),
        }).encoded();
        signatures.push_back(
            Encode::array({
                Encode::uint(0), // type
                Encode::tag(Address::PayloadTag,
                    Encode::bytes(signatureCbor)
                ),
            })
        );
    }

    Data encoded = Encode::array({
        Encode::fromRaw(unisgnedEncodedCborData),
        Encode::array(signatures),
    }).encoded();
    output.set_encoded(encoded.data(), encoded.size());
    output.set_transaction_id(hex(txId));
}
