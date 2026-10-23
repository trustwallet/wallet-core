# SIP-58 transactions executed on Sui devnet

Each `*.bcs` file is the `TransactionData` BCS **as the Sui node serialises it** (GraphQL
`transaction(digest) { transactionBcs }`) for a transaction that wallet-core built and signed with a throwaway
ed25519 key and that was **executed successfully on Sui devnet**. `manifest.json` lists scenario and digest.

| File | Shape |
|---|---|
| `01-send-address-balance-only` | `PaySui`, no coins, withdrawal = amount, gas from the address balance |
| `02-stake-address-balance-only` | `RequestAddStake`, withdrawal, gas from the address balance |
| `03-unstake-gas-from-address-balance` | `RequestWithdrawStake` (owned `StakedSui`), gas from the address balance |
| `04-send-coin-plus-address-balance` | `PaySui`, owned coin + withdrawal merged, gas from the address balance |
| `05-send-gas-coin-plus-withdrawal` | `PaySui`, gas coin + withdrawal merged into `GasCoin`, no `ValidDuring` |

`devnet_sip58_roundtrip.rs` decodes them with our `TransactionData` types and encodes them again: the bytes must be
identical, i.e. variant indexes and field order are what the chain accepts.

- Generated: 2026-10-22 on Sui devnet. The accounts are throwaway keys funded from the public devnet faucet: no real
  wallet, no secrets, no third-party data. Devnet is wiped regularly, so the digests stop resolving; the files are
  the record.

## Regenerate

The generator is not part of the repository: a public network brings flakiness, and the project does not commit
scripts that talk to a network. To refresh the fixtures, write a throwaway script that builds the transactions with
wallet-core (`AnySigner`, `Sui.proto`), asks the node (GraphQL `transactionBcs`) for the canonical bytes of the
executed transactions, and keep the transaction only if those bytes equal what wallet-core produced. Copy the `.bcs`
files here and run:

```bash
cd rust
cargo test -p tw_sui --test devnet_sip58_roundtrip
```

A new run yields different transactions (new keys, epoch, nonce) of the same shapes.

## Test coin (tokens held in the address balance)

`test_coin/` is our own Move package: an open-mint coin (`TEST_COIN`, shared `TreasuryCap`), so tests need no key to mint.
Publish and mint (devnet only, throwaway):

```bash
cd test_coin && sui client switch --env devnet && sui client faucet
sui client test-publish --build-env devnet --gas-budget 200000000      # prints package id + shared TreasuryCap id
sui client ptb --move-call <PKG>::test_coin::mint @<TREASURY> 5000000 --assign c \
  --move-call 0x2::coin::send_funds "<<PKG>::test_coin::TEST_COIN>" c @<ADDRESS>   # into the ADDRESS BALANCE
```

The tsdk devnet test reads the ids from `SUI_DEVNET_TEST_COIN_PACKAGE` / `SUI_DEVNET_TEST_COIN_TREASURY`.
Devnet is wiped periodically: republish when the ids stop resolving.
