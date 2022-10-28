// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "../PublicKey.h"
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
        auto body = proto::TransactionBody();
        auto amount = input.body().transfer().amount();
        auto fromAccount = Address(input.body().transfer().from());
        auto toAccount = Address(input.body().transfer().to());
        auto* transferMsg = body.mutable_cryptotransfer();

        // from
        auto* from = transferMsg->mutable_transfers()->add_accountamounts();
        from->set_amount(-amount);
        auto* fromAccountId = from->mutable_accountid();
        fromAccountId->set_shardnum(static_cast<int64_t>(fromAccount.shard()));
        fromAccountId->set_realmnum(static_cast<int64_t>(fromAccount.realm()));
        fromAccountId->set_accountnum(static_cast<int64_t>(fromAccount.num()));

        // to
        auto* to = transferMsg->mutable_transfers()->add_accountamounts();
        to->set_amount(amount);
        auto* toAccountId = from->mutable_accountid();
        toAccountId->set_shardnum(static_cast<int64_t>(toAccount.shard()));
        toAccountId->set_realmnum(static_cast<int64_t>(toAccount.realm()));
        toAccountId->set_accountnum(static_cast<int64_t>(toAccount.num()));

        *body.mutable_memo() = input.body().memo();
        auto duration = proto::Duration();
        duration.set_seconds(input.body().transactionvalidduration());
        *body.mutable_transactionvalidduration() = duration;
        body.set_transactionfee(input.body().transactionfee());

        auto transactionId = proto::TransactionID();
        auto timestamp = proto::Timestamp();
        auto transactionAccountID = proto::AccountID();
        auto addressTransactionAccountId = Address(input.body().transactionid().accountid());

        transactionAccountID.set_accountnum(static_cast<int64_t>(addressTransactionAccountId.num()));
        transactionAccountID.set_realmnum(static_cast<int64_t>(addressTransactionAccountId.realm()));
        transactionAccountID.set_shardnum(static_cast<int64_t>(addressTransactionAccountId.shard()));
        timestamp.set_seconds(input.body().transactionid().transactionvalidstart().seconds());
        timestamp.set_nanos(input.body().transactionid().transactionvalidstart().nanos());
        *transactionId.mutable_transactionvalidstart() = timestamp;
        *transactionId.mutable_accountid() = transactionAccountID;

        auto encodedBody = data(body.SerializeAsString());
        auto signedBody = privateKey.sign(encodedBody, TWCurveED25519);
        auto sigMap = proto::SignatureMap();
        auto *sigPair = sigMap.add_sigpair();
        sigPair->set_ed25519(signedBody.data(), signedBody.size());
        auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
        sigPair->set_pubkeyprefix(pubKey.bytes.data(), pubKey.bytes.size());
        auto signedTx = proto::SignedTransaction();
        signedTx.set_bodybytes(encodedBody.data(), encodedBody.size());
        *sigMap.add_sigpair() = *sigPair;
        *signedTx.mutable_sigmap() = sigMap;
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
