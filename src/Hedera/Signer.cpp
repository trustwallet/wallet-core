// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "HexCoding.h"
#include "../PublicKey.h"
#include "google/protobuf/util/json_util.h"
#include "Protobuf/transaction_body.pb.h"
#include "Protobuf/transaction_contents.pb.h"

namespace TW::Hedera {

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    auto protoOutput = Proto::SigningOutput();
    Data encoded;
    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));

    switch (input.body().data_case()) {

    case Proto::TransactionBody::kTransfer: {
        // TODO: refactor with auto conversion operator etc
        auto amount = input.body().transfer().amount();
        auto fromAccount = Address(input.body().transfer().from());
        auto toAccount = Address(input.body().transfer().to());
        auto nodeAccount = Address(input.body().nodeaccountid());
        auto body = proto::TransactionBody();
        {
            auto transferList = proto::TransferList();
            {
                auto* to = transferList.add_accountamounts();
                to->set_amount(amount);
                auto* accountIdTo = to->mutable_accountid();
                accountIdTo->set_shardnum(0);
                accountIdTo->set_realmnum(0);
                accountIdTo->set_accountnum(toAccount.num());
            }

            {
                auto* from = transferList.add_accountamounts();
                from->set_amount(-amount);
                auto* accountIdFrom = from->mutable_accountid();
                accountIdFrom->set_shardnum(0);
                accountIdFrom->set_realmnum(0);
                accountIdFrom->set_accountnum(fromAccount.num());
            }

            auto cryptoTransfer = proto::CryptoTransferTransactionBody();
            *cryptoTransfer.mutable_transfers() = transferList;
            *body.mutable_cryptotransfer() = cryptoTransfer;
            body.set_memo(input.body().memo());
            body.set_transactionfee(input.body().transactionfee());
            body.mutable_nodeaccountid()->set_accountnum(nodeAccount.num());
            body.mutable_transactionvalidduration()->set_seconds(input.body().transactionvalidduration());
            auto& transactionID = *body.mutable_transactionid();
            transactionID.mutable_accountid()->set_accountnum(fromAccount.num());
            transactionID.mutable_transactionvalidstart()->set_nanos(input.body().transactionid().transactionvalidstart().nanos());
            transactionID.mutable_transactionvalidstart()->set_seconds(input.body().transactionid().transactionvalidstart().seconds());
        }
        auto encodedBody = data(body.SerializeAsString());
        auto signedBody = privateKey.sign(encodedBody, TWCurveED25519);
        auto sigMap = proto::SignatureMap();
        auto *sigPair = sigMap.add_sigpair();
        sigPair->set_ed25519(signedBody.data(), signedBody.size());
        auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes;
        sigPair->set_pubkeyprefix(pubKey.data(), pubKey.size());
        auto signedTx = proto::SignedTransaction();
        signedTx.set_bodybytes(encodedBody.data(), encodedBody.size());
        *sigMap.add_sigpair() = *sigPair;
        *signedTx.mutable_sigmap() = sigMap;
        // TODO: delete print
        std::cout << "Encoded Body: " << hex(encodedBody) << std::endl;
        std::cout << "Encoded Signed Body: " << hex(signedBody) << std::endl;
        std::cout << "Signed Tx Body: " << hex(signedTx.SerializeAsString()) << std::endl;
        encoded = data(signedTx.SerializeAsString());
        protoOutput.set_encoded(encoded.data(), encoded.size());
        break;
    }
    case Proto::TransactionBody::DATA_NOT_SET:
        break;
    }
    return protoOutput;
}

} // namespace TW::Hedera
