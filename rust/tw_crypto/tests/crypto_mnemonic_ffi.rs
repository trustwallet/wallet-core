// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_crypto::ffi::crypto_mnemonic::*;
use tw_encoding::hex;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::RawPtrTrait;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;

#[test]
fn test_bip39_generate_and_check_mnemonic() {
    let strength = 128;
    let res = unsafe { tw_mnemonic_generate(strength) };
    let res = unsafe { TWString::from_ptr_as_ref(res).unwrap() };
    let mnemonic_string = res.as_str().unwrap();
    let mnemonic_words = mnemonic_string.split(" ").collect::<Vec<_>>();
    assert_eq!(mnemonic_words.len(), 12);

    let is_valid = unsafe { tw_mnemonic_is_valid(res) };
    assert!(is_valid);

    let strength = 192;
    let res = unsafe { tw_mnemonic_generate(strength) };
    let res = unsafe { TWString::from_ptr_as_ref(res).unwrap() };
    let mnemonic_string = res.as_str().unwrap();
    let mnemonic_words = mnemonic_string.split(" ").collect::<Vec<_>>();
    assert_eq!(mnemonic_words.len(), 18);

    let is_valid = unsafe { tw_mnemonic_is_valid(res) };
    assert!(is_valid);

    let strength = 256;
    let res = unsafe { tw_mnemonic_generate(strength) };
    let res = unsafe { TWString::from_ptr_as_ref(res).unwrap() };
    let mnemonic_string = res.as_str().unwrap();
    let mnemonic_words = mnemonic_string.split(" ").collect::<Vec<_>>();
    assert_eq!(mnemonic_words.len(), 24);

    let is_valid = unsafe { tw_mnemonic_is_valid(res) };
    assert!(is_valid);
}

#[test]
fn test_bip39_generate_mnemonic_from_data() {
    let test_vectors = [
        // entropy, expected mnemonic, expected seed with passphrase "TREZOR"
        ("00000000000000000000000000000000",
         "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about",
         "c55257c360c07c72029aebc1b53c05ed0362ada38ead3e3e9efa3708e53495531f09a6987599d18264c1e1c92f2cf141630c7a3c4ab7c81b2f001698e7463b04"),

        ("7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
         "legal winner thank year wave sausage worth useful legal winner thank yellow",
         "2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6fa457fe1296106559a3c80937a1c1069be3a3a5bd381ee6260e8d9739fce1f607"),

        ("80808080808080808080808080808080",
         "letter advice cage absurd amount doctor acoustic avoid letter advice cage above",
         "d71de856f81a8acc65e6fc851a38d4d7ec216fd0796d0a6827a3ad6ed5511a30fa280f12eb2e47ed2ac03b5c462a0358d18d69fe4f985ec81778c1b370b652a8"),

        ("ffffffffffffffffffffffffffffffff",
         "zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo wrong",
         "ac27495480225222079d7be181583751e86f571027b0497b5b5d11218e0a8a13332572917f0f8e5a589620c6f15b11c61dee327651a14c34e18231052e48c069"),

        ("0000000000000000000000000000000000000000000000000000000000000000",
         "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon art",
         "bda85446c68413707090a52022edd26a1c9462295029f2e60cd7c4f2bbd3097170af7a4d73245cafa9c3cca8d561a7c3de6f5d4a10be8ed2a5e608d68f92fcc8"),

        ("18ab19a9f54a9274f03e5209a2ac8a91",
         "board flee heavy tunnel powder denial science ski answer betray cargo cat",
         "6eff1bb21562918509c73cb990260db07c0ce34ff0e3cc4a8cb3276129fbcb300bddfe005831350efd633909f476c45c88253276d9fd0df6ef48609e8bb7dca8"),

        ("15da872c95a13dd738fbf50e427583ad61f18fd99f628c417a61cf8343c90419",
         "beyond stage sleep clip because twist token leaf atom beauty genius food business side grid unable middle armed observe pair crouch tonight away coconut",
         "b15509eaa2d09d3efd3e006ef42151b30367dc6e3aa5e44caba3fe4d3e352e65101fbdb86a96776b91946ff06f8eac594dc6ee1d3e82a42dfe1b40fef6bcc3fd"),
    ];

    for (entropy_hex, expected_mnemonic, expected_seed) in test_vectors.iter() {
        let entropy = hex::decode(entropy_hex).unwrap();
        let entropy_data = TWDataHelper::create(entropy);

        let mnemonic_ptr = unsafe { tw_mnemonic_generate_from_data(entropy_data.ptr()) };
        let mnemonic_data = unsafe { TWString::from_ptr_as_ref(mnemonic_ptr).unwrap() };
        let mnemonic_string = mnemonic_data.as_str().unwrap();

        assert_eq!(mnemonic_string, *expected_mnemonic);

        let is_valid = unsafe { tw_mnemonic_is_valid(mnemonic_ptr) };
        assert!(is_valid);

        let passphrase_string = TWStringHelper::create("TREZOR");

        let seed_ptr = unsafe { tw_mnemonic_to_seed(mnemonic_ptr, passphrase_string.ptr()) };
        let seed_data = unsafe { TWData::from_ptr_as_ref(seed_ptr).unwrap() };
        assert_eq!(hex::encode(seed_data.to_vec(), false), *expected_seed);
    }
}

