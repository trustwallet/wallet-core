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

## Scrypt cost parameters are capped at load (PR #4899, 2026-11)

`ScryptParameters::validate()` now also rejects `log2(n) >= 19 * r`, total scrypt memory
`128*r*n + 128*r*p + 256*r + 64 > 512 MiB` (`maxScryptMemory`), and `n * r * p > 2^25`
(`maxScryptWork`, 16x the Standard preset, ~16 s to open). Before, only the 4 GiB `uint32_t`
overflow bound applied, so a third-party file with larger parameters decrypted before this PR and
fails to load after it. This breaks the rule above on purpose: the caps exist to stop an imported
file from forcing a multi-GiB allocation or hours of CPU, and no cap can do that without rejecting
some file that used to open. Boundary shapes: `n=2^18, r=8, p=1` (Standard, geth, `livepeer.json`)
and `n=2^18, r=1, p=8` pass; `n=2^19, r=8, p=1` (needs just over 512 MiB) and `n=2^19, r=1` are
rejected. The memory sum covers `B` and `XY` as well as `V`, since `n=2, r=1, p=33554431` has a tiny
`V` but a 4 GiB `B`.

`EncryptedPayload` re-runs `validate()` before encrypting, so a caller that builds `ScryptParameters`
by hand with such values now gets `std::invalid_argument`. The preset levels (minimal, weak,
standard) are all well inside the caps, so `StoredKey::create*` is unaffected.

**Rule:** the 512 MiB and 2^25 limits are the new contract. Raise them only if a real emitter above
them turns up, and raise the work and memory caps together, because each alone leaves the other
as the way in. Do not tighten them: the Standard preset already uses half of the memory cap.
