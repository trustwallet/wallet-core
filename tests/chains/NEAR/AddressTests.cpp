// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "NEAR/Address.h"
#include "Base58.h"
#include "PrivateKey.h"
#include <TrustWalletCore/TWPublicKeyType.h>

#include <gtest/gtest.h>

namespace TW::NEAR::tests {

TEST(NEARAddress, Validation) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF"));
    ASSERT_FALSE(Address::isValid("EOS65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjT"));
    ASSERT_FALSE(Address::isValid("NEAR2V8xUQn8gLZUd9ofzNbDz5ahVJwaQNHKoDSbysPA6GYfsJS4hbDPTSFzfrRKs8UsSkakdoC623ZYbfAJKYRbx4UJHjPoi9SdF7nh6ZM3ditbDYaXC9aan8VEtrEicpXsgnYdD91qEPfUL9jd1j7Uipz1jHR1SG3mTMHsZbE1JQyYJBB6sGRQ6GsLe5Yks5tuYuEU88cpu6rk8iHEL6hzXNhWSbg6MJLoaA6FdEddnBZjLpqM5fZahaLpL9HTPo8PpsCgoRKDEifakbHPRg3NSQnGR4vV3oonz1bisVL5gF1yfZTsH9VYTMJ6CiNqZ7Dk2fPu1WFn3Sp91e34ysyuddqTHeypsAgVtWBNP7bTTvomxVR9xwEH1bBZ83oqwnnyXwMNqi1aHGouyneYPNU363cjEiKHz2mXYBs3NFX5kB9NPqPBETGptFh71EHrbB1XQT2WTqCEeKmQ8RR28E82Ei2GsxSbVPLP8TDTk7XpHy5UtbHf1CS5bY8SkiSUfFPJnNabx2FeLtHTTdxWmxPRa2dYgFANQnjt6g4wKnGAorbaqeu3jHuUvbNeUuhCNuwWe176fhisoW4gxz2VrK2w4qNeMrgWkWt4N1PPGyfuF3bWigi8Kdek9c3HGCk1vuTs1okartuuibCyzahPDnGLYBLdSEVU4endvxBsMmfXSrhcXpxXtxMPih88HjGR69MMWrjYPrVJvhvbU2AYejpLE1QqCpbJ4pNesUet9zddS1QHE23TMmY1Gk2UhQV6SocFqeAgf1Wu17RjnZKF4p9MQNjXM2oaZvWL7ZEf46ZT9uizKPSrGfP5rqJ1JyYA7Mj7KsfYspf5HuFhLynfybQSwerqLKJk8s4EGA8KkeekGJTspBGnxtgqQThPmqfy9hKqP5JrhEcBC68nhC7kokhC8qgdERUL2LzAtguQG1ZrfhcQDdhePExiFa9QKNuzXzPfzuTwBAByrNSW54nH5wkWXUKuZ1fmV7XrAEzNJK2ozRgbxsnxBdqmkZfpyzzC5zGaH4Eisw4e8o7jYmtK8udp4vrxxQMzsHkKa9Xpgn2tKmyfMQPk9afSrV9GJK7HoMwWPBMaPEm4DgBRWYksSNgQQuLQm4SeLzmeyZpGycida5MfnTyB8jH8jMPEYTonxE7bgFwNDBkZxwDa4FNAkBhbKELp3inYsvHWJ18QG7XbLNa9F74Xug3wFzoXC7Z34xv8rnW2Quj9CPfvkMwz3qF3E1XJnm3Adamg799yEjahcbR1bKHSVLTkThoNMkF2z8D1XswB7ZiMqV7TCPVYu94GrWxbo6Mr8Jhs76eH9EvXapqCdMEiC62zycDWvhruF44f4F9E1WVxpdXQAbhCXtwqHzSJV18SFGjUmF91AU9oevKP5EW82jY7JP2"));
    ASSERT_FALSE(Address::isValid("NEAR5y2"));
    ASSERT_FALSE(Address::isValid("NEAR2fk7ax"));
    ASSERT_FALSE(Address::isValid("NEAR2758Nk7CMUcxTwXdjVdSxNEidiZQWMZN3USJzj76q5ia3v2v"));
    ASSERT_FALSE(Address::isValid("NEAR2758Nk7CMUcxTwXdjVdSxNEidiZQWMZN3USJzj76q5ia3v2v2v3"));
    ASSERT_FALSE(Address::isValid("917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb7786"));
    ASSERT_FALSE(Address::isValid("917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d1f"));

    ASSERT_TRUE(Address::isValid("NEAR2758Nk7CMUcxTwXdjVdSxNEidiZQWMZN3USJzj76q5ia3v2v2v"));
    ASSERT_TRUE(Address::isValid("917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d"));
}

TEST(NEARAddress, FromString) {
    ASSERT_EQ(
        Address("NEAR2758Nk7CMUcxTwXdjVdSxNEidiZQWMZN3USJzj76q5ia3v2v2v").string(),
        "917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d");
    ASSERT_EQ(
        Address("9685af3fe2dc231e5069ccff8ec6950eb961d42ebb9116a8ab9c0d38f9e45249").string(),
        "9685af3fe2dc231e5069ccff8ec6950eb961d42ebb9116a8ab9c0d38f9e45249");
}

TEST(NEARAddress, FromPrivateKey) {
    auto fullKey = Base58::decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    auto key = PrivateKey(Data(fullKey.begin(), fullKey.begin() + 32));
    auto publicKey = key.getPublicKey(TWPublicKeyTypeED25519);
    auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d");
}

} // namespace TW::NEAR::tests