#[test]
fn test_bip39_check_mnemonic() {
    let vectors_ok = [
        "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about",
        "legal winner thank year wave sausage worth useful legal winner thank yellow",
        "letter advice cage absurd amount doctor acoustic avoid letter advice cage above",
        "zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo wrong",
        "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon agent",
        "legal winner thank year wave sausage worth useful legal winner thank year wave sausage worth useful legal will",
        "letter advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic avoid letter always",
        "zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo when",
        "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon art",
        "legal winner thank year wave sausage worth useful legal winner thank year wave sausage worth useful legal winner thank year wave sausage worth title",
        "letter advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic bless",
        "zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo vote",
        "jelly better achieve collect unaware mountain thought cargo oxygen act hood bridge",
        "renew stay biology evidence goat welcome casual join adapt armor shuffle fault little machine walk stumble urge swap",
        "dignity pass list indicate nasty swamp pool script soccer toe leaf photo multiply desk host tomato cradle drill spread actor shine dismiss champion exotic",
        "afford alter spike radar gate glance object seek swamp infant panel yellow",
        "indicate race push merry suffer human cruise dwarf pole review arch keep canvas theme poem divorce alter left",
        "clutch control vehicle tonight unusual clog visa ice plunge glimpse recipe series open hour vintage deposit universe tip job dress radar refuse motion taste",
        "turtle front uncle idea crush write shrug there lottery flower risk shell",
        "kiss carry display unusual confirm curtain upgrade antique rotate hello void custom frequent obey nut hole price segment",
        "exile ask congress lamp submit jacket era scheme attend cousin alcohol catch course end lucky hurt sentence oven short ball bird grab wing top",
        "board flee heavy tunnel powder denial science ski answer betray cargo cat",
        "board blade invite damage undo sun mimic interest slam gaze truly inherit resist great inject rocket museum chief",
        "beyond stage sleep clip because twist token leaf atom beauty genius food business side grid unable middle armed observe pair crouch tonight away coconut",
    ];

    let vectors_fail = [
        "above abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about",
        "above winner thank year wave sausage worth useful legal winner thank yellow",
        "above advice cage absurd amount doctor acoustic avoid letter advice cage above",
        "above zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo wrong",
        "above abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon agent",
        "above winner thank year wave sausage worth useful legal winner thank year wave sausage worth useful legal will",
        "above advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic avoid letter always",
        "above zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo when",
        "above abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon art",
        "above winner thank year wave sausage worth useful legal winner thank year wave sausage worth useful legal winner thank year wave sausage worth title",
        "above advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic bless",
        "above zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo vote",
        "above better achieve collect unaware mountain thought cargo oxygen act hood bridge",
        "above stay biology evidence goat welcome casual join adapt armor shuffle fault little machine walk stumble urge swap",
        "above pass list indicate nasty swamp pool script soccer toe leaf photo multiply desk host tomato cradle drill spread actor shine dismiss champion exotic",
        "above alter spike radar gate glance object seek swamp infant panel yellow",
        "above race push merry suffer human cruise dwarf pole review arch keep canvas theme poem divorce alter left",
        "above control vehicle tonight unusual clog visa ice plunge glimpse recipe series open hour vintage deposit universe tip job dress radar refuse motion taste",
        "above front uncle idea crush write shrug there lottery flower risk shell",
        "above carry display unusual confirm curtain upgrade antique rotate hello void custom frequent obey nut hole price segment",
        "above ask congress lamp submit jacket era scheme attend cousin alcohol catch course end lucky hurt sentence oven short ball bird grab wing top",
        "above flee heavy tunnel powder denial science ski answer betray cargo cat",
        "above blade invite damage undo sun mimic interest slam gaze truly inherit resist great inject rocket museum chief",
        "above stage sleep clip because twist token leaf atom beauty genius food business side grid unable middle armed observe pair crouch tonight away coconut",
        "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about",
        "winner thank year wave sausage worth useful legal winner thank yellow",
        "advice cage absurd amount doctor acoustic avoid letter advice cage above",
        "zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo wrong",
        "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon agent",
        "winner thank year wave sausage worth useful legal winner thank year wave sausage worth useful legal will",
        "advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic avoid letter always",
        "zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo when",
        "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon art",
        "winner thank year wave sausage worth useful legal winner thank year wave sausage worth useful legal winner thank year wave sausage worth title",
        "advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic bless",
        "zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo vote",
        "better achieve collect unaware mountain thought cargo oxygen act hood bridge",
        "stay biology evidence goat welcome casual join adapt armor shuffle fault little machine walk stumble urge swap",
        "pass list indicate nasty swamp pool script soccer toe leaf photo multiply desk host tomato cradle drill spread actor shine dismiss champion exotic",
        "alter spike radar gate glance object seek swamp infant panel yellow",
        "race push merry suffer human cruise dwarf pole review arch keep canvas theme poem divorce alter left",
        "control vehicle tonight unusual clog visa ice plunge glimpse recipe series open hour vintage deposit universe tip job dress radar refuse motion taste",
        "front uncle idea crush write shrug there lottery flower risk shell",
        "carry display unusual confirm curtain upgrade antique rotate hello void custom frequent obey nut hole price segment",
        "ask congress lamp submit jacket era scheme attend cousin alcohol catch course end lucky hurt sentence oven short ball bird grab wing top",
        "flee heavy tunnel powder denial science ski answer betray cargo cat",
        "blade invite damage undo sun mimic interest slam gaze truly inherit resist great inject rocket museum chief",
        "stage sleep clip because twist token leaf atom beauty genius food business side grid unable middle armed observe pair crouch tonight away coconut",
    ];

    for mnemonic in vectors_ok {
        let mnemonic_string = TWStringHelper::create(mnemonic);
        let is_valid = unsafe { tw_mnemonic_is_valid(mnemonic_string.ptr()) };
        assert!(is_valid);
    }

    for mnemonic in vectors_fail {
        let mnemonic_string = TWStringHelper::create(mnemonic);
        let is_valid = unsafe { tw_mnemonic_is_valid(mnemonic_string.ptr()) };
        assert!(!is_valid);
    }
}

