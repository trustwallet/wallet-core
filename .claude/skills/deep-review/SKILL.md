---
name: deep-review
description: Deep code review of the current branch with P0/P1/P2 issues, architecture validation, unit test suggestions, and a bundled `/simplify` cleanup pass. Use when the user asks to review code, check quality, or validate before PR.
user-invocable: true
---

# Deep Code Review

Performs a structured code review of the current branch against `master`. Produces a prioritized inline report with findings categorized as P0/P1/P2 across five lenses (architecture, code quality, logic, performance, security), plus unit test suggestions. After analysis, the bundled `/simplify` skill is invoked unconditionally in Step 12a to handle simplification & reuse. For reviews with >15 findings, the report can be written to `~/.claude/reviews/` instead (see Step 11).

## When to Use

Activate this skill when:
- User asks to "review code", "review my changes", or "deep review"
- User asks to "check quality" or "validate before PR"
- User wants a structured code review of their branch

## Instructions

### 1. Accept Optional Context

The user may provide:
- A ticket description or requirements
- A PR description
- Any extra context about what the changes should accomplish

If none given, proceed without it. Store whatever is provided for reference in the report header.

### 2. Identify Base Branch and Gather Diff

Default base is `master`:

```bash
if git rev-parse --verify origin/master >/dev/null 2>&1; then
  BASE="origin/master"
elif git rev-parse --verify master >/dev/null 2>&1; then
  BASE="master"
else
  echo "No master branch found" >&2
  exit 1
fi

MB=$(git merge-base "$BASE" HEAD)
COMMITS_AHEAD=$(git rev-list --count "$MB"..HEAD)
echo "Base: $BASE ($COMMITS_AHEAD commits ahead)"
```

Use the detected `{base}` for all subsequent diff commands:

```bash
git diff {base}...HEAD
git diff {base}...HEAD --stat
git diff {base}...HEAD --shortstat
git log --oneline {base}..HEAD
```

If the diff is empty, inform the user and stop.

#### 2.1 Evaluate PR Size

| Size | Action |
|------|--------|
| ≤400 lines | Proceed with review |
| 401–999 lines | If changes mix concerns (e.g., refactor + feature, multiple unrelated fixes), suggest splitting before review. If cohesive, proceed. |
| ≥1000 lines | Strongly recommend splitting — large PRs are harder to review and more likely to contain hidden issues. |
| >15 files changed | Suggest splitting even if line count is low. |

If a split is recommended, use `AskUserQuestion` to ask the user whether to split first or continue. If they choose to split, stop. If they choose to continue, proceed with the review.

### 3. Detect Affected Layers

Inspect changed file paths to determine which layers are affected:

| Path pattern | Layer | Key concerns |
|---|---|---|
| `include/TrustWalletCore/` | C ABI / public API | Breaking signature changes; nullability; `TW_EXPORT_*` macro |
| `src/interface/` | C interface implementations | Exception safety at ABI boundary; memory ownership |
| `src/` (non-interface) | C++ core | Exception correctness; memory; correctness |
| `src/proto/` | Protobuf definitions | Must run `tools/generate-files native` after any change |
| `rust/` | Rust layer | FFI safety; `into_ptr`/`from_ptr` ownership; null handling |
| `swift/Sources/Generated/` | Swift bindings | Must not be hand-edited — regenerate with `tools/generate-files ios` |
| `kotlin/wallet-core-kotlin/src/*/generated/` | Kotlin/KMP bindings | Must not be hand-edited — regenerate with `tools/generate-files android` |
| `wasm/src/generated/` | WASM C++ bindings | Must not be hand-edited — regenerate with `tools/generate-files wasm` |
| `wasm/tests/` | WASM/TypeScript tests | Explicit `.delete()` required on all WASM objects |
| `tests/` | C++ tests | GoogleTest; `WRAP`/`WRAPS`/`WRAPD` macros for auto-delete |
| `rust/tw_tests/` | Rust integration tests | Per-chain test files |
| `registry.json` | Coin metadata | Run `tools/generate-files` after change; verify SLIP44, curve, derivation path |
| `codegen/` or `codegen-v2/` | Code generator | Changes affect all generated files in all layers |
| `android/app/src/androidTest/` | Android instrumented tests | Requires connected device/emulator to run |
| `swift/Tests/` | iOS/Swift tests | Run with `tools/ios-test` |

