// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import { KeyStore } from "./keystore";
import * as fs from "fs/promises";

export class FileSystemStorage implements KeyStore.IKeyStoreStorage {
  private readonly directory: string;

  constructor(directory: string) {
    this.directory = directory.endsWith("/") ? directory : directory + "/";
  }

  get(id: string): Promise<KeyStore.Wallet> {
    return fs.readFile(this.directory + id).then((data) => {
      let wallet = JSON.parse(data.toString()) as KeyStore.Wallet;
      return wallet;
    });
  }

  set(id: string, wallet: KeyStore.Wallet): Promise<void> {
    return fs.writeFile(this.directory + id, JSON.stringify(wallet));
  }

  loadAll(): Promise<KeyStore.Wallet[]> {
    return fs.readdir(this.directory).then((files) => {
      return Promise.all(
        files.map((file) => {
          return this.get(file);
        })
      );
    });
  }

  delete(id: string, password: string): Promise<void> {
    return fs.unlink(this.directory + id);
  }
}
