// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import * as Types from "./types";
import * as fs from "fs/promises";

// FileSystem Storage
export class FileSystemStorage implements Types.IStorage {
  private readonly directory: string;

  constructor(directory: string) {
    this.directory = directory.endsWith("/") ? directory : directory + "/";
  }

  getFilename(id): string {
    return this.directory + id + ".json";
  }

  get(id: string): Promise<Types.Wallet> {
    return fs
      .readFile(this.getFilename(id))
      .then((data) => JSON.parse(data.toString()) as Types.Wallet);
  }

  set(id: string, wallet: Types.Wallet): Promise<void> {
    return fs.writeFile(this.getFilename(id), JSON.stringify(wallet));
  }

  loadAll(): Promise<Types.Wallet[]> {
    return fs.readdir(this.directory).then((files) => {
      return Promise.all(
        files
          .filter((file) => file.endsWith(".json"))
          .map((file) => this.get(file.replace(".json", "")))
      );
    });
  }

  delete(id: string, password: string): Promise<void> {
    return fs.unlink(this.getFilename(id));
  }
}
