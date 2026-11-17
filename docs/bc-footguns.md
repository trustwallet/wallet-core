# BC Footguns

Known backward-compatibility constraints for WalletCore. Every entry records a real incident or investigated risk so future audits can skip re-deriving it.

---

## TWStoredKeyFixEncryption return type (PR #4787, 2026-06)

`TWStoredKeyFixEncryption` changed return type from `bool` to `struct TWResultVoid*`.
Old compiled binaries reading the return as `bool` will always see `true` (pointer ≠ 0),
causing a silent no-op when the password is wrong. No keystore data is corrupted.

**Rule:** Any future change to a `TW*` function's return type (bool → pointer, pointer → enum, etc.)
is a C ABI breaking change and requires a version bump + release note entry.
---

## Scrypt `dklen` is rejected unless 32 at load (PR #4897, 2026-09)

`ScryptParameters::validate()` requires `desiredKeyLength == 32` and runs from the JSON
constructor. wallet-core has only ever written 32 (`a4223c31f`, 2019), but `decrypt()` never reads
`dklen` and the MAC window `dk[16:32]` is prefix-consistent under scrypt, so a third-party file
declaring `dklen > 32` decrypted before this PR and is rejected at load after it. No known emitter
writes another value; ethers.js derives 64 but writes 32.

**Rule:** any further tightening of `kdfparams` must distinguish "wallet-core never wrote it" from
"no third-party file decrypts with it". Only the second makes a load-time rejection safe. If a
`dklen > 32` emitter is ever found, relax to `>= 32` at read and normalize to 32 on write.

---

## nlohmann integer storage types (PR #4897, 2026-09)

nlohmann stores a non-negative integer parsed from text as `number_unsigned`, but a value
assigned in memory from a signed literal (`j["n"] = 16384`) as `number_integer`. A check written as
`is_number_unsigned()` rejects the in-memory form while accepting the same document after a
`dump()`/`parse()` round trip. Use `Keystore/JsonParsing.h` (`parseUnsigned` / `parseU32`), which
accepts both and rejects negatives, floats, strings, booleans and null.

---

## WASM null returns don't trap (PR #4950, 2026-11)

In WebAssembly, address 0 is ordinary readable memory, so dereferencing a null `TWString*`,
`TWData*` or object pointer does not crash the way it does natively. Before #4950 the embind
bindings ignored `_Nullable`, and a failing call did whatever the bytes at address 0 led to:
`Base32.decode("!!!")` aborted the module, `EthereumAbi.getFunctionSignature("not an abi")`
returned `""`, `storedKey.account(99)` returned a wrapper whose `.address()` was `""`, and
`accountForCoin(coin, null)` went out of bounds. JS callers could have depended on the silent
empty values, so #4950 (all `_Nullable` returns → JS `null`, `.d.ts` typed `T | null`) is a
breaking change for the npm package even though it only fixes crashes and garbage.

**Rule:** a `_Nullable` C return must map to JS `null` in the WASM bindings, never to an empty
value, and the generated `.d.ts` must say `| null`. Making an existing `TW*` function `_Nullable`
(or changing how the bindings surface null) changes the JS runtime behavior and TypeScript types
of that function, so treat it as a breaking change for the npm package and list it in the
release notes. Native tests will not catch it: check the WASM tests too.
