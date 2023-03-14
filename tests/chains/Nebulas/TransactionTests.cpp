// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Nebulas/Signer.h"
#include "HexCoding.h"
#include "Base64.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

extern std::string htmlescape(const std::string& str);

namespace TW::Nebulas::tests {

using namespace std;

TEST(NebulasTransaction, serialize) {
    auto from = Address("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
    auto to = Address("n1SAeQRVn33bamxN4ehWUT7JGdxipwn8b17");
    auto transaction = Transaction(
        /* to: */ from,
        /* nonce: */ 7,
        /* gasPrice: */ 1000000,
        /* gasLimit: */ 200000,
        /* to: */ to,
        /* amount: */ 11000000000000000000ULL,
        /* timestamp: */ 1560052938,
        /* payload: */ std::string());

    const auto privateKey = PrivateKey(parse_hex("d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b"));
    auto signer = Signer(1);
    signer.sign(privateKey, transaction);
    transaction.serializeToRaw();

    ASSERT_EQ(TW::Base64::encode(transaction.raw), "CiBQXdR2neMqnEu21q/U+OHqZHSBX9Q0hNiRfL2eCZO4hRIaGVefwtw23wEobqA40/7aIwQHghETxH4r+50aGhlXf89CeLWgHFjKu9/6tn4KNbelsMDAIIi2IhAAAAAAAAAAAJin2bgxTAAAKAcwyony5wU6CAoGYmluYXJ5QAFKEAAAAAAAAAAAAAAAAAAPQkBSEAAAAAAAAAAAAAAAAAADDUBYAWJB9T9KkUH/jkYrCUE47M2MOl14Zfnpq1CWJseEYKngsPw19+1boXlc64Gl5Gt1gKb3+0MdRP26klFTmc9qjkfnFQA=");
}

TEST(NebulasTransaction, binaryPayload) {
    auto from = Address("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
    auto to = Address("n1SAeQRVn33bamxN4ehWUT7JGdxipwn8b17");
    auto transaction = Transaction(
        /* to: */ from,
        /* nonce: */ 7,
        /* gasPrice: */ 1000000,
        /* gasLimit: */ 200000,
        /* to: */ to,
        /* amount: */ 11000000000000000000ULL,
        /* timestamp: */ 1560052938,
        /* payload: */ std::string("{\"binary\":\"test\"}"));

    const auto privateKey = PrivateKey(parse_hex("d2fd0ec9f6268fc8d1f563e3e976436936708bdf0dc60c66f35890f5967a8d2b"));
    auto signer = Signer(1);
    signer.sign(privateKey, transaction);
    ASSERT_EQ(TW::Base64::encode(transaction.raw), "CiB1Oqj7bxLQMHEoNyg/vFHmsTrGdkpTf/5qFDkYPB3bkxIaGVefwtw23wEobqA40/7aIwQHghETxH4r+50aGhlXf89CeLWgHFjKu9/6tn4KNbelsMDAIIi2IhAAAAAAAAAAAJin2bgxTAAAKAcwyony5wU6PQoGYmluYXJ5EjN7IkRhdGEiOnsiZGF0YSI6WzExNiwxMDEsMTE1LDExNl0sInR5cGUiOiJCdWZmZXIifX1AAUoQAAAAAAAAAAAAAAAAAA9CQFIQAAAAAAAAAAAAAAAAAAMNQFgBYkGHXq+JWPaEyeB19bqL3QB5jyM961WLq7PMTpnGM4iLtBjCkngjS81kgPM2TE4qKDcpzqjum/NccrZtUPQLGk0MAQ==");
}

TEST(NebulasTransaction, htmlescape) {
    // test for escaped label
    auto test = ("test&<>\x20\x28\x20\x29");
    auto result = htmlescape(test);
    ASSERT_EQ(result, "test\\u0026\\u003c\\u003e\\u2028\\u2029");
}

TEST(NebulasTransaction, serializeUnsigned) {
    auto from = Address("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
    auto to = Address("n1SAeQRVn33bamxN4ehWUT7JGdxipwn8b17");
    auto transaction = Transaction(
        /* to: */ from,
        /* nonce: */ 7,
        /* gasPrice: */ 1000000,
        /* gasLimit: */ 200000,
        /* to: */ to,
        /* amount: */ 11000000000000000000ULL,
        /* timestamp: */ 1560052938,
        /* payload: */ std::string());

    ASSERT_THROW(transaction.serializeToRaw(), std::logic_error);
}

} // namespace TW::Nebulas::tests
