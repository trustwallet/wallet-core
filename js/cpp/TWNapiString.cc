// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWNapi.h"

using namespace Napi;

String TWStringJsString(Env *_Nonnull env, TWString *_Nonnull string) {
    return String::New(*env, TWStringUTF8Bytes(string));
}

TWString *_Nonnull TWStringCreateWithJsString(String string) {
    const char* chars = ((std::string) string).c_str();
    return TWStringCreateWithUTF8Bytes(chars);
}
