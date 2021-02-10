START_TEST(test_bip32_nano_vector_1)
{
    const char *mnemonic = "edge defense waste choose enrich upon flee junk siren film clown finish luggage leader kid quick brick print evidence swap drill paddle truly occur";
    const char *password = "some password";

    uint8_t seed[64];
    mnemonic_to_seed(mnemonic, password, seed, NULL);

    HDNode node;
    hdnode_from_seed(seed, sizeof(seed), ED25519_BLAKE2B_NANO_NAME, &node);
    hdnode_private_ckd_prime(&node, 44);
    hdnode_private_ckd_prime(&node, 165);
    hdnode_private_ckd_prime(&node, 0);
    hdnode_fill_public_key(&node);

    ck_assert_mem_eq(node.private_key, fromhex("3be4fc2ef3f3b7374e6fc4fb6e7bb153f8a2998b3b3dab50853eabe128024143"), 32);
    ck_assert_mem_eq(node.public_key+1, fromhex("5b65b0e8173ee0802c2c3e6c9080d1a16b06de1176c938a924f58670904e82c4"), 32);
}
END_TEST

START_TEST(test_base32_nano)
{
    const char *in_hex = "0000005114aad86a390897d2a91b33b931b3a59a7df9e63eb3694f9430122f5622ae505c6ff6b58e";
    const char *out = "11111nanode8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg";

    uint8_t in[40];
    memcpy(in, fromhex(in_hex), sizeof(in));

    char buffer[96];

    ck_assert(base32_encode(in, sizeof(in), buffer, sizeof(buffer), BASE32_ALPHABET_NANO) != NULL);
    ck_assert_str_eq(buffer, out);

    ck_assert(base32_decode(out, strlen(out), (uint8_t *)buffer, sizeof(buffer), BASE32_ALPHABET_NANO) != NULL);
    ck_assert_mem_eq(buffer, in, sizeof(in));
}
END_TEST

START_TEST(test_nano_get_address)
{
    const char *prefix = "nano_";

    struct {
        const char *pk_hex;
        const char *address;
    } tests[] = {
        /* 1 */ { "5114aad86a390897d2a91b33b931b3a59a7df9e63eb3694f9430122f5622ae50",
                  "nano_1nanode8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg" },
        /* 2 */ { "ea6f44048f04bf1f7083f5ad115a9f4e62ecb61b89a83e8bc68ad1e4d9575469",
                  "nano_3tmhai4ay37z5xra9xff47fbymm4xku3q4fa9t7wf4pjwmeogo5ba8wyccku" },
        /* 3 */ { "0000000000000000000000000000000000000000000000000000000000000000",
                  "nano_1111111111111111111111111111111111111111111111111111hifc8npp" },
        /* 4 */ { "e89208dd038fbb269987689621d52292ae9c35941a7484756ecced92a65093ba",
                  "nano_3t6k35gi95xu6tergt6p69ck76ogmitsa8mnijtpxm9fkcm736xtoncuohr3" },
    };

    for (int i = 0; i < 4; i++) {
        ed25519_public_key public_key;
        memcpy(public_key, fromhex(tests[i].pk_hex), sizeof(public_key));

        char buffer[96];
        size_t count;
        count = nano_get_address(
            public_key,
            prefix, strlen(prefix),
            buffer, sizeof(buffer)
        );
        ck_assert_int_eq(count, 66);
        ck_assert_str_eq(buffer, tests[i].address);
    }
}

END_TEST
START_TEST(test_nano_validate_address)
{
    const char *prefix = "nano_";

    struct {
        const char *address;
        const char *pk_hex;
    } tests[] = {
        /* 1 */ { "nano_1nanode8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg",
                  "5114aad86a390897d2a91b33b931b3a59a7df9e63eb3694f9430122f5622ae50" },
        /* 2 */ { "xrb_1nanode8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg",
                  NULL },
        /* 3 */ { "nano_1nanode7ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg",
                  NULL },
    };

    for (int i = 0; i < 3; i++) {
        ed25519_public_key public_key;
        bool is_valid;
        is_valid = nano_validate_address(
            prefix, strlen(prefix),
            tests[i].address, strlen(tests[i].address),
            public_key
        );
        if (tests[i].pk_hex == NULL) {
            ck_assert(is_valid == false);
        } else {
            ck_assert(is_valid == true);
            ck_assert_mem_eq(public_key, fromhex(tests[i].pk_hex), sizeof(public_key));
        }
    }
}

END_TEST