#[test]
fn test_bip39_find_word() {
    let word_string = TWStringHelper::create("aaaa");
    let index = unsafe { tw_mnemonic_find_word(word_string.ptr()) };
    assert_eq!(index, -1);

    let word_string = TWStringHelper::create("zzzz");
    let index = unsafe { tw_mnemonic_find_word(word_string.ptr()) };
    assert_eq!(index, -1);

    for i in 0..2048 {
        let word = unsafe { tw_mnemonic_get_word(i) };
        let word_string = unsafe { TWString::from_ptr_as_ref(word).unwrap() };
        let index = unsafe { tw_mnemonic_find_word(word_string) };
        assert_eq!(index, i as i32);
    }
}

#[test]
fn test_mnemonic_is_valid() {
    let valid_inputs = [
        "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal",
        // 12
        "credit expect life fade cover suit response wash pear what skull force", 
        // 15
        "rebuild park fatigue flame one clap grocery scheme upon symbol rifle flush brave feed clutch",
        // 18
        "find view amazing inject mistake school zone ticket deposit edit deer fuel expect pioneer alpha mirror joke private",
        // 21
        "tiger parent future endorse chuckle crazy seat tomato orient prevent swarm nerve duty crazy chief cruel purity team happy strategy level",
        // 24
        "admit smart swim bulk empty mystery state lyrics wrap welcome install seat supreme sunny sting roof once accuse envelope uncover arrive twice spoon squeeze", 
    ];

    for mnemonic in valid_inputs {
        let mnemonic_string = TWStringHelper::create(mnemonic);
        let is_valid = unsafe { tw_mnemonic_is_valid(mnemonic_string.ptr()) };
        assert!(is_valid, "Expected valid: {}", mnemonic);
    }

    let invalid_inputs = [
        // invalid word
        "ripple scissors hisc mammal hire column oak again sun offer wealth tomorrow",
        // invalid word
        "high culture ostrich wrist exist ignore interest hybridous exclude width more",
        // invalid checksum
        "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow",
        // invalid word count
        "credit expect life fade cover suit response wash what skull force",
        // extra space
        "  credit expect life fade   cover suit response wash pear what skull force    ",
        // upper
        "CREDIT expect life fade cover suit response wash pear what skull force",
        // back is invalid word
        "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn back",
        // Spanish
        "llanto radical atraer riesgo actuar masa fondo cielo dieta archivo sonrisa mamut",
    ];

    for mnemonic in invalid_inputs {
        let mnemonic_string = TWStringHelper::create(mnemonic);
        let is_valid = unsafe { tw_mnemonic_is_valid(mnemonic_string.ptr()) };
        assert!(!is_valid, "Expected invalid: {}", mnemonic);
    }
}

