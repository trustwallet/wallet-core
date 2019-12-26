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
using namespace std;


Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) {
    Data txId;
    Data signedTxEncoded = prepareSignedTx(input, txId);
    Proto::SigningOutput output;
    //output.set_transaction // TODO
    output.set_encoded(signedTxEncoded.data(), signedTxEncoded.size());
    output.set_transaction_id(hex(txId));
    output.set_fee(input.fee());
    return output;
}

Data Signer::prepareUnsignedTx(const Proto::SigningInput& input) {
    // inputs array
    uint64_t sum_utxo = 0;
    auto inputsArray = Cbor::Encode::indefArray();
    for (int i = 0; i < input.utxo_size(); ++i) {
        Data outPointData = Cbor::Encode::array({
            Cbor::Encode::bytes(parse_hex(input.utxo(i).out_point().txid())),
            Cbor::Encode::uint(input.utxo(i).out_point().index())
        }).encoded();
        inputsArray.addIndefArrayElem(
            Cbor::Encode::array({
                Cbor::Encode::uint(0), // type
                Cbor::Encode::tag(Address::PayloadTag, Cbor::Encode::bytes(outPointData))
            })
        );
        sum_utxo += input.utxo(i).amount();
    }
    inputsArray.closeIndefArray();

    uint64_t amount = input.amount();
    uint64_t fee = input.fee();
    // compute change -- TODO check if enough
    uint64_t changeAmount = sum_utxo - amount - fee;

    Address addrTo = Address(input.to_address());
    Address addrChange = Address(input.change_address());
    Data enc = Cbor::Encode::array({
        // inputs array
        inputsArray,
        // outputs array
        Cbor::Encode::indefArray()
            .addIndefArrayElem(
                Cbor::Encode::array({
                    Cbor::Encode::fromRaw(addrTo.getCborData()),
                    Cbor::Encode::uint(amount)
                })
            )
            .addIndefArrayElem(
                Cbor::Encode::array({
                    Cbor::Encode::fromRaw(addrChange.getCborData()),
                    Cbor::Encode::uint(changeAmount)
                })
            )
        .closeIndefArray(),
        // attributes
        Cbor::Encode::map({})
    }).encoded();
    return enc;
}

TW::Data Signer::prepareSignedTx( const Proto::SigningInput& input, TW::Data& txId_out) {
    Data unsignedTxCbor = prepareUnsignedTx(input);

    txId_out = Hash::blake2b(unsignedTxCbor, 32);

    // array with signatures
    vector<Cbor::Encode> signatures;
    for (int i = 0; i < input.private_key_size(); ++i) {
        PrivateKey fromPri = PrivateKey(input.private_key(i));
        PublicKey fromPub = fromPri.getPublicKey(TWPublicKeyTypeED25519Extended);
        // sign; msg is txId with prefix
        Data txToSign = parse_hex("01"); // transaction prefix
        TW::append(txToSign, Cbor::Encode::uint(Network_Mainnet_Protocol_Magic).encoded());
        TW::append(txToSign, Cbor::Encode::bytes(txId_out).encoded());
        Data signature = fromPri.sign(txToSign, TWCurveED25519Extended);
        Data signatureCbor = Cbor::Encode::array({
            Cbor::Encode::bytes(fromPub.bytes),
            Cbor::Encode::bytes(signature),
        }).encoded();
        signatures.push_back(
            Cbor::Encode::array({
                Cbor::Encode::uint(0), // type
                Cbor::Encode::tag(Address::PayloadTag,
                    Cbor::Encode::bytes(signatureCbor)
                ),
            })
        );
    }

    Data enc = Cbor::Encode::array({
        Cbor::Encode::fromRaw(unsignedTxCbor),
        Cbor::Encode::array(signatures),
    }).encoded();
    return enc;
}
