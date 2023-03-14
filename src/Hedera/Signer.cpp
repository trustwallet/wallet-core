// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "HexCoding.h"
#include "Protobuf/transaction_body.pb.h"
#include "Protobuf/transaction_contents.pb.h"
#include "../PublicKey.h"

namespace TW::Hedera::internals {
static inline proto::AccountID accountIDfromStr(const std::string& input) {
    const auto hederaAccount = Address(input);
    auto accountID = proto::AccountID();
    accountID.set_accountnum(static_cast<std::int64_t>(hederaAccount.num()));
    accountID.set_realmnum(static_cast<std::int64_t>(hederaAccount.realm()));
    accountID.set_shardnum(static_cast<std::int64_t>(hederaAccount.shard()));
    return accountID;
}

static inline proto::Timestamp timestampFromTWProto(const Proto::Timestamp& input) {
    auto timestamp = proto::Timestamp();
    timestamp.set_seconds(input.seconds());
    timestamp.set_nanos(input.nanos());
    return timestamp;
}

static inline proto::TransactionID transactionIDFromSigningInput(const Proto::SigningInput& input) {
    auto transactionID = proto::TransactionID();
    *transactionID.mutable_transactionvalidstart() = timestampFromTWProto(input.body().transactionid().transactionvalidstart());
    *transactionID.mutable_accountid() = accountIDfromStr(input.body().transactionid().accountid());
    return transactionID;
}

static inline proto::TransactionBody transactionBodyPrerequisites(const Proto::SigningInput& input) {
    auto body = proto::TransactionBody();
    body.set_memo(input.body().memo());
    body.set_transactionfee(input.body().transactionfee());
    *body.mutable_nodeaccountid() = accountIDfromStr(input.body().nodeaccountid());
    body.mutable_transactionvalidduration()->set_seconds(input.body().transactionvalidduration());
    *body.mutable_transactionid() = transactionIDFromSigningInput(input);
    return body;
}

static inline proto::TransferList transferListFromInput(const Proto::SigningInput& input) {
    auto transferList = proto::TransferList();
    auto fromAccountID = accountIDfromStr(input.body().transfer().from());
    auto amount = input.body().transfer().amount();
    auto* to = transferList.add_accountamounts();
    to->set_amount(amount);
    *to->mutable_accountid() = accountIDfromStr(input.body().transfer().to());
    auto* from = transferList.add_accountamounts();
    from->set_amount(-amount);
    *from->mutable_accountid() = fromAccountID;
    return transferList;
}

static inline proto::CryptoTransferTransactionBody cryptoTransferFromInput(const Proto::SigningInput& input) {
    auto transferList = transferListFromInput(input);
    auto cryptoTransfer = proto::CryptoTransferTransactionBody();
    *cryptoTransfer.mutable_transfers() = transferList;
    return cryptoTransfer;
}

static inline Proto::SigningOutput sign(const proto::TransactionBody& body, const PrivateKey& privateKey) {
    auto protoOutput = Proto::SigningOutput();
    Data encoded;
    auto encodedBody = data(body.SerializeAsString());
    auto signedBody = privateKey.sign(encodedBody, TWCurveED25519);
    auto sigMap = proto::SignatureMap();
    auto* sigPair = sigMap.add_sigpair();
    sigPair->set_ed25519(signedBody.data(), signedBody.size());
    auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes;
    sigPair->set_pubkeyprefix(pubKey.data(), pubKey.size());
    auto signedTx = proto::SignedTransaction();
    signedTx.set_bodybytes(encodedBody.data(), encodedBody.size());
    *signedTx.mutable_sigmap() = sigMap;
    encoded = data(signedTx.SerializeAsString());
    protoOutput.set_encoded(encoded.data(), encoded.size());
    return protoOutput;
}

} // namespace TW::Hedera::internals

namespace TW::Hedera {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto body = internals::transactionBodyPrerequisites(input);

    switch (input.body().data_case()) {
    case Proto::TransactionBody::kTransfer: {
        *body.mutable_cryptotransfer() = internals::cryptoTransferFromInput(input);
        break;
    }
    case Proto::TransactionBody::DATA_NOT_SET:
        break;
    default:
        break;
    }
    return internals::sign(body, privateKey);
}

} // namespace TW::Hedera
