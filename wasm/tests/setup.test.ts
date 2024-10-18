import "mocha";
import { initWasm } from "../dist";

before(async () => {
  globalThis.mnemonic =
    "team engine square letter hero song dizzy scrub tornado fabric divert saddle";
  globalThis.tonMnemonic =
      "laundry myself fitness beyond prize piano match acid vacuum already abandon dance occur pause grocery company inject excuse weasel carpet fog grunt trick spike";
  globalThis.password = "password";
  globalThis.core = await initWasm();
});
