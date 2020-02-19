// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

@import Foundation;

#include "TWFoundationString.h"

TWString *_Nonnull TWStringCreateWithNSString(NSString *string) {
    return TWStringCreateWithUTF8Bytes([string cStringUsingEncoding:NSUTF8StringEncoding]);
}

NSString *_Nonnull TWStringNSString(TWString *_Nonnull string) {
    NSString *str = [[NSString alloc] initWithUTF8String:TWStringUTF8Bytes(string)];
    TWStringDelete(string);
    return str;
}
