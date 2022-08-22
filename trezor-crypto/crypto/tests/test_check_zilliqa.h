#include <TrezorCrypto/zilliqa.h>
#include <TrezorCrypto/ecdsa.h>

START_TEST(test_zil_schnorr_sign_verify) {
  static struct {
    const char *message;
    const char *priv_key;
    const char *k_hex;
    const char *s_hex;
    const char *r_hex;
  } test_cases[] = {
      {
          "123",
          "3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6",
          "669301F724C555D7BB1185C04909E9CACA3EC7A292B3A1C92DDCCD5A5A7DDDD3",
          "FFD72C290B98C93A4BCEDC0EDCDF040C35579BE962FE83E6821D4F3CB4B795D2",
          "74AAE9C3E069E2806E1B0D890970BE387AEBED8040F37991AACAD70B27895E39",
      },
      {
          "1234",
          "51a2758eed776c40b367364909c8a9c98cc969104f69ff316f7a287495c37c9b",
          "A0A1A9B3570AAE963535B8D4376C58A61646C18182C9FDDA5FB13703F88D4D1E",
          "99A0CB942C81571B77C682F79CD3CB663CE9E1C55BB425BA24B9F11A0DE84FE2",
          "C3C10363E38158BBA20556A36DE9358DFD81A31C180ABC9E7617C1CC1CAF03B3",
      },
      {
          "12345",
          "2685adffdbb4b2c515054cffc25cfcbfe2e462df65bbe82fb50f71e1e68dd285",
          "38DE7B3315F201433D271E91FBE62966576CA05CBFEC1770B77D7EC9D6A01D6D",
          "28982FA6C2B620CBC550F7EF9EAB605F409C584FBE5A765678877B79AB517086",
          "9A0788E5B0947DEDEDE386DF57A006CF3FE43919A74D9CA630F8A1A9D97B4650",
      },
      {
          "fun",
          "7457dc574d927e5dae84b05264a5b637b5a68e34a85b3965084ed6fed5b7f12d",
          "E005ABD242C7C602AB5EED080C5083C7C5F8DAEC6D046A54F384A8B8CDECF740",
          "51070ABCA039DAC294F6BA3BFC8C36CFC66020EDF66D1ACF1A9B545B0BF09F52",
          "330A924525EF722FA20E8E25CB6E8BD7DF4394886FA4414E4A0B6812AA25BBC0",
      },
      {
          "funny",
          "52c395a6d304de1a959e73e4604e32c5ad3f2bf01c8f730af426b38d7d5dd908",
          "0CF28B5C40A8830F3195BB99A9F0E2808F576105F41D16ABCF596AC5A8CFE88A",
          "3D60FB4664C994AD956378B9402BC68F7B4799D74F4783A6199C0D74865EA2B6",
          "5ED5EDEE0314DFFBEE39EE4E9C76DE8BC3EB8CB891AEC32B83957514284B205B",
      },
      {
          "What is great in man is that he is a bridge and not a goal",
          "52c395a6d304de1a959e73e4604e32c5ad3f2bf01c8f730af426b38d7d5dd908",
          "000000000000000000000000000000000000000000000000000000000000007B",
          "546F70AA1FEE3718C95508240CDC073B9FEFED05959C5319DD8E2BF07A1DD028",
          "B8667BE5E10B113608BFE5327C44E9F0462BE26F789177E10DCE53019AA33DAA",
      },
      {
          "123456789147258369qwertyuiopasdfghjklzxcvbnm,",
          "2685adffdbb4b2c515054cffc25cfcbfe2e462df65bbe82fb50f71e1e68dd285",
          "1D0CB70310C4D793A4561FE592B7C156771E3E26283B28AB588E968243B52DD0",
          "54D7A435E5E3F2811AA542F8895C20CCB760F2713DBDDB7291DAB6DA4E4F927E",
          "20A3BDABFFF2C1BF8E2AF709F6CDCAFE70DA9A1DBC22305B6332E36844092984",
      },
      {
          "11111111111111111111111111111111111111111111111111111111111111111"
          "11111111111111111111111111111111111111111111111111111111111111111"
          "111111111111111111",
          "3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6",
          "A669F372B3C2EEA351210082CAEC3B96767A7B222D19FF2EE3D814860F0D703A",
          "4890F9AC3A8D102EE3A2A473930C01CAD29DCE3860ACB7A5DADAEF16FE808991",
          "979F088E58F1814D5E462CB9F935D2924ABD8D32211D8F02DD7E0991726DF573",
      },
      {
          "qwertyuiop[]asdfghjkl;'zxcvbnm,./1234567890-=",
          "7457dc574d927e5dae84b05264a5b637b5a68e34a85b3965084ed6fed5b7f12d",
          "000000000000000000000000000000000000000000000000000000000000007C",
          "0AA595A649E517133D3448CA657424DD07BBED289030F0C0AA6738D26AB9A910",
          "83812632F1443A70B198D112D075D886BE7BBC6EC6275AE52661E52B7358BB8B",
      },
  };

  const ecdsa_curve *curve = &secp256k1;
  bignum256 k;
  uint8_t priv_key[32];
  uint8_t pub_key[33];
  uint8_t buf_raw[32];
  schnorr_sign_pair result;
  schnorr_sign_pair expected;
  int res;

  for (size_t i = 0; i < sizeof(test_cases) / sizeof(*test_cases); i++) {
    memcpy(priv_key, fromhex(test_cases[i].priv_key), 32);
    memcpy(&buf_raw, fromhex(test_cases[i].k_hex), 32);
    bn_read_be(buf_raw, &k);
    zil_schnorr_sign_k(curve, priv_key, &k, (const uint8_t *)test_cases[i].message,
                 strlen(test_cases[i].message), &result);

    memcpy(&expected.s, fromhex(test_cases[i].s_hex), 32);
    memcpy(&expected.r, fromhex(test_cases[i].r_hex), 32);

    ck_assert_mem_eq(&expected.r, &result.r, 32);
    ck_assert_mem_eq(&expected.s, &result.s, 32);

    ecdsa_get_public_key33(curve, priv_key, pub_key);
    res = zil_schnorr_verify_pair(curve, pub_key, (const uint8_t *)test_cases[i].message,
                       strlen(test_cases[i].message), &result);
    ck_assert_int_eq(res, 0);
  }
}
END_TEST

