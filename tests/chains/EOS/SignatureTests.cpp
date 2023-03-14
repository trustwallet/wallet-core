// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EOS/Transaction.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::EOS::tests {

TEST(EOSSignature, Serialization) {
    Data buf;
    Signature* sig = new Signature(parse_hex("1f14262320d5b145220fb94d8fe204117edd25e52bbe9557b6e0909dd00307af266f5be1deef001446979523ac9de32c7eae5e5be4180b5a60c0e6bf14b2dd3e05"), Type::ModernK1);
    sig->serialize(buf);

    ASSERT_EQ(
        hex(buf),
        "001f14262320d5b145220fb94d8fe204117edd25e52bbe9557b6e0909dd00307af266f5be1deef001446979523ac9de32c7eae5e5be4180b5a60c0e6bf14b2dd3e05");

    ASSERT_EQ(
        sig->string(),
        "SIG_K1_JwtfgsdSx5RuF5aejedQ7FJTexaKMrQyYosPUWUrU1mzdLx6JUgLTZJd7zWA8q8VdnXht3YmVt7jafmD2eEK7hTRpT9rY5");

    delete sig;
    sig = new Signature(parse_hex("1f5c419d16f573ddbf07d2eb959621f690f9cb856ea2d113e3af02b3b40005488410e82ffa37a079e119844d213f4eb066a640507db68851752bea6e61eb864d84"), Type::ModernR1);
    buf.clear();
    sig->serialize(buf);

    ASSERT_EQ(
        hex(buf),
        "011f5c419d16f573ddbf07d2eb959621f690f9cb856ea2d113e3af02b3b40005488410e82ffa37a079e119844d213f4eb066a640507db68851752bea6e61eb864d84");

    ASSERT_EQ(
        sig->string(),
        "SIG_R1_K7KpdLYqa6ebCP22TuiYAY9YoJh1dTWTZEVkdPzdoadFL6f8PkMYk5N8wtsF11cneEJ91XnEZP6wDJHhRyqr1fr68ouYcz");

    delete sig;
    ASSERT_THROW(sig = new Signature(parse_hex("1f5c419d16f573ddbf07d2eb959621f690f9cb856ea2d113e3af02b3b40005488410e82ffa37a079e119844d213f4eb066a640507db68851752bea6e61eb864d84"), Type::Legacy), std::invalid_argument);
    ASSERT_THROW(sig = new Signature(parse_hex("011f5c419d16f573ddbf07d2eb959621f690f9cb856ea2d113e3af02b3b40005488410e82ffa37a079e119844d213f4eb066a640507db68851752bea6e61eb864d84"), Type::ModernR1), std::invalid_argument);
}

} // namespace TW::EOS::tests