Read ALL changed files before writing findings — cross-layer issues (broken C ABI contract, generated file drift, missing binding updates) can only be found by reading everything first.

### 4. Read ALL Changed Files and Their Context

Read every changed file in its entirety before writing any findings. Then identify files that import or reference the changed files — especially consumers of modified C headers, public C++ classes, and Rust FFI functions. Read those too.

Pay special attention to:
- All language binding layers that consume a changed C header function
- All coin `Entry.h` implementations if `CoinEntry.h` was changed
- Generated files that correspond to changed source files (e.g., if `src/interface/TWFoo.cpp` changed, check `swift/Sources/Generated/Foo.swift`)

### 5. Analyze Per Layer

Review across five lenses for each affected layer.

**Architecture compliance**
- C ABI contract: `TW*` functions in `include/TrustWalletCore/` are the public API surface — never break signatures silently
- Generated files (`swift/Sources/Generated/`, `kotlin/.../generated/`, `wasm/src/generated/`, JNI `cpp/generated/`) must not be hand-edited; changes belong in codegen templates or source definitions
- Coin plugin system: new coins must register in all three `// #coin-list#` locations in `src/Coin.cpp` (include, static instance, dispatcher `case`)
- Rust delegation: chains migrated to Rust must go through `src/rust/RustCoinEntry.h`; C++ entry points for migrated chains should only delegate, not contain crypto logic
- Proto files in `src/proto/` are source of truth; generated `.pb.h`/`.pb.cc` files must never be manually edited
- After any change to `registry.json`, `.proto` files, or `include/TrustWalletCore/` headers, `tools/generate-files` must be run

**Code quality**
- Exception safety at C ABI boundary: every `TW*` function body must have a `catch(...)` that returns a safe null/false/error value — an uncaught exception across the C boundary is UB
- Memory ownership: objects returned by `Create*`/`Import*`/`TWResultVoidCreate*` functions are caller-owned and must be deleted; document this in the header
- WASM objects: every object returned from a WASM binding must be `.delete()`-d after use; inline chaining (e.g. `foo().bar()`) leaks the intermediate object
- JNI string/data ownership: `TWStringJString` and `TWDataJByteArray` consume (delete) their input pointer — do not use the pointer after calling them
- Rust FFI ownership: `into_ptr()` transfers heap ownership to the caller; `from_ptr()` reclaims it (and drops). A function that takes `Nonnull<T>` must guard with `from_ptr_as_ref` and handle `None` — never dereference directly
- Null safety: `_Nullable` return values must be null-checked at every call site before use
- Thread safety: `StoredKey` and `HDWallet` are not thread-safe; concurrent access without synchronization is a bug

**Logic correctness**
- Cryptographic correctness: changes to `decrypt()`, `sign()`, `deriveAddress()`, `privateKey()`, `validateAddress()` need extra scrutiny — subtle bugs here cause fund loss
- Scrypt/PBKDF2 parameter changes: validate against RFC 7914 / web3 standards; do not regress security defaults
- MAC verification must happen before decryption (encrypt-then-MAC)
- Atomicity of multi-step operations: if step 1 succeeds but step 2 fails, the object must not be left in a partially-mutated state
- Proto serialization round-trips: if a `SigningInput` is built and serialized, verify field names and types match the `.proto` definition

**Security**
- Private key material must never appear in error messages, log output, or `assert` strings — even partially
- `e.what()` passed through to callers: ensure it cannot contain secrets (e.g., never pass raw key bytes through `std::runtime_error`)
- Input validation on JSON and external data at parsing boundaries (`loadJson`, `importJSON`, JSON proto decoding)
- `TWResultVoid` error messages exposed to callers must not leak internal key state
- Verify that wrong-password errors consistently return `"Invalid password: MAC verification failed"` — no variance that could be exploited as an oracle
- C++ exceptions must not propagate across the C ABI (`extern "C"`) — wrap all `TW*` functions