START_TEST(test_zil_schnorr_fail_verify) {
  static struct {
    const char *message;
    const char *priv_key;
    const char *k_hex;
    const char *s_hex;
    const char *r_hex;
  } test_case = {
      "123",
      "3382266517e2ebe6df51faf4bfe612236ad46fb8bd59ac982a223b045e080ac6",
      "669301F724C555D7BB1185C04909E9CACA3EC7A292B3A1C92DDCCD5A5A7DDDD3",
      "FFD72C290B98C93A4BCEDC0EDCDF040C35579BE962FE83E6821D4F3CB4B795D2",
      "74AAE9C3E069E2806E1B0D890970BE387AEBED8040F37991AACAD70B27895E39",
  };

  const ecdsa_curve *curve = &secp256k1;
  bignum256 k;
  bignum256 bn_temp;
  uint8_t priv_key[32];
  uint8_t pub_key[33];
  uint8_t buf_raw[32];
  schnorr_sign_pair result;
  schnorr_sign_pair bad_result;
  int res;

  memcpy(priv_key, fromhex(test_case.priv_key), 32);
  memcpy(&buf_raw, fromhex(test_case.k_hex), 32);
  bn_read_be(buf_raw, &k);

  zil_schnorr_sign_k(curve, priv_key, &k, (const uint8_t *)test_case.message,
               strlen(test_case.message), &result);
  
  ecdsa_get_public_key33(curve, priv_key, pub_key);

  // Test result = 0 (OK)
  res = zil_schnorr_verify_pair(curve, pub_key, (const uint8_t *)test_case.message,
                           strlen(test_case.message), &result);
  ck_assert_int_eq(res, 0);

  // Test result = 1 (empty message)
  res = zil_schnorr_verify_pair(curve, pub_key, (const uint8_t *)test_case.message, 0,
                       &result);
  ck_assert_int_eq(res, 1);

  // Test result = 2 (r = 0)
  bn_zero(&bn_temp);
  bn_write_be(&bn_temp, bad_result.r);
  memcpy(bad_result.s, result.s, 32);
  res = zil_schnorr_verify_pair(curve, pub_key, (const uint8_t *)test_case.message,
                       strlen(test_case.message), &bad_result);
  ck_assert_int_eq(res, 2);

  // Test result = 3 (s = 0)
  memcpy(bad_result.r, result.r, 32);
  bn_zero(&bn_temp);
  bn_write_be(&bn_temp, bad_result.s);
  res = zil_schnorr_verify_pair(curve, pub_key, (const uint8_t *)test_case.message,
                       strlen(test_case.message), &bad_result);
  ck_assert_int_eq(res, 3);

  // Test result = 4 (curve->order < r)
  bn_copy(&curve->order, &bn_temp);
  bn_addi(&bn_temp, 1);
  bn_write_be(&bn_temp, bad_result.r);
  memcpy(bad_result.s, result.s, 32);
  res = zil_schnorr_verify_pair(curve, pub_key, (const uint8_t *)test_case.message,
                       strlen(test_case.message), &bad_result);
  ck_assert_int_eq(res, 4);

  // Test result = 5 (curve->order < s)
  memcpy(bad_result.r, result.r, 32);
  bn_copy(&curve->order, &bn_temp);
  bn_addi(&bn_temp, 1);
  bn_write_be(&bn_temp, bad_result.s);
  res = zil_schnorr_verify_pair(curve, pub_key, (const uint8_t *)test_case.message,
                       strlen(test_case.message), &bad_result);
  ck_assert_int_eq(res, 5);

  // Test result = 6 (curve->order = r)
  bn_copy(&curve->order, &bn_temp);
  bn_write_be(&bn_temp, bad_result.r);
  memcpy(bad_result.s, result.s, 32);
  res = zil_schnorr_verify_pair(curve, pub_key, (const uint8_t *)test_case.message,
                       strlen(test_case.message), &bad_result);
  ck_assert_int_eq(res, 6);

  // Test result = 7 (curve->order = s)
  memcpy(bad_result.r, result.r, 32);
  bn_copy(&curve->order, &bn_temp);
  bn_write_be(&bn_temp, bad_result.s);
  res = zil_schnorr_verify_pair(curve, pub_key, (const uint8_t *)test_case.message,
                       strlen(test_case.message), &bad_result);
  ck_assert_int_eq(res, 7);

  // Test result = 8 (failed ecdsa_read_pubkey)
  // TBD

  // Test result = 10 (r != r')
  memcpy(bad_result.r, result.r, 32);
  memcpy(bad_result.s, result.s, 32);
  test_case.message = "12";
  res = zil_schnorr_verify_pair(curve, pub_key, (const uint8_t *)test_case.message,
                       strlen(test_case.message), &bad_result);
  ck_assert_int_eq(res, 10);
}
END_TEST
