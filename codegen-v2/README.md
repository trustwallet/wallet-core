# About

This is a _work-in-progress_ parser meant to deprecate the existing Ruby parser
in `codegen/`. This project will progess over multiple stages (PRs).

- [x] Write a (minimal) parser with full coverage of the C headers in `include/` ([#3065](https://github.com/trustwallet/wallet-core/pull/3065)).
- [ ] Implement a templating engine, write easy to read templates.
	- [ ] Start with Swift codegen, replicate existing API (no breakage).
	- [ ] Complete codegen for remaining languages.
- [ ] Extend the parser to handle all valid C header syntax.
	- [ ] Add explicit error types (right now it's just `Error::Todo`).
	- [ ] Seperate parser logic from C header grammer (no refactoring required, just reorganization).
- [ ] Update the entire building system.

## Execution

```bash
$ cd codegen-v2
$ cargo run
```

This command reads the C headers in `include/` and generates a
`include_manifest.json` file.
