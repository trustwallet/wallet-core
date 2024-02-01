# Documentation for registry.json

The file `registry.json` contains meta info about supported blockchains.
It is the input for some generated source files and documentation ([registry.md](registry.md)), and values from it are used during runtime.

## Fields

**`id`**
Internal ID of the chain. Lowercase letters only.  Should be never changed.
Ex.: `'bitcoin'`.

**`name`**
More readable name, can include lower/uppercase, space.
Ex.: `'Bitcoin'`.

**`displayName`**
Optional, if present, overrides **name** for places where it is visible to the user. 
Ex.: `'BNB Beacon Chain'`.

**`coinId`**
Internal numerical ID for the chain. In most cases it is the ID used in BIP-44 address derivation.
See quasi-standard repository here:  https://github.com/satoshilabs/slips/blob/master/slip-0044.md
Ex.: `0` for Bitcoin, `60` for Ethereum.

Some typical special cases:

- Multiple chains/coins with the same ID: adding value `10000000` to distinguish.  Possible multiple times.
Ex.: `10000118` for Osmosis, `118` for Cosmos; `20000714` for BNB Smart Chain.
- Ethereum-clone chains with no own BIP-44 ID: use the `10000000 + chainID` as coinID.

See also: `slip44` and `chainId`.

**`slip44`**
Optionally, SLIP-44 (BIP-44) coin ID can be specified here, in case it differs from `coinId`.  In most cases the two are the same, so this can be omitted.
Ex.: `60` for Optimism (coinID is `10000070`).

**`symbol`**
Symbol of the native coin.  Typically a short, upper-case-only string.
Ex.: `BTC`, `ETH`.

**`decimals`**
Number of decimals in coin amounts.  Amounts are typically expressed as (large) integer values, with all decimal values, like `100000` for `0.00100000` BTC (decimals=8).
Ex.: `8` for Bitcoin, `18` for Ethereum.

**`blockchain`**
Some chains are very similar and share the implementation code.
This flag is used to direct logic to the right implementation.
Ex. `'Cosmos'` for Oasis, as Oasis is handled by Cosmos implementation.

**`derivation`**
Defines properties for address derivation, most importantly derivation paths.

Typically only one derivation is supported per chain, in this case the definition looks like:

```
  "derivation": [
    {
      "path": "m/44'/60'/0'/0/0"
    }
  ],
```

It contains the derivation path used to derive private key (and address) from the wallet mnemonic (HD wallet).
Derivation path is usually well known for a chain implementation, and different wallet implementations use the same path (so cross-import is possible).
Note that the second number, the BIP-44 ID, usually matches the coinId.

Some blockchains may support additional alternative derivations.  These have:

- a name
- an alternative derivation path (optional)

Derivation may differ in the derivation path, or by address generation method (based on the derivation name).
The first derivation is considered the default.

Examples:
Bitcoin uses Segwit address by default, but also supports earlier P2PKH addresses:

```
    "derivation": [
      {
        "name": "segwit",
        "path": "m/84'/0'/0'/0/0",
        "xpub": "zpub",
        "xprv": "zprv"
      },
      {
        "name": "legacy",
        "path": "m/44'/0'/0'/0/0",
        "xpub": "xpub",
        "xprv": "xprv"
      }
    ],
```

Solana supports two derivations, which differ in derivation path:

```
    "derivation": [
      {
        "path": "m/44'/501'/0'"
      },
      {
        "name": "solana",
        "path": "m/44'/501'/0'/0'"
      }
    ],
```

**`xpub` and `xprv`**
Defines the XPub and XPriv format used, Bitcoin-style. Defined inside the derivation section (as they may differ per derivation).

**`curve`**
Defines the elliptic curve used in private-public key generation and signing.
Ex.: `'secp256k1'` for Bitcoin and Ethereum, `'ed25519'` for Polkadot.

**`publicKeyType`**
The type of public key used.
Ex.: `'secp256k1'` for Bitcoin, `'secp256k1Extended'` for Ethereum.

**`staticPrefix`**
Optional byte prefix, used in some Bitcoin-like chains.
Ex.: `7` for Decred.

**`p2pkhPrefix` and `p2shPrefix`**
Defines the prefix byte used in P2PKH and P2SH addresses, Bitcoin style.
Ex. `0` and `5` for Bitcoin.

**`hrp`**
Human Readable Prefix used to prefix an address, used to indicate type of address, to minimize risk of accidental address mix-up across chains.
Ex. `'bc'` for Bitcoin, `'cosmos'` for Cosmos.

**`chainId`**
Chain identifier, used by forks, e.g. in case of Ethereum (a decimal number), or Cosomos (a string ID).
Chain identifier, in case of Ethereum it's a constant decimal number;
for Cosmos, it's a dynamic string network id (usually changes with network upgrades).

Please note the chain id might not be always latest in registry.  In transaction building current value has to be supplied each time.

Ex.: `'1'` for Ethereum, `'61'` for Ethereum Classic, `'osmosis-1'`for Osmosis.

**`publicKeyHasher`**
Hash method used in XPub derivation.
Default is `sha256ripemd`.
Ex.: `'sha256ripemd'` for Bitcoin, `'blake256ripemd'` for Decred.

**`base58Hasher`**
Hash method used in extended private and public key derivation, for checksumming within Base58 addresses.
Default is `sha256d`.
Ex.: `'sha256d'` for Bitcoin, `'blake256d'` for Decred.

**`addressHasher`**
Hash method used in the publicKey -> address generation.
Only some chain implementations use this setting, in most implementations this is fixed (and value here is only informative).
Default is `sha256ripemd`.
Ex.: missing ('sha256ripemd') for Bitcoin, `'keccak256'` for Ethereum, `'sha256ripemd'` for Cosmos, `'keccak256'` for Native Evmos, despite being a Cosmos fork.

**`explorer`**
Explorer web service for this chain. Sub-fields are used so that full URLs can be built for any address or transactions.
Note that the sample values should include existing IDs, so that the resulting full URL is valid.

Example:

```
  "explorer": {
    "url": "https://blockchair.com",
    "txPath": "/bitcoin/transaction/",
    "accountPath": "/bitcoin/address/",
    "sampleTx": "0607f62530b68cfcc91c57a1702841dd399a899d0eecda8e31ecca3f52f01df2",
    "sampleAccount": "17A16QmavnUfCW11DAApiJxp7ARnxN5pGX"
  },
```

This results in the full URL for the sample address:
https://blockchair.com/bitcoin/address/17A16QmavnUfCW11DAApiJxp7ARnxN5pGX
which is a working URL.

Beware of the starting-ending slashes used.

**`info`**
Section with project info:

**`info/url`**
Main project website.

**`info/source`**
Link to the default implementation of the node or RPC gateway that can be used by a wallet.

**`info/rpc`**
Optional URL to an available public RPC service.

**`info/documentation`**
Main project documentation site/subsite.

**`deprecated`**
If set to `true`, the project is considered deprecated: its info is kept here, but it will not be supported.
Ex. `'true'` for Kin.
