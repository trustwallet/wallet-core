// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

// Taken from https://github.com/rust-bitcoin/rust-bip39/blob/master/src/lib.rs

use rand_core::RngCore;
use tw_hash::sha2::sha256;
use unicode_normalization::UnicodeNormalization;
use zeroize::{Zeroize, ZeroizeOnDrop, Zeroizing};

use super::{error::Error, language::Language};

const MIN_NB_WORDS: usize = 12;
const MAX_NB_WORDS: usize = 24;
const EOF: u16 = u16::MAX;

const SUGGEST_MAX_COUNT: usize = 10;
const PBKDF2_ROUNDS: u32 = 2048;
const SEED_SIZE: usize = 64;

fn is_invalid_word_count(word_count: usize) -> bool {
    word_count < MIN_NB_WORDS || !word_count.is_multiple_of(3) || word_count > MAX_NB_WORDS
}

#[derive(PartialEq, Eq, Debug, Zeroize, ZeroizeOnDrop)]
pub struct Mnemonic {
    /// The indices of the words.
    /// Mnemonics with less than the max nb of words are terminated with EOF.
    words: [u16; MAX_NB_WORDS],
}

impl Mnemonic {
    /// Create a new [Mnemonic] in the specified language from the given entropy.
    /// Entropy must be a multiple of 32 bits (4 bytes) and 128-256 bits in length.
    fn from_entropy_in(entropy: &[u8]) -> Result<Mnemonic, Error> {
        const MAX_ENTROPY_BITS: usize = 256;
        const MIN_ENTROPY_BITS: usize = 128;
        const MAX_CHECKSUM_BITS: usize = 8;

        let nb_bytes = entropy.len();
        let nb_bits = nb_bytes * 8;

        if !nb_bits.is_multiple_of(32) {
            return Err(Error::BadEntropyBitCount(nb_bits));
        }
        if !(MIN_ENTROPY_BITS..=MAX_ENTROPY_BITS).contains(&nb_bits) {
            return Err(Error::BadEntropyBitCount(nb_bits));
        }

        let check = sha256(entropy);
        let mut bits = [false; MAX_ENTROPY_BITS + MAX_CHECKSUM_BITS];
        for i in 0..nb_bytes {
            for j in 0..8 {
                bits[i * 8 + j] = (entropy[i] & (1 << (7 - j))) > 0;
            }
        }
        for i in 0..nb_bytes / 4 {
            bits[8 * nb_bytes + i] = (check[i / 8] & (1 << (7 - (i % 8)))) > 0;
        }

        let mut words = [EOF; MAX_NB_WORDS];
        let nb_words = nb_bytes * 3 / 4;
        for i in 0..nb_words {
            let mut idx = 0;
            // Constant-time bit accumulation to avoid conditional branching
            for j in 0..11 {
                // Convert bool to u16: true -> 1, false -> 0
                let bit = bits[i * 11 + j] as u16;
                // Always perform the addition, but only add if bit is 1
                idx += bit << (10 - j);
            }
            words[i] = idx;
        }

        bits.zeroize();

        Ok(Mnemonic { words })
    }

    fn generate_in_with(rng: &mut impl RngCore, word_count: usize) -> Result<Mnemonic, Error> {
        if is_invalid_word_count(word_count) {
            return Err(Error::BadWordCount(word_count));
        }

        let entropy_bytes = (word_count / 3) * 4;
        let mut entropy = [0u8; (MAX_NB_WORDS / 3) * 4];
        RngCore::try_fill_bytes(rng, &mut entropy[0..entropy_bytes])
            .map_err(|_err| Error::RandGeneratorError)?;
        let mnemonic = Mnemonic::from_entropy_in(&entropy[0..entropy_bytes])?;
        entropy.zeroize();
        Ok(mnemonic)
    }

    pub fn generate(strength: u32) -> Result<Self, Error> {
        if !strength.is_multiple_of(32) || !(128..=256).contains(&strength) {
            return Err(Error::BadEntropyBitCount(strength as usize));
        }

        let length = strength / 8;
        let mnemonic_length = length * 3 / 4;

        Self::generate_in_with(&mut rand::thread_rng(), mnemonic_length as usize)
    }

    pub fn generate_from_data(data: &[u8]) -> Result<Self, Error> {
        Self::from_entropy_in(data)
    }

