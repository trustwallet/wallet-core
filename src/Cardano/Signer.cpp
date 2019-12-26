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

    uint64_t amount = input.amount();
    uint64_t fee = input.fee();
    // compute change -- TODO check if enough
    uint64_t changeAmount = sum_utxo - amount - fee;

    // outputs array
    Address addrTo = Address(input.to_address());
    Address addrChange = Address(input.change_address());
    auto outputsArray = Encode::indefArray();
    outputsArray.addIndefArrayElem(
        Encode::array({
            Encode::fromRaw(addrTo.getCborData()),
            Encode::uint(amount)
        })
    );
    if (changeAmount != 0) {
        outputsArray.addIndefArrayElem(
            Encode::array({
                Encode::fromRaw(addrChange.getCborData()),
                Encode::uint(changeAmount)
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

TW::Data Signer::prepareSignedTx( const Proto::SigningInput& input, TW::Data& txId_out) {
    Data unsignedTxCbor = prepareUnsignedTx(input);

    txId_out = Hash::blake2b(unsignedTxCbor, 32);

    // array with signatures
    vector<Encode> signatures;
    for (int i = 0; i < input.private_key_size(); ++i) {
        PrivateKey fromPri = PrivateKey(input.private_key(i));
        PublicKey fromPub = fromPri.getPublicKey(TWPublicKeyTypeED25519Extended);
        // sign; msg is txId with prefix
        Data txToSign = parse_hex("01"); // transaction prefix
        TW::append(txToSign, Encode::uint(Network_Mainnet_Protocol_Magic).encoded());
        TW::append(txToSign, Encode::bytes(txId_out).encoded());
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

    Data enc = Encode::array({
        Encode::fromRaw(unsignedTxCbor),
        Encode::array(signatures),
    }).encoded();
    return enc;
}
