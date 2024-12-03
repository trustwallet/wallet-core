import "mocha";
import { initWasm } from "../dist";

before(async () => {
  globalThis.mnemonic =
    "team engine square letter hero song dizzy scrub tornado fabric divert saddle";
  globalThis.password = "password";
  globalThis.core = await initWasm();
});
