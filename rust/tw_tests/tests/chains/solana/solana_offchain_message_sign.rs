// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Solana off-chain message signing, header version 0, with a labeled-line body.
//!
//! The fixtures in `fixtures/offchain_identity/` are copied verbatim from the Identity service
//! (`internal/domain/signing/solana/testdata` in backend-identity), which rebuilds these exact
//! bytes server-side and verifies the signature against them. They are the contract between the
//! two implementations: the same fields signed with the same key must produce the same envelope,
//! the same content digest and the same signature on both sides. A drift on either side fails
//! here rather than reaching production as an unexplainable signature mismatch.

use serde::Deserialize;
use tw_any_coin::ffi::tw_message_signer::{
    tw_message_signer_pre_image_hashes, tw_message_signer_sign, tw_message_signer_verify,
};
use tw_coin_entry::error::prelude::SigningErrorType;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::base58::Alphabet;
use tw_encoding::base64::STANDARD;
use tw_encoding::{base58, base64, hex};
use tw_hash::sha2::sha256;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::Solana::Proto as SolanaProto;
use tw_proto::{deserialize, serialize, TxCompiler};

use SolanaProto::mod_MessageSigningInput::OneOfmessage_payload as SigningPayload;
use SolanaProto::mod_MessageVerifyingInput::OneOfmessage_payload as VerifyingPayload;

/// The Identity service's domain separator. The application domain in the header is its sha256,
/// derived here the way the service derives it rather than copied out of a fixture, so the two
/// cannot silently disagree.
const IDENTITY_DOMAIN_SEPARATOR: &str = "trustwallet.identity.v1";

/// The bare first line of every Identity body.
const IDENTITY_HEADER: &str = "Trust Wallet Identity v1";

/// The fixed header: signing domain, header version, application domain, message format, signer
/// count, one signer, and the u16 message length.
const PREAMBLE_SIZE: usize = 85;

/// The standard's cap on the preamble and the body together, for message formats 0 and 1.
const MAX_MESSAGE_SIZE: usize = 1232;

const FIXTURES: [&str; 6] = [
    include_str!("fixtures/offchain_identity/username_claim.json"),
    include_str!("fixtures/offchain_identity/username_claim_no_x.json"),
    include_str!("fixtures/offchain_identity/username_rename.json"),
    include_str!("fixtures/offchain_identity/session_challenge.json"),
    include_str!("fixtures/offchain_identity/session_challenge_revoke_all.json"),
    include_str!("fixtures/offchain_identity/address_proof.json"),
];

/// The fixture file as the Identity service writes it: everything a second implementation needs
/// to reproduce the signature.
#[derive(Deserialize)]
struct Fixture {
    scheme: String,
    #[serde(rename = "type")]
    message_type: String,
    seed: String,
    signer: String,
    message: Fields,
    statement: String,
    body: String,
    envelope: String,
    digest: String,
    signature: String,
}

/// The canonical message as a request carries it.
#[derive(Deserialize)]
struct Fields {
    address: String,
    #[serde(default)]
    username: String,
    #[serde(default)]
    previous_username: String,
    #[serde(default)]
    x_user_id: String,
    #[serde(default)]
    purpose: String,
    #[serde(default)]
    anchor_coin: u32,
    #[serde(default)]
    anchor_address: String,
    nonce: String,
    issued_at: i64,
    expires_at: i64,
}

fn fixtures() -> Vec<Fixture> {
    FIXTURES
        .iter()
        .map(|raw| serde_json::from_str(raw).expect("Invalid fixture JSON"))
        .collect()
}

fn application_domain() -> Vec<u8> {
    sha256(IDENTITY_DOMAIN_SEPARATOR.as_bytes())
}

fn field(label: &str, value: &str) -> SolanaProto::StructuredField<'static> {
    SolanaProto::StructuredField {
        label: label.to_string().into(),
        value: value.to_string().into(),
    }
}

fn or_dash(value: &str) -> &str {
    if value.is_empty() {
        "-"
    } else {
        value
    }
}

/// Builds the structured message the app would pass for this fixture. The field order is the
/// Identity signing spec's, per message type, and it is part of the signed bytes.
fn structured_message(fixture: &Fixture) -> SolanaProto::StructuredMessage<'static> {
    let m = &fixture.message;
    let mut fields = vec![field("Type", &fixture.message_type)];

    match fixture.message_type.as_str() {
        "UsernameClaim" => {
            fields.push(field("Username", &m.username));
            fields.push(field("Address", &m.address));
            // An absent X connection is written out as a dash: a bare "X User ID:" line reads
            // like a truncated message to anyone auditing the proof, and the renderer refuses an
            // empty value precisely so two implementations cannot disagree about this.
            fields.push(field("X User ID", or_dash(&m.x_user_id)));
        },
        "UsernameRename" => {
            fields.push(field("Previous Username", &m.previous_username));
            fields.push(field("Username", &m.username));
            fields.push(field("Address", &m.address));
        },
        "SessionChallenge" => {
            fields.push(field("Address", &m.address));
            fields.push(field("Purpose", &m.purpose));
        },
        "AddressProof" => {
            fields.push(field("Address", &m.address));
            // The coin belongs in the line: an address alone does not say which family's anchor
            // it is. There is no Username line — an address proof carries no name.
            fields.push(field(
                "Anchor",
                &format!("{}:{}", m.anchor_coin, m.anchor_address),
            ));
        },
        other => panic!("Unexpected message type {other}"),
    }

    fields.push(field("Nonce", &m.nonce));
    fields.push(field("Issued At", &rfc3339(m.issued_at)));
    fields.push(field("Expires At", &rfc3339(m.expires_at)));

    SolanaProto::StructuredMessage {
        header: IDENTITY_HEADER.into(),
        fields,
        statement: fixture.statement.clone().into(),
    }
}

/// Renders a unix second as the body carries it: RFC 3339 in UTC, second granularity.
///
/// Hand-rolled to keep a date crate out of the test dependencies. Its correctness is not taken on
/// trust: a wrong date changes the body, and every body here is pinned against the fixture the
/// service generated.
fn rfc3339(unix: i64) -> String {
    let days = unix.div_euclid(86_400);
    let secs = unix.rem_euclid(86_400);
    let (year, month, day) = civil_from_days(days);

    format!(
        "{year:04}-{month:02}-{day:02}T{:02}:{:02}:{:02}Z",
        secs / 3600,
        (secs % 3600) / 60,
        secs % 60
    )
}

/// Howard Hinnant's `civil_from_days`: days since the unix epoch to a proleptic Gregorian date.
fn civil_from_days(days: i64) -> (i64, i64, i64) {
    let z = days + 719_468;
    let era = if z >= 0 { z } else { z - 146_096 } / 146_097;
    let doe = z - era * 146_097;
    let yoe = (doe - doe / 1460 + doe / 36_524 - doe / 146_096) / 365;
    let year = yoe + era * 400;
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let mp = (5 * doy + 2) / 153;
    let day = doy - (153 * mp + 2) / 5 + 1;
    let month = if mp < 10 { mp + 3 } else { mp - 9 };

    (if month <= 2 { year + 1 } else { year }, month, day)
}

fn private_key(fixture: &Fixture) -> Vec<u8> {
    hex::decode(fixture.seed.trim_start_matches("0x")).unwrap()
}

fn signing_input(fixture: &Fixture) -> SolanaProto::MessageSigningInput<'static> {
    SolanaProto::MessageSigningInput {
        private_key: private_key(fixture).into(),
        message_type: SolanaProto::MessageType::MessageType_offchain_v0,
        application_domain: application_domain().into(),
        message_payload: SigningPayload::structured_message(structured_message(fixture)),
        ..Default::default()
    }
}

/// Signs through the FFI and returns the output owned: the deserialized proto borrows from the
/// response buffer, so it cannot outlive this function.
fn sign(input: &SolanaProto::MessageSigningInput) -> (SigningErrorType, String) {
    let input_data = TWDataHelper::create(serialize(input).unwrap());
    let output = TWDataHelper::wrap(unsafe {
        tw_message_signer_sign(CoinType::Solana as u32, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_message_signer_sign returned nullptr");

    let output: SolanaProto::MessageSigningOutput = deserialize(&output).unwrap();
    assert_eq!(
        output.error_message.is_empty(),
        output.error == SigningErrorType::OK,
        "an error always carries a message, and a success never does"
    );

    (output.error, output.signature.to_string())
}

/// The pre-image: for Solana it is not a hash but the exact bytes the key signs.
fn pre_image(input: &SolanaProto::MessageSigningInput) -> (SigningErrorType, Vec<u8>) {
    let input_data = TWDataHelper::create(serialize(input).unwrap());
    let output = TWDataHelper::wrap(unsafe {
        tw_message_signer_pre_image_hashes(CoinType::Solana as u32, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_message_signer_pre_image_hashes returned nullptr");

    let output: TxCompiler::Proto::PreSigningOutput = deserialize(&output).unwrap();

    (output.error, output.data.to_vec())
}

fn verify(input: &SolanaProto::MessageVerifyingInput) -> bool {
    let input_data = TWDataHelper::create(serialize(input).unwrap());
    unsafe { tw_message_signer_verify(CoinType::Solana as u32, input_data.ptr()) }
}

fn verifying_input(
    fixture: &Fixture,
    signature: &str,
) -> SolanaProto::MessageVerifyingInput<'static> {
    SolanaProto::MessageVerifyingInput {
        public_key: base58::decode(&fixture.signer, Alphabet::Bitcoin)
            .unwrap()
            .into(),
        signature: signature.to_string().into(),
        message_type: SolanaProto::MessageType::MessageType_offchain_v0,
        application_domain: application_domain().into(),
        message_payload: VerifyingPayload::structured_message(structured_message(fixture)),
        ..Default::default()
    }
}

/// The contract with the Identity service, one fixture per message type: the rebuilt envelope,
/// its content digest and the signature must all match the bytes the service generated.
#[test]
fn test_solana_offchain_identity_fixtures() {
    for fixture in fixtures() {
        let label = &fixture.message_type;
        assert_eq!(fixture.scheme, "solana_offchain_v0");

        let input = signing_input(&fixture);

        // The envelope, byte for byte. This covers the body too: the body is inside it.
        let (error, envelope) = pre_image(&input);
        assert_eq!(error, SigningErrorType::OK, "{label}");
        assert_eq!(hex::encode(&envelope, true), fixture.envelope, "{label}");

        // The content digest the service stores on the proof row: sha256 of the envelope.
        // ed25519 signs the whole message, so this is a content hash for forensics and a dispute
        // that needs to point at exact bytes — never a signing digest.
        assert_eq!(
            hex::encode(sha256(&envelope), true),
            fixture.digest,
            "{label}"
        );

        let (error, signature) = sign(&input);
        assert_eq!(error, SigningErrorType::OK, "{label}");
        assert_eq!(
            base58::decode(&signature, Alphabet::Bitcoin).unwrap(),
            base64::decode(&fixture.signature, STANDARD).unwrap(),
            "{label}: the app's signature must equal the service's fixture"
        );

        // And it verifies against the key the header states.
        assert!(verify(&verifying_input(&fixture, &signature)), "{label}");
    }
}

/// The rendered body, pinned per message type. The envelope test covers these bytes too, but a
/// body mismatch is far easier to read as text than as 400 hex characters.
#[test]
fn test_solana_offchain_body() {
    for fixture in fixtures() {
        let (_, envelope) = pre_image(&signing_input(&fixture));

        let body = String::from_utf8(envelope[PREAMBLE_SIZE..].to_vec()).unwrap();
        assert_eq!(body, fixture.body, "{}", fixture.message_type);
        assert!(
            !body.ends_with('\n'),
            "the body ends with the statement, never a trailing newline"
        );
    }
}

/// The header, field by field. Every width and the byte order are the standard's, not ours, so
/// they are asserted rather than trusted to a round trip through our own encoder.
#[test]
fn test_solana_offchain_envelope_layout() {
    let fixture = &fixtures()[0];
    let (_, envelope) = pre_image(&signing_input(fixture));

    assert_eq!(&envelope[0..16], b"\xffsolana offchain", "signing domain");
    assert_eq!(envelope[16], 0, "header version 0");
    assert_eq!(
        &envelope[17..49],
        application_domain(),
        "application domain"
    );
    assert_eq!(envelope[49], 1, "message format 1, UTF-8");
    assert_eq!(envelope[50], 1, "exactly one signer");
    assert_eq!(
        &envelope[51..83],
        base58::decode(&fixture.signer, Alphabet::Bitcoin).unwrap(),
        "the signer is stated inside the signed bytes"
    );

    let body_len = u16::from_le_bytes([envelope[83], envelope[84]]);
    assert_eq!(
        body_len as usize,
        fixture.body.len(),
        "the body length is a little-endian u16"
    );
    assert_eq!(envelope.len(), PREAMBLE_SIZE + fixture.body.len());
}

/// A value carrying a newline would forge further labeled lines — a second "Purpose:", an
/// "Anchor:" naming another account — in the message a wallet displays and in whatever consent
/// record the verifier stores, under a signature that is perfectly genuine. Every rendered line
/// is screened, so this is refused before anything signs it.
#[test]
fn test_solana_offchain_refuses_a_forged_line() {
    let forged = "1234567890\nPurpose: revoke_all\nAnchor: 60:0xdeadbeef";

    let cases = [
        (
            "a newline in a value",
            IDENTITY_HEADER,
            field("X User ID", forged),
            "A statement.",
        ),
        (
            "a newline in a label",
            IDENTITY_HEADER,
            field("X User ID\nPurpose", "session"),
            "A statement.",
        ),
        (
            "a newline in the header",
            "Trust Wallet Identity v1\nPurpose: revoke_all",
            field("Purpose", "session"),
            "A statement.",
        ),
        (
            "a multi-line statement",
            IDENTITY_HEADER,
            field("Purpose", "session"),
            "A statement.\nAnchor: 60:0xdeadbeef",
        ),
        (
            "a carriage return",
            IDENTITY_HEADER,
            field("Purpose", "session\rPurpose: revoke_all"),
            "A statement.",
        ),
        (
            "a tab",
            IDENTITY_HEADER,
            field("Purpose", "session\tx"),
            "A statement.",
        ),
        (
            "a Unicode line separator",
            IDENTITY_HEADER,
            field("Purpose", "session\u{2028}Purpose: revoke_all"),
            "A statement.",
        ),
        (
            "an empty label",
            IDENTITY_HEADER,
            field("", "session"),
            "A statement.",
        ),
        (
            "an empty value, which must be a placeholder instead",
            IDENTITY_HEADER,
            field("X User ID", ""),
            "A statement.",
        ),
    ];

    for (name, header, bad_field, statement) in cases {
        let mut input = signing_input(&fixtures()[0]);
        input.message_payload =
            SigningPayload::structured_message(SolanaProto::StructuredMessage {
                header: header.into(),
                fields: vec![bad_field],
                statement: statement.into(),
            });

        let (error, signature) = sign(&input);
        assert_eq!(
            error,
            SigningErrorType::Error_invalid_params,
            "{name} must be refused"
        );
        assert!(signature.is_empty(), "{name} must not be signed");
    }
}

/// The off-chain envelope cannot be built without the caller's own domain separator, and it is
/// exactly 32 bytes. A wrong length is a caller error, not something to pad or truncate.
#[test]
fn test_solana_offchain_requires_an_application_domain() {
    for (name, domain) in [
        ("no application domain", vec![]),
        ("a short application domain", vec![0x11; 31]),
        ("a long application domain", vec![0x11; 33]),
    ] {
        let mut input = signing_input(&fixtures()[0]);
        input.application_domain = domain.into();

        let (error, _) = sign(&input);
        assert_eq!(
            error,
            SigningErrorType::Error_invalid_params,
            "{name} must be refused"
        );
    }
}

/// The standard's 1232-byte cap covers the preamble and the body together, and enforcing it is a
/// MUST on the signer. The check also has to run before the length field is written: it is a
/// u16, and a body over 64 KiB would wrap it into a plausible-looking value.
#[test]
fn test_solana_offchain_refuses_an_oversized_message() {
    const MAX_BODY: usize = MAX_MESSAGE_SIZE - PREAMBLE_SIZE;

    let header_only = |len: usize| {
        SigningPayload::structured_message(SolanaProto::StructuredMessage {
            header: "x".repeat(len).into(),
            fields: vec![],
            statement: "".into(),
        })
    };

    let mut input = signing_input(&fixtures()[0]);

    input.message_payload = header_only(MAX_BODY);
    let (error, _) = sign(&input);
    assert_eq!(error, SigningErrorType::OK, "{MAX_BODY} bytes must fit");

    for (name, len) in [
        ("one byte over the cap", MAX_BODY + 1),
        (
            "a body that would wrap the u16 length",
            u16::MAX as usize + 1,
        ),
    ] {
        input.message_payload = header_only(len);
        let (error, _) = sign(&input);
        assert_eq!(
            error,
            SigningErrorType::Error_invalid_params,
            "{name} must be refused"
        );
    }
}

/// ed25519 has no key recovery, so the signer is stated in the header rather than recovered.
/// That is not the weaker guarantee it sounds like: the key is inside the signed bytes, so bytes
/// signed for one address cannot be presented as a proof of another.
#[test]
fn test_solana_offchain_refuses_another_signers_envelope() {
    let fixture = &fixtures()[0];
    let (_, signature) = sign(&signing_input(fixture));

    let mut input = verifying_input(fixture, &signature);
    input.public_key = base58::decode(
        "9xQeWvG816bUx9EPjHmaT23yvVM2ZWbrrpZb9PusVFin",
        Alphabet::Bitcoin,
    )
    .unwrap()
    .into();

    assert!(!verify(&input));
}

/// A signature over one application's messages must not verify against another's. That is what
/// the application domain is for, and it is inside the signed bytes.
#[test]
fn test_solana_offchain_domain_separates() {
    let fixture = &fixtures()[0];
    let (_, signature) = sign(&signing_input(fixture));

    let mut input = verifying_input(fixture, &signature);
    input.application_domain = sha256(b"trustwallet.identity.v2").into();

    assert!(!verify(&input));
}

/// The envelope and the body are independent choices: the same labeled body can be signed raw,
/// in which case the signed bytes are the rendered text and nothing else.
#[test]
fn test_solana_raw_signs_a_structured_body() {
    let fixture = &fixtures()[0];

    let mut input = signing_input(fixture);
    input.message_type = SolanaProto::MessageType::MessageType_raw;
    input.application_domain = Default::default();

    let (error, raw_bytes) = pre_image(&input);
    assert_eq!(error, SigningErrorType::OK);
    assert_eq!(
        String::from_utf8(raw_bytes).unwrap(),
        fixture.body,
        "raw signs the rendered body and adds nothing to it"
    );

    // The same body signed raw and wrapped cannot produce the same signature — which is the
    // point of the envelope.
    let (_, raw_signature) = sign(&input);
    let (_, wrapped_signature) = sign(&signing_input(fixture));
    assert_ne!(raw_signature, wrapped_signature);
}

/// Raw is the default, so an input that names no message type keeps the behaviour this coin had
/// before the off-chain type existed.
#[test]
fn test_solana_raw_is_the_default() {
    let fixture = &fixtures()[0];

    let explicit = SolanaProto::MessageSigningInput {
        private_key: private_key(fixture).into(),
        message_type: SolanaProto::MessageType::MessageType_raw,
        application_domain: Default::default(),
        message_payload: SigningPayload::message("Hello world".into()),
        ..Default::default()
    };
    let defaulted = SolanaProto::MessageSigningInput {
        private_key: private_key(fixture).into(),
        message_payload: SigningPayload::message("Hello world".into()),
        ..Default::default()
    };

    let (error, explicit_signature) = sign(&explicit);
    assert_eq!(error, SigningErrorType::OK);
    assert_eq!(sign(&defaulted).1, explicit_signature);
}

/// An input with neither payload set is a caller error, not an empty message to sign.
#[test]
fn test_solana_message_requires_a_payload() {
    let mut input = signing_input(&fixtures()[0]);
    input.message_payload = SigningPayload::None;

    let (error, _) = sign(&input);
    assert_eq!(error, SigningErrorType::Error_invalid_params);
}

/// An external signer holds no private key — that is the whole reason to ask for a pre-image —
/// so a public key has to be enough to build one. The envelope states the signer, so the bytes
/// must come out identical to the ones the holder of that key would sign.
#[test]
fn test_solana_offchain_pre_image_without_a_private_key() {
    let fixture = &fixtures()[0];

    let (with_secret_error, with_secret) = pre_image(&signing_input(fixture));
    assert_eq!(with_secret_error, SigningErrorType::OK);

    let mut input = signing_input(fixture);
    input.private_key = Default::default();
    input.public_key = base58::decode(&fixture.signer, Alphabet::Bitcoin)
        .unwrap()
        .into();

    let (error, with_public_key) = pre_image(&input);
    assert_eq!(error, SigningErrorType::OK);
    assert_eq!(
        with_public_key, with_secret,
        "the pre-image is the same bytes whichever key names the signer"
    );
}

/// Raw signs the body and nothing else, so its pre-image needs no key at all.
#[test]
fn test_solana_raw_pre_image_without_any_key() {
    let fixture = &fixtures()[0];

    let mut input = signing_input(fixture);
    input.message_type = SolanaProto::MessageType::MessageType_raw;
    input.application_domain = Default::default();
    input.private_key = Default::default();

    let (error, raw_bytes) = pre_image(&input);
    assert_eq!(error, SigningErrorType::OK);
    assert_eq!(String::from_utf8(raw_bytes).unwrap(), fixture.body);
}

/// The off-chain envelope, on the other hand, cannot be built without knowing the signer: it is
/// inside the signed bytes. Neither key is a caller error, and the error says which to set.
#[test]
fn test_solana_offchain_pre_image_requires_a_signer() {
    let mut input = signing_input(&fixtures()[0]);
    input.private_key = Default::default();

    let (error, _) = pre_image(&input);
    assert_eq!(error, SigningErrorType::Error_invalid_params);
}

/// `MessageType_raw` is the proto3 default, so an input that sets a domain separator and leaves
/// the type unset would otherwise be signed raw, dropping the domain: a signature that looks
/// fine and fails at whatever rebuilds the envelope. It is refused instead, on both paths.
#[test]
fn test_solana_raw_refuses_an_application_domain() {
    let fixture = &fixtures()[0];

    let mut input = signing_input(fixture);
    input.message_type = SolanaProto::MessageType::MessageType_raw;

    let (error, signature) = sign(&input);
    assert_eq!(error, SigningErrorType::Error_invalid_params);
    assert!(signature.is_empty());

    let (error, _) = pre_image(&input);
    assert_eq!(error, SigningErrorType::Error_invalid_params);

    // The same on the verifying side, where ignoring the domain would have reported a signature
    // over other bytes as valid.
    let (_, signature) = sign(&signing_input(fixture));
    let mut input = verifying_input(fixture, &signature);
    input.message_type = SolanaProto::MessageType::MessageType_raw;

    assert!(!verify(&input));
}

/// An off-chain input over a plain body. The Identity layout is not needed to exercise the
/// header fields, and restricted ASCII cannot carry it anyway.
fn offchain_signing_input(
    fixture: &Fixture,
    body: &str,
) -> SolanaProto::MessageSigningInput<'static> {
    SolanaProto::MessageSigningInput {
        private_key: private_key(fixture).into(),
        message_type: SolanaProto::MessageType::MessageType_offchain_v0,
        application_domain: application_domain().into(),
        message_payload: SigningPayload::message(body.to_string().into()),
        ..Default::default()
    }
}

fn offchain_verifying_input(
    fixture: &Fixture,
    body: &str,
    signature: &str,
) -> SolanaProto::MessageVerifyingInput<'static> {
    SolanaProto::MessageVerifyingInput {
        public_key: base58::decode(&fixture.signer, Alphabet::Bitcoin)
            .unwrap()
            .into(),
        signature: signature.to_string().into(),
        message_type: SolanaProto::MessageType::MessageType_offchain_v0,
        application_domain: application_domain().into(),
        message_payload: VerifyingPayload::message(body.to_string().into()),
        ..Default::default()
    }
}

/// Restricted ASCII is the only format a hardware wallet renders rather than blind-signs, so it
/// has to be reachable: the header states the standard's format byte 0, the body is unchanged,
/// and a verifier naming the same format accepts the signature.
#[test]
fn test_solana_offchain_restricted_ascii() {
    let fixture = &fixtures()[0];
    let body = "Trust Wallet Identity v1 - approve session 7f3a2b";

    let mut input = offchain_signing_input(fixture, body);
    input.message_format = SolanaProto::MessageFormat::MessageFormat_restricted_ascii;

    let (error, envelope) = pre_image(&input);
    assert_eq!(error, SigningErrorType::OK);
    assert_eq!(envelope[49], 0, "message format 0, restricted ASCII");
    assert_eq!(
        String::from_utf8(envelope[PREAMBLE_SIZE..].to_vec()).unwrap(),
        body,
        "the format byte describes the body, it does not change it"
    );

    let (error, signature) = sign(&input);
    assert_eq!(error, SigningErrorType::OK);

    let mut verifying = offchain_verifying_input(fixture, body, &signature);
    verifying.message_format = SolanaProto::MessageFormat::MessageFormat_restricted_ascii;
    assert!(verify(&verifying));
}

/// UTF-8 stays the default, so an input that names no format signs what it signed before the
/// field existed — the same bytes, over the same body.
#[test]
fn test_solana_offchain_utf8_is_the_default() {
    let fixture = &fixtures()[0];
    let body = "Trust Wallet Identity v1 - approve session 7f3a2b";

    let defaulted = offchain_signing_input(fixture, body);
    let mut explicit = offchain_signing_input(fixture, body);
    explicit.message_format = SolanaProto::MessageFormat::MessageFormat_utf8;

    let (error, envelope) = pre_image(&defaulted);
    assert_eq!(error, SigningErrorType::OK);
    assert_eq!(envelope[49], 1, "message format 1, UTF-8");
    assert_eq!(pre_image(&explicit).1, envelope);
}

/// The format byte is inside the signed bytes, so claiming a different one is a different
/// message. The body here is valid in both formats: what fails is the byte, not the content.
#[test]
fn test_solana_offchain_format_is_signed() {
    let fixture = &fixtures()[0];
    let body = "Trust Wallet Identity v1 - approve session 7f3a2b";

    let (error, signature) = sign(&offchain_signing_input(fixture, body));
    assert_eq!(error, SigningErrorType::OK);

    let mut verifying = offchain_verifying_input(fixture, body, &signature);
    verifying.message_format = SolanaProto::MessageFormat::MessageFormat_restricted_ascii;
    assert!(
        !verify(&verifying),
        "a UTF-8 signature must not verify as restricted ASCII"
    );
}

/// A format byte is a claim about the body, so a body that would make it a lie is refused rather
/// than signed and mislabeled — a hardware wallet trusting the byte would render something else.
#[test]
fn test_solana_offchain_restricted_ascii_refuses_what_it_cannot_carry() {
    let fixture = &fixtures()[0];

    let cases = [
        (
            "a newline",
            SigningPayload::message("line one\nline two".into()),
        ),
        ("a tab", SigningPayload::message("label:\tvalue".into())),
        (
            "a non-ASCII character",
            SigningPayload::message("héllo".into()),
        ),
        (
            "a labeled body, which is one field per line",
            SigningPayload::structured_message(structured_message(fixture)),
        ),
    ];

    for (name, payload) in cases {
        let mut input = offchain_signing_input(fixture, "");
        input.message_format = SolanaProto::MessageFormat::MessageFormat_restricted_ascii;
        input.message_payload = payload;

        let (error, signature) = sign(&input);
        assert_eq!(
            error,
            SigningErrorType::Error_invalid_params,
            "{name} must be refused"
        );
        assert!(signature.is_empty(), "{name} must not be signed");
    }
}

/// Raw has no header to state a format in, so naming one there is a caller error for the same
/// reason an application domain is: the field would be silently dropped from the signed bytes.
#[test]
fn test_solana_raw_refuses_a_message_format() {
    let fixture = &fixtures()[0];

    let mut input = offchain_signing_input(fixture, "Hello world");
    input.message_type = SolanaProto::MessageType::MessageType_raw;
    input.application_domain = Default::default();
    input.message_format = SolanaProto::MessageFormat::MessageFormat_restricted_ascii;

    let (error, signature) = sign(&input);
    assert_eq!(error, SigningErrorType::Error_invalid_params);
    assert!(signature.is_empty());

    let (error, _) = pre_image(&input);
    assert_eq!(error, SigningErrorType::Error_invalid_params);
}