    /// Parse a mnemonic in normalized UTF8 in the given language.
    fn parse_in_normalized(s: &str) -> Result<Mnemonic, Error> {
        let nb_words = s.split_whitespace().count();
        if is_invalid_word_count(nb_words) {
            return Err(Error::BadWordCount(nb_words));
        }

        // Here we will store the eventual words.
        let mut words = [EOF; MAX_NB_WORDS];

        // And here we keep track of the bits to calculate and validate the checksum.
        // We only use `nb_words * 11` elements in this array.
        let mut bits = [false; MAX_NB_WORDS * 11];

        for (i, word) in s.split_whitespace().enumerate() {
            let idx = Language::English
                .find_word(word)
                .ok_or(Error::UnknownWord(i))?;

            words[i] = idx;

            for j in 0..11 {
                bits[i * 11 + j] = ((idx >> (10 - j)) & 1) == 1;
            }
        }

        // Verify the checksum.
        // We only use `nb_words / 3 * 4` elements in this array.
        let mut entropy = [0u8; MAX_NB_WORDS / 3 * 4];
        let nb_bytes_entropy = nb_words / 3 * 4;
        for i in 0..nb_bytes_entropy {
            for j in 0..8 {
                // Constant-time bit accumulation to avoid conditional branching
                entropy[i] += (bits[i * 8 + j] as u8) << (7 - j);
            }
        }

        let check = sha256(&entropy[0..nb_bytes_entropy]);
        for i in 0..nb_bytes_entropy / 4 {
            if bits[8 * nb_bytes_entropy + i] != ((check[i / 8] & (1 << (7 - (i % 8)))) > 0) {
                return Err(Error::InvalidChecksum);
            }
        }

        bits.zeroize();
        entropy.zeroize();

        Ok(Mnemonic { words })
    }

    pub fn parse(mnemonic: &str) -> Result<Mnemonic, Error> {
        if mnemonic.trim() != mnemonic || mnemonic.contains("  ") {
            return Err(Error::InvalidChecksum);
        }
        let mnemonic = Zeroizing::new(mnemonic.nfkd().collect::<String>());
        Mnemonic::parse_in_normalized(mnemonic.as_ref())
    }

    pub fn is_valid(mnemonic: &str) -> bool {
        Self::parse(mnemonic).is_ok()
    }

    pub fn is_valid_word(word: &str) -> bool {
        let language = Language::English;
        language.find_word(word).is_some()
    }

    pub fn get_word(index: u32) -> Option<String> {
        let language = Language::English;
        language
            .word_list()
            .get(index as usize)
            .map(|w| w.to_string())
    }

    pub fn find_word(word: &str) -> Option<u32> {
        let language = Language::English;
        language.find_word(word).map(|index| index as u32)
    }

    pub fn suggest(prefix: &str) -> String {
        if prefix.is_empty() {
            return "".to_string();
        }
        let language = Language::English;
        let prefix_string = prefix.to_lowercase();
        let words = language.words_by_prefix(&prefix_string);
        let words_string = words
            .iter()
            .take(SUGGEST_MAX_COUNT)
            .map(|w| w.to_string())
            .collect::<Vec<_>>()
            .join(" ");
        words_string
    }

    // Taken from https://github.com/iqlusioninc/crates/blob/95c6b87ce657dc51a0bd11159ef39c603a197f8d/bip32/src/mnemonic/phrase.rs#L134
    pub fn to_seed(mnemonic: &str, passphrase: &str) -> Zeroizing<[u8; SEED_SIZE]> {
        let mnemonic = Zeroizing::new(mnemonic.nfkd().collect::<String>());
        let passphrase = Zeroizing::new(passphrase.nfkd().collect::<String>());
        let mut seed = [0u8; SEED_SIZE];
        let salt = Zeroizing::new(format!("mnemonic{}", passphrase.as_str()));
        pbkdf2::pbkdf2_hmac::<sha2::Sha512>(
            mnemonic.as_bytes(),
            salt.as_bytes(),
            PBKDF2_ROUNDS,
            &mut seed,
        );
        Zeroizing::new(seed)
    }

