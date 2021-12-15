// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWElrondTransactionFactory.h>

#include "Elrond/TransactionFactory.h"
#include "Data.h"

using namespace TW;
using namespace TW::Elrond;

template <typename ProtoMessage>
TWData *_Nonnull marshalProto(const ProtoMessage& message);
std::string unmarshalString(const TWString *_Nonnull value);
Address unmarshalAddress(const TWString *_Nonnull address);
uint256_t unmarshalBigUInt(const TWString *_Nonnull value);

struct TWElrondTransactionFactory *_Nonnull TWElrondTransactionFactoryCreate(struct TWElrondNetworkConfig *_Nonnull networkConfig) {
    auto factory = TransactionFactory(networkConfig->impl);
    return new TWElrondTransactionFactory{ factory };
}

void TWElrondTransactionFactoryDelete(struct TWElrondTransactionFactory *_Nonnull self) {
    assert(self != nullptr);
    delete self;
}

TWData *_Nonnull TWElrondTransactionFactoryCreateEGLDTransfer(
    struct TWElrondTransactionFactory *_Nonnull self,
    TWString *_Nonnull sender,
    TWString *_Nonnull receiver,
    TWString *_Nonnull amount
) {
    auto factory = self->impl;

    auto transaction = factory.createEGLDTransfer(
        unmarshalAddress(sender),
        unmarshalAddress(receiver),
        unmarshalBigUInt(amount)
    );

    return marshalProto(transaction);
}

TWData *_Nonnull TWElrondTransactionFactoryCreateESDTTransfer(
    struct TWElrondTransactionFactory *_Nonnull self,
    TWString *_Nonnull sender,
    TWString *_Nonnull receiver,
    TWString *_Nonnull tokenIdentifier, 
    TWString *_Nonnull amount
) {
    auto factory = self->impl;

    auto transaction = factory.createESDTTransfer(
        unmarshalAddress(sender),
        unmarshalAddress(receiver),
        unmarshalString(tokenIdentifier),
        unmarshalBigUInt(amount)
    );
    
    return marshalProto(transaction);
}

TWData *_Nonnull TWElrondTransactionFactoryCreateESDTNFTTransfer(
    struct TWElrondTransactionFactory *_Nonnull self, 
    TWString *_Nonnull sender,
    TWString *_Nonnull receiver,
    TWString *_Nonnull collection,
    uint64_t nonce,
    TWString *_Nonnull quantity
) {
    auto factory = self->impl;

    auto transaction = factory.createESDTNFTTransfer(
        unmarshalAddress(sender),
        unmarshalAddress(receiver),
        unmarshalString(collection),
        nonce,
        unmarshalBigUInt(quantity)
    );
    
    return marshalProto(transaction);
}

template <typename ProtoMessage>
TWData *_Nonnull marshalProto(const ProtoMessage& message) {
    long size = message.ByteSizeLong();
    Data raw(size);
    message.SerializeToArray(raw.data(), (int)size);
    auto result = TWDataCreateWithBytes(raw.data(), size);
    return result;
}

std::string unmarshalString(const TWString *_Nonnull value) {
    return TWStringUTF8Bytes(value);
}

Address unmarshalAddress(const TWString *_Nonnull address) {
    Address result;
    Address::decode(TWStringUTF8Bytes(address), result);
    return result;
}

uint256_t unmarshalBigUInt(const TWString *_Nonnull value) {
    return uint256_t(TWStringUTF8Bytes(value));
}