#[test]
fn test_mnemonic_is_valid_word() {
    assert!(unsafe { tw_mnemonic_is_valid_word(TWStringHelper::create("credit").ptr()) });
    assert!(unsafe { tw_mnemonic_is_valid_word(TWStringHelper::create("airport").ptr()) });
    assert!(unsafe { tw_mnemonic_is_valid_word(TWStringHelper::create("robot").ptr()) });

    assert!(!unsafe { tw_mnemonic_is_valid_word(TWStringHelper::create("hybridous").ptr()) });
    assert!(!unsafe { tw_mnemonic_is_valid_word(TWStringHelper::create("CREDIT").ptr()) });
    assert!(!unsafe { tw_mnemonic_is_valid_word(TWStringHelper::create("credit  ").ptr()) });
    assert!(!unsafe { tw_mnemonic_is_valid_word(TWStringHelper::create("back").ptr()) });
}

#[test]
fn test_mnemonic_suggest() {
    let test_cases = [
        ("air", "air airport"),
        ("AIR", "air airport"),
        ("abc", ""),
        ("ai", "aim air airport aisle"),
        (
            "an",
            "analyst anchor ancient anger angle angry animal ankle announce annual",
        ),
        (
            "a",
            "abandon ability able about above absent absorb abstract absurd abuse",
        ),
        ("str", "strategy street strike strong struggle"),
        ("rob", "robot robust"),
        ("saus", "sausage"),
        ("saos", ""),
        ("", ""),
        ("3", ""),
        (" a", ""),
        (" ", ""),
        (
            "f",
            "fabric face faculty fade faint faith fall false fame family",
        ),
        (
            "fa",
            "fabric face faculty fade faint faith fall false fame family",
        ),
        ("fam", "fame family famous"),
        ("fami", "family"),
        ("famil", "family"),
        ("family", "family"),
        (
            "p",
            "pact paddle page pair palace palm panda panel panic panther",
        ),
        (
            "pr",
            "practice praise predict prefer prepare present pretty prevent price pride",
        ),
        (
            "pro",
            "problem process produce profit program project promote proof property prosper",
        ),
        ("prog", "program"),
        ("progr", "program"),
        ("progra", "program"),
        ("program", "program"),
    ];

    for (prefix, expected) in test_cases {
        let prefix_string = TWStringHelper::create(prefix);
        let result = unsafe { tw_mnemonic_suggest(prefix_string.ptr()) };
        let result_string = unsafe { TWString::from_ptr_as_ref(result).unwrap() };
        let result_string = result_string.as_str().unwrap();
        assert_eq!(result_string, expected);
    }
}

#[test]
fn test_spanish_mnemonic() {
    let mnemonic =
        "llanto radical atraer riesgo actuar masa fondo cielo dieta archivo sonrisa mamut";
    let mnemonic_string = TWStringHelper::create(mnemonic);

    let entropy = unsafe { tw_mnemonic_to_entropy(mnemonic_string.ptr()) };
    let entropy_data = unsafe { TWData::from_ptr_as_ref(entropy).unwrap() };
    assert_eq!(hex::encode(entropy_data.to_vec(), false), "");

    let seed =
        unsafe { tw_mnemonic_to_seed(mnemonic_string.ptr(), TWStringHelper::create("").ptr()) };
    let seed_data = unsafe { TWData::from_ptr_as_ref(seed).unwrap() };
    assert_eq!(hex::encode(seed_data.to_vec(), false), "ec8f8703432fc7d32e699ee056e9d84b1435e6a64a6a40ad63dbde11eab189a276ddcec20f3326d3c6ee39cbd018585b104fc3633b801c011063ae4c318fb9b6");
}
