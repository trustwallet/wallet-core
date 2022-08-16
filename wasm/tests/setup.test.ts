import "mocha";
import { initWasm } from "../dist";

before(async () => {
  globalThis.core = await initWasm();
});
