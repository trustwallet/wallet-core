// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWFIOSigner.h>

#include "../FIO/TransactionBuilder.h"
//#include "../FIO/Signer.h"

#include "../Data.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"
#include "../proto/FIO.pb.h"
//#include "../proto/Common.pb.h"

#include <string>

using namespace TW;
using namespace TW::FIO;
using namespace std;


/// Internal helper
ChainParams getChainParams(const FIO::Proto::SigningInput& input) {
    return ChainParams{
        TW::data(input.chain_params().chain_id()),
        input.chain_params().head_block_number(),
        input.chain_params().ref_block_prefix()
    };
}

TW_FIO_Proto_SigningOutput TWFIOSignerSign(TW_FIO_Proto_SigningInput input) {
    FIO::Proto::SigningOutput out;
    try
    {    
        FIO::Proto::SigningInput in;
        if (!in.ParseFromArray(TWDataBytes(input), static_cast<int>(TWDataSize(input)))) {
            out.set_error("Error: could not parse input");
        } else {
            PrivateKey privateKey(in.private_key());
            PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
            Address owner(publicKey);
            
            string json;
            if (in.action().has_register_fio_address_message()) {
                const auto action = in.action().register_fio_address_message();
                json = TransactionBuilder::createRegisterFioAddress(owner, privateKey, 
                    in.action().register_fio_address_message().fio_address(), owner.string(),
                    getChainParams(in), action.fee(), action.tpid(), in.expiry());
            } else if (in.action().has_add_pub_address_message()) {
                const auto action = in.action().add_pub_address_message();
                // process addresses
                std::vector<std::pair<std::string, std::string>> addresses;
                for (int i = 0; i < action.public_addresses_size(); ++i) {
                    addresses.push_back(std::make_pair(action.public_addresses(i).token_code(), action.public_addresses(i).address()));
                }
                json = TransactionBuilder::createAddPubAddress(owner, privateKey,
                    action.fio_address(), addresses, 
                    getChainParams(in), action.fee(), action.tpid(), in.expiry());
            } else if (in.action().has_transfer_message()) {
                const auto action = in.action().transfer_message();
                json = TransactionBuilder::createTransfer(owner, privateKey,
                    action.payee_public_key(), action.amount(),
                    getChainParams(in), action.fee(), action.tpid(), in.expiry());
            }

            out.set_json(json);
        }
    }
    catch(const std::exception& e)
    {
        out.set_error("Internal error");
    }
    auto serialized = out.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()), serialized.size());
}
