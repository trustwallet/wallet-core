---
name: bc-check
description: Audit the current branch's diff for backward-compatibility risk against persisted user data and wire formats. Use when a PR tightens validation, changes deserialization of stored data, modifies a wire/backup format, or changes which exception a public API throws. Outputs a structured markdown report to paste into a `[bc-check: ...]` PR comment.
---

You are auditing the current branch's diff against the configured base branch
(`origin/master` by default) for **backward-compatibility risk against persisted user
data and wire formats**. Find cases where this PR will reject, mis-parse, or
mishandle inputs that older versions of our own software already wrote to disk, to
backups, or onto the network.

**Do not trust the PR description's framing.** "Just a security fix" / "stricter
validation" is precisely the framing that hides this class of bug.

Before starting:
1. Read `docs/bc-footguns.md` if it exists. Every entry is a known constraint.
2. `git diff origin/master...HEAD` to get the full diff.
3. Identify which files are on persistence paths (see WalletCore context below).

---

### WalletCore persistence context

| Path | What lives there | Backup / off-device |
|------|------------------|---------------------|
| `src/Keystore/` | JSON keystore files — user's encrypted private keys and mnemonics | iCloud, Google Drive, manual export |
| `src/proto/*.proto` | Protobuf `SigningInput` / `SigningOutput` wire formats | Potentially stored by app layer |
| `include/TrustWalletCore/TW*.h` | Public C ABI — function signatures, enum values, exception types | SDK consumers, compiled language bindings |
| `registry.json` | Coin metadata: SLIP44 ID, derivation paths, address encoding, curve | Drives every derived address for all wallets |
| `src/Keystore/StoredKey.cpp` | Keystore load / save / migration logic | Same as Keystore above |
| `src/PrivateKey*` | Private key encoding, validation, and signing entry points | Keys serialised into keystore files |
| `src/PublicKey*` | Public key parsing and validation | Derived and stored in account records |
| `src/HDWallet*` | HD wallet derivation — BIP32/39/44 logic | Drives all derived addresses; mnemonic import/export |
| `swift/Sources/KeyStore*` | Swift-layer keystore read/write/migration | Same backup destinations as C++ keystore |
| `swift/Sources/Wallet.swift` | Swift wallet account type | Serialised in Swift keystore JSON |
| `swift/Sources/Watch.swift` | Swift watch-only account type | Serialised in Swift keystore JSON |
| `wasm/src/keystore/` | WASM/TypeScript keystore bindings | Browser-side keystore files (localStorage, export) |

**Blast-radius framing:** a bug in any of these paths means a user cannot decrypt their
wallet or derives wrong addresses → cannot access funds. Even a 0.01 % blast radius is
unacceptable. Treat BLOCKER verdicts as such.

---

Walk the 5 steps below. Cite file:line and commit SHAs everywhere.

### Step 1 — Classify
(a) Tighten input validation, (b) change parsing of a persisted format, (c) change
which exception type a public API throws, (d) remove an `if missing use default`
fallback, or (e) move validation earlier? If none → "No persistence risk", stop.

### Step 2 — Historical baseline
For each tightened rule:
- Could a prior version have *produced* data that violates the new rule? `git log -p`
  / `git blame` going back ≥2 years. Cite SHAs.
- Was there a partial migration ("regenerate on next user action")? Assume a
  non-trivial fraction of users **never triggered it**.
- Does the format ever leave the device? Backup / export / sync.

### Step 3 — Concrete failure scenarios
For every "yes": old client version → action that produced the data → where it
lives → exact upgrade code path (file:line) → user-visible symptom → blast radius.
No hand-waving.

### Step 4 — Red-flag checklist
Answer yes/no with file:line evidence:

**Generic:**
- New `throw` on read/load/decode/import path?
- Removed `if (field missing) use default`?
- New length/range/enum check on >1-year-old field?
- Constructor: lenient parse → parse + validate?
- Changed which exception type a public API throws?
- Format ever in backup/export/sync?
- Prior PR shipped "regenerate on next user action" partial migration? (cite SHA)

**WalletCore-specific:**
- Proto: field number reused or removed from an existing message?
- Proto: field type changed (e.g. `bytes` → `string`) or `required` added to existing field?
- Proto: enum value renumbered or removed?
- Keystore: JSON key renamed, removed, or made required without a default fallback?
- Keystore: `validate()` now called from the JSON-load constructor for the first time?
- Registry: `slip44`, `curve`, or address-encoding field changed? (changes derived addresses)
- Registry: `TWCoinType` enum value reordered or removed?
- C ABI: `TW*` function removed, renamed, or its return / exception behaviour changed?

### Step 5 — Required mitigations
- Accept legacy at read time, normalize on write *(preferred)*.
- Gate strict check behind "newly created" flag.
- One-time migration with explicit failure UX.
- Apply tightening to write paths only.

### Output format
```
# BC-risk audit — <branch> @ <git SHA> — <ISO timestamp>

## Verdict
<SAFE / RISK / BLOCKER> — one sentence.

## Step 1 — Classification
...
## Step 2 — Historical baseline
...
## Step 3 — Concrete failure scenarios
...
## Step 4 — Red-flag checklist
...
## Step 5 — Required mitigations
...

## Suggested PR comment
[bc-check: <Pass|Mitigated|N/A>]
<one-paragraph summary>

## Suggested addition to docs/bc-footguns.md
<draft entry, OR "none">
```

The skill never auto-posts to GitHub. Author copies the verdict + suggested comment
into the PR; the human signs off.