### 6. Check Git History for Repeated Patterns

Review `git log --oneline {base}..HEAD`. If the same kind of fix appears multiple times (e.g., adding missing `catch(...)`, adding `.delete()` calls, fixing null checks), flag it as a pattern risk — the same mistake may exist in other files touched on this branch.

### 7. Validate Cross-Layer API Contracts

When a C header (`include/TrustWalletCore/TW*.h`) changes a public function:

1. **Identify the change type:**
   - Return type changed (e.g., `bool` → `struct TWResultVoid*`)
   - Parameter added, removed, or reordered
   - Nullability annotation changed (`_Nonnull` ↔ `_Nullable`)
   - New function added

2. **Check each binding layer for consumers and updates:**

   | Binding layer | Generated file pattern | Regenerate command |
   |---|---|---|
   | Swift | `swift/Sources/Generated/{Name}.swift` | `tools/generate-files ios` |
   | Kotlin common | `kotlin/.../commonMain/generated/.../{Name}.kt` | `tools/generate-files android` |
   | Kotlin Android/JVM | `kotlin/.../commonAndroidJvmMain/generated/.../{Name}.kt` | same |
   | Kotlin iOS | `kotlin/.../iosMain/generated/.../{Name}.kt` | same |
   | Kotlin JS | `kotlin/.../jsMain/generated/.../{Name}.kt` | same |
   | JNI C | `kotlin/.../cpp/generated/{Name}.c` | same |
   | WASM C++ | `wasm/src/generated/{Name}.cpp` | `tools/generate-files wasm` |
   | C++ generated wrapper | `src/Generated/TW{Name}.cpp` | `tools/generate-files native` |

3. **Classify impact:**

   | Change | Binding updated? | Priority |
   |---|---|---|
   | Return type changed | No | **P0** — build failure or silent type mismatch |
   | Parameter added/removed | No | **P0** — build failure |
   | New function without binding | N/A | **P1** — function unreachable from language SDKs |
   | Nullability changed | No | **P1** — null safety assumptions broken |
   | Added to `.gitignore` without committing | N/A | **P1** — generated file missing from repo |

4. **For each finding:** include the header change (file, line), the binding file that needs updating (file, line), and a concrete fix suggestion.

### 7.5. Verify Secret Zeroization

Run this step whenever the diff touches any of: `src/Keystore/`, `src/PrivateKey.*`, `src/HDWallet.*`, `src/interface/TW*`, `rust/tw_keypair/`, or any file that calls `decrypt()`, `sign()`, `privateKey()`, or a KDF function (`scrypt`, `pbkdf2_hmac_sha256`).

**What counts as secret material:**
- KDF output (Scrypt / PBKDF2 derived key bytes)
- Decrypted private key bytes
- Mnemonic phrases when decrypted or copied
- AES cipher context structs (`aes_encrypt_ctr_ctx`, `aes_decrypt_ctr_ctx`, etc.)
- Any intermediate buffer produced by a signing operation that contains key-derived data

#### C++ checks

1. **RAII is preferred.** Every local `Data` or `std::string` holding secret material should be wrapped in `ZeroizingData` / `ZeroizingString` (from `src/memory/memzero_wrapper.h`). RAII guarantees zeroization on all exit paths, including exceptions. A bare `Data derivedKey` that relies on a manual `memzero` at the end of the function is fragile — if a new exception path is added later, the zeroization is silently skipped.

   ```cpp
   // Preferred — zeroized even if an exception is thrown midway
   ZeroizingData derivedKey(Data(scryptParams.desiredKeyLength));

   // Fragile — misses any early-return or exception path added later
   Data derivedKey(scryptParams.desiredKeyLength);
   // ... work ...
   memzero(derivedKey.data(), derivedKey.size()); // ← skipped on throw
   ```

2. **Manual `memzero` calls are acceptable but must cover every exit path.** Scan all `return` statements and `throw` sites in the function. If any exit path is reached before a `memzero` call on a secret local, flag it as **P1**.

3. **AES context structs must be zeroed.** Any function that calls `aes_encrypt_key*` or `aes_decrypt_key*` must call `memzero(&ctx, sizeof(ctx))` before the context goes out of scope, including on every exception path.

4. **Flag these antipatterns:**

   | Antipattern | Priority | Preferred fix |
   |---|---|---|
   | `Data key = decrypt(...)` instead of `ZeroizingData key(decrypt(...))` | **P1** | Wrap in `ZeroizingData` |
   | `std::string mnemonic = ...` for decrypted mnemonic | **P1** | Wrap in `ZeroizingString` |
   | `memzero` present on success path but missing from at least one `throw` path | **P1** | Add `memzero` before every `throw`, or switch to RAII |
   | AES `ctx` not zeroed | **P1** | `memzero(&ctx, sizeof(ctx))` after use |
   | Secret passed as `const Data&` into a function that copies it into a plain `Data` local | **P2** | Receiver should use `ZeroizingData` |

#### Rust checks

1. **New private key structs must derive `ZeroizeOnDrop`.** Any struct that holds raw private key bytes must have `#[derive(ZeroizeOnDrop)]` from the `zeroize` crate. Without it, the bytes are not cleared when the struct is dropped, even if the `Drop` impl is otherwise correct. Example from existing code:
   ```rust
   #[derive(ZeroizeOnDrop)]
   pub struct PrivateKey { bytes: Vec<u8> }
   ```

2. **Functions returning private key bytes must return `Zeroizing<Vec<u8>>`.** The `ToBytesZeroizing` trait (in `tw_misc::traits`) wraps the return in a `zeroize::Zeroizing<Vec<u8>>`, which zeroes the buffer when dropped. Returning a plain `Vec<u8>` for key bytes is a **P1** — callers can't guarantee the buffer is zeroed when they're done.

3. **Intermediate `Vec<u8>` buffers holding key-derived bytes** in signing or derivation logic should be wrapped in `zeroize::Zeroizing<Vec<u8>>`.

4. **Flag these antipatterns:**

   | Antipattern | Priority | Preferred fix |
   |---|---|---|
   | Private key struct without `#[derive(ZeroizeOnDrop)]` | **P1** | Add the derive |
   | Function returning raw private key bytes as `Vec<u8>` | **P1** | Return `Zeroizing<Vec<u8>>` |
   | `let key_bytes: Vec<u8> = ...` holding derived key material | **P2** | `let key_bytes = zeroize::Zeroizing::new(...)` |

#### Skip condition

If the diff does not touch any file that handles raw key bytes, KDF output, or mnemonic data, skip this step silently.

### 8. Categorize Issues

| Priority | Criteria |
|---|---|
| **P0 Critical** | Crash, fund loss, private key exposure, UB across C ABI, fundamental architecture violation. Must fix before merge. |
| **P1 Important** | Logic error, missing edge case, memory leak, broken cross-layer contract, missing `catch(...)` at ABI boundary, WASM object not deleted. Should fix. |
| **P2 Minor** | Naming, readability, style, doc comment accuracy, missing newline at EOF. Fix if time permits. |

**Decision guide:**
- Crash, UB, fund access risk, or key exposure → P0
- Incorrect behavior, memory leak, broken binding → P1
- Meaningful quality improvement → P2
- Trivial nitpick or personal preference → don't report

Be strict but honest. Do not manufacture issues. If the code is solid, say so.

### 9. Identify Unit Test Opportunities

For each changed layer, suggest tests using the appropriate framework and location:

| Layer | Framework | Location |
|---|---|---|
| C++ core | GoogleTest | `tests/common/{Module}/` or `tests/chains/{Blockchain}/` |
| C ABI interface | GoogleTest + `WRAP`/`WRAPS`/`WRAPD` | `tests/interface/TW{Name}Tests.cpp` |
| Rust FFI | `#[test]` in a `tests/` file | `rust/tw_{crate}/tests/` |
| Rust chain | integration test | `rust/tw_tests/tests/chains/{blockchain}/` |
| WASM/TS | mocha + chai | `wasm/tests/` |
| Swift | XCTest | `swift/Tests/` |
| Android | JUnit/Instrumented | `android/app/src/androidTest/` |

Each suggestion must include:
1. What to test (function/method/flow)
2. Which scenarios to cover (success, wrong password, malformed input, null input, edge cases)
3. A skeleton following the target framework's conventions (e.g., `WRAP` macro for C interface tests, `unsafe` block for Rust FFI tests)

### 10. Present Review

Default: output the full review inline in chat. **Escape hatch:** if P0+P1+P2 count exceeds 15, use `AskUserQuestion` to offer writing the full report to `~/.claude/reviews/wallet-core/{branch-slug}-{YYYY-MM-DD}.md` and posting only a summary inline. Default to inline if the user declines.

Report structure:

**Header:** branch name, date, base, commit count, detected layers, context summary.

**Summary:** 2-3 sentence overall assessment + P0/P1/P2 counts table.

**P0 Critical section:** Each issue with file path + line, impact, description, and concrete code fix. If none, state "No issues found."

**P1 Important section:** Same format as P0. If none, state "No issues found."

**P2 Minor section:** Lighter format — file path + line and brief description with suggestion. If none, state "No issues found."

**Unit Test Suggestions:** What to test, scenarios, and skeleton code per layer conventions.

**Positive Observations:** Only call out non-obvious judgment calls — an elegant solution to a hard problem, a deliberate defensive pattern that prevents a class of bugs, a subtle crypto invariant correctly preserved. Skip generic praise. Omit the section entirely if nothing qualifies.

### 11. Offer to Apply Fixes and Tests

After presenting the review, if there are P0/P1/P2 findings or test suggestions, build the option list (include only tiers with at least one finding, add "Add suggested unit tests" only if suggestions exist):

- **Apply P0 fixes**
- **Apply P1 fixes**
- **Apply P2 fixes**
- **Add suggested unit tests**

**If 2 or more entries:** use `AskUserQuestion` with `multiSelect: true`.
**If exactly 1 entry:** use `AskUserQuestion` as yes/no — `[Yes, apply <that tier>]` and `[No, skip]`.

Execution order:
1. P0 → P1 → P2 fixes (skip any tier not selected)
2. Unit tests after all fix tiers
3. Run `/simplify` per Step 11a — always, even on review-only runs

### 11a. /simplify post-fix cleanup

Unconditionally invoke the `/simplify` skill, scoped to the entire branch diff vs base — not just files modified during this deep-review session.

Recompute the branch-diff file list:

```bash
git diff --name-only --diff-filter=ACMRT $(git merge-base {base} HEAD)..HEAD
```

Pass these constraints to `/simplify`:
- **Scope override:** Review the entire branch diff vs base, not just files modified in this session. The file list below is authoritative.
- **File list:** `{file-list}` — paste the output of the command above.
- Do NOT edit shared/helper code that isn't in the file list. Leave any shared-code observations as notes only.
- Preserve functionality.

If `/simplify` made changes, note them. If it made no changes, proceed. If it errors, report and continue.

### 12. Create Deep Review Verification Commit

After the review is complete (regardless of whether fixes were applied), create a verification commit:

```bash
git commit --allow-empty -m "deep-review: completed

Deep-Review-By: claude-code"
```

### 13. Suggest Next Steps

- If a PR already exists: "Push with `git push` to update the PR."
- If no PR exists: "Run `gh pr create` to open a PR for this branch."
- If generated files may need regeneration: "Run `tools/generate-files native` (and `ios`/`android`/`wasm` as applicable) and commit the result."

## Key Rules

- Read ALL changed files BEFORE writing any findings
- Every P0 and P1 must include a concrete code fix suggestion
- Do NOT run build commands during review
- Most of the `TW*` C interface function need `catch(...)` — flag any that don't
- Every WASM object returned from a binding must be `.delete()`-d — flag inline chains that skip this
- Hand-edits to generated files (`Generated/`, `swift/Sources/Generated/`, `kotlin/.../generated/`, `wasm/src/generated/`) are always a finding unless the change is purely additive and the generated path is gitignored
- Cross-layer awareness: if a C header changes, check every binding layer for matching updates
- Be strict but honest — acknowledge good code, don't inflate issue counts