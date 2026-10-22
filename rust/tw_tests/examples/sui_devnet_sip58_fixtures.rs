// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Generates `rust/chains/tw_sui/tests/fixtures/devnet_sip58/`: SIP-58 transactions built and signed by
//! wallet-core, EXECUTED on Sui devnet, then fetched back as the node serialises them.
//!
//! Local tool, never run by CI (a public network brings flakiness). Needs network and `curl`.
//!
//! ```text
//! cargo run -p tw_tests --example sui_devnet_sip58_fixtures --release -- <output dir>
//! ```
//!
//! Throwaway keys, funded from the public devnet faucet. Fails if the node's canonical BCS differs from the
//! bytes wallet-core produced. See `rust/chains/tw_sui/tests/fixtures/devnet_sip58/README.md`.

use move_core_types::identifier::Identifier;
use move_core_types::language_storage::{StructTag, TypeTag};
use serde_json::{json, Value};
use std::io::Write;
use std::process::{Command as Process, Stdio};
use std::str::FromStr;
use std::time::{Duration, Instant};
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::base64::{self, STANDARD};
use tw_encoding::bcs;
use tw_keypair::ed25519;
use tw_keypair::traits::KeyPairTrait;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Sui::Proto::{self, mod_SigningInput::OneOftransaction_payload as Payload};
use tw_sui::address::SuiAddress;
use tw_sui::transaction::command::{Argument, Command};
use tw_sui::transaction::programmable_transaction::ProgrammableTransaction;
use tw_sui::transaction::sui_types::{
    CallArg, GasData, ObjectDigest, ObjectID, ObjectRef, SequenceNumber, TransactionExpiration,
};
use tw_sui::transaction::transaction_data::{TransactionData, TransactionDataV1, TransactionKind};

const GRAPHQL: &str = "https://graphql.devnet.sui.io/graphql";
const FAUCET: &str = "https://faucet.devnet.sui.io/v2/gas";
const ONE: u64 = 1_000_000_000;
const BUDGET: u64 = 5_000_000;
const SUI_COIN: &str = "0x2::coin::Coin<0x2::sui::SUI>";
const STAKED_SUI: &str = "0x3::staking_pool::StakedSui";

type Result<T> = std::result::Result<T, String>;

// ----------------------------------- network -----------------------------------

fn curl(url: &str, body: &Value) -> Result<Value> {
    let mut child = Process::new("curl")
        .args([
            "-sS",
            "-X",
            "POST",
            "-H",
            "content-type: application/json",
            "--data-binary",
            "@-",
            url,
        ])
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .spawn()
        .map_err(|e| format!("cannot run curl: {e}"))?;
    child
        .stdin
        .take()
        .unwrap()
        .write_all(body.to_string().as_bytes())
        .unwrap();
    let out = child.wait_with_output().map_err(|e| e.to_string())?;
    serde_json::from_slice(&out.stdout).map_err(|e| {
        format!(
            "{url}: bad reply ({e}): {}",
            String::from_utf8_lossy(&out.stdout)
        )
    })
}

fn graphql(query: &str, variables: Value) -> Result<Value> {
    let reply = curl(GRAPHQL, &json!({ "query": query, "variables": variables }))?;
    if let Some(errors) = reply.get("errors") {
        return Err(format!("graphql: {errors}"));
    }
    Ok(reply["data"].clone())
}

fn eventually<T>(what: &str, mut f: impl FnMut() -> Result<Option<T>>) -> Result<T> {
    let deadline = Instant::now() + Duration::from_secs(90);
    loop {
        if let Ok(Some(v)) = f() {
            return Ok(v);
        }
        if Instant::now() > deadline {
            return Err(format!("timed out waiting for {what}"));
        }
        std::thread::sleep(Duration::from_millis(1500));
    }
}

fn faucet(address: &str) -> Result<()> {
    let reply = curl(
        FAUCET,
        &json!({ "FixedAmountRequest": { "recipient": address } }),
    )?;
    (reply["status"] == "Success")
        .then_some(())
        .ok_or(format!("faucet refused: {reply}"))
}

struct Chain {
    epoch: u64,
    price: u64,
    chain_id: String,
}

fn chain() -> Result<Chain> {
    let d = graphql(
        "{ chainIdentifier epoch { epochId referenceGasPrice } }",
        json!({}),
    )?;
    Ok(Chain {
        epoch: d["epoch"]["epochId"].as_u64().ok_or("epoch")?,
        price: d["epoch"]["referenceGasPrice"]
            .as_str()
            .and_then(|s| s.parse().ok())
            .ok_or("price")?,
        chain_id: d["chainIdentifier"].as_str().ok_or("chain")?.to_string(),
    })
}

struct Owned {
    id: String,
    version: u64,
    digest: String,
    balance: u64,
}

fn owned_objects(address: &str, object_type: &str) -> Result<Vec<Owned>> {
    let q = "query($a: SuiAddress!, $t: String!) { address(address: $a) { objects(filter: { type: $t }, first: 50) { nodes { address version digest contents { json } } } } }";
    let d = graphql(q, json!({ "a": address, "t": object_type }))?;
    Ok(d["address"]["objects"]["nodes"]
        .as_array()
        .cloned()
        .unwrap_or_default()
        .iter()
        .map(|n| Owned {
            id: n["address"].as_str().unwrap().to_string(),
            version: n["version"].as_u64().unwrap(),
            digest: n["digest"].as_str().unwrap().to_string(),
            balance: n["contents"]["json"]["balance"]
                .as_str()
                .and_then(|b| b.parse().ok())
                .unwrap_or(0),
        })
        .collect())
}

fn address_balance(address: &str) -> Result<u64> {
    let q = "query($a: SuiAddress!) { address(address: $a) { balance(coinType: \"0x2::sui::SUI\") { addressBalance } } }";
    let d = graphql(q, json!({ "a": address }))?;
    Ok(d["address"]["balance"]["addressBalance"]
        .as_str()
        .and_then(|s| s.parse().ok())
        .unwrap_or(0))
}

fn execute(tx_base64: &str, signature: &str) -> Result<String> {
    let q = "mutation($tx: Base64!, $sigs: [Base64!]!) { executeTransaction(transactionDataBcs: $tx, signatures: $sigs) { effects { digest status executionError { message } } } }";
    let d = graphql(q, json!({ "tx": tx_base64, "sigs": [signature] }))?;
    let effects = &d["executeTransaction"]["effects"];
    if effects["status"] != "SUCCESS" {
        return Err(format!("transaction failed: {effects}"));
    }
    Ok(effects["digest"].as_str().ok_or("digest")?.to_string())
}

/// The node's canonical `TransactionData` BCS (waits for the index to catch up).
fn canonical_bcs(digest: &str) -> Result<String> {
    eventually("transaction in the index", || {
        let d = graphql(
            "query($d: String!) { transaction(digest: $d) { transactionBcs } }",
            json!({ "d": digest }),
        )?;
        Ok(d["transaction"]["transactionBcs"]
            .as_str()
            .map(str::to_string))
    })
}

// ------------------------------------ accounts ----------------------------------

struct Account {
    seed: [u8; 32],
    address: SuiAddress,
}

impl Account {
    fn new() -> Self {
        let mut seed = [0u8; 32];
        std::fs::File::open("/dev/urandom")
            .and_then(|mut f| std::io::Read::read_exact(&mut f, &mut seed))
            .expect("random seed");
        let keypair = ed25519::sha512::KeyPair::try_from(seed.as_slice()).unwrap();
        let address = SuiAddress::with_ed25519_pubkey(keypair.public()).unwrap();
        Account { seed, address }
    }

    fn hex(&self) -> String {
        self.address.to_string()
    }

    /// Signs a `Sui.SigningInput` with wallet-core's real `AnySigner`: (unsigned tx, signature) in base64.
    fn sign(&self, mut input: Proto::SigningInput<'static>) -> Result<(String, String)> {
        input.private_key = self.seed.to_vec().into();
        let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
        let out = signer.sign(CoinType::Sui, input);
        if out.error != SigningError::OK {
            return Err(format!(
                "wallet-core: {:?} {}",
                out.error, out.error_message
            ));
        }
        Ok((out.unsigned_tx.to_string(), out.signature.to_string()))
    }
}

fn object_ref(o: &Owned) -> Proto::ObjectRef<'static> {
    Proto::ObjectRef {
        object_id: o.id.clone().into(),
        version: o.version,
        object_digest: o.digest.clone().into(),
    }
}

// ------------------------------------ funding ------------------------------------

#[derive(Clone, Copy, PartialEq)]
enum As {
    Object,
    AddressBalance,
}

/// faucet -> throwaway funder -> `deposits` into `account`. The funding transaction pays gas with a coin
/// and splits the deposits off it; `coin::send_funds` is what puts SUI into an address balance.
fn fund(account: &Account, deposits: &[(u64, As)]) -> Result<()> {
    let funder = Account::new();
    faucet(&funder.hex())?;
    let gas = eventually("faucet coin", || {
        Ok(owned_objects(&funder.hex(), SUI_COIN)?.into_iter().next())
    })?;
    let chain = chain()?;

    let mut inputs = vec![];
    let mut commands = vec![];
    let recipient = CallArg::Pure(bcs::encode(&account.address).map_err(|e| format!("{e:?}"))?);
    inputs.push(recipient);
    for (i, (amount, kind)) in deposits.iter().enumerate() {
        inputs.push(CallArg::Pure(amount.to_le_bytes().to_vec()));
        let amount_input = Argument::Input(inputs.len() as u16 - 1);
        commands.push(Command::SplitCoins(Argument::GasCoin, vec![amount_input]));
        let coin = Argument::Result((commands.len() - 1) as u16);
        let to = Argument::Input(0);
        commands.push(match kind {
            As::Object => Command::TransferObjects(vec![coin], to),
            As::AddressBalance => Command::move_call(
                ObjectID(
                    SuiAddress::from_str(
                        "0x0000000000000000000000000000000000000000000000000000000000000002",
                    )
                    .unwrap()
                    .into_inner(),
                ),
                Identifier::from_str("coin").unwrap(),
                Identifier::from_str("send_funds").unwrap(),
                vec![TypeTag::Struct(Box::new(StructTag {
                    address: SuiAddress::from_str(
                        "0x0000000000000000000000000000000000000000000000000000000000000002",
                    )
                    .unwrap()
                    .into_inner(),
                    module: Identifier::from_str("sui").unwrap(),
                    name: Identifier::from_str("SUI").unwrap(),
                    type_params: vec![],
                }))],
                vec![coin, to],
            ),
        });
        let _ = i;
    }
    let gas_ref: ObjectRef = (
        ObjectID(SuiAddress::from_str(&gas.id).unwrap().into_inner()),
        SequenceNumber(gas.version),
        ObjectDigest::from_str(&gas.digest).map_err(|e| format!("{e:?}"))?,
    );
    let tx = TransactionData::V1(TransactionDataV1 {
        kind: TransactionKind::ProgrammableTransaction(ProgrammableTransaction {
            inputs,
            commands,
        }),
        sender: funder.address,
        gas_data: GasData {
            payment: vec![gas_ref],
            owner: funder.address,
            price: chain.price,
            budget: 50_000_000,
        },
        expiration: TransactionExpiration::None,
    });
    // `SignDirect`: wallet-core signs the bytes as they are.
    let tx_base64 = base64::encode(&bcs::encode(&tx).map_err(|e| format!("{e:?}"))?, STANDARD);
    let input = Proto::SigningInput {
        transaction_payload: Payload::sign_direct_message(Proto::SignDirect {
            unsigned_tx_msg: tx_base64.clone().into(),
        }),
        ..Proto::SigningInput::default()
    };
    let (_, signature) = funder.sign(input)?;
    execute(&tx_base64, &signature)?;

    let want_ab: u64 = deposits
        .iter()
        .filter(|d| d.1 == As::AddressBalance)
        .map(|d| d.0)
        .sum();
    let want_coins: u64 = deposits
        .iter()
        .filter(|d| d.1 == As::Object)
        .map(|d| d.0)
        .sum();
    eventually("account funded", || {
        let coins: u64 = owned_objects(&account.hex(), SUI_COIN)?
            .iter()
            .map(|c| c.balance)
            .sum();
        Ok((address_balance(&account.hex())? == want_ab && coins == want_coins).then_some(()))
    })
}

// ----------------------------------- scenarios -----------------------------------

struct Fixture {
    file: String,
    digest: String,
    scenario: String,
    bytes: Vec<u8>,
}

/// Build + sign with wallet-core, execute, fetch the node's canonical bytes and require they are the bytes sent.
fn run(
    account: &Account,
    scenario: &str,
    file: &str,
    payload: Payload<'static>,
    ab_gas: bool,
) -> Result<Fixture> {
    let chain = chain()?;
    let input = Proto::SigningInput {
        transaction_payload: payload,
        gas_budget: BUDGET,
        reference_gas_price: chain.price,
        valid_during: ab_gas.then(|| Proto::ValidDuring {
            min_epoch: chain.epoch,
            max_epoch: chain.epoch + 1,
            chain: chain.chain_id.clone().into(),
            nonce: 4242,
        }),
        ..Proto::SigningInput::default()
    };
    let (tx, signature) = account.sign(input)?;
    let digest = execute(&tx, &signature)?;
    let canonical = canonical_bcs(&digest)?;
    if canonical != tx {
        return Err(format!("{scenario}: the node serialises it differently than wallet-core\n sent: {tx}\n node: {canonical}"));
    }
    println!("ok  {file}  {digest}  {scenario}");
    Ok(Fixture {
        file: file.to_string(),
        digest,
        scenario: scenario.to_string(),
        bytes: base64::decode(&canonical, STANDARD).map_err(|e| format!("{e:?}"))?,
    })
}

fn main() -> Result<()> {
    let out = std::path::PathBuf::from(std::env::args().nth(1).ok_or("usage: <output dir>")?);
    std::fs::create_dir_all(&out).map_err(|e| e.to_string())?;
    let recipient = Account::new().hex();
    let validator = graphql(
        "{ epoch { validatorSet { activeValidators(first: 1) { nodes { contents { json } } } } } }",
        json!({}),
    )?["epoch"]["validatorSet"]["activeValidators"]["nodes"][0]["contents"]["json"]["metadata"]
        ["sui_address"]
        .as_str()
        .ok_or("no active validator")?
        .to_string();
    let mut fixtures = vec![];

    // Address balance only (the shape of the original bug report): send, stake, unstake.
    let pure = Account::new();
    fund(&pure, &[(4 * ONE, As::AddressBalance)])?;
    fixtures.push(run(
        &pure,
        "send 1 SUI from the address balance only",
        "01-send-address-balance-only.bcs",
        Payload::pay_sui(Proto::PaySui {
            recipients: vec![recipient.clone().into()],
            amounts: vec![ONE],
            address_balance_amount: ONE,
            ..Proto::PaySui::default()
        }),
        true,
    )?);
    fixtures.push(run(
        &pure,
        "stake 1 SUI from the address balance only",
        "02-stake-address-balance-only.bcs",
        Payload::request_add_stake(Proto::RequestAddStake {
            amount: Some(Proto::Amount { amount: ONE }),
            validator: validator.into(),
            address_balance_amount: ONE,
            ..Proto::RequestAddStake::default()
        }),
        true,
    )?);
    let staked = eventually("StakedSui object", || {
        Ok(owned_objects(&pure.hex(), STAKED_SUI)?.into_iter().next())
    })?;
    fixtures.push(run(
        &pure,
        "unstake, gas from the address balance",
        "03-unstake-gas-from-address-balance.bcs",
        Payload::request_withdraw_stake(Proto::RequestWithdrawStake {
            staked_sui: Some(object_ref(&staked)),
            ..Proto::RequestWithdrawStake::default()
        }),
        true,
    )?);

    // A coin object AND an address balance, gas from the address balance.
    let mixed = Account::new();
    fund(
        &mixed,
        &[(2 * ONE, As::Object), (3 * ONE, As::AddressBalance)],
    )?;
    let coin = owned_objects(&mixed.hex(), SUI_COIN)?.remove(0);
    fixtures.push(run(
        &mixed,
        "send 4 SUI out of a 2 SUI coin + 2 SUI withdrawn, gas from the address balance",
        "04-send-coin-plus-address-balance.bcs",
        Payload::pay_sui(Proto::PaySui {
            input_coins: vec![object_ref(&coin)],
            recipients: vec![recipient.clone().into()],
            amounts: vec![4 * ONE],
            address_balance_amount: 2 * ONE,
        }),
        true,
    )?);

    // Gas paid with a coin (no valid_during) and a withdrawal merged into the gas coin.
    let coin_gas = Account::new();
    fund(
        &coin_gas,
        &[(3 * ONE, As::Object), (2 * ONE, As::AddressBalance)],
    )?;
    let coin = owned_objects(&coin_gas.hex(), SUI_COIN)?.remove(0);
    fixtures.push(run(
        &coin_gas,
        "send 2.5 SUI, gas coin plus a 1 SUI withdrawal",
        "05-send-gas-coin-plus-withdrawal.bcs",
        Payload::pay_sui(Proto::PaySui {
            input_coins: vec![object_ref(&coin)],
            recipients: vec![recipient.into()],
            amounts: vec![5 * ONE / 2],
            address_balance_amount: ONE,
        }),
        false,
    )?);

    let mut manifest = vec![];
    for f in &fixtures {
        std::fs::write(out.join(&f.file), &f.bytes).map_err(|e| e.to_string())?;
        manifest.push(json!({ "file": f.file, "digest": f.digest, "scenario": f.scenario }));
    }
    let manifest = serde_json::to_string_pretty(&manifest).unwrap() + "\n";
    std::fs::write(out.join("manifest.json"), manifest).map_err(|e| e.to_string())?;
    println!("wrote {} fixtures to {}", fixtures.len(), out.display());
    Ok(())
}