    fn word_indices(&self) -> impl Iterator<Item = usize> + Clone + '_ {
        self.words
            .iter()
            .take_while(|&&w| w != EOF)
            .map(|w| *w as usize)
    }

    pub fn words(&self) -> impl Iterator<Item = &'static str> + Clone + '_ {
        let list = Language::English.word_list();
        self.word_indices().map(move |i| list[i])
    }

    pub fn to_entropy(&self) -> Vec<u8> {
        // We unwrap errors here because this method can only be called on
        // values that were already previously validated.

        let language = Language::English;

        // Preallocate enough space for the longest possible word list
        let mut entropy = [0; 33];
        let mut cursor = 0;
        let mut offset = 0;
        let mut remainder = 0;

        let nb_words = self.word_indices().count();
        for word in self.words() {
            let idx = language.find_word(word).expect("invalid mnemonic");

            remainder |= ((idx as u32) << (32 - 11)) >> offset;
            offset += 11;

            while offset >= 8 {
                entropy[cursor] = (remainder >> 24) as u8;
                cursor += 1;
                remainder <<= 8;
                offset -= 8;
            }
        }

        if offset != 0 {
            entropy[cursor] = (remainder >> 24) as u8;
        }

        let entropy_bytes = (nb_words / 3) * 4;
        entropy[0..entropy_bytes].to_vec()
    }

    pub fn to_string_zeroizing(&self) -> Zeroizing<String> {
        let mut string = String::new();
        for (i, word) in self.words().enumerate() {
            if i > 0 {
                string.push(' ');
            }
            string.push_str(word);
        }
        Zeroizing::new(string)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex;

    #[test]
    fn test_generate() {
        let _ = Mnemonic::generate_in_with(&mut rand::thread_rng(), 24).unwrap();
    }

    #[test]
    fn test_generate_word_counts() {
        for word_count in [12, 15, 18, 21, 24].iter() {
            let _ = Mnemonic::generate_in_with(&mut rand::thread_rng(), *word_count).unwrap();
        }
    }

    #[test]
    fn test_vectors_english() {
        // These vectors are tuples of
        // (entropy, mnemonic, seed)
        let test_vectors = [
			(
				"00000000000000000000000000000000",
				"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about",
				"c55257c360c07c72029aebc1b53c05ed0362ada38ead3e3e9efa3708e53495531f09a6987599d18264c1e1c92f2cf141630c7a3c4ab7c81b2f001698e7463b04",
			),
			(
				"7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
				"legal winner thank year wave sausage worth useful legal winner thank yellow",
				"2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6fa457fe1296106559a3c80937a1c1069be3a3a5bd381ee6260e8d9739fce1f607",
			),
			(
				"80808080808080808080808080808080",
				"letter advice cage absurd amount doctor acoustic avoid letter advice cage above",
				"d71de856f81a8acc65e6fc851a38d4d7ec216fd0796d0a6827a3ad6ed5511a30fa280f12eb2e47ed2ac03b5c462a0358d18d69fe4f985ec81778c1b370b652a8",
			),
			(
				"ffffffffffffffffffffffffffffffff",
				"zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo wrong",
				"ac27495480225222079d7be181583751e86f571027b0497b5b5d11218e0a8a13332572917f0f8e5a589620c6f15b11c61dee327651a14c34e18231052e48c069",
			),
			(
				"000000000000000000000000000000000000000000000000",
				"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon agent",
				"035895f2f481b1b0f01fcf8c289c794660b289981a78f8106447707fdd9666ca06da5a9a565181599b79f53b844d8a71dd9f439c52a3d7b3e8a79c906ac845fa",
			),
			(
				"7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
				"legal winner thank year wave sausage worth useful legal winner thank year wave sausage worth useful legal will",
				"f2b94508732bcbacbcc020faefecfc89feafa6649a5491b8c952cede496c214a0c7b3c392d168748f2d4a612bada0753b52a1c7ac53c1e93abd5c6320b9e95dd",
			),
			(
				"808080808080808080808080808080808080808080808080",
				"letter advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic avoid letter always",
				"107d7c02a5aa6f38c58083ff74f04c607c2d2c0ecc55501dadd72d025b751bc27fe913ffb796f841c49b1d33b610cf0e91d3aa239027f5e99fe4ce9e5088cd65",
			),
			(
				"ffffffffffffffffffffffffffffffffffffffffffffffff",
				"zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo when",
				"0cd6e5d827bb62eb8fc1e262254223817fd068a74b5b449cc2f667c3f1f985a76379b43348d952e2265b4cd129090758b3e3c2c49103b5051aac2eaeb890a528",
			),
			(
				"0000000000000000000000000000000000000000000000000000000000000000",
				"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon art",
				"bda85446c68413707090a52022edd26a1c9462295029f2e60cd7c4f2bbd3097170af7a4d73245cafa9c3cca8d561a7c3de6f5d4a10be8ed2a5e608d68f92fcc8",
			),
			(
				"7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
				"legal winner thank year wave sausage worth useful legal winner thank year wave sausage worth useful legal winner thank year wave sausage worth title",
				"bc09fca1804f7e69da93c2f2028eb238c227f2e9dda30cd63699232578480a4021b146ad717fbb7e451ce9eb835f43620bf5c514db0f8add49f5d121449d3e87",
			),
			(
				"8080808080808080808080808080808080808080808080808080808080808080",
				"letter advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic avoid letter advice cage absurd amount doctor acoustic bless",
				"c0c519bd0e91a2ed54357d9d1ebef6f5af218a153624cf4f2da911a0ed8f7a09e2ef61af0aca007096df430022f7a2b6fb91661a9589097069720d015e4e982f",
			),
			(
				"ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
				"zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo vote",
				"dd48c104698c30cfe2b6142103248622fb7bb0ff692eebb00089b32d22484e1613912f0a5b694407be899ffd31ed3992c456cdf60f5d4564b8ba3f05a69890ad",
			),
			(
				"9e885d952ad362caeb4efe34a8e91bd2",
				"ozone drill grab fiber curtain grace pudding thank cruise elder eight picnic",
				"274ddc525802f7c828d8ef7ddbcdc5304e87ac3535913611fbbfa986d0c9e5476c91689f9c8a54fd55bd38606aa6a8595ad213d4c9c9f9aca3fb217069a41028",
			),
			(
				"6610b25967cdcca9d59875f5cb50b0ea75433311869e930b",
				"gravity machine north sort system female filter attitude volume fold club stay feature office ecology stable narrow fog",
				"628c3827a8823298ee685db84f55caa34b5cc195a778e52d45f59bcf75aba68e4d7590e101dc414bc1bbd5737666fbbef35d1f1903953b66624f910feef245ac",
			),
			(
				"68a79eaca2324873eacc50cb9c6eca8cc68ea5d936f98787c60c7ebc74e6ce7c",
				"hamster diagram private dutch cause delay private meat slide toddler razor book happy fancy gospel tennis maple dilemma loan word shrug inflict delay length",
				"64c87cde7e12ecf6704ab95bb1408bef047c22db4cc7491c4271d170a1b213d20b385bc1588d9c7b38f1b39d415665b8a9030c9ec653d75e65f847d8fc1fc440",
			),
			(
				"c0ba5a8e914111210f2bd131f3d5e08d",
				"scheme spot photo card baby mountain device kick cradle pact join borrow",
				"ea725895aaae8d4c1cf682c1bfd2d358d52ed9f0f0591131b559e2724bb234fca05aa9c02c57407e04ee9dc3b454aa63fbff483a8b11de949624b9f1831a9612",
			),
			(
				"6d9be1ee6ebd27a258115aad99b7317b9c8d28b6d76431c3",
				"horn tenant knee talent sponsor spell gate clip pulse soap slush warm silver nephew swap uncle crack brave",
				"fd579828af3da1d32544ce4db5c73d53fc8acc4ddb1e3b251a31179cdb71e853c56d2fcb11aed39898ce6c34b10b5382772db8796e52837b54468aeb312cfc3d",
			),
			(
				"9f6a2878b2520799a44ef18bc7df394e7061a224d2c33cd015b157d746869863",
				"panda eyebrow bullet gorilla call smoke muffin taste mesh discover soft ostrich alcohol speed nation flash devote level hobby quick inner drive ghost inside",
				"72be8e052fc4919d2adf28d5306b5474b0069df35b02303de8c1729c9538dbb6fc2d731d5f832193cd9fb6aeecbc469594a70e3dd50811b5067f3b88b28c3e8d",
			),
			(
				"23db8160a31d3e0dca3688ed941adbf3",
				"cat swing flag economy stadium alone churn speed unique patch report train",
				"deb5f45449e615feff5640f2e49f933ff51895de3b4381832b3139941c57b59205a42480c52175b6efcffaa58a2503887c1e8b363a707256bdd2b587b46541f5",
			),
			(
				"8197a4a47f0425faeaa69deebc05ca29c0a5b5cc76ceacc0",
				"light rule cinnamon wrap drastic word pride squirrel upgrade then income fatal apart sustain crack supply proud access",
				"4cbdff1ca2db800fd61cae72a57475fdc6bab03e441fd63f96dabd1f183ef5b782925f00105f318309a7e9c3ea6967c7801e46c8a58082674c860a37b93eda02",
			),
			(
				"066dca1a2bb7e8a1db2832148ce9933eea0f3ac9548d793112d9a95c9407efad",
				"all hour make first leader extend hole alien behind guard gospel lava path output census museum junior mass reopen famous sing advance salt reform",
				"26e975ec644423f4a4c4f4215ef09b4bd7ef924e85d1d17c4cf3f136c2863cf6df0a475045652c57eb5fb41513ca2a2d67722b77e954b4b3fc11f7590449191d",
			),
			(
				"f30f8c1da665478f49b001d94c5fc452",
				"vessel ladder alter error federal sibling chat ability sun glass valve picture",
				"2aaa9242daafcee6aa9d7269f17d4efe271e1b9a529178d7dc139cd18747090bf9d60295d0ce74309a78852a9caadf0af48aae1c6253839624076224374bc63f",
			),
			(
				"c10ec20dc3cd9f652c7fac2f1230f7a3c828389a14392f05",
				"scissors invite lock maple supreme raw rapid void congress muscle digital elegant little brisk hair mango congress clump",
				"7b4a10be9d98e6cba265566db7f136718e1398c71cb581e1b2f464cac1ceedf4f3e274dc270003c670ad8d02c4558b2f8e39edea2775c9e232c7cb798b069e88",
			),
			(
				"f585c11aec520db57dd353c69554b21a89b20fb0650966fa0a9d6f74fd989d8f",
				"void come effort suffer camp survey warrior heavy shoot primary clutch crush open amazing screen patrol group space point ten exist slush involve unfold",
				"01f5bced59dec48e362f2c45b5de68b9fd6c92c6634f44d6d40aab69056506f0e35524a518034ddc1192e1dacd32c1ed3eaa3c3b131c88ed8e7e54c49a5d0998",
			)
		];

        for vector in &test_vectors {
            let entropy = hex::decode(&vector.0).unwrap();
            let mnemonic_str = vector.1;

            let mnemonic = Mnemonic::from_entropy_in(&entropy).unwrap();

            assert_eq!(
                mnemonic,
                Mnemonic::parse_in_normalized(mnemonic_str).unwrap(),
                "failed vector: {}",
                mnemonic_str
            );

            {
                assert_eq!(
                    mnemonic.to_string_zeroizing().as_str(),
                    mnemonic_str,
                    "failed vector: {}",
                    mnemonic_str
                );
                assert_eq!(
                    mnemonic,
                    Mnemonic::parse(mnemonic_str).unwrap(),
                    "failed vector: {}",
                    mnemonic_str
                );
                assert_eq!(
                    &entropy,
                    &mnemonic.to_entropy(),
                    "failed vector: {}",
                    mnemonic_str
                );
            }
        }
    }

    #[test]
    fn test_invalid_engish() {
        // correct phrase:
        // "letter advice cage absurd amount doctor acoustic avoid letter advice cage above"

        assert_eq!(
            Mnemonic::parse(
                "getter advice cage absurd amount doctor acoustic avoid letter advice cage above",
            ),
            Err(Error::UnknownWord(0))
        );

        assert_eq!(
            Mnemonic::parse(
                "letter advice cagex absurd amount doctor acoustic avoid letter advice cage above",
            ),
            Err(Error::UnknownWord(2))
        );

        assert_eq!(
            Mnemonic::parse(
                "advice cage absurd amount doctor acoustic avoid letter advice cage above",
            ),
            Err(Error::BadWordCount(11))
        );

        assert_eq!(
            Mnemonic::parse(
                "primary advice cage absurd amount doctor acoustic avoid letter advice cage above",
            ),
            Err(Error::InvalidChecksum)
        );
    }

    #[test]
    fn test_invalid_entropy() {
        //between 128 and 256 bits, but not divisible by 32
        assert_eq!(
            Mnemonic::from_entropy_in(&vec![b'x'; 17]),
            Err(Error::BadEntropyBitCount(136))
        );

        //less than 128 bits
        assert_eq!(
            Mnemonic::from_entropy_in(&vec![b'x'; 4]),
            Err(Error::BadEntropyBitCount(32))
        );

        //greater than 256 bits
        assert_eq!(
            Mnemonic::from_entropy_in(&vec![b'x'; 36]),
            Err(Error::BadEntropyBitCount(288))
        );
    }
}
