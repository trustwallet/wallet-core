import * as $protobuf from "protobufjs";
/** Namespace TW. */
export namespace TW {

    /** Namespace Aeternity. */
    namespace Aeternity {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput fromAddress */
                fromAddress?: (string|null);

                /** SigningInput toAddress */
                toAddress?: (string|null);

                /** SigningInput amount */
                amount?: (Uint8Array|null);

                /** SigningInput fee */
                fee?: (Uint8Array|null);

                /** SigningInput payload */
                payload?: (string|null);

                /** SigningInput ttl */
                ttl?: (Long|null);

                /** SigningInput nonce */
                nonce?: (Long|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Aeternity.Proto.ISigningInput);

                /** SigningInput fromAddress. */
                public fromAddress: string;

                /** SigningInput toAddress. */
                public toAddress: string;

                /** SigningInput amount. */
                public amount: Uint8Array;

                /** SigningInput fee. */
                public fee: Uint8Array;

                /** SigningInput payload. */
                public payload: string;

                /** SigningInput ttl. */
                public ttl: Long;

                /** SigningInput nonce. */
                public nonce: Long;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Aeternity.Proto.ISigningInput): TW.Aeternity.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Aeternity.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Aeternity.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Aeternity.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Aeternity.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Aeternity.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (string|null);

                /** SigningOutput signature */
                signature?: (string|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Aeternity.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: string;

                /** SigningOutput signature. */
                public signature: string;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Aeternity.Proto.ISigningOutput): TW.Aeternity.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Aeternity.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Aeternity.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Aeternity.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Aeternity.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Aeternity.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Aion. */
    namespace Aion {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput nonce */
                nonce?: (Uint8Array|null);

                /** SigningInput gasPrice */
                gasPrice?: (Uint8Array|null);

                /** SigningInput gasLimit */
                gasLimit?: (Uint8Array|null);

                /** SigningInput toAddress */
                toAddress?: (string|null);

                /** SigningInput amount */
                amount?: (Uint8Array|null);

                /** SigningInput payload */
                payload?: (Uint8Array|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput timestamp */
                timestamp?: (Long|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Aion.Proto.ISigningInput);

                /** SigningInput nonce. */
                public nonce: Uint8Array;

                /** SigningInput gasPrice. */
                public gasPrice: Uint8Array;

                /** SigningInput gasLimit. */
                public gasLimit: Uint8Array;

                /** SigningInput toAddress. */
                public toAddress: string;

                /** SigningInput amount. */
                public amount: Uint8Array;

                /** SigningInput payload. */
                public payload: Uint8Array;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput timestamp. */
                public timestamp: Long;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Aion.Proto.ISigningInput): TW.Aion.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Aion.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Aion.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Aion.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Aion.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Aion.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);

                /** SigningOutput signature */
                signature?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Aion.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /** SigningOutput signature. */
                public signature: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Aion.Proto.ISigningOutput): TW.Aion.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Aion.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Aion.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Aion.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Aion.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Aion.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Algorand. */
    namespace Algorand {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a TransactionPay. */
            interface ITransactionPay {

                /** TransactionPay toAddress */
                toAddress?: (string|null);

                /** TransactionPay fee */
                fee?: (Long|null);

                /** TransactionPay amount */
                amount?: (Long|null);

                /** TransactionPay firstRound */
                firstRound?: (Long|null);

                /** TransactionPay lastRound */
                lastRound?: (Long|null);
            }

            /** Represents a TransactionPay. */
            class TransactionPay implements ITransactionPay {

                /**
                 * Constructs a new TransactionPay.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Algorand.Proto.ITransactionPay);

                /** TransactionPay toAddress. */
                public toAddress: string;

                /** TransactionPay fee. */
                public fee: Long;

                /** TransactionPay amount. */
                public amount: Long;

                /** TransactionPay firstRound. */
                public firstRound: Long;

                /** TransactionPay lastRound. */
                public lastRound: Long;

                /**
                 * Creates a new TransactionPay instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionPay instance
                 */
                public static create(properties?: TW.Algorand.Proto.ITransactionPay): TW.Algorand.Proto.TransactionPay;

                /**
                 * Encodes the specified TransactionPay message. Does not implicitly {@link TW.Algorand.Proto.TransactionPay.verify|verify} messages.
                 * @param message TransactionPay message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Algorand.Proto.ITransactionPay, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionPay message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionPay
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Algorand.Proto.TransactionPay;

                /**
                 * Verifies a TransactionPay message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionPay message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionPay
                 */
                public static fromObject(object: { [k: string]: any }): TW.Algorand.Proto.TransactionPay;

                /**
                 * Creates a plain object from a TransactionPay message. Also converts values to other types if specified.
                 * @param message TransactionPay
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Algorand.Proto.TransactionPay, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionPay to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput genesisId */
                genesisId?: (string|null);

                /** SigningInput genesisHash */
                genesisHash?: (Uint8Array|null);

                /** SigningInput note */
                note?: (Uint8Array|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput transactionPay */
                transactionPay?: (TW.Algorand.Proto.ITransactionPay|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Algorand.Proto.ISigningInput);

                /** SigningInput genesisId. */
                public genesisId: string;

                /** SigningInput genesisHash. */
                public genesisHash: Uint8Array;

                /** SigningInput note. */
                public note: Uint8Array;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput transactionPay. */
                public transactionPay?: (TW.Algorand.Proto.ITransactionPay|null);

                /** SigningInput messageOneof. */
                public messageOneof?: "transactionPay";

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Algorand.Proto.ISigningInput): TW.Algorand.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Algorand.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Algorand.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Algorand.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Algorand.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Algorand.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Algorand.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Algorand.Proto.ISigningOutput): TW.Algorand.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Algorand.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Algorand.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Algorand.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Algorand.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Algorand.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Binance. */
    namespace Binance {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a Transaction. */
            interface ITransaction {

                /** Transaction msgs */
                msgs?: (Uint8Array[]|null);

                /** Transaction signatures */
                signatures?: (Uint8Array[]|null);

                /** Transaction memo */
                memo?: (string|null);

                /** Transaction source */
                source?: (Long|null);

                /** Transaction data */
                data?: (Uint8Array|null);
            }

            /** Represents a Transaction. */
            class Transaction implements ITransaction {

                /**
                 * Constructs a new Transaction.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ITransaction);

                /** Transaction msgs. */
                public msgs: Uint8Array[];

                /** Transaction signatures. */
                public signatures: Uint8Array[];

                /** Transaction memo. */
                public memo: string;

                /** Transaction source. */
                public source: Long;

                /** Transaction data. */
                public data: Uint8Array;

                /**
                 * Creates a new Transaction instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Transaction instance
                 */
                public static create(properties?: TW.Binance.Proto.ITransaction): TW.Binance.Proto.Transaction;

                /**
                 * Encodes the specified Transaction message. Does not implicitly {@link TW.Binance.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Transaction message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.Transaction;

                /**
                 * Verifies a Transaction message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Transaction message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Transaction
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.Transaction;

                /**
                 * Creates a plain object from a Transaction message. Also converts values to other types if specified.
                 * @param message Transaction
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.Transaction, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Transaction to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Signature. */
            interface ISignature {

                /** Signature pubKey */
                pubKey?: (Uint8Array|null);

                /** Signature signature */
                signature?: (Uint8Array|null);

                /** Signature accountNumber */
                accountNumber?: (Long|null);

                /** Signature sequence */
                sequence?: (Long|null);
            }

            /** Represents a Signature. */
            class Signature implements ISignature {

                /**
                 * Constructs a new Signature.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ISignature);

                /** Signature pubKey. */
                public pubKey: Uint8Array;

                /** Signature signature. */
                public signature: Uint8Array;

                /** Signature accountNumber. */
                public accountNumber: Long;

                /** Signature sequence. */
                public sequence: Long;

                /**
                 * Creates a new Signature instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Signature instance
                 */
                public static create(properties?: TW.Binance.Proto.ISignature): TW.Binance.Proto.Signature;

                /**
                 * Encodes the specified Signature message. Does not implicitly {@link TW.Binance.Proto.Signature.verify|verify} messages.
                 * @param message Signature message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ISignature, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Signature message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Signature
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.Signature;

                /**
                 * Verifies a Signature message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Signature message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Signature
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.Signature;

                /**
                 * Creates a plain object from a Signature message. Also converts values to other types if specified.
                 * @param message Signature
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.Signature, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Signature to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            namespace Signature {

                /** Properties of a PubKey. */
                interface IPubKey {
                }

                /** Represents a PubKey. */
                class PubKey implements IPubKey {

                    /**
                     * Constructs a new PubKey.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Binance.Proto.Signature.IPubKey);

                    /**
                     * Creates a new PubKey instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns PubKey instance
                     */
                    public static create(properties?: TW.Binance.Proto.Signature.IPubKey): TW.Binance.Proto.Signature.PubKey;

                    /**
                     * Encodes the specified PubKey message. Does not implicitly {@link TW.Binance.Proto.Signature.PubKey.verify|verify} messages.
                     * @param message PubKey message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Binance.Proto.Signature.IPubKey, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a PubKey message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns PubKey
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.Signature.PubKey;

                    /**
                     * Verifies a PubKey message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a PubKey message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns PubKey
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.Signature.PubKey;

                    /**
                     * Creates a plain object from a PubKey message. Also converts values to other types if specified.
                     * @param message PubKey
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Binance.Proto.Signature.PubKey, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this PubKey to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }
            }

            /** Properties of a TradeOrder. */
            interface ITradeOrder {

                /** TradeOrder sender */
                sender?: (Uint8Array|null);

                /** TradeOrder id */
                id?: (string|null);

                /** TradeOrder symbol */
                symbol?: (string|null);

                /** TradeOrder ordertype */
                ordertype?: (Long|null);

                /** TradeOrder side */
                side?: (Long|null);

                /** TradeOrder price */
                price?: (Long|null);

                /** TradeOrder quantity */
                quantity?: (Long|null);

                /** TradeOrder timeinforce */
                timeinforce?: (Long|null);
            }

            /** Represents a TradeOrder. */
            class TradeOrder implements ITradeOrder {

                /**
                 * Constructs a new TradeOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ITradeOrder);

                /** TradeOrder sender. */
                public sender: Uint8Array;

                /** TradeOrder id. */
                public id: string;

                /** TradeOrder symbol. */
                public symbol: string;

                /** TradeOrder ordertype. */
                public ordertype: Long;

                /** TradeOrder side. */
                public side: Long;

                /** TradeOrder price. */
                public price: Long;

                /** TradeOrder quantity. */
                public quantity: Long;

                /** TradeOrder timeinforce. */
                public timeinforce: Long;

                /**
                 * Creates a new TradeOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TradeOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.ITradeOrder): TW.Binance.Proto.TradeOrder;

                /**
                 * Encodes the specified TradeOrder message. Does not implicitly {@link TW.Binance.Proto.TradeOrder.verify|verify} messages.
                 * @param message TradeOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ITradeOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TradeOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TradeOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.TradeOrder;

                /**
                 * Verifies a TradeOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TradeOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TradeOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.TradeOrder;

                /**
                 * Creates a plain object from a TradeOrder message. Also converts values to other types if specified.
                 * @param message TradeOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.TradeOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TradeOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a CancelTradeOrder. */
            interface ICancelTradeOrder {

                /** CancelTradeOrder sender */
                sender?: (Uint8Array|null);

                /** CancelTradeOrder symbol */
                symbol?: (string|null);

                /** CancelTradeOrder refid */
                refid?: (string|null);
            }

            /** Represents a CancelTradeOrder. */
            class CancelTradeOrder implements ICancelTradeOrder {

                /**
                 * Constructs a new CancelTradeOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ICancelTradeOrder);

                /** CancelTradeOrder sender. */
                public sender: Uint8Array;

                /** CancelTradeOrder symbol. */
                public symbol: string;

                /** CancelTradeOrder refid. */
                public refid: string;

                /**
                 * Creates a new CancelTradeOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns CancelTradeOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.ICancelTradeOrder): TW.Binance.Proto.CancelTradeOrder;

                /**
                 * Encodes the specified CancelTradeOrder message. Does not implicitly {@link TW.Binance.Proto.CancelTradeOrder.verify|verify} messages.
                 * @param message CancelTradeOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ICancelTradeOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a CancelTradeOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns CancelTradeOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.CancelTradeOrder;

                /**
                 * Verifies a CancelTradeOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a CancelTradeOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns CancelTradeOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.CancelTradeOrder;

                /**
                 * Creates a plain object from a CancelTradeOrder message. Also converts values to other types if specified.
                 * @param message CancelTradeOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.CancelTradeOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this CancelTradeOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SendOrder. */
            interface ISendOrder {

                /** SendOrder inputs */
                inputs?: (TW.Binance.Proto.SendOrder.IInput[]|null);

                /** SendOrder outputs */
                outputs?: (TW.Binance.Proto.SendOrder.IOutput[]|null);
            }

            /** Represents a SendOrder. */
            class SendOrder implements ISendOrder {

                /**
                 * Constructs a new SendOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ISendOrder);

                /** SendOrder inputs. */
                public inputs: TW.Binance.Proto.SendOrder.IInput[];

                /** SendOrder outputs. */
                public outputs: TW.Binance.Proto.SendOrder.IOutput[];

                /**
                 * Creates a new SendOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SendOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.ISendOrder): TW.Binance.Proto.SendOrder;

                /**
                 * Encodes the specified SendOrder message. Does not implicitly {@link TW.Binance.Proto.SendOrder.verify|verify} messages.
                 * @param message SendOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ISendOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SendOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SendOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.SendOrder;

                /**
                 * Verifies a SendOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SendOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SendOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.SendOrder;

                /**
                 * Creates a plain object from a SendOrder message. Also converts values to other types if specified.
                 * @param message SendOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.SendOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SendOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            namespace SendOrder {

                /** Properties of a Token. */
                interface IToken {

                    /** Token denom */
                    denom?: (string|null);

                    /** Token amount */
                    amount?: (Long|null);
                }

                /** Represents a Token. */
                class Token implements IToken {

                    /**
                     * Constructs a new Token.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Binance.Proto.SendOrder.IToken);

                    /** Token denom. */
                    public denom: string;

                    /** Token amount. */
                    public amount: Long;

                    /**
                     * Creates a new Token instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Token instance
                     */
                    public static create(properties?: TW.Binance.Proto.SendOrder.IToken): TW.Binance.Proto.SendOrder.Token;

                    /**
                     * Encodes the specified Token message. Does not implicitly {@link TW.Binance.Proto.SendOrder.Token.verify|verify} messages.
                     * @param message Token message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Binance.Proto.SendOrder.IToken, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Token message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Token
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.SendOrder.Token;

                    /**
                     * Verifies a Token message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Token message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Token
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.SendOrder.Token;

                    /**
                     * Creates a plain object from a Token message. Also converts values to other types if specified.
                     * @param message Token
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Binance.Proto.SendOrder.Token, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Token to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of an Input. */
                interface IInput {

                    /** Input address */
                    address?: (Uint8Array|null);

                    /** Input coins */
                    coins?: (TW.Binance.Proto.SendOrder.IToken[]|null);
                }

                /** Represents an Input. */
                class Input implements IInput {

                    /**
                     * Constructs a new Input.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Binance.Proto.SendOrder.IInput);

                    /** Input address. */
                    public address: Uint8Array;

                    /** Input coins. */
                    public coins: TW.Binance.Proto.SendOrder.IToken[];

                    /**
                     * Creates a new Input instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Input instance
                     */
                    public static create(properties?: TW.Binance.Proto.SendOrder.IInput): TW.Binance.Proto.SendOrder.Input;

                    /**
                     * Encodes the specified Input message. Does not implicitly {@link TW.Binance.Proto.SendOrder.Input.verify|verify} messages.
                     * @param message Input message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Binance.Proto.SendOrder.IInput, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes an Input message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Input
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.SendOrder.Input;

                    /**
                     * Verifies an Input message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates an Input message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Input
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.SendOrder.Input;

                    /**
                     * Creates a plain object from an Input message. Also converts values to other types if specified.
                     * @param message Input
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Binance.Proto.SendOrder.Input, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Input to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of an Output. */
                interface IOutput {

                    /** Output address */
                    address?: (Uint8Array|null);

                    /** Output coins */
                    coins?: (TW.Binance.Proto.SendOrder.IToken[]|null);
                }

                /** Represents an Output. */
                class Output implements IOutput {

                    /**
                     * Constructs a new Output.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Binance.Proto.SendOrder.IOutput);

                    /** Output address. */
                    public address: Uint8Array;

                    /** Output coins. */
                    public coins: TW.Binance.Proto.SendOrder.IToken[];

                    /**
                     * Creates a new Output instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Output instance
                     */
                    public static create(properties?: TW.Binance.Proto.SendOrder.IOutput): TW.Binance.Proto.SendOrder.Output;

                    /**
                     * Encodes the specified Output message. Does not implicitly {@link TW.Binance.Proto.SendOrder.Output.verify|verify} messages.
                     * @param message Output message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Binance.Proto.SendOrder.IOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes an Output message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Output
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.SendOrder.Output;

                    /**
                     * Verifies an Output message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates an Output message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Output
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.SendOrder.Output;

                    /**
                     * Creates a plain object from an Output message. Also converts values to other types if specified.
                     * @param message Output
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Binance.Proto.SendOrder.Output, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Output to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }
            }

            /** Properties of a TokenIssueOrder. */
            interface ITokenIssueOrder {

                /** TokenIssueOrder from */
                from?: (Uint8Array|null);

                /** TokenIssueOrder name */
                name?: (string|null);

                /** TokenIssueOrder symbol */
                symbol?: (string|null);

                /** TokenIssueOrder totalSupply */
                totalSupply?: (Long|null);

                /** TokenIssueOrder mintable */
                mintable?: (boolean|null);
            }

            /** Represents a TokenIssueOrder. */
            class TokenIssueOrder implements ITokenIssueOrder {

                /**
                 * Constructs a new TokenIssueOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ITokenIssueOrder);

                /** TokenIssueOrder from. */
                public from: Uint8Array;

                /** TokenIssueOrder name. */
                public name: string;

                /** TokenIssueOrder symbol. */
                public symbol: string;

                /** TokenIssueOrder totalSupply. */
                public totalSupply: Long;

                /** TokenIssueOrder mintable. */
                public mintable: boolean;

                /**
                 * Creates a new TokenIssueOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TokenIssueOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.ITokenIssueOrder): TW.Binance.Proto.TokenIssueOrder;

                /**
                 * Encodes the specified TokenIssueOrder message. Does not implicitly {@link TW.Binance.Proto.TokenIssueOrder.verify|verify} messages.
                 * @param message TokenIssueOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ITokenIssueOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TokenIssueOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TokenIssueOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.TokenIssueOrder;

                /**
                 * Verifies a TokenIssueOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TokenIssueOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TokenIssueOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.TokenIssueOrder;

                /**
                 * Creates a plain object from a TokenIssueOrder message. Also converts values to other types if specified.
                 * @param message TokenIssueOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.TokenIssueOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TokenIssueOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TokenMintOrder. */
            interface ITokenMintOrder {

                /** TokenMintOrder from */
                from?: (Uint8Array|null);

                /** TokenMintOrder symbol */
                symbol?: (string|null);

                /** TokenMintOrder amount */
                amount?: (Long|null);
            }

            /** Represents a TokenMintOrder. */
            class TokenMintOrder implements ITokenMintOrder {

                /**
                 * Constructs a new TokenMintOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ITokenMintOrder);

                /** TokenMintOrder from. */
                public from: Uint8Array;

                /** TokenMintOrder symbol. */
                public symbol: string;

                /** TokenMintOrder amount. */
                public amount: Long;

                /**
                 * Creates a new TokenMintOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TokenMintOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.ITokenMintOrder): TW.Binance.Proto.TokenMintOrder;

                /**
                 * Encodes the specified TokenMintOrder message. Does not implicitly {@link TW.Binance.Proto.TokenMintOrder.verify|verify} messages.
                 * @param message TokenMintOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ITokenMintOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TokenMintOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TokenMintOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.TokenMintOrder;

                /**
                 * Verifies a TokenMintOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TokenMintOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TokenMintOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.TokenMintOrder;

                /**
                 * Creates a plain object from a TokenMintOrder message. Also converts values to other types if specified.
                 * @param message TokenMintOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.TokenMintOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TokenMintOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TokenBurnOrder. */
            interface ITokenBurnOrder {

                /** TokenBurnOrder from */
                from?: (Uint8Array|null);

                /** TokenBurnOrder symbol */
                symbol?: (string|null);

                /** TokenBurnOrder amount */
                amount?: (Long|null);
            }

            /** Represents a TokenBurnOrder. */
            class TokenBurnOrder implements ITokenBurnOrder {

                /**
                 * Constructs a new TokenBurnOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ITokenBurnOrder);

                /** TokenBurnOrder from. */
                public from: Uint8Array;

                /** TokenBurnOrder symbol. */
                public symbol: string;

                /** TokenBurnOrder amount. */
                public amount: Long;

                /**
                 * Creates a new TokenBurnOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TokenBurnOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.ITokenBurnOrder): TW.Binance.Proto.TokenBurnOrder;

                /**
                 * Encodes the specified TokenBurnOrder message. Does not implicitly {@link TW.Binance.Proto.TokenBurnOrder.verify|verify} messages.
                 * @param message TokenBurnOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ITokenBurnOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TokenBurnOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TokenBurnOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.TokenBurnOrder;

                /**
                 * Verifies a TokenBurnOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TokenBurnOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TokenBurnOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.TokenBurnOrder;

                /**
                 * Creates a plain object from a TokenBurnOrder message. Also converts values to other types if specified.
                 * @param message TokenBurnOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.TokenBurnOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TokenBurnOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TokenFreezeOrder. */
            interface ITokenFreezeOrder {

                /** TokenFreezeOrder from */
                from?: (Uint8Array|null);

                /** TokenFreezeOrder symbol */
                symbol?: (string|null);

                /** TokenFreezeOrder amount */
                amount?: (Long|null);
            }

            /** Represents a TokenFreezeOrder. */
            class TokenFreezeOrder implements ITokenFreezeOrder {

                /**
                 * Constructs a new TokenFreezeOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ITokenFreezeOrder);

                /** TokenFreezeOrder from. */
                public from: Uint8Array;

                /** TokenFreezeOrder symbol. */
                public symbol: string;

                /** TokenFreezeOrder amount. */
                public amount: Long;

                /**
                 * Creates a new TokenFreezeOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TokenFreezeOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.ITokenFreezeOrder): TW.Binance.Proto.TokenFreezeOrder;

                /**
                 * Encodes the specified TokenFreezeOrder message. Does not implicitly {@link TW.Binance.Proto.TokenFreezeOrder.verify|verify} messages.
                 * @param message TokenFreezeOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ITokenFreezeOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TokenFreezeOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TokenFreezeOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.TokenFreezeOrder;

                /**
                 * Verifies a TokenFreezeOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TokenFreezeOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TokenFreezeOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.TokenFreezeOrder;

                /**
                 * Creates a plain object from a TokenFreezeOrder message. Also converts values to other types if specified.
                 * @param message TokenFreezeOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.TokenFreezeOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TokenFreezeOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TokenUnfreezeOrder. */
            interface ITokenUnfreezeOrder {

                /** TokenUnfreezeOrder from */
                from?: (Uint8Array|null);

                /** TokenUnfreezeOrder symbol */
                symbol?: (string|null);

                /** TokenUnfreezeOrder amount */
                amount?: (Long|null);
            }

            /** Represents a TokenUnfreezeOrder. */
            class TokenUnfreezeOrder implements ITokenUnfreezeOrder {

                /**
                 * Constructs a new TokenUnfreezeOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ITokenUnfreezeOrder);

                /** TokenUnfreezeOrder from. */
                public from: Uint8Array;

                /** TokenUnfreezeOrder symbol. */
                public symbol: string;

                /** TokenUnfreezeOrder amount. */
                public amount: Long;

                /**
                 * Creates a new TokenUnfreezeOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TokenUnfreezeOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.ITokenUnfreezeOrder): TW.Binance.Proto.TokenUnfreezeOrder;

                /**
                 * Encodes the specified TokenUnfreezeOrder message. Does not implicitly {@link TW.Binance.Proto.TokenUnfreezeOrder.verify|verify} messages.
                 * @param message TokenUnfreezeOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ITokenUnfreezeOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TokenUnfreezeOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TokenUnfreezeOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.TokenUnfreezeOrder;

                /**
                 * Verifies a TokenUnfreezeOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TokenUnfreezeOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TokenUnfreezeOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.TokenUnfreezeOrder;

                /**
                 * Creates a plain object from a TokenUnfreezeOrder message. Also converts values to other types if specified.
                 * @param message TokenUnfreezeOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.TokenUnfreezeOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TokenUnfreezeOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a HTLTOrder. */
            interface IHTLTOrder {

                /** HTLTOrder from */
                from?: (Uint8Array|null);

                /** HTLTOrder to */
                to?: (Uint8Array|null);

                /** HTLTOrder recipientOtherChain */
                recipientOtherChain?: (string|null);

                /** HTLTOrder senderOtherChain */
                senderOtherChain?: (string|null);

                /** HTLTOrder randomNumberHash */
                randomNumberHash?: (Uint8Array|null);

                /** HTLTOrder timestamp */
                timestamp?: (Long|null);

                /** HTLTOrder amount */
                amount?: (TW.Binance.Proto.SendOrder.IToken[]|null);

                /** HTLTOrder expectedIncome */
                expectedIncome?: (string|null);

                /** HTLTOrder heightSpan */
                heightSpan?: (Long|null);

                /** HTLTOrder crossChain */
                crossChain?: (boolean|null);
            }

            /** Represents a HTLTOrder. */
            class HTLTOrder implements IHTLTOrder {

                /**
                 * Constructs a new HTLTOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.IHTLTOrder);

                /** HTLTOrder from. */
                public from: Uint8Array;

                /** HTLTOrder to. */
                public to: Uint8Array;

                /** HTLTOrder recipientOtherChain. */
                public recipientOtherChain: string;

                /** HTLTOrder senderOtherChain. */
                public senderOtherChain: string;

                /** HTLTOrder randomNumberHash. */
                public randomNumberHash: Uint8Array;

                /** HTLTOrder timestamp. */
                public timestamp: Long;

                /** HTLTOrder amount. */
                public amount: TW.Binance.Proto.SendOrder.IToken[];

                /** HTLTOrder expectedIncome. */
                public expectedIncome: string;

                /** HTLTOrder heightSpan. */
                public heightSpan: Long;

                /** HTLTOrder crossChain. */
                public crossChain: boolean;

                /**
                 * Creates a new HTLTOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns HTLTOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.IHTLTOrder): TW.Binance.Proto.HTLTOrder;

                /**
                 * Encodes the specified HTLTOrder message. Does not implicitly {@link TW.Binance.Proto.HTLTOrder.verify|verify} messages.
                 * @param message HTLTOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.IHTLTOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a HTLTOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns HTLTOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.HTLTOrder;

                /**
                 * Verifies a HTLTOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a HTLTOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns HTLTOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.HTLTOrder;

                /**
                 * Creates a plain object from a HTLTOrder message. Also converts values to other types if specified.
                 * @param message HTLTOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.HTLTOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this HTLTOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a DepositHTLTOrder. */
            interface IDepositHTLTOrder {

                /** DepositHTLTOrder from */
                from?: (Uint8Array|null);

                /** DepositHTLTOrder amount */
                amount?: (TW.Binance.Proto.SendOrder.IToken[]|null);

                /** DepositHTLTOrder swapId */
                swapId?: (Uint8Array|null);
            }

            /** Represents a DepositHTLTOrder. */
            class DepositHTLTOrder implements IDepositHTLTOrder {

                /**
                 * Constructs a new DepositHTLTOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.IDepositHTLTOrder);

                /** DepositHTLTOrder from. */
                public from: Uint8Array;

                /** DepositHTLTOrder amount. */
                public amount: TW.Binance.Proto.SendOrder.IToken[];

                /** DepositHTLTOrder swapId. */
                public swapId: Uint8Array;

                /**
                 * Creates a new DepositHTLTOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns DepositHTLTOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.IDepositHTLTOrder): TW.Binance.Proto.DepositHTLTOrder;

                /**
                 * Encodes the specified DepositHTLTOrder message. Does not implicitly {@link TW.Binance.Proto.DepositHTLTOrder.verify|verify} messages.
                 * @param message DepositHTLTOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.IDepositHTLTOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a DepositHTLTOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns DepositHTLTOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.DepositHTLTOrder;

                /**
                 * Verifies a DepositHTLTOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a DepositHTLTOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns DepositHTLTOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.DepositHTLTOrder;

                /**
                 * Creates a plain object from a DepositHTLTOrder message. Also converts values to other types if specified.
                 * @param message DepositHTLTOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.DepositHTLTOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this DepositHTLTOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a ClaimHTLOrder. */
            interface IClaimHTLOrder {

                /** ClaimHTLOrder from */
                from?: (Uint8Array|null);

                /** ClaimHTLOrder swapId */
                swapId?: (Uint8Array|null);

                /** ClaimHTLOrder randomNumber */
                randomNumber?: (Uint8Array|null);
            }

            /** Represents a ClaimHTLOrder. */
            class ClaimHTLOrder implements IClaimHTLOrder {

                /**
                 * Constructs a new ClaimHTLOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.IClaimHTLOrder);

                /** ClaimHTLOrder from. */
                public from: Uint8Array;

                /** ClaimHTLOrder swapId. */
                public swapId: Uint8Array;

                /** ClaimHTLOrder randomNumber. */
                public randomNumber: Uint8Array;

                /**
                 * Creates a new ClaimHTLOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns ClaimHTLOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.IClaimHTLOrder): TW.Binance.Proto.ClaimHTLOrder;

                /**
                 * Encodes the specified ClaimHTLOrder message. Does not implicitly {@link TW.Binance.Proto.ClaimHTLOrder.verify|verify} messages.
                 * @param message ClaimHTLOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.IClaimHTLOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a ClaimHTLOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns ClaimHTLOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.ClaimHTLOrder;

                /**
                 * Verifies a ClaimHTLOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a ClaimHTLOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns ClaimHTLOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.ClaimHTLOrder;

                /**
                 * Creates a plain object from a ClaimHTLOrder message. Also converts values to other types if specified.
                 * @param message ClaimHTLOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.ClaimHTLOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this ClaimHTLOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a RefundHTLTOrder. */
            interface IRefundHTLTOrder {

                /** RefundHTLTOrder from */
                from?: (Uint8Array|null);

                /** RefundHTLTOrder swapId */
                swapId?: (Uint8Array|null);
            }

            /** Represents a RefundHTLTOrder. */
            class RefundHTLTOrder implements IRefundHTLTOrder {

                /**
                 * Constructs a new RefundHTLTOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.IRefundHTLTOrder);

                /** RefundHTLTOrder from. */
                public from: Uint8Array;

                /** RefundHTLTOrder swapId. */
                public swapId: Uint8Array;

                /**
                 * Creates a new RefundHTLTOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns RefundHTLTOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.IRefundHTLTOrder): TW.Binance.Proto.RefundHTLTOrder;

                /**
                 * Encodes the specified RefundHTLTOrder message. Does not implicitly {@link TW.Binance.Proto.RefundHTLTOrder.verify|verify} messages.
                 * @param message RefundHTLTOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.IRefundHTLTOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a RefundHTLTOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns RefundHTLTOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.RefundHTLTOrder;

                /**
                 * Verifies a RefundHTLTOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a RefundHTLTOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns RefundHTLTOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.RefundHTLTOrder;

                /**
                 * Creates a plain object from a RefundHTLTOrder message. Also converts values to other types if specified.
                 * @param message RefundHTLTOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.RefundHTLTOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this RefundHTLTOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransferOut. */
            interface ITransferOut {

                /** TransferOut from */
                from?: (Uint8Array|null);

                /** TransferOut to */
                to?: (Uint8Array|null);

                /** TransferOut amount */
                amount?: (TW.Binance.Proto.SendOrder.IToken|null);

                /** TransferOut expireTime */
                expireTime?: (Long|null);
            }

            /** Represents a TransferOut. */
            class TransferOut implements ITransferOut {

                /**
                 * Constructs a new TransferOut.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ITransferOut);

                /** TransferOut from. */
                public from: Uint8Array;

                /** TransferOut to. */
                public to: Uint8Array;

                /** TransferOut amount. */
                public amount?: (TW.Binance.Proto.SendOrder.IToken|null);

                /** TransferOut expireTime. */
                public expireTime: Long;

                /**
                 * Creates a new TransferOut instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransferOut instance
                 */
                public static create(properties?: TW.Binance.Proto.ITransferOut): TW.Binance.Proto.TransferOut;

                /**
                 * Encodes the specified TransferOut message. Does not implicitly {@link TW.Binance.Proto.TransferOut.verify|verify} messages.
                 * @param message TransferOut message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ITransferOut, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransferOut message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransferOut
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.TransferOut;

                /**
                 * Verifies a TransferOut message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransferOut message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransferOut
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.TransferOut;

                /**
                 * Creates a plain object from a TransferOut message. Also converts values to other types if specified.
                 * @param message TransferOut
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.TransferOut, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransferOut to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SideChainDelegate. */
            interface ISideChainDelegate {

                /** SideChainDelegate delegatorAddr */
                delegatorAddr?: (Uint8Array|null);

                /** SideChainDelegate validatorAddr */
                validatorAddr?: (Uint8Array|null);

                /** SideChainDelegate delegation */
                delegation?: (TW.Binance.Proto.SendOrder.IToken|null);

                /** SideChainDelegate chainId */
                chainId?: (string|null);
            }

            /** Represents a SideChainDelegate. */
            class SideChainDelegate implements ISideChainDelegate {

                /**
                 * Constructs a new SideChainDelegate.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ISideChainDelegate);

                /** SideChainDelegate delegatorAddr. */
                public delegatorAddr: Uint8Array;

                /** SideChainDelegate validatorAddr. */
                public validatorAddr: Uint8Array;

                /** SideChainDelegate delegation. */
                public delegation?: (TW.Binance.Proto.SendOrder.IToken|null);

                /** SideChainDelegate chainId. */
                public chainId: string;

                /**
                 * Creates a new SideChainDelegate instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SideChainDelegate instance
                 */
                public static create(properties?: TW.Binance.Proto.ISideChainDelegate): TW.Binance.Proto.SideChainDelegate;

                /**
                 * Encodes the specified SideChainDelegate message. Does not implicitly {@link TW.Binance.Proto.SideChainDelegate.verify|verify} messages.
                 * @param message SideChainDelegate message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ISideChainDelegate, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SideChainDelegate message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SideChainDelegate
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.SideChainDelegate;

                /**
                 * Verifies a SideChainDelegate message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SideChainDelegate message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SideChainDelegate
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.SideChainDelegate;

                /**
                 * Creates a plain object from a SideChainDelegate message. Also converts values to other types if specified.
                 * @param message SideChainDelegate
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.SideChainDelegate, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SideChainDelegate to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SideChainRedelegate. */
            interface ISideChainRedelegate {

                /** SideChainRedelegate delegatorAddr */
                delegatorAddr?: (Uint8Array|null);

                /** SideChainRedelegate validatorSrcAddr */
                validatorSrcAddr?: (Uint8Array|null);

                /** SideChainRedelegate validatorDstAddr */
                validatorDstAddr?: (Uint8Array|null);

                /** SideChainRedelegate amount */
                amount?: (TW.Binance.Proto.SendOrder.IToken|null);

                /** SideChainRedelegate chainId */
                chainId?: (string|null);
            }

            /** Represents a SideChainRedelegate. */
            class SideChainRedelegate implements ISideChainRedelegate {

                /**
                 * Constructs a new SideChainRedelegate.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ISideChainRedelegate);

                /** SideChainRedelegate delegatorAddr. */
                public delegatorAddr: Uint8Array;

                /** SideChainRedelegate validatorSrcAddr. */
                public validatorSrcAddr: Uint8Array;

                /** SideChainRedelegate validatorDstAddr. */
                public validatorDstAddr: Uint8Array;

                /** SideChainRedelegate amount. */
                public amount?: (TW.Binance.Proto.SendOrder.IToken|null);

                /** SideChainRedelegate chainId. */
                public chainId: string;

                /**
                 * Creates a new SideChainRedelegate instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SideChainRedelegate instance
                 */
                public static create(properties?: TW.Binance.Proto.ISideChainRedelegate): TW.Binance.Proto.SideChainRedelegate;

                /**
                 * Encodes the specified SideChainRedelegate message. Does not implicitly {@link TW.Binance.Proto.SideChainRedelegate.verify|verify} messages.
                 * @param message SideChainRedelegate message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ISideChainRedelegate, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SideChainRedelegate message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SideChainRedelegate
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.SideChainRedelegate;

                /**
                 * Verifies a SideChainRedelegate message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SideChainRedelegate message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SideChainRedelegate
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.SideChainRedelegate;

                /**
                 * Creates a plain object from a SideChainRedelegate message. Also converts values to other types if specified.
                 * @param message SideChainRedelegate
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.SideChainRedelegate, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SideChainRedelegate to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SideChainUndelegate. */
            interface ISideChainUndelegate {

                /** SideChainUndelegate delegatorAddr */
                delegatorAddr?: (Uint8Array|null);

                /** SideChainUndelegate validatorAddr */
                validatorAddr?: (Uint8Array|null);

                /** SideChainUndelegate amount */
                amount?: (TW.Binance.Proto.SendOrder.IToken|null);

                /** SideChainUndelegate chainId */
                chainId?: (string|null);
            }

            /** Represents a SideChainUndelegate. */
            class SideChainUndelegate implements ISideChainUndelegate {

                /**
                 * Constructs a new SideChainUndelegate.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ISideChainUndelegate);

                /** SideChainUndelegate delegatorAddr. */
                public delegatorAddr: Uint8Array;

                /** SideChainUndelegate validatorAddr. */
                public validatorAddr: Uint8Array;

                /** SideChainUndelegate amount. */
                public amount?: (TW.Binance.Proto.SendOrder.IToken|null);

                /** SideChainUndelegate chainId. */
                public chainId: string;

                /**
                 * Creates a new SideChainUndelegate instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SideChainUndelegate instance
                 */
                public static create(properties?: TW.Binance.Proto.ISideChainUndelegate): TW.Binance.Proto.SideChainUndelegate;

                /**
                 * Encodes the specified SideChainUndelegate message. Does not implicitly {@link TW.Binance.Proto.SideChainUndelegate.verify|verify} messages.
                 * @param message SideChainUndelegate message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ISideChainUndelegate, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SideChainUndelegate message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SideChainUndelegate
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.SideChainUndelegate;

                /**
                 * Verifies a SideChainUndelegate message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SideChainUndelegate message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SideChainUndelegate
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.SideChainUndelegate;

                /**
                 * Creates a plain object from a SideChainUndelegate message. Also converts values to other types if specified.
                 * @param message SideChainUndelegate
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.SideChainUndelegate, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SideChainUndelegate to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TimeLockOrder. */
            interface ITimeLockOrder {

                /** TimeLockOrder fromAddress */
                fromAddress?: (Uint8Array|null);

                /** TimeLockOrder description */
                description?: (string|null);

                /** TimeLockOrder amount */
                amount?: (TW.Binance.Proto.SendOrder.IToken[]|null);

                /** TimeLockOrder lockTime */
                lockTime?: (Long|null);
            }

            /** Represents a TimeLockOrder. */
            class TimeLockOrder implements ITimeLockOrder {

                /**
                 * Constructs a new TimeLockOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ITimeLockOrder);

                /** TimeLockOrder fromAddress. */
                public fromAddress: Uint8Array;

                /** TimeLockOrder description. */
                public description: string;

                /** TimeLockOrder amount. */
                public amount: TW.Binance.Proto.SendOrder.IToken[];

                /** TimeLockOrder lockTime. */
                public lockTime: Long;

                /**
                 * Creates a new TimeLockOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TimeLockOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.ITimeLockOrder): TW.Binance.Proto.TimeLockOrder;

                /**
                 * Encodes the specified TimeLockOrder message. Does not implicitly {@link TW.Binance.Proto.TimeLockOrder.verify|verify} messages.
                 * @param message TimeLockOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ITimeLockOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TimeLockOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TimeLockOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.TimeLockOrder;

                /**
                 * Verifies a TimeLockOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TimeLockOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TimeLockOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.TimeLockOrder;

                /**
                 * Creates a plain object from a TimeLockOrder message. Also converts values to other types if specified.
                 * @param message TimeLockOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.TimeLockOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TimeLockOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TimeRelockOrder. */
            interface ITimeRelockOrder {

                /** TimeRelockOrder fromAddress */
                fromAddress?: (Uint8Array|null);

                /** TimeRelockOrder id */
                id?: (Long|null);

                /** TimeRelockOrder description */
                description?: (string|null);

                /** TimeRelockOrder amount */
                amount?: (TW.Binance.Proto.SendOrder.IToken[]|null);

                /** TimeRelockOrder lockTime */
                lockTime?: (Long|null);
            }

            /** Represents a TimeRelockOrder. */
            class TimeRelockOrder implements ITimeRelockOrder {

                /**
                 * Constructs a new TimeRelockOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ITimeRelockOrder);

                /** TimeRelockOrder fromAddress. */
                public fromAddress: Uint8Array;

                /** TimeRelockOrder id. */
                public id: Long;

                /** TimeRelockOrder description. */
                public description: string;

                /** TimeRelockOrder amount. */
                public amount: TW.Binance.Proto.SendOrder.IToken[];

                /** TimeRelockOrder lockTime. */
                public lockTime: Long;

                /**
                 * Creates a new TimeRelockOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TimeRelockOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.ITimeRelockOrder): TW.Binance.Proto.TimeRelockOrder;

                /**
                 * Encodes the specified TimeRelockOrder message. Does not implicitly {@link TW.Binance.Proto.TimeRelockOrder.verify|verify} messages.
                 * @param message TimeRelockOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ITimeRelockOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TimeRelockOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TimeRelockOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.TimeRelockOrder;

                /**
                 * Verifies a TimeRelockOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TimeRelockOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TimeRelockOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.TimeRelockOrder;

                /**
                 * Creates a plain object from a TimeRelockOrder message. Also converts values to other types if specified.
                 * @param message TimeRelockOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.TimeRelockOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TimeRelockOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TimeUnlockOrder. */
            interface ITimeUnlockOrder {

                /** TimeUnlockOrder fromAddress */
                fromAddress?: (Uint8Array|null);

                /** TimeUnlockOrder id */
                id?: (Long|null);
            }

            /** Represents a TimeUnlockOrder. */
            class TimeUnlockOrder implements ITimeUnlockOrder {

                /**
                 * Constructs a new TimeUnlockOrder.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ITimeUnlockOrder);

                /** TimeUnlockOrder fromAddress. */
                public fromAddress: Uint8Array;

                /** TimeUnlockOrder id. */
                public id: Long;

                /**
                 * Creates a new TimeUnlockOrder instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TimeUnlockOrder instance
                 */
                public static create(properties?: TW.Binance.Proto.ITimeUnlockOrder): TW.Binance.Proto.TimeUnlockOrder;

                /**
                 * Encodes the specified TimeUnlockOrder message. Does not implicitly {@link TW.Binance.Proto.TimeUnlockOrder.verify|verify} messages.
                 * @param message TimeUnlockOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ITimeUnlockOrder, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TimeUnlockOrder message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TimeUnlockOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.TimeUnlockOrder;

                /**
                 * Verifies a TimeUnlockOrder message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TimeUnlockOrder message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TimeUnlockOrder
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.TimeUnlockOrder;

                /**
                 * Creates a plain object from a TimeUnlockOrder message. Also converts values to other types if specified.
                 * @param message TimeUnlockOrder
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.TimeUnlockOrder, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TimeUnlockOrder to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput chainId */
                chainId?: (string|null);

                /** SigningInput accountNumber */
                accountNumber?: (Long|null);

                /** SigningInput sequence */
                sequence?: (Long|null);

                /** SigningInput source */
                source?: (Long|null);

                /** SigningInput memo */
                memo?: (string|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput tradeOrder */
                tradeOrder?: (TW.Binance.Proto.ITradeOrder|null);

                /** SigningInput cancelTradeOrder */
                cancelTradeOrder?: (TW.Binance.Proto.ICancelTradeOrder|null);

                /** SigningInput sendOrder */
                sendOrder?: (TW.Binance.Proto.ISendOrder|null);

                /** SigningInput freezeOrder */
                freezeOrder?: (TW.Binance.Proto.ITokenFreezeOrder|null);

                /** SigningInput unfreezeOrder */
                unfreezeOrder?: (TW.Binance.Proto.ITokenUnfreezeOrder|null);

                /** SigningInput htltOrder */
                htltOrder?: (TW.Binance.Proto.IHTLTOrder|null);

                /** SigningInput depositHTLTOrder */
                depositHTLTOrder?: (TW.Binance.Proto.IDepositHTLTOrder|null);

                /** SigningInput claimHTLTOrder */
                claimHTLTOrder?: (TW.Binance.Proto.IClaimHTLOrder|null);

                /** SigningInput refundHTLTOrder */
                refundHTLTOrder?: (TW.Binance.Proto.IRefundHTLTOrder|null);

                /** SigningInput issueOrder */
                issueOrder?: (TW.Binance.Proto.ITokenIssueOrder|null);

                /** SigningInput mintOrder */
                mintOrder?: (TW.Binance.Proto.ITokenMintOrder|null);

                /** SigningInput burnOrder */
                burnOrder?: (TW.Binance.Proto.ITokenBurnOrder|null);

                /** SigningInput transferOutOrder */
                transferOutOrder?: (TW.Binance.Proto.ITransferOut|null);

                /** SigningInput sideDelegateOrder */
                sideDelegateOrder?: (TW.Binance.Proto.ISideChainDelegate|null);

                /** SigningInput sideRedelegateOrder */
                sideRedelegateOrder?: (TW.Binance.Proto.ISideChainRedelegate|null);

                /** SigningInput sideUndelegateOrder */
                sideUndelegateOrder?: (TW.Binance.Proto.ISideChainUndelegate|null);

                /** SigningInput timeLockOrder */
                timeLockOrder?: (TW.Binance.Proto.ITimeLockOrder|null);

                /** SigningInput timeRelockOrder */
                timeRelockOrder?: (TW.Binance.Proto.ITimeRelockOrder|null);

                /** SigningInput timeUnlockOrder */
                timeUnlockOrder?: (TW.Binance.Proto.ITimeUnlockOrder|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ISigningInput);

                /** SigningInput chainId. */
                public chainId: string;

                /** SigningInput accountNumber. */
                public accountNumber: Long;

                /** SigningInput sequence. */
                public sequence: Long;

                /** SigningInput source. */
                public source: Long;

                /** SigningInput memo. */
                public memo: string;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput tradeOrder. */
                public tradeOrder?: (TW.Binance.Proto.ITradeOrder|null);

                /** SigningInput cancelTradeOrder. */
                public cancelTradeOrder?: (TW.Binance.Proto.ICancelTradeOrder|null);

                /** SigningInput sendOrder. */
                public sendOrder?: (TW.Binance.Proto.ISendOrder|null);

                /** SigningInput freezeOrder. */
                public freezeOrder?: (TW.Binance.Proto.ITokenFreezeOrder|null);

                /** SigningInput unfreezeOrder. */
                public unfreezeOrder?: (TW.Binance.Proto.ITokenUnfreezeOrder|null);

                /** SigningInput htltOrder. */
                public htltOrder?: (TW.Binance.Proto.IHTLTOrder|null);

                /** SigningInput depositHTLTOrder. */
                public depositHTLTOrder?: (TW.Binance.Proto.IDepositHTLTOrder|null);

                /** SigningInput claimHTLTOrder. */
                public claimHTLTOrder?: (TW.Binance.Proto.IClaimHTLOrder|null);

                /** SigningInput refundHTLTOrder. */
                public refundHTLTOrder?: (TW.Binance.Proto.IRefundHTLTOrder|null);

                /** SigningInput issueOrder. */
                public issueOrder?: (TW.Binance.Proto.ITokenIssueOrder|null);

                /** SigningInput mintOrder. */
                public mintOrder?: (TW.Binance.Proto.ITokenMintOrder|null);

                /** SigningInput burnOrder. */
                public burnOrder?: (TW.Binance.Proto.ITokenBurnOrder|null);

                /** SigningInput transferOutOrder. */
                public transferOutOrder?: (TW.Binance.Proto.ITransferOut|null);

                /** SigningInput sideDelegateOrder. */
                public sideDelegateOrder?: (TW.Binance.Proto.ISideChainDelegate|null);

                /** SigningInput sideRedelegateOrder. */
                public sideRedelegateOrder?: (TW.Binance.Proto.ISideChainRedelegate|null);

                /** SigningInput sideUndelegateOrder. */
                public sideUndelegateOrder?: (TW.Binance.Proto.ISideChainUndelegate|null);

                /** SigningInput timeLockOrder. */
                public timeLockOrder?: (TW.Binance.Proto.ITimeLockOrder|null);

                /** SigningInput timeRelockOrder. */
                public timeRelockOrder?: (TW.Binance.Proto.ITimeRelockOrder|null);

                /** SigningInput timeUnlockOrder. */
                public timeUnlockOrder?: (TW.Binance.Proto.ITimeUnlockOrder|null);

                /** SigningInput orderOneof. */
                public orderOneof?: ("tradeOrder"|"cancelTradeOrder"|"sendOrder"|"freezeOrder"|"unfreezeOrder"|"htltOrder"|"depositHTLTOrder"|"claimHTLTOrder"|"refundHTLTOrder"|"issueOrder"|"mintOrder"|"burnOrder"|"transferOutOrder"|"sideDelegateOrder"|"sideRedelegateOrder"|"sideUndelegateOrder"|"timeLockOrder"|"timeRelockOrder"|"timeUnlockOrder");

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Binance.Proto.ISigningInput): TW.Binance.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Binance.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Binance.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Binance.Proto.ISigningOutput): TW.Binance.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Binance.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Binance.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Binance.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Binance.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Binance.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Bitcoin. */
    namespace Bitcoin {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a Transaction. */
            interface ITransaction {

                /** Transaction version */
                version?: (number|null);

                /** Transaction lockTime */
                lockTime?: (number|null);

                /** Transaction inputs */
                inputs?: (TW.Bitcoin.Proto.ITransactionInput[]|null);

                /** Transaction outputs */
                outputs?: (TW.Bitcoin.Proto.ITransactionOutput[]|null);
            }

            /** Represents a Transaction. */
            class Transaction implements ITransaction {

                /**
                 * Constructs a new Transaction.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Bitcoin.Proto.ITransaction);

                /** Transaction version. */
                public version: number;

                /** Transaction lockTime. */
                public lockTime: number;

                /** Transaction inputs. */
                public inputs: TW.Bitcoin.Proto.ITransactionInput[];

                /** Transaction outputs. */
                public outputs: TW.Bitcoin.Proto.ITransactionOutput[];

                /**
                 * Creates a new Transaction instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Transaction instance
                 */
                public static create(properties?: TW.Bitcoin.Proto.ITransaction): TW.Bitcoin.Proto.Transaction;

                /**
                 * Encodes the specified Transaction message. Does not implicitly {@link TW.Bitcoin.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Bitcoin.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Transaction message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Bitcoin.Proto.Transaction;

                /**
                 * Verifies a Transaction message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Transaction message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Transaction
                 */
                public static fromObject(object: { [k: string]: any }): TW.Bitcoin.Proto.Transaction;

                /**
                 * Creates a plain object from a Transaction message. Also converts values to other types if specified.
                 * @param message Transaction
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Bitcoin.Proto.Transaction, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Transaction to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransactionInput. */
            interface ITransactionInput {

                /** TransactionInput previousOutput */
                previousOutput?: (TW.Bitcoin.Proto.IOutPoint|null);

                /** TransactionInput sequence */
                sequence?: (number|null);

                /** TransactionInput script */
                script?: (Uint8Array|null);
            }

            /** Represents a TransactionInput. */
            class TransactionInput implements ITransactionInput {

                /**
                 * Constructs a new TransactionInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Bitcoin.Proto.ITransactionInput);

                /** TransactionInput previousOutput. */
                public previousOutput?: (TW.Bitcoin.Proto.IOutPoint|null);

                /** TransactionInput sequence. */
                public sequence: number;

                /** TransactionInput script. */
                public script: Uint8Array;

                /**
                 * Creates a new TransactionInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionInput instance
                 */
                public static create(properties?: TW.Bitcoin.Proto.ITransactionInput): TW.Bitcoin.Proto.TransactionInput;

                /**
                 * Encodes the specified TransactionInput message. Does not implicitly {@link TW.Bitcoin.Proto.TransactionInput.verify|verify} messages.
                 * @param message TransactionInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Bitcoin.Proto.ITransactionInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Bitcoin.Proto.TransactionInput;

                /**
                 * Verifies a TransactionInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Bitcoin.Proto.TransactionInput;

                /**
                 * Creates a plain object from a TransactionInput message. Also converts values to other types if specified.
                 * @param message TransactionInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Bitcoin.Proto.TransactionInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an OutPoint. */
            interface IOutPoint {

                /** OutPoint hash */
                hash?: (Uint8Array|null);

                /** OutPoint index */
                index?: (number|null);

                /** OutPoint sequence */
                sequence?: (number|null);
            }

            /** Represents an OutPoint. */
            class OutPoint implements IOutPoint {

                /**
                 * Constructs a new OutPoint.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Bitcoin.Proto.IOutPoint);

                /** OutPoint hash. */
                public hash: Uint8Array;

                /** OutPoint index. */
                public index: number;

                /** OutPoint sequence. */
                public sequence: number;

                /**
                 * Creates a new OutPoint instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns OutPoint instance
                 */
                public static create(properties?: TW.Bitcoin.Proto.IOutPoint): TW.Bitcoin.Proto.OutPoint;

                /**
                 * Encodes the specified OutPoint message. Does not implicitly {@link TW.Bitcoin.Proto.OutPoint.verify|verify} messages.
                 * @param message OutPoint message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Bitcoin.Proto.IOutPoint, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an OutPoint message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns OutPoint
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Bitcoin.Proto.OutPoint;

                /**
                 * Verifies an OutPoint message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an OutPoint message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns OutPoint
                 */
                public static fromObject(object: { [k: string]: any }): TW.Bitcoin.Proto.OutPoint;

                /**
                 * Creates a plain object from an OutPoint message. Also converts values to other types if specified.
                 * @param message OutPoint
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Bitcoin.Proto.OutPoint, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this OutPoint to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransactionOutput. */
            interface ITransactionOutput {

                /** TransactionOutput value */
                value?: (Long|null);

                /** TransactionOutput script */
                script?: (Uint8Array|null);
            }

            /** Represents a TransactionOutput. */
            class TransactionOutput implements ITransactionOutput {

                /**
                 * Constructs a new TransactionOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Bitcoin.Proto.ITransactionOutput);

                /** TransactionOutput value. */
                public value: Long;

                /** TransactionOutput script. */
                public script: Uint8Array;

                /**
                 * Creates a new TransactionOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionOutput instance
                 */
                public static create(properties?: TW.Bitcoin.Proto.ITransactionOutput): TW.Bitcoin.Proto.TransactionOutput;

                /**
                 * Encodes the specified TransactionOutput message. Does not implicitly {@link TW.Bitcoin.Proto.TransactionOutput.verify|verify} messages.
                 * @param message TransactionOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Bitcoin.Proto.ITransactionOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Bitcoin.Proto.TransactionOutput;

                /**
                 * Verifies a TransactionOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Bitcoin.Proto.TransactionOutput;

                /**
                 * Creates a plain object from a TransactionOutput message. Also converts values to other types if specified.
                 * @param message TransactionOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Bitcoin.Proto.TransactionOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an UnspentTransaction. */
            interface IUnspentTransaction {

                /** UnspentTransaction outPoint */
                outPoint?: (TW.Bitcoin.Proto.IOutPoint|null);

                /** UnspentTransaction script */
                script?: (Uint8Array|null);

                /** UnspentTransaction amount */
                amount?: (Long|null);
            }

            /** Represents an UnspentTransaction. */
            class UnspentTransaction implements IUnspentTransaction {

                /**
                 * Constructs a new UnspentTransaction.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Bitcoin.Proto.IUnspentTransaction);

                /** UnspentTransaction outPoint. */
                public outPoint?: (TW.Bitcoin.Proto.IOutPoint|null);

                /** UnspentTransaction script. */
                public script: Uint8Array;

                /** UnspentTransaction amount. */
                public amount: Long;

                /**
                 * Creates a new UnspentTransaction instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns UnspentTransaction instance
                 */
                public static create(properties?: TW.Bitcoin.Proto.IUnspentTransaction): TW.Bitcoin.Proto.UnspentTransaction;

                /**
                 * Encodes the specified UnspentTransaction message. Does not implicitly {@link TW.Bitcoin.Proto.UnspentTransaction.verify|verify} messages.
                 * @param message UnspentTransaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Bitcoin.Proto.IUnspentTransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an UnspentTransaction message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns UnspentTransaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Bitcoin.Proto.UnspentTransaction;

                /**
                 * Verifies an UnspentTransaction message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an UnspentTransaction message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns UnspentTransaction
                 */
                public static fromObject(object: { [k: string]: any }): TW.Bitcoin.Proto.UnspentTransaction;

                /**
                 * Creates a plain object from an UnspentTransaction message. Also converts values to other types if specified.
                 * @param message UnspentTransaction
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Bitcoin.Proto.UnspentTransaction, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this UnspentTransaction to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput hashType */
                hashType?: (number|null);

                /** SigningInput amount */
                amount?: (Long|null);

                /** SigningInput byteFee */
                byteFee?: (Long|null);

                /** SigningInput toAddress */
                toAddress?: (string|null);

                /** SigningInput changeAddress */
                changeAddress?: (string|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array[]|null);

                /** SigningInput scripts */
                scripts?: ({ [k: string]: Uint8Array }|null);

                /** SigningInput utxo */
                utxo?: (TW.Bitcoin.Proto.IUnspentTransaction[]|null);

                /** SigningInput useMaxAmount */
                useMaxAmount?: (boolean|null);

                /** SigningInput coinType */
                coinType?: (number|null);

                /** SigningInput plan */
                plan?: (TW.Bitcoin.Proto.ITransactionPlan|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Bitcoin.Proto.ISigningInput);

                /** SigningInput hashType. */
                public hashType: number;

                /** SigningInput amount. */
                public amount: Long;

                /** SigningInput byteFee. */
                public byteFee: Long;

                /** SigningInput toAddress. */
                public toAddress: string;

                /** SigningInput changeAddress. */
                public changeAddress: string;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array[];

                /** SigningInput scripts. */
                public scripts: { [k: string]: Uint8Array };

                /** SigningInput utxo. */
                public utxo: TW.Bitcoin.Proto.IUnspentTransaction[];

                /** SigningInput useMaxAmount. */
                public useMaxAmount: boolean;

                /** SigningInput coinType. */
                public coinType: number;

                /** SigningInput plan. */
                public plan?: (TW.Bitcoin.Proto.ITransactionPlan|null);

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Bitcoin.Proto.ISigningInput): TW.Bitcoin.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Bitcoin.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Bitcoin.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Bitcoin.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Bitcoin.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Bitcoin.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransactionPlan. */
            interface ITransactionPlan {

                /** TransactionPlan amount */
                amount?: (Long|null);

                /** TransactionPlan availableAmount */
                availableAmount?: (Long|null);

                /** TransactionPlan fee */
                fee?: (Long|null);

                /** TransactionPlan change */
                change?: (Long|null);

                /** TransactionPlan utxos */
                utxos?: (TW.Bitcoin.Proto.IUnspentTransaction[]|null);

                /** TransactionPlan branchId */
                branchId?: (Uint8Array|null);

                /** TransactionPlan error */
                error?: (TW.Common.Proto.SigningError|null);
            }

            /** Represents a TransactionPlan. */
            class TransactionPlan implements ITransactionPlan {

                /**
                 * Constructs a new TransactionPlan.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Bitcoin.Proto.ITransactionPlan);

                /** TransactionPlan amount. */
                public amount: Long;

                /** TransactionPlan availableAmount. */
                public availableAmount: Long;

                /** TransactionPlan fee. */
                public fee: Long;

                /** TransactionPlan change. */
                public change: Long;

                /** TransactionPlan utxos. */
                public utxos: TW.Bitcoin.Proto.IUnspentTransaction[];

                /** TransactionPlan branchId. */
                public branchId: Uint8Array;

                /** TransactionPlan error. */
                public error: TW.Common.Proto.SigningError;

                /**
                 * Creates a new TransactionPlan instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionPlan instance
                 */
                public static create(properties?: TW.Bitcoin.Proto.ITransactionPlan): TW.Bitcoin.Proto.TransactionPlan;

                /**
                 * Encodes the specified TransactionPlan message. Does not implicitly {@link TW.Bitcoin.Proto.TransactionPlan.verify|verify} messages.
                 * @param message TransactionPlan message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Bitcoin.Proto.ITransactionPlan, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionPlan message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionPlan
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Bitcoin.Proto.TransactionPlan;

                /**
                 * Verifies a TransactionPlan message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionPlan message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionPlan
                 */
                public static fromObject(object: { [k: string]: any }): TW.Bitcoin.Proto.TransactionPlan;

                /**
                 * Creates a plain object from a TransactionPlan message. Also converts values to other types if specified.
                 * @param message TransactionPlan
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Bitcoin.Proto.TransactionPlan, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionPlan to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput transaction */
                transaction?: (TW.Bitcoin.Proto.ITransaction|null);

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);

                /** SigningOutput transactionId */
                transactionId?: (string|null);

                /** SigningOutput error */
                error?: (TW.Common.Proto.SigningError|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Bitcoin.Proto.ISigningOutput);

                /** SigningOutput transaction. */
                public transaction?: (TW.Bitcoin.Proto.ITransaction|null);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /** SigningOutput transactionId. */
                public transactionId: string;

                /** SigningOutput error. */
                public error: TW.Common.Proto.SigningError;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Bitcoin.Proto.ISigningOutput): TW.Bitcoin.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Bitcoin.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Bitcoin.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Bitcoin.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Bitcoin.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Bitcoin.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Common. */
    namespace Common {

        /** Namespace Proto. */
        namespace Proto {

            /** SigningError enum. */
            enum SigningError {
                OK = 0,
                Error_general = 1,
                Error_internal = 2,
                Error_low_balance = 3,
                Error_zero_amount_requested = 4,
                Error_missing_private_key = 5,
                Error_wrong_fee = 6,
                Error_signing = 7,
                Error_tx_too_big = 8,
                Error_missing_input_utxos = 9,
                Error_not_enough_utxos = 10,
                Error_script_redeem = 11,
                Error_script_output = 12,
                Error_script_witness_program = 13
            }
        }
    }

    /** Namespace Cosmos. */
    namespace Cosmos {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of an Amount. */
            interface IAmount {

                /** Amount denom */
                denom?: (string|null);

                /** Amount amount */
                amount?: (Long|null);
            }

            /** Represents an Amount. */
            class Amount implements IAmount {

                /**
                 * Constructs a new Amount.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Cosmos.Proto.IAmount);

                /** Amount denom. */
                public denom: string;

                /** Amount amount. */
                public amount: Long;

                /**
                 * Creates a new Amount instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Amount instance
                 */
                public static create(properties?: TW.Cosmos.Proto.IAmount): TW.Cosmos.Proto.Amount;

                /**
                 * Encodes the specified Amount message. Does not implicitly {@link TW.Cosmos.Proto.Amount.verify|verify} messages.
                 * @param message Amount message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Cosmos.Proto.IAmount, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an Amount message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Amount
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.Amount;

                /**
                 * Verifies an Amount message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an Amount message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Amount
                 */
                public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.Amount;

                /**
                 * Creates a plain object from an Amount message. Also converts values to other types if specified.
                 * @param message Amount
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Cosmos.Proto.Amount, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Amount to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Fee. */
            interface IFee {

                /** Fee amounts */
                amounts?: (TW.Cosmos.Proto.IAmount[]|null);

                /** Fee gas */
                gas?: (Long|null);
            }

            /** Represents a Fee. */
            class Fee implements IFee {

                /**
                 * Constructs a new Fee.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Cosmos.Proto.IFee);

                /** Fee amounts. */
                public amounts: TW.Cosmos.Proto.IAmount[];

                /** Fee gas. */
                public gas: Long;

                /**
                 * Creates a new Fee instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Fee instance
                 */
                public static create(properties?: TW.Cosmos.Proto.IFee): TW.Cosmos.Proto.Fee;

                /**
                 * Encodes the specified Fee message. Does not implicitly {@link TW.Cosmos.Proto.Fee.verify|verify} messages.
                 * @param message Fee message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Cosmos.Proto.IFee, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Fee message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Fee
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.Fee;

                /**
                 * Verifies a Fee message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Fee message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Fee
                 */
                public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.Fee;

                /**
                 * Creates a plain object from a Fee message. Also converts values to other types if specified.
                 * @param message Fee
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Cosmos.Proto.Fee, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Fee to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** BroadcastMode enum. */
            enum BroadcastMode {
                BLOCK = 0,
                SYNC = 1,
                ASYNC = 2
            }

            /** Properties of a Message. */
            interface IMessage {

                /** Message sendCoinsMessage */
                sendCoinsMessage?: (TW.Cosmos.Proto.Message.ISend|null);

                /** Message stakeMessage */
                stakeMessage?: (TW.Cosmos.Proto.Message.IDelegate|null);

                /** Message unstakeMessage */
                unstakeMessage?: (TW.Cosmos.Proto.Message.IUndelegate|null);

                /** Message restakeMessage */
                restakeMessage?: (TW.Cosmos.Proto.Message.IBeginRedelegate|null);

                /** Message withdrawStakeRewardMessage */
                withdrawStakeRewardMessage?: (TW.Cosmos.Proto.Message.IWithdrawDelegationReward|null);

                /** Message rawJsonMessage */
                rawJsonMessage?: (TW.Cosmos.Proto.Message.IRawJSON|null);
            }

            /** Represents a Message. */
            class Message implements IMessage {

                /**
                 * Constructs a new Message.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Cosmos.Proto.IMessage);

                /** Message sendCoinsMessage. */
                public sendCoinsMessage?: (TW.Cosmos.Proto.Message.ISend|null);

                /** Message stakeMessage. */
                public stakeMessage?: (TW.Cosmos.Proto.Message.IDelegate|null);

                /** Message unstakeMessage. */
                public unstakeMessage?: (TW.Cosmos.Proto.Message.IUndelegate|null);

                /** Message restakeMessage. */
                public restakeMessage?: (TW.Cosmos.Proto.Message.IBeginRedelegate|null);

                /** Message withdrawStakeRewardMessage. */
                public withdrawStakeRewardMessage?: (TW.Cosmos.Proto.Message.IWithdrawDelegationReward|null);

                /** Message rawJsonMessage. */
                public rawJsonMessage?: (TW.Cosmos.Proto.Message.IRawJSON|null);

                /** Message messageOneof. */
                public messageOneof?: ("sendCoinsMessage"|"stakeMessage"|"unstakeMessage"|"restakeMessage"|"withdrawStakeRewardMessage"|"rawJsonMessage");

                /**
                 * Creates a new Message instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Message instance
                 */
                public static create(properties?: TW.Cosmos.Proto.IMessage): TW.Cosmos.Proto.Message;

                /**
                 * Encodes the specified Message message. Does not implicitly {@link TW.Cosmos.Proto.Message.verify|verify} messages.
                 * @param message Message message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Cosmos.Proto.IMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Message message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Message
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.Message;

                /**
                 * Verifies a Message message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Message message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Message
                 */
                public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.Message;

                /**
                 * Creates a plain object from a Message message. Also converts values to other types if specified.
                 * @param message Message
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Cosmos.Proto.Message, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Message to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            namespace Message {

                /** Properties of a Send. */
                interface ISend {

                    /** Send fromAddress */
                    fromAddress?: (string|null);

                    /** Send toAddress */
                    toAddress?: (string|null);

                    /** Send amounts */
                    amounts?: (TW.Cosmos.Proto.IAmount[]|null);

                    /** Send typePrefix */
                    typePrefix?: (string|null);
                }

                /** Represents a Send. */
                class Send implements ISend {

                    /**
                     * Constructs a new Send.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Cosmos.Proto.Message.ISend);

                    /** Send fromAddress. */
                    public fromAddress: string;

                    /** Send toAddress. */
                    public toAddress: string;

                    /** Send amounts. */
                    public amounts: TW.Cosmos.Proto.IAmount[];

                    /** Send typePrefix. */
                    public typePrefix: string;

                    /**
                     * Creates a new Send instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Send instance
                     */
                    public static create(properties?: TW.Cosmos.Proto.Message.ISend): TW.Cosmos.Proto.Message.Send;

                    /**
                     * Encodes the specified Send message. Does not implicitly {@link TW.Cosmos.Proto.Message.Send.verify|verify} messages.
                     * @param message Send message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Cosmos.Proto.Message.ISend, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Send message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Send
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.Message.Send;

                    /**
                     * Verifies a Send message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Send message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Send
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.Message.Send;

                    /**
                     * Creates a plain object from a Send message. Also converts values to other types if specified.
                     * @param message Send
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Cosmos.Proto.Message.Send, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Send to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a Delegate. */
                interface IDelegate {

                    /** Delegate delegatorAddress */
                    delegatorAddress?: (string|null);

                    /** Delegate validatorAddress */
                    validatorAddress?: (string|null);

                    /** Delegate amount */
                    amount?: (TW.Cosmos.Proto.IAmount|null);

                    /** Delegate typePrefix */
                    typePrefix?: (string|null);
                }

                /** Represents a Delegate. */
                class Delegate implements IDelegate {

                    /**
                     * Constructs a new Delegate.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Cosmos.Proto.Message.IDelegate);

                    /** Delegate delegatorAddress. */
                    public delegatorAddress: string;

                    /** Delegate validatorAddress. */
                    public validatorAddress: string;

                    /** Delegate amount. */
                    public amount?: (TW.Cosmos.Proto.IAmount|null);

                    /** Delegate typePrefix. */
                    public typePrefix: string;

                    /**
                     * Creates a new Delegate instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Delegate instance
                     */
                    public static create(properties?: TW.Cosmos.Proto.Message.IDelegate): TW.Cosmos.Proto.Message.Delegate;

                    /**
                     * Encodes the specified Delegate message. Does not implicitly {@link TW.Cosmos.Proto.Message.Delegate.verify|verify} messages.
                     * @param message Delegate message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Cosmos.Proto.Message.IDelegate, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Delegate message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Delegate
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.Message.Delegate;

                    /**
                     * Verifies a Delegate message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Delegate message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Delegate
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.Message.Delegate;

                    /**
                     * Creates a plain object from a Delegate message. Also converts values to other types if specified.
                     * @param message Delegate
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Cosmos.Proto.Message.Delegate, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Delegate to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of an Undelegate. */
                interface IUndelegate {

                    /** Undelegate delegatorAddress */
                    delegatorAddress?: (string|null);

                    /** Undelegate validatorAddress */
                    validatorAddress?: (string|null);

                    /** Undelegate amount */
                    amount?: (TW.Cosmos.Proto.IAmount|null);

                    /** Undelegate typePrefix */
                    typePrefix?: (string|null);
                }

                /** Represents an Undelegate. */
                class Undelegate implements IUndelegate {

                    /**
                     * Constructs a new Undelegate.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Cosmos.Proto.Message.IUndelegate);

                    /** Undelegate delegatorAddress. */
                    public delegatorAddress: string;

                    /** Undelegate validatorAddress. */
                    public validatorAddress: string;

                    /** Undelegate amount. */
                    public amount?: (TW.Cosmos.Proto.IAmount|null);

                    /** Undelegate typePrefix. */
                    public typePrefix: string;

                    /**
                     * Creates a new Undelegate instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Undelegate instance
                     */
                    public static create(properties?: TW.Cosmos.Proto.Message.IUndelegate): TW.Cosmos.Proto.Message.Undelegate;

                    /**
                     * Encodes the specified Undelegate message. Does not implicitly {@link TW.Cosmos.Proto.Message.Undelegate.verify|verify} messages.
                     * @param message Undelegate message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Cosmos.Proto.Message.IUndelegate, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes an Undelegate message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Undelegate
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.Message.Undelegate;

                    /**
                     * Verifies an Undelegate message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates an Undelegate message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Undelegate
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.Message.Undelegate;

                    /**
                     * Creates a plain object from an Undelegate message. Also converts values to other types if specified.
                     * @param message Undelegate
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Cosmos.Proto.Message.Undelegate, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Undelegate to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a BeginRedelegate. */
                interface IBeginRedelegate {

                    /** BeginRedelegate delegatorAddress */
                    delegatorAddress?: (string|null);

                    /** BeginRedelegate validatorSrcAddress */
                    validatorSrcAddress?: (string|null);

                    /** BeginRedelegate validatorDstAddress */
                    validatorDstAddress?: (string|null);

                    /** BeginRedelegate amount */
                    amount?: (TW.Cosmos.Proto.IAmount|null);

                    /** BeginRedelegate typePrefix */
                    typePrefix?: (string|null);
                }

                /** Represents a BeginRedelegate. */
                class BeginRedelegate implements IBeginRedelegate {

                    /**
                     * Constructs a new BeginRedelegate.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Cosmos.Proto.Message.IBeginRedelegate);

                    /** BeginRedelegate delegatorAddress. */
                    public delegatorAddress: string;

                    /** BeginRedelegate validatorSrcAddress. */
                    public validatorSrcAddress: string;

                    /** BeginRedelegate validatorDstAddress. */
                    public validatorDstAddress: string;

                    /** BeginRedelegate amount. */
                    public amount?: (TW.Cosmos.Proto.IAmount|null);

                    /** BeginRedelegate typePrefix. */
                    public typePrefix: string;

                    /**
                     * Creates a new BeginRedelegate instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns BeginRedelegate instance
                     */
                    public static create(properties?: TW.Cosmos.Proto.Message.IBeginRedelegate): TW.Cosmos.Proto.Message.BeginRedelegate;

                    /**
                     * Encodes the specified BeginRedelegate message. Does not implicitly {@link TW.Cosmos.Proto.Message.BeginRedelegate.verify|verify} messages.
                     * @param message BeginRedelegate message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Cosmos.Proto.Message.IBeginRedelegate, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a BeginRedelegate message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns BeginRedelegate
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.Message.BeginRedelegate;

                    /**
                     * Verifies a BeginRedelegate message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a BeginRedelegate message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns BeginRedelegate
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.Message.BeginRedelegate;

                    /**
                     * Creates a plain object from a BeginRedelegate message. Also converts values to other types if specified.
                     * @param message BeginRedelegate
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Cosmos.Proto.Message.BeginRedelegate, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this BeginRedelegate to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a WithdrawDelegationReward. */
                interface IWithdrawDelegationReward {

                    /** WithdrawDelegationReward delegatorAddress */
                    delegatorAddress?: (string|null);

                    /** WithdrawDelegationReward validatorAddress */
                    validatorAddress?: (string|null);

                    /** WithdrawDelegationReward typePrefix */
                    typePrefix?: (string|null);
                }

                /** Represents a WithdrawDelegationReward. */
                class WithdrawDelegationReward implements IWithdrawDelegationReward {

                    /**
                     * Constructs a new WithdrawDelegationReward.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Cosmos.Proto.Message.IWithdrawDelegationReward);

                    /** WithdrawDelegationReward delegatorAddress. */
                    public delegatorAddress: string;

                    /** WithdrawDelegationReward validatorAddress. */
                    public validatorAddress: string;

                    /** WithdrawDelegationReward typePrefix. */
                    public typePrefix: string;

                    /**
                     * Creates a new WithdrawDelegationReward instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns WithdrawDelegationReward instance
                     */
                    public static create(properties?: TW.Cosmos.Proto.Message.IWithdrawDelegationReward): TW.Cosmos.Proto.Message.WithdrawDelegationReward;

                    /**
                     * Encodes the specified WithdrawDelegationReward message. Does not implicitly {@link TW.Cosmos.Proto.Message.WithdrawDelegationReward.verify|verify} messages.
                     * @param message WithdrawDelegationReward message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Cosmos.Proto.Message.IWithdrawDelegationReward, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a WithdrawDelegationReward message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns WithdrawDelegationReward
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.Message.WithdrawDelegationReward;

                    /**
                     * Verifies a WithdrawDelegationReward message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a WithdrawDelegationReward message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns WithdrawDelegationReward
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.Message.WithdrawDelegationReward;

                    /**
                     * Creates a plain object from a WithdrawDelegationReward message. Also converts values to other types if specified.
                     * @param message WithdrawDelegationReward
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Cosmos.Proto.Message.WithdrawDelegationReward, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this WithdrawDelegationReward to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a RawJSON. */
                interface IRawJSON {

                    /** RawJSON type */
                    type?: (string|null);

                    /** RawJSON value */
                    value?: (string|null);
                }

                /** Represents a RawJSON. */
                class RawJSON implements IRawJSON {

                    /**
                     * Constructs a new RawJSON.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Cosmos.Proto.Message.IRawJSON);

                    /** RawJSON type. */
                    public type: string;

                    /** RawJSON value. */
                    public value: string;

                    /**
                     * Creates a new RawJSON instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns RawJSON instance
                     */
                    public static create(properties?: TW.Cosmos.Proto.Message.IRawJSON): TW.Cosmos.Proto.Message.RawJSON;

                    /**
                     * Encodes the specified RawJSON message. Does not implicitly {@link TW.Cosmos.Proto.Message.RawJSON.verify|verify} messages.
                     * @param message RawJSON message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Cosmos.Proto.Message.IRawJSON, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a RawJSON message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns RawJSON
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.Message.RawJSON;

                    /**
                     * Verifies a RawJSON message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a RawJSON message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns RawJSON
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.Message.RawJSON;

                    /**
                     * Creates a plain object from a RawJSON message. Also converts values to other types if specified.
                     * @param message RawJSON
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Cosmos.Proto.Message.RawJSON, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this RawJSON to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput accountNumber */
                accountNumber?: (Long|null);

                /** SigningInput chainId */
                chainId?: (string|null);

                /** SigningInput fee */
                fee?: (TW.Cosmos.Proto.IFee|null);

                /** SigningInput memo */
                memo?: (string|null);

                /** SigningInput sequence */
                sequence?: (Long|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput messages */
                messages?: (TW.Cosmos.Proto.IMessage[]|null);

                /** SigningInput mode */
                mode?: (TW.Cosmos.Proto.BroadcastMode|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Cosmos.Proto.ISigningInput);

                /** SigningInput accountNumber. */
                public accountNumber: Long;

                /** SigningInput chainId. */
                public chainId: string;

                /** SigningInput fee. */
                public fee?: (TW.Cosmos.Proto.IFee|null);

                /** SigningInput memo. */
                public memo: string;

                /** SigningInput sequence. */
                public sequence: Long;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput messages. */
                public messages: TW.Cosmos.Proto.IMessage[];

                /** SigningInput mode. */
                public mode: TW.Cosmos.Proto.BroadcastMode;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Cosmos.Proto.ISigningInput): TW.Cosmos.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Cosmos.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Cosmos.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Cosmos.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput signature */
                signature?: (Uint8Array|null);

                /** SigningOutput json */
                json?: (string|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Cosmos.Proto.ISigningOutput);

                /** SigningOutput signature. */
                public signature: Uint8Array;

                /** SigningOutput json. */
                public json: string;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Cosmos.Proto.ISigningOutput): TW.Cosmos.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Cosmos.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Cosmos.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Cosmos.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Decred. */
    namespace Decred {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a Transaction. */
            interface ITransaction {

                /** Serialization format */
                serializeType?: (number|null);

                /** Transaction data format version */
                version?: (number|null);

                /** Transaction inputs */
                inputs?: (TW.Decred.Proto.ITransactionInput[]|null);

                /** Transaction outputs */
                outputs?: (TW.Decred.Proto.ITransactionOutput[]|null);

                /** The time when a transaction can be spent (usually zero, in which case it has no effect). */
                lockTime?: (number|null);

                /** The block height at which the transaction expires and is no longer valid. */
                expiry?: (number|null);
            }

            /** Represents a Transaction. */
            class Transaction implements ITransaction {

                /**
                 * Constructs a new Transaction.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Decred.Proto.ITransaction);

                /** Serialization format */
                public serializeType: number;

                /** Transaction data format version */
                public version: number;

                /** Transaction inputs. */
                public inputs: TW.Decred.Proto.ITransactionInput[];

                /** Transaction outputs. */
                public outputs: TW.Decred.Proto.ITransactionOutput[];

                /** The time when a transaction can be spent (usually zero, in which case it has no effect). */
                public lockTime: number;

                /** The block height at which the transaction expires and is no longer valid. */
                public expiry: number;

                /**
                 * Creates a new Transaction instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Transaction instance
                 */
                public static create(properties?: TW.Decred.Proto.ITransaction): TW.Decred.Proto.Transaction;

                /**
                 * Encodes the specified Transaction message. Does not implicitly {@link TW.Decred.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Decred.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Transaction message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Decred.Proto.Transaction;

                /**
                 * Verifies a Transaction message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Transaction message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Transaction
                 */
                public static fromObject(object: { [k: string]: any }): TW.Decred.Proto.Transaction;

                /**
                 * Creates a plain object from a Transaction message. Also converts values to other types if specified.
                 * @param message Transaction
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Decred.Proto.Transaction, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Transaction to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransactionInput. */
            interface ITransactionInput {

                /** TransactionInput previousOutput */
                previousOutput?: (TW.Bitcoin.Proto.IOutPoint|null);

                /** TransactionInput sequence */
                sequence?: (number|null);

                /** TransactionInput valueIn */
                valueIn?: (Long|null);

                /** TransactionInput blockHeight */
                blockHeight?: (number|null);

                /** TransactionInput blockIndex */
                blockIndex?: (number|null);

                /** TransactionInput script */
                script?: (Uint8Array|null);
            }

            /** Represents a TransactionInput. */
            class TransactionInput implements ITransactionInput {

                /**
                 * Constructs a new TransactionInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Decred.Proto.ITransactionInput);

                /** TransactionInput previousOutput. */
                public previousOutput?: (TW.Bitcoin.Proto.IOutPoint|null);

                /** TransactionInput sequence. */
                public sequence: number;

                /** TransactionInput valueIn. */
                public valueIn: Long;

                /** TransactionInput blockHeight. */
                public blockHeight: number;

                /** TransactionInput blockIndex. */
                public blockIndex: number;

                /** TransactionInput script. */
                public script: Uint8Array;

                /**
                 * Creates a new TransactionInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionInput instance
                 */
                public static create(properties?: TW.Decred.Proto.ITransactionInput): TW.Decred.Proto.TransactionInput;

                /**
                 * Encodes the specified TransactionInput message. Does not implicitly {@link TW.Decred.Proto.TransactionInput.verify|verify} messages.
                 * @param message TransactionInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Decred.Proto.ITransactionInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Decred.Proto.TransactionInput;

                /**
                 * Verifies a TransactionInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Decred.Proto.TransactionInput;

                /**
                 * Creates a plain object from a TransactionInput message. Also converts values to other types if specified.
                 * @param message TransactionInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Decred.Proto.TransactionInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransactionOutput. */
            interface ITransactionOutput {

                /** TransactionOutput value */
                value?: (Long|null);

                /** Transaction output version. */
                version?: (number|null);

                /** TransactionOutput script */
                script?: (Uint8Array|null);
            }

            /** Represents a TransactionOutput. */
            class TransactionOutput implements ITransactionOutput {

                /**
                 * Constructs a new TransactionOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Decred.Proto.ITransactionOutput);

                /** TransactionOutput value. */
                public value: Long;

                /** Transaction output version. */
                public version: number;

                /** TransactionOutput script. */
                public script: Uint8Array;

                /**
                 * Creates a new TransactionOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionOutput instance
                 */
                public static create(properties?: TW.Decred.Proto.ITransactionOutput): TW.Decred.Proto.TransactionOutput;

                /**
                 * Encodes the specified TransactionOutput message. Does not implicitly {@link TW.Decred.Proto.TransactionOutput.verify|verify} messages.
                 * @param message TransactionOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Decred.Proto.ITransactionOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Decred.Proto.TransactionOutput;

                /**
                 * Verifies a TransactionOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Decred.Proto.TransactionOutput;

                /**
                 * Creates a plain object from a TransactionOutput message. Also converts values to other types if specified.
                 * @param message TransactionOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Decred.Proto.TransactionOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput transaction */
                transaction?: (TW.Decred.Proto.ITransaction|null);

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);

                /** SigningOutput transactionId */
                transactionId?: (string|null);

                /** SigningOutput error */
                error?: (TW.Common.Proto.SigningError|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Decred.Proto.ISigningOutput);

                /** SigningOutput transaction. */
                public transaction?: (TW.Decred.Proto.ITransaction|null);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /** SigningOutput transactionId. */
                public transactionId: string;

                /** SigningOutput error. */
                public error: TW.Common.Proto.SigningError;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Decred.Proto.ISigningOutput): TW.Decred.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Decred.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Decred.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Decred.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Decred.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Decred.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Elrond. */
    namespace Elrond {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a TransactionMessage. */
            interface ITransactionMessage {

                /** TransactionMessage nonce */
                nonce?: (Long|null);

                /** TransactionMessage value */
                value?: (string|null);

                /** TransactionMessage receiver */
                receiver?: (string|null);

                /** TransactionMessage sender */
                sender?: (string|null);

                /** TransactionMessage gasPrice */
                gasPrice?: (Long|null);

                /** TransactionMessage gasLimit */
                gasLimit?: (Long|null);

                /** TransactionMessage data */
                data?: (string|null);

                /** TransactionMessage chainId */
                chainId?: (string|null);

                /** TransactionMessage version */
                version?: (number|null);
            }

            /** Represents a TransactionMessage. */
            class TransactionMessage implements ITransactionMessage {

                /**
                 * Constructs a new TransactionMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Elrond.Proto.ITransactionMessage);

                /** TransactionMessage nonce. */
                public nonce: Long;

                /** TransactionMessage value. */
                public value: string;

                /** TransactionMessage receiver. */
                public receiver: string;

                /** TransactionMessage sender. */
                public sender: string;

                /** TransactionMessage gasPrice. */
                public gasPrice: Long;

                /** TransactionMessage gasLimit. */
                public gasLimit: Long;

                /** TransactionMessage data. */
                public data: string;

                /** TransactionMessage chainId. */
                public chainId: string;

                /** TransactionMessage version. */
                public version: number;

                /**
                 * Creates a new TransactionMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionMessage instance
                 */
                public static create(properties?: TW.Elrond.Proto.ITransactionMessage): TW.Elrond.Proto.TransactionMessage;

                /**
                 * Encodes the specified TransactionMessage message. Does not implicitly {@link TW.Elrond.Proto.TransactionMessage.verify|verify} messages.
                 * @param message TransactionMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Elrond.Proto.ITransactionMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Elrond.Proto.TransactionMessage;

                /**
                 * Verifies a TransactionMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Elrond.Proto.TransactionMessage;

                /**
                 * Creates a plain object from a TransactionMessage message. Also converts values to other types if specified.
                 * @param message TransactionMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Elrond.Proto.TransactionMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput transaction */
                transaction?: (TW.Elrond.Proto.ITransactionMessage|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Elrond.Proto.ISigningInput);

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput transaction. */
                public transaction?: (TW.Elrond.Proto.ITransactionMessage|null);

                /** SigningInput messageOneof. */
                public messageOneof?: "transaction";

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Elrond.Proto.ISigningInput): TW.Elrond.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Elrond.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Elrond.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Elrond.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Elrond.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Elrond.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (string|null);

                /** SigningOutput signature */
                signature?: (string|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Elrond.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: string;

                /** SigningOutput signature. */
                public signature: string;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Elrond.Proto.ISigningOutput): TW.Elrond.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Elrond.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Elrond.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Elrond.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Elrond.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Elrond.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace EOS. */
    namespace EOS {

        /** Namespace Proto. */
        namespace Proto {

            /** KeyType enum. */
            enum KeyType {
                LEGACY = 0,
                MODERNK1 = 1,
                MODERNR1 = 2
            }

            /** Properties of an Asset. */
            interface IAsset {

                /** Asset amount */
                amount?: (Long|null);

                /** Asset decimals */
                decimals?: (number|null);

                /** Asset symbol */
                symbol?: (string|null);
            }

            /** Represents an Asset. */
            class Asset implements IAsset {

                /**
                 * Constructs a new Asset.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.EOS.Proto.IAsset);

                /** Asset amount. */
                public amount: Long;

                /** Asset decimals. */
                public decimals: number;

                /** Asset symbol. */
                public symbol: string;

                /**
                 * Creates a new Asset instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Asset instance
                 */
                public static create(properties?: TW.EOS.Proto.IAsset): TW.EOS.Proto.Asset;

                /**
                 * Encodes the specified Asset message. Does not implicitly {@link TW.EOS.Proto.Asset.verify|verify} messages.
                 * @param message Asset message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.EOS.Proto.IAsset, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an Asset message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Asset
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.EOS.Proto.Asset;

                /**
                 * Verifies an Asset message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an Asset message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Asset
                 */
                public static fromObject(object: { [k: string]: any }): TW.EOS.Proto.Asset;

                /**
                 * Creates a plain object from an Asset message. Also converts values to other types if specified.
                 * @param message Asset
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.EOS.Proto.Asset, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Asset to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput chainId */
                chainId?: (Uint8Array|null);

                /** SigningInput referenceBlockId */
                referenceBlockId?: (Uint8Array|null);

                /** SigningInput referenceBlockTime */
                referenceBlockTime?: (number|null);

                /** SigningInput currency */
                currency?: (string|null);

                /** SigningInput sender */
                sender?: (string|null);

                /** SigningInput recipient */
                recipient?: (string|null);

                /** SigningInput memo */
                memo?: (string|null);

                /** SigningInput asset */
                asset?: (TW.EOS.Proto.IAsset|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput privateKeyType */
                privateKeyType?: (TW.EOS.Proto.KeyType|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.EOS.Proto.ISigningInput);

                /** SigningInput chainId. */
                public chainId: Uint8Array;

                /** SigningInput referenceBlockId. */
                public referenceBlockId: Uint8Array;

                /** SigningInput referenceBlockTime. */
                public referenceBlockTime: number;

                /** SigningInput currency. */
                public currency: string;

                /** SigningInput sender. */
                public sender: string;

                /** SigningInput recipient. */
                public recipient: string;

                /** SigningInput memo. */
                public memo: string;

                /** SigningInput asset. */
                public asset?: (TW.EOS.Proto.IAsset|null);

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput privateKeyType. */
                public privateKeyType: TW.EOS.Proto.KeyType;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.EOS.Proto.ISigningInput): TW.EOS.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.EOS.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.EOS.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.EOS.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.EOS.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.EOS.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput jsonEncoded */
                jsonEncoded?: (string|null);

                /** SigningOutput error */
                error?: (TW.Common.Proto.SigningError|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.EOS.Proto.ISigningOutput);

                /** SigningOutput jsonEncoded. */
                public jsonEncoded: string;

                /** SigningOutput error. */
                public error: TW.Common.Proto.SigningError;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.EOS.Proto.ISigningOutput): TW.EOS.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.EOS.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.EOS.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.EOS.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.EOS.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.EOS.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Ethereum. */
    namespace Ethereum {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a Transaction. */
            interface ITransaction {

                /** Transaction transfer */
                transfer?: (TW.Ethereum.Proto.Transaction.ITransfer|null);

                /** Transaction erc20Transfer */
                erc20Transfer?: (TW.Ethereum.Proto.Transaction.IERC20Transfer|null);

                /** Transaction erc20Approve */
                erc20Approve?: (TW.Ethereum.Proto.Transaction.IERC20Approve|null);

                /** Transaction erc721Transfer */
                erc721Transfer?: (TW.Ethereum.Proto.Transaction.IERC721Transfer|null);

                /** Transaction erc1155Transfer */
                erc1155Transfer?: (TW.Ethereum.Proto.Transaction.IERC1155Transfer|null);

                /** Transaction contractGeneric */
                contractGeneric?: (TW.Ethereum.Proto.Transaction.IContractGeneric|null);
            }

            /** Represents a Transaction. */
            class Transaction implements ITransaction {

                /**
                 * Constructs a new Transaction.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Ethereum.Proto.ITransaction);

                /** Transaction transfer. */
                public transfer?: (TW.Ethereum.Proto.Transaction.ITransfer|null);

                /** Transaction erc20Transfer. */
                public erc20Transfer?: (TW.Ethereum.Proto.Transaction.IERC20Transfer|null);

                /** Transaction erc20Approve. */
                public erc20Approve?: (TW.Ethereum.Proto.Transaction.IERC20Approve|null);

                /** Transaction erc721Transfer. */
                public erc721Transfer?: (TW.Ethereum.Proto.Transaction.IERC721Transfer|null);

                /** Transaction erc1155Transfer. */
                public erc1155Transfer?: (TW.Ethereum.Proto.Transaction.IERC1155Transfer|null);

                /** Transaction contractGeneric. */
                public contractGeneric?: (TW.Ethereum.Proto.Transaction.IContractGeneric|null);

                /** Transaction transactionOneof. */
                public transactionOneof?: ("transfer"|"erc20Transfer"|"erc20Approve"|"erc721Transfer"|"erc1155Transfer"|"contractGeneric");

                /**
                 * Creates a new Transaction instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Transaction instance
                 */
                public static create(properties?: TW.Ethereum.Proto.ITransaction): TW.Ethereum.Proto.Transaction;

                /**
                 * Encodes the specified Transaction message. Does not implicitly {@link TW.Ethereum.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Ethereum.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Transaction message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Ethereum.Proto.Transaction;

                /**
                 * Verifies a Transaction message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Transaction message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Transaction
                 */
                public static fromObject(object: { [k: string]: any }): TW.Ethereum.Proto.Transaction;

                /**
                 * Creates a plain object from a Transaction message. Also converts values to other types if specified.
                 * @param message Transaction
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Ethereum.Proto.Transaction, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Transaction to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            namespace Transaction {

                /** Properties of a Transfer. */
                interface ITransfer {

                    /** Transfer amount */
                    amount?: (Uint8Array|null);

                    /** Transfer data */
                    data?: (Uint8Array|null);
                }

                /** Represents a Transfer. */
                class Transfer implements ITransfer {

                    /**
                     * Constructs a new Transfer.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Ethereum.Proto.Transaction.ITransfer);

                    /** Transfer amount. */
                    public amount: Uint8Array;

                    /** Transfer data. */
                    public data: Uint8Array;

                    /**
                     * Creates a new Transfer instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Transfer instance
                     */
                    public static create(properties?: TW.Ethereum.Proto.Transaction.ITransfer): TW.Ethereum.Proto.Transaction.Transfer;

                    /**
                     * Encodes the specified Transfer message. Does not implicitly {@link TW.Ethereum.Proto.Transaction.Transfer.verify|verify} messages.
                     * @param message Transfer message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Ethereum.Proto.Transaction.ITransfer, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Transfer message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Transfer
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Ethereum.Proto.Transaction.Transfer;

                    /**
                     * Verifies a Transfer message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Transfer message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Transfer
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Ethereum.Proto.Transaction.Transfer;

                    /**
                     * Creates a plain object from a Transfer message. Also converts values to other types if specified.
                     * @param message Transfer
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Ethereum.Proto.Transaction.Transfer, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Transfer to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a ERC20Transfer. */
                interface IERC20Transfer {

                    /** ERC20Transfer to */
                    to?: (string|null);

                    /** ERC20Transfer amount */
                    amount?: (Uint8Array|null);
                }

                /** Represents a ERC20Transfer. */
                class ERC20Transfer implements IERC20Transfer {

                    /**
                     * Constructs a new ERC20Transfer.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Ethereum.Proto.Transaction.IERC20Transfer);

                    /** ERC20Transfer to. */
                    public to: string;

                    /** ERC20Transfer amount. */
                    public amount: Uint8Array;

                    /**
                     * Creates a new ERC20Transfer instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns ERC20Transfer instance
                     */
                    public static create(properties?: TW.Ethereum.Proto.Transaction.IERC20Transfer): TW.Ethereum.Proto.Transaction.ERC20Transfer;

                    /**
                     * Encodes the specified ERC20Transfer message. Does not implicitly {@link TW.Ethereum.Proto.Transaction.ERC20Transfer.verify|verify} messages.
                     * @param message ERC20Transfer message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Ethereum.Proto.Transaction.IERC20Transfer, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a ERC20Transfer message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns ERC20Transfer
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Ethereum.Proto.Transaction.ERC20Transfer;

                    /**
                     * Verifies a ERC20Transfer message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a ERC20Transfer message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns ERC20Transfer
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Ethereum.Proto.Transaction.ERC20Transfer;

                    /**
                     * Creates a plain object from a ERC20Transfer message. Also converts values to other types if specified.
                     * @param message ERC20Transfer
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Ethereum.Proto.Transaction.ERC20Transfer, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this ERC20Transfer to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a ERC20Approve. */
                interface IERC20Approve {

                    /** ERC20Approve spender */
                    spender?: (string|null);

                    /** ERC20Approve amount */
                    amount?: (Uint8Array|null);
                }

                /** Represents a ERC20Approve. */
                class ERC20Approve implements IERC20Approve {

                    /**
                     * Constructs a new ERC20Approve.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Ethereum.Proto.Transaction.IERC20Approve);

                    /** ERC20Approve spender. */
                    public spender: string;

                    /** ERC20Approve amount. */
                    public amount: Uint8Array;

                    /**
                     * Creates a new ERC20Approve instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns ERC20Approve instance
                     */
                    public static create(properties?: TW.Ethereum.Proto.Transaction.IERC20Approve): TW.Ethereum.Proto.Transaction.ERC20Approve;

                    /**
                     * Encodes the specified ERC20Approve message. Does not implicitly {@link TW.Ethereum.Proto.Transaction.ERC20Approve.verify|verify} messages.
                     * @param message ERC20Approve message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Ethereum.Proto.Transaction.IERC20Approve, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a ERC20Approve message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns ERC20Approve
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Ethereum.Proto.Transaction.ERC20Approve;

                    /**
                     * Verifies a ERC20Approve message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a ERC20Approve message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns ERC20Approve
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Ethereum.Proto.Transaction.ERC20Approve;

                    /**
                     * Creates a plain object from a ERC20Approve message. Also converts values to other types if specified.
                     * @param message ERC20Approve
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Ethereum.Proto.Transaction.ERC20Approve, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this ERC20Approve to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a ERC721Transfer. */
                interface IERC721Transfer {

                    /** ERC721Transfer from */
                    from?: (string|null);

                    /** ERC721Transfer to */
                    to?: (string|null);

                    /** ERC721Transfer tokenId */
                    tokenId?: (Uint8Array|null);
                }

                /** Represents a ERC721Transfer. */
                class ERC721Transfer implements IERC721Transfer {

                    /**
                     * Constructs a new ERC721Transfer.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Ethereum.Proto.Transaction.IERC721Transfer);

                    /** ERC721Transfer from. */
                    public from: string;

                    /** ERC721Transfer to. */
                    public to: string;

                    /** ERC721Transfer tokenId. */
                    public tokenId: Uint8Array;

                    /**
                     * Creates a new ERC721Transfer instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns ERC721Transfer instance
                     */
                    public static create(properties?: TW.Ethereum.Proto.Transaction.IERC721Transfer): TW.Ethereum.Proto.Transaction.ERC721Transfer;

                    /**
                     * Encodes the specified ERC721Transfer message. Does not implicitly {@link TW.Ethereum.Proto.Transaction.ERC721Transfer.verify|verify} messages.
                     * @param message ERC721Transfer message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Ethereum.Proto.Transaction.IERC721Transfer, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a ERC721Transfer message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns ERC721Transfer
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Ethereum.Proto.Transaction.ERC721Transfer;

                    /**
                     * Verifies a ERC721Transfer message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a ERC721Transfer message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns ERC721Transfer
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Ethereum.Proto.Transaction.ERC721Transfer;

                    /**
                     * Creates a plain object from a ERC721Transfer message. Also converts values to other types if specified.
                     * @param message ERC721Transfer
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Ethereum.Proto.Transaction.ERC721Transfer, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this ERC721Transfer to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a ERC1155Transfer. */
                interface IERC1155Transfer {

                    /** ERC1155Transfer from */
                    from?: (string|null);

                    /** ERC1155Transfer to */
                    to?: (string|null);

                    /** ERC1155Transfer tokenId */
                    tokenId?: (Uint8Array|null);

                    /** ERC1155Transfer value */
                    value?: (Uint8Array|null);

                    /** ERC1155Transfer data */
                    data?: (Uint8Array|null);
                }

                /** Represents a ERC1155Transfer. */
                class ERC1155Transfer implements IERC1155Transfer {

                    /**
                     * Constructs a new ERC1155Transfer.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Ethereum.Proto.Transaction.IERC1155Transfer);

                    /** ERC1155Transfer from. */
                    public from: string;

                    /** ERC1155Transfer to. */
                    public to: string;

                    /** ERC1155Transfer tokenId. */
                    public tokenId: Uint8Array;

                    /** ERC1155Transfer value. */
                    public value: Uint8Array;

                    /** ERC1155Transfer data. */
                    public data: Uint8Array;

                    /**
                     * Creates a new ERC1155Transfer instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns ERC1155Transfer instance
                     */
                    public static create(properties?: TW.Ethereum.Proto.Transaction.IERC1155Transfer): TW.Ethereum.Proto.Transaction.ERC1155Transfer;

                    /**
                     * Encodes the specified ERC1155Transfer message. Does not implicitly {@link TW.Ethereum.Proto.Transaction.ERC1155Transfer.verify|verify} messages.
                     * @param message ERC1155Transfer message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Ethereum.Proto.Transaction.IERC1155Transfer, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a ERC1155Transfer message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns ERC1155Transfer
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Ethereum.Proto.Transaction.ERC1155Transfer;

                    /**
                     * Verifies a ERC1155Transfer message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a ERC1155Transfer message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns ERC1155Transfer
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Ethereum.Proto.Transaction.ERC1155Transfer;

                    /**
                     * Creates a plain object from a ERC1155Transfer message. Also converts values to other types if specified.
                     * @param message ERC1155Transfer
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Ethereum.Proto.Transaction.ERC1155Transfer, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this ERC1155Transfer to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a ContractGeneric. */
                interface IContractGeneric {

                    /** ContractGeneric amount */
                    amount?: (Uint8Array|null);

                    /** ContractGeneric data */
                    data?: (Uint8Array|null);
                }

                /** Represents a ContractGeneric. */
                class ContractGeneric implements IContractGeneric {

                    /**
                     * Constructs a new ContractGeneric.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Ethereum.Proto.Transaction.IContractGeneric);

                    /** ContractGeneric amount. */
                    public amount: Uint8Array;

                    /** ContractGeneric data. */
                    public data: Uint8Array;

                    /**
                     * Creates a new ContractGeneric instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns ContractGeneric instance
                     */
                    public static create(properties?: TW.Ethereum.Proto.Transaction.IContractGeneric): TW.Ethereum.Proto.Transaction.ContractGeneric;

                    /**
                     * Encodes the specified ContractGeneric message. Does not implicitly {@link TW.Ethereum.Proto.Transaction.ContractGeneric.verify|verify} messages.
                     * @param message ContractGeneric message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Ethereum.Proto.Transaction.IContractGeneric, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a ContractGeneric message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns ContractGeneric
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Ethereum.Proto.Transaction.ContractGeneric;

                    /**
                     * Verifies a ContractGeneric message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a ContractGeneric message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns ContractGeneric
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Ethereum.Proto.Transaction.ContractGeneric;

                    /**
                     * Creates a plain object from a ContractGeneric message. Also converts values to other types if specified.
                     * @param message ContractGeneric
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Ethereum.Proto.Transaction.ContractGeneric, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this ContractGeneric to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput chainId */
                chainId?: (Uint8Array|null);

                /** SigningInput nonce */
                nonce?: (Uint8Array|null);

                /** SigningInput gasPrice */
                gasPrice?: (Uint8Array|null);

                /** SigningInput gasLimit */
                gasLimit?: (Uint8Array|null);

                /** SigningInput toAddress */
                toAddress?: (string|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput transaction */
                transaction?: (TW.Ethereum.Proto.ITransaction|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Ethereum.Proto.ISigningInput);

                /** SigningInput chainId. */
                public chainId: Uint8Array;

                /** SigningInput nonce. */
                public nonce: Uint8Array;

                /** SigningInput gasPrice. */
                public gasPrice: Uint8Array;

                /** SigningInput gasLimit. */
                public gasLimit: Uint8Array;

                /** SigningInput toAddress. */
                public toAddress: string;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput transaction. */
                public transaction?: (TW.Ethereum.Proto.ITransaction|null);

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Ethereum.Proto.ISigningInput): TW.Ethereum.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Ethereum.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Ethereum.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Ethereum.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Ethereum.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Ethereum.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);

                /** SigningOutput v */
                v?: (Uint8Array|null);

                /** SigningOutput r */
                r?: (Uint8Array|null);

                /** SigningOutput s */
                s?: (Uint8Array|null);

                /** SigningOutput data */
                data?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Ethereum.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /** SigningOutput v. */
                public v: Uint8Array;

                /** SigningOutput r. */
                public r: Uint8Array;

                /** SigningOutput s. */
                public s: Uint8Array;

                /** SigningOutput data. */
                public data: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Ethereum.Proto.ISigningOutput): TW.Ethereum.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Ethereum.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Ethereum.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Ethereum.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Ethereum.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Ethereum.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Filecoin. */
    namespace Filecoin {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput to */
                to?: (string|null);

                /** SigningInput nonce */
                nonce?: (Long|null);

                /** SigningInput value */
                value?: (Uint8Array|null);

                /** SigningInput gasLimit */
                gasLimit?: (Long|null);

                /** SigningInput gasFeeCap */
                gasFeeCap?: (Uint8Array|null);

                /** SigningInput gasPremium */
                gasPremium?: (Uint8Array|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Filecoin.Proto.ISigningInput);

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput to. */
                public to: string;

                /** SigningInput nonce. */
                public nonce: Long;

                /** SigningInput value. */
                public value: Uint8Array;

                /** SigningInput gasLimit. */
                public gasLimit: Long;

                /** SigningInput gasFeeCap. */
                public gasFeeCap: Uint8Array;

                /** SigningInput gasPremium. */
                public gasPremium: Uint8Array;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Filecoin.Proto.ISigningInput): TW.Filecoin.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Filecoin.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Filecoin.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Filecoin.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Filecoin.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Filecoin.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput json */
                json?: (string|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Filecoin.Proto.ISigningOutput);

                /** SigningOutput json. */
                public json: string;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Filecoin.Proto.ISigningOutput): TW.Filecoin.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Filecoin.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Filecoin.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Filecoin.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Filecoin.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Filecoin.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace FIO. */
    namespace FIO {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a PublicAddress. */
            interface IPublicAddress {

                /** PublicAddress coinSymbol */
                coinSymbol?: (string|null);

                /** PublicAddress address */
                address?: (string|null);
            }

            /** Represents a PublicAddress. */
            class PublicAddress implements IPublicAddress {

                /**
                 * Constructs a new PublicAddress.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.FIO.Proto.IPublicAddress);

                /** PublicAddress coinSymbol. */
                public coinSymbol: string;

                /** PublicAddress address. */
                public address: string;

                /**
                 * Creates a new PublicAddress instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns PublicAddress instance
                 */
                public static create(properties?: TW.FIO.Proto.IPublicAddress): TW.FIO.Proto.PublicAddress;

                /**
                 * Encodes the specified PublicAddress message. Does not implicitly {@link TW.FIO.Proto.PublicAddress.verify|verify} messages.
                 * @param message PublicAddress message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.FIO.Proto.IPublicAddress, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a PublicAddress message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns PublicAddress
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.FIO.Proto.PublicAddress;

                /**
                 * Verifies a PublicAddress message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a PublicAddress message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns PublicAddress
                 */
                public static fromObject(object: { [k: string]: any }): TW.FIO.Proto.PublicAddress;

                /**
                 * Creates a plain object from a PublicAddress message. Also converts values to other types if specified.
                 * @param message PublicAddress
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.FIO.Proto.PublicAddress, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this PublicAddress to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a NewFundsContent. */
            interface INewFundsContent {

                /** NewFundsContent payeePublicAddress */
                payeePublicAddress?: (string|null);

                /** NewFundsContent amount */
                amount?: (string|null);

                /** NewFundsContent coinSymbol */
                coinSymbol?: (string|null);

                /** NewFundsContent memo */
                memo?: (string|null);

                /** NewFundsContent hash */
                hash?: (string|null);

                /** NewFundsContent offlineUrl */
                offlineUrl?: (string|null);
            }

            /** Represents a NewFundsContent. */
            class NewFundsContent implements INewFundsContent {

                /**
                 * Constructs a new NewFundsContent.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.FIO.Proto.INewFundsContent);

                /** NewFundsContent payeePublicAddress. */
                public payeePublicAddress: string;

                /** NewFundsContent amount. */
                public amount: string;

                /** NewFundsContent coinSymbol. */
                public coinSymbol: string;

                /** NewFundsContent memo. */
                public memo: string;

                /** NewFundsContent hash. */
                public hash: string;

                /** NewFundsContent offlineUrl. */
                public offlineUrl: string;

                /**
                 * Creates a new NewFundsContent instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns NewFundsContent instance
                 */
                public static create(properties?: TW.FIO.Proto.INewFundsContent): TW.FIO.Proto.NewFundsContent;

                /**
                 * Encodes the specified NewFundsContent message. Does not implicitly {@link TW.FIO.Proto.NewFundsContent.verify|verify} messages.
                 * @param message NewFundsContent message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.FIO.Proto.INewFundsContent, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a NewFundsContent message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns NewFundsContent
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.FIO.Proto.NewFundsContent;

                /**
                 * Verifies a NewFundsContent message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a NewFundsContent message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns NewFundsContent
                 */
                public static fromObject(object: { [k: string]: any }): TW.FIO.Proto.NewFundsContent;

                /**
                 * Creates a plain object from a NewFundsContent message. Also converts values to other types if specified.
                 * @param message NewFundsContent
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.FIO.Proto.NewFundsContent, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this NewFundsContent to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an Action. */
            interface IAction {

                /** Action registerFioAddressMessage */
                registerFioAddressMessage?: (TW.FIO.Proto.Action.IRegisterFioAddress|null);

                /** Action addPubAddressMessage */
                addPubAddressMessage?: (TW.FIO.Proto.Action.IAddPubAddress|null);

                /** Action transferMessage */
                transferMessage?: (TW.FIO.Proto.Action.ITransfer|null);

                /** Action renewFioAddressMessage */
                renewFioAddressMessage?: (TW.FIO.Proto.Action.IRenewFioAddress|null);

                /** Action newFundsRequestMessage */
                newFundsRequestMessage?: (TW.FIO.Proto.Action.INewFundsRequest|null);
            }

            /** Represents an Action. */
            class Action implements IAction {

                /**
                 * Constructs a new Action.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.FIO.Proto.IAction);

                /** Action registerFioAddressMessage. */
                public registerFioAddressMessage?: (TW.FIO.Proto.Action.IRegisterFioAddress|null);

                /** Action addPubAddressMessage. */
                public addPubAddressMessage?: (TW.FIO.Proto.Action.IAddPubAddress|null);

                /** Action transferMessage. */
                public transferMessage?: (TW.FIO.Proto.Action.ITransfer|null);

                /** Action renewFioAddressMessage. */
                public renewFioAddressMessage?: (TW.FIO.Proto.Action.IRenewFioAddress|null);

                /** Action newFundsRequestMessage. */
                public newFundsRequestMessage?: (TW.FIO.Proto.Action.INewFundsRequest|null);

                /** Action messageOneof. */
                public messageOneof?: ("registerFioAddressMessage"|"addPubAddressMessage"|"transferMessage"|"renewFioAddressMessage"|"newFundsRequestMessage");

                /**
                 * Creates a new Action instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Action instance
                 */
                public static create(properties?: TW.FIO.Proto.IAction): TW.FIO.Proto.Action;

                /**
                 * Encodes the specified Action message. Does not implicitly {@link TW.FIO.Proto.Action.verify|verify} messages.
                 * @param message Action message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.FIO.Proto.IAction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an Action message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Action
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.FIO.Proto.Action;

                /**
                 * Verifies an Action message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an Action message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Action
                 */
                public static fromObject(object: { [k: string]: any }): TW.FIO.Proto.Action;

                /**
                 * Creates a plain object from an Action message. Also converts values to other types if specified.
                 * @param message Action
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.FIO.Proto.Action, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Action to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            namespace Action {

                /** Properties of a RegisterFioAddress. */
                interface IRegisterFioAddress {

                    /** RegisterFioAddress fioAddress */
                    fioAddress?: (string|null);

                    /** RegisterFioAddress ownerFioPublicKey */
                    ownerFioPublicKey?: (string|null);

                    /** RegisterFioAddress fee */
                    fee?: (Long|null);
                }

                /** Represents a RegisterFioAddress. */
                class RegisterFioAddress implements IRegisterFioAddress {

                    /**
                     * Constructs a new RegisterFioAddress.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.FIO.Proto.Action.IRegisterFioAddress);

                    /** RegisterFioAddress fioAddress. */
                    public fioAddress: string;

                    /** RegisterFioAddress ownerFioPublicKey. */
                    public ownerFioPublicKey: string;

                    /** RegisterFioAddress fee. */
                    public fee: Long;

                    /**
                     * Creates a new RegisterFioAddress instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns RegisterFioAddress instance
                     */
                    public static create(properties?: TW.FIO.Proto.Action.IRegisterFioAddress): TW.FIO.Proto.Action.RegisterFioAddress;

                    /**
                     * Encodes the specified RegisterFioAddress message. Does not implicitly {@link TW.FIO.Proto.Action.RegisterFioAddress.verify|verify} messages.
                     * @param message RegisterFioAddress message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.FIO.Proto.Action.IRegisterFioAddress, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a RegisterFioAddress message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns RegisterFioAddress
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.FIO.Proto.Action.RegisterFioAddress;

                    /**
                     * Verifies a RegisterFioAddress message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a RegisterFioAddress message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns RegisterFioAddress
                     */
                    public static fromObject(object: { [k: string]: any }): TW.FIO.Proto.Action.RegisterFioAddress;

                    /**
                     * Creates a plain object from a RegisterFioAddress message. Also converts values to other types if specified.
                     * @param message RegisterFioAddress
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.FIO.Proto.Action.RegisterFioAddress, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this RegisterFioAddress to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of an AddPubAddress. */
                interface IAddPubAddress {

                    /** AddPubAddress fioAddress */
                    fioAddress?: (string|null);

                    /** AddPubAddress publicAddresses */
                    publicAddresses?: (TW.FIO.Proto.IPublicAddress[]|null);

                    /** AddPubAddress fee */
                    fee?: (Long|null);
                }

                /** Represents an AddPubAddress. */
                class AddPubAddress implements IAddPubAddress {

                    /**
                     * Constructs a new AddPubAddress.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.FIO.Proto.Action.IAddPubAddress);

                    /** AddPubAddress fioAddress. */
                    public fioAddress: string;

                    /** AddPubAddress publicAddresses. */
                    public publicAddresses: TW.FIO.Proto.IPublicAddress[];

                    /** AddPubAddress fee. */
                    public fee: Long;

                    /**
                     * Creates a new AddPubAddress instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns AddPubAddress instance
                     */
                    public static create(properties?: TW.FIO.Proto.Action.IAddPubAddress): TW.FIO.Proto.Action.AddPubAddress;

                    /**
                     * Encodes the specified AddPubAddress message. Does not implicitly {@link TW.FIO.Proto.Action.AddPubAddress.verify|verify} messages.
                     * @param message AddPubAddress message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.FIO.Proto.Action.IAddPubAddress, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes an AddPubAddress message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns AddPubAddress
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.FIO.Proto.Action.AddPubAddress;

                    /**
                     * Verifies an AddPubAddress message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates an AddPubAddress message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns AddPubAddress
                     */
                    public static fromObject(object: { [k: string]: any }): TW.FIO.Proto.Action.AddPubAddress;

                    /**
                     * Creates a plain object from an AddPubAddress message. Also converts values to other types if specified.
                     * @param message AddPubAddress
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.FIO.Proto.Action.AddPubAddress, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this AddPubAddress to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a Transfer. */
                interface ITransfer {

                    /** Transfer payeePublicKey */
                    payeePublicKey?: (string|null);

                    /** Transfer amount */
                    amount?: (Long|null);

                    /** Transfer fee */
                    fee?: (Long|null);
                }

                /** Represents a Transfer. */
                class Transfer implements ITransfer {

                    /**
                     * Constructs a new Transfer.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.FIO.Proto.Action.ITransfer);

                    /** Transfer payeePublicKey. */
                    public payeePublicKey: string;

                    /** Transfer amount. */
                    public amount: Long;

                    /** Transfer fee. */
                    public fee: Long;

                    /**
                     * Creates a new Transfer instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Transfer instance
                     */
                    public static create(properties?: TW.FIO.Proto.Action.ITransfer): TW.FIO.Proto.Action.Transfer;

                    /**
                     * Encodes the specified Transfer message. Does not implicitly {@link TW.FIO.Proto.Action.Transfer.verify|verify} messages.
                     * @param message Transfer message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.FIO.Proto.Action.ITransfer, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Transfer message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Transfer
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.FIO.Proto.Action.Transfer;

                    /**
                     * Verifies a Transfer message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Transfer message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Transfer
                     */
                    public static fromObject(object: { [k: string]: any }): TW.FIO.Proto.Action.Transfer;

                    /**
                     * Creates a plain object from a Transfer message. Also converts values to other types if specified.
                     * @param message Transfer
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.FIO.Proto.Action.Transfer, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Transfer to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a RenewFioAddress. */
                interface IRenewFioAddress {

                    /** RenewFioAddress fioAddress */
                    fioAddress?: (string|null);

                    /** RenewFioAddress ownerFioPublicKey */
                    ownerFioPublicKey?: (string|null);

                    /** RenewFioAddress fee */
                    fee?: (Long|null);
                }

                /** Represents a RenewFioAddress. */
                class RenewFioAddress implements IRenewFioAddress {

                    /**
                     * Constructs a new RenewFioAddress.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.FIO.Proto.Action.IRenewFioAddress);

                    /** RenewFioAddress fioAddress. */
                    public fioAddress: string;

                    /** RenewFioAddress ownerFioPublicKey. */
                    public ownerFioPublicKey: string;

                    /** RenewFioAddress fee. */
                    public fee: Long;

                    /**
                     * Creates a new RenewFioAddress instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns RenewFioAddress instance
                     */
                    public static create(properties?: TW.FIO.Proto.Action.IRenewFioAddress): TW.FIO.Proto.Action.RenewFioAddress;

                    /**
                     * Encodes the specified RenewFioAddress message. Does not implicitly {@link TW.FIO.Proto.Action.RenewFioAddress.verify|verify} messages.
                     * @param message RenewFioAddress message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.FIO.Proto.Action.IRenewFioAddress, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a RenewFioAddress message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns RenewFioAddress
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.FIO.Proto.Action.RenewFioAddress;

                    /**
                     * Verifies a RenewFioAddress message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a RenewFioAddress message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns RenewFioAddress
                     */
                    public static fromObject(object: { [k: string]: any }): TW.FIO.Proto.Action.RenewFioAddress;

                    /**
                     * Creates a plain object from a RenewFioAddress message. Also converts values to other types if specified.
                     * @param message RenewFioAddress
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.FIO.Proto.Action.RenewFioAddress, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this RenewFioAddress to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a NewFundsRequest. */
                interface INewFundsRequest {

                    /** NewFundsRequest payerFioName */
                    payerFioName?: (string|null);

                    /** NewFundsRequest payerFioAddress */
                    payerFioAddress?: (string|null);

                    /** NewFundsRequest payeeFioName */
                    payeeFioName?: (string|null);

                    /** NewFundsRequest content */
                    content?: (TW.FIO.Proto.INewFundsContent|null);

                    /** NewFundsRequest fee */
                    fee?: (Long|null);
                }

                /** Represents a NewFundsRequest. */
                class NewFundsRequest implements INewFundsRequest {

                    /**
                     * Constructs a new NewFundsRequest.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.FIO.Proto.Action.INewFundsRequest);

                    /** NewFundsRequest payerFioName. */
                    public payerFioName: string;

                    /** NewFundsRequest payerFioAddress. */
                    public payerFioAddress: string;

                    /** NewFundsRequest payeeFioName. */
                    public payeeFioName: string;

                    /** NewFundsRequest content. */
                    public content?: (TW.FIO.Proto.INewFundsContent|null);

                    /** NewFundsRequest fee. */
                    public fee: Long;

                    /**
                     * Creates a new NewFundsRequest instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns NewFundsRequest instance
                     */
                    public static create(properties?: TW.FIO.Proto.Action.INewFundsRequest): TW.FIO.Proto.Action.NewFundsRequest;

                    /**
                     * Encodes the specified NewFundsRequest message. Does not implicitly {@link TW.FIO.Proto.Action.NewFundsRequest.verify|verify} messages.
                     * @param message NewFundsRequest message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.FIO.Proto.Action.INewFundsRequest, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a NewFundsRequest message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns NewFundsRequest
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.FIO.Proto.Action.NewFundsRequest;

                    /**
                     * Verifies a NewFundsRequest message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a NewFundsRequest message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns NewFundsRequest
                     */
                    public static fromObject(object: { [k: string]: any }): TW.FIO.Proto.Action.NewFundsRequest;

                    /**
                     * Creates a plain object from a NewFundsRequest message. Also converts values to other types if specified.
                     * @param message NewFundsRequest
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.FIO.Proto.Action.NewFundsRequest, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this NewFundsRequest to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }
            }

            /** Properties of a ChainParams. */
            interface IChainParams {

                /** ChainParams chainId */
                chainId?: (Uint8Array|null);

                /** ChainParams headBlockNumber */
                headBlockNumber?: (Long|null);

                /** ChainParams refBlockPrefix */
                refBlockPrefix?: (Long|null);
            }

            /** Represents a ChainParams. */
            class ChainParams implements IChainParams {

                /**
                 * Constructs a new ChainParams.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.FIO.Proto.IChainParams);

                /** ChainParams chainId. */
                public chainId: Uint8Array;

                /** ChainParams headBlockNumber. */
                public headBlockNumber: Long;

                /** ChainParams refBlockPrefix. */
                public refBlockPrefix: Long;

                /**
                 * Creates a new ChainParams instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns ChainParams instance
                 */
                public static create(properties?: TW.FIO.Proto.IChainParams): TW.FIO.Proto.ChainParams;

                /**
                 * Encodes the specified ChainParams message. Does not implicitly {@link TW.FIO.Proto.ChainParams.verify|verify} messages.
                 * @param message ChainParams message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.FIO.Proto.IChainParams, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a ChainParams message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns ChainParams
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.FIO.Proto.ChainParams;

                /**
                 * Verifies a ChainParams message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a ChainParams message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns ChainParams
                 */
                public static fromObject(object: { [k: string]: any }): TW.FIO.Proto.ChainParams;

                /**
                 * Creates a plain object from a ChainParams message. Also converts values to other types if specified.
                 * @param message ChainParams
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.FIO.Proto.ChainParams, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this ChainParams to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput expiry */
                expiry?: (number|null);

                /** SigningInput chainParams */
                chainParams?: (TW.FIO.Proto.IChainParams|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput tpid */
                tpid?: (string|null);

                /** SigningInput action */
                action?: (TW.FIO.Proto.IAction|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.FIO.Proto.ISigningInput);

                /** SigningInput expiry. */
                public expiry: number;

                /** SigningInput chainParams. */
                public chainParams?: (TW.FIO.Proto.IChainParams|null);

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput tpid. */
                public tpid: string;

                /** SigningInput action. */
                public action?: (TW.FIO.Proto.IAction|null);

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.FIO.Proto.ISigningInput): TW.FIO.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.FIO.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.FIO.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.FIO.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.FIO.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.FIO.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput json */
                json?: (string|null);

                /** SigningOutput error */
                error?: (TW.Common.Proto.SigningError|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.FIO.Proto.ISigningOutput);

                /** SigningOutput json. */
                public json: string;

                /** SigningOutput error. */
                public error: TW.Common.Proto.SigningError;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.FIO.Proto.ISigningOutput): TW.FIO.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.FIO.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.FIO.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.FIO.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.FIO.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.FIO.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Harmony. */
    namespace Harmony {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput chainId */
                chainId?: (Uint8Array|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput transactionMessage */
                transactionMessage?: (TW.Harmony.Proto.ITransactionMessage|null);

                /** SigningInput stakingMessage */
                stakingMessage?: (TW.Harmony.Proto.IStakingMessage|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Harmony.Proto.ISigningInput);

                /** SigningInput chainId. */
                public chainId: Uint8Array;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput transactionMessage. */
                public transactionMessage?: (TW.Harmony.Proto.ITransactionMessage|null);

                /** SigningInput stakingMessage. */
                public stakingMessage?: (TW.Harmony.Proto.IStakingMessage|null);

                /** SigningInput messageOneof. */
                public messageOneof?: ("transactionMessage"|"stakingMessage");

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Harmony.Proto.ISigningInput): TW.Harmony.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Harmony.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Harmony.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Harmony.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Harmony.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Harmony.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);

                /** SigningOutput v */
                v?: (Uint8Array|null);

                /** SigningOutput r */
                r?: (Uint8Array|null);

                /** SigningOutput s */
                s?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Harmony.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /** SigningOutput v. */
                public v: Uint8Array;

                /** SigningOutput r. */
                public r: Uint8Array;

                /** SigningOutput s. */
                public s: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Harmony.Proto.ISigningOutput): TW.Harmony.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Harmony.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Harmony.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Harmony.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Harmony.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Harmony.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransactionMessage. */
            interface ITransactionMessage {

                /** TransactionMessage nonce */
                nonce?: (Uint8Array|null);

                /** TransactionMessage gasPrice */
                gasPrice?: (Uint8Array|null);

                /** TransactionMessage gasLimit */
                gasLimit?: (Uint8Array|null);

                /** TransactionMessage toAddress */
                toAddress?: (string|null);

                /** TransactionMessage amount */
                amount?: (Uint8Array|null);

                /** TransactionMessage payload */
                payload?: (Uint8Array|null);

                /** TransactionMessage fromShardId */
                fromShardId?: (Uint8Array|null);

                /** TransactionMessage toShardId */
                toShardId?: (Uint8Array|null);
            }

            /** Represents a TransactionMessage. */
            class TransactionMessage implements ITransactionMessage {

                /**
                 * Constructs a new TransactionMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Harmony.Proto.ITransactionMessage);

                /** TransactionMessage nonce. */
                public nonce: Uint8Array;

                /** TransactionMessage gasPrice. */
                public gasPrice: Uint8Array;

                /** TransactionMessage gasLimit. */
                public gasLimit: Uint8Array;

                /** TransactionMessage toAddress. */
                public toAddress: string;

                /** TransactionMessage amount. */
                public amount: Uint8Array;

                /** TransactionMessage payload. */
                public payload: Uint8Array;

                /** TransactionMessage fromShardId. */
                public fromShardId: Uint8Array;

                /** TransactionMessage toShardId. */
                public toShardId: Uint8Array;

                /**
                 * Creates a new TransactionMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionMessage instance
                 */
                public static create(properties?: TW.Harmony.Proto.ITransactionMessage): TW.Harmony.Proto.TransactionMessage;

                /**
                 * Encodes the specified TransactionMessage message. Does not implicitly {@link TW.Harmony.Proto.TransactionMessage.verify|verify} messages.
                 * @param message TransactionMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Harmony.Proto.ITransactionMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Harmony.Proto.TransactionMessage;

                /**
                 * Verifies a TransactionMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Harmony.Proto.TransactionMessage;

                /**
                 * Creates a plain object from a TransactionMessage message. Also converts values to other types if specified.
                 * @param message TransactionMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Harmony.Proto.TransactionMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a StakingMessage. */
            interface IStakingMessage {

                /** StakingMessage createValidatorMessage */
                createValidatorMessage?: (TW.Harmony.Proto.IDirectiveCreateValidator|null);

                /** StakingMessage editValidatorMessage */
                editValidatorMessage?: (TW.Harmony.Proto.IDirectiveEditValidator|null);

                /** StakingMessage delegateMessage */
                delegateMessage?: (TW.Harmony.Proto.IDirectiveDelegate|null);

                /** StakingMessage undelegateMessage */
                undelegateMessage?: (TW.Harmony.Proto.IDirectiveUndelegate|null);

                /** StakingMessage collectRewards */
                collectRewards?: (TW.Harmony.Proto.IDirectiveCollectRewards|null);

                /** StakingMessage nonce */
                nonce?: (Uint8Array|null);

                /** StakingMessage gasPrice */
                gasPrice?: (Uint8Array|null);

                /** StakingMessage gasLimit */
                gasLimit?: (Uint8Array|null);
            }

            /** Represents a StakingMessage. */
            class StakingMessage implements IStakingMessage {

                /**
                 * Constructs a new StakingMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Harmony.Proto.IStakingMessage);

                /** StakingMessage createValidatorMessage. */
                public createValidatorMessage?: (TW.Harmony.Proto.IDirectiveCreateValidator|null);

                /** StakingMessage editValidatorMessage. */
                public editValidatorMessage?: (TW.Harmony.Proto.IDirectiveEditValidator|null);

                /** StakingMessage delegateMessage. */
                public delegateMessage?: (TW.Harmony.Proto.IDirectiveDelegate|null);

                /** StakingMessage undelegateMessage. */
                public undelegateMessage?: (TW.Harmony.Proto.IDirectiveUndelegate|null);

                /** StakingMessage collectRewards. */
                public collectRewards?: (TW.Harmony.Proto.IDirectiveCollectRewards|null);

                /** StakingMessage nonce. */
                public nonce: Uint8Array;

                /** StakingMessage gasPrice. */
                public gasPrice: Uint8Array;

                /** StakingMessage gasLimit. */
                public gasLimit: Uint8Array;

                /** StakingMessage stakeMsg. */
                public stakeMsg?: ("createValidatorMessage"|"editValidatorMessage"|"delegateMessage"|"undelegateMessage"|"collectRewards");

                /**
                 * Creates a new StakingMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns StakingMessage instance
                 */
                public static create(properties?: TW.Harmony.Proto.IStakingMessage): TW.Harmony.Proto.StakingMessage;

                /**
                 * Encodes the specified StakingMessage message. Does not implicitly {@link TW.Harmony.Proto.StakingMessage.verify|verify} messages.
                 * @param message StakingMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Harmony.Proto.IStakingMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a StakingMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns StakingMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Harmony.Proto.StakingMessage;

                /**
                 * Verifies a StakingMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a StakingMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns StakingMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Harmony.Proto.StakingMessage;

                /**
                 * Creates a plain object from a StakingMessage message. Also converts values to other types if specified.
                 * @param message StakingMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Harmony.Proto.StakingMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this StakingMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Description. */
            interface IDescription {

                /** Description name */
                name?: (string|null);

                /** Description identity */
                identity?: (string|null);

                /** Description website */
                website?: (string|null);

                /** Description securityContact */
                securityContact?: (string|null);

                /** Description details */
                details?: (string|null);
            }

            /** Represents a Description. */
            class Description implements IDescription {

                /**
                 * Constructs a new Description.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Harmony.Proto.IDescription);

                /** Description name. */
                public name: string;

                /** Description identity. */
                public identity: string;

                /** Description website. */
                public website: string;

                /** Description securityContact. */
                public securityContact: string;

                /** Description details. */
                public details: string;

                /**
                 * Creates a new Description instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Description instance
                 */
                public static create(properties?: TW.Harmony.Proto.IDescription): TW.Harmony.Proto.Description;

                /**
                 * Encodes the specified Description message. Does not implicitly {@link TW.Harmony.Proto.Description.verify|verify} messages.
                 * @param message Description message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Harmony.Proto.IDescription, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Description message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Description
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Harmony.Proto.Description;

                /**
                 * Verifies a Description message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Description message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Description
                 */
                public static fromObject(object: { [k: string]: any }): TW.Harmony.Proto.Description;

                /**
                 * Creates a plain object from a Description message. Also converts values to other types if specified.
                 * @param message Description
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Harmony.Proto.Description, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Description to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Decimal. */
            interface IDecimal {

                /** Decimal value */
                value?: (Uint8Array|null);

                /** Decimal precision */
                precision?: (Uint8Array|null);
            }

            /** Represents a Decimal. */
            class Decimal implements IDecimal {

                /**
                 * Constructs a new Decimal.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Harmony.Proto.IDecimal);

                /** Decimal value. */
                public value: Uint8Array;

                /** Decimal precision. */
                public precision: Uint8Array;

                /**
                 * Creates a new Decimal instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Decimal instance
                 */
                public static create(properties?: TW.Harmony.Proto.IDecimal): TW.Harmony.Proto.Decimal;

                /**
                 * Encodes the specified Decimal message. Does not implicitly {@link TW.Harmony.Proto.Decimal.verify|verify} messages.
                 * @param message Decimal message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Harmony.Proto.IDecimal, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Decimal message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Decimal
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Harmony.Proto.Decimal;

                /**
                 * Verifies a Decimal message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Decimal message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Decimal
                 */
                public static fromObject(object: { [k: string]: any }): TW.Harmony.Proto.Decimal;

                /**
                 * Creates a plain object from a Decimal message. Also converts values to other types if specified.
                 * @param message Decimal
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Harmony.Proto.Decimal, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Decimal to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a CommissionRate. */
            interface ICommissionRate {

                /** CommissionRate rate */
                rate?: (TW.Harmony.Proto.IDecimal|null);

                /** CommissionRate maxRate */
                maxRate?: (TW.Harmony.Proto.IDecimal|null);

                /** CommissionRate maxChangeRate */
                maxChangeRate?: (TW.Harmony.Proto.IDecimal|null);
            }

            /** Represents a CommissionRate. */
            class CommissionRate implements ICommissionRate {

                /**
                 * Constructs a new CommissionRate.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Harmony.Proto.ICommissionRate);

                /** CommissionRate rate. */
                public rate?: (TW.Harmony.Proto.IDecimal|null);

                /** CommissionRate maxRate. */
                public maxRate?: (TW.Harmony.Proto.IDecimal|null);

                /** CommissionRate maxChangeRate. */
                public maxChangeRate?: (TW.Harmony.Proto.IDecimal|null);

                /**
                 * Creates a new CommissionRate instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns CommissionRate instance
                 */
                public static create(properties?: TW.Harmony.Proto.ICommissionRate): TW.Harmony.Proto.CommissionRate;

                /**
                 * Encodes the specified CommissionRate message. Does not implicitly {@link TW.Harmony.Proto.CommissionRate.verify|verify} messages.
                 * @param message CommissionRate message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Harmony.Proto.ICommissionRate, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a CommissionRate message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns CommissionRate
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Harmony.Proto.CommissionRate;

                /**
                 * Verifies a CommissionRate message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a CommissionRate message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns CommissionRate
                 */
                public static fromObject(object: { [k: string]: any }): TW.Harmony.Proto.CommissionRate;

                /**
                 * Creates a plain object from a CommissionRate message. Also converts values to other types if specified.
                 * @param message CommissionRate
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Harmony.Proto.CommissionRate, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this CommissionRate to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a DirectiveCreateValidator. */
            interface IDirectiveCreateValidator {

                /** DirectiveCreateValidator validatorAddress */
                validatorAddress?: (string|null);

                /** DirectiveCreateValidator description */
                description?: (TW.Harmony.Proto.IDescription|null);

                /** DirectiveCreateValidator commissionRates */
                commissionRates?: (TW.Harmony.Proto.ICommissionRate|null);

                /** DirectiveCreateValidator minSelfDelegation */
                minSelfDelegation?: (Uint8Array|null);

                /** DirectiveCreateValidator maxTotalDelegation */
                maxTotalDelegation?: (Uint8Array|null);

                /** DirectiveCreateValidator slotPubKeys */
                slotPubKeys?: (Uint8Array[]|null);

                /** DirectiveCreateValidator slotKeySigs */
                slotKeySigs?: (Uint8Array[]|null);

                /** DirectiveCreateValidator amount */
                amount?: (Uint8Array|null);
            }

            /** Represents a DirectiveCreateValidator. */
            class DirectiveCreateValidator implements IDirectiveCreateValidator {

                /**
                 * Constructs a new DirectiveCreateValidator.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Harmony.Proto.IDirectiveCreateValidator);

                /** DirectiveCreateValidator validatorAddress. */
                public validatorAddress: string;

                /** DirectiveCreateValidator description. */
                public description?: (TW.Harmony.Proto.IDescription|null);

                /** DirectiveCreateValidator commissionRates. */
                public commissionRates?: (TW.Harmony.Proto.ICommissionRate|null);

                /** DirectiveCreateValidator minSelfDelegation. */
                public minSelfDelegation: Uint8Array;

                /** DirectiveCreateValidator maxTotalDelegation. */
                public maxTotalDelegation: Uint8Array;

                /** DirectiveCreateValidator slotPubKeys. */
                public slotPubKeys: Uint8Array[];

                /** DirectiveCreateValidator slotKeySigs. */
                public slotKeySigs: Uint8Array[];

                /** DirectiveCreateValidator amount. */
                public amount: Uint8Array;

                /**
                 * Creates a new DirectiveCreateValidator instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns DirectiveCreateValidator instance
                 */
                public static create(properties?: TW.Harmony.Proto.IDirectiveCreateValidator): TW.Harmony.Proto.DirectiveCreateValidator;

                /**
                 * Encodes the specified DirectiveCreateValidator message. Does not implicitly {@link TW.Harmony.Proto.DirectiveCreateValidator.verify|verify} messages.
                 * @param message DirectiveCreateValidator message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Harmony.Proto.IDirectiveCreateValidator, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a DirectiveCreateValidator message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns DirectiveCreateValidator
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Harmony.Proto.DirectiveCreateValidator;

                /**
                 * Verifies a DirectiveCreateValidator message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a DirectiveCreateValidator message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns DirectiveCreateValidator
                 */
                public static fromObject(object: { [k: string]: any }): TW.Harmony.Proto.DirectiveCreateValidator;

                /**
                 * Creates a plain object from a DirectiveCreateValidator message. Also converts values to other types if specified.
                 * @param message DirectiveCreateValidator
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Harmony.Proto.DirectiveCreateValidator, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this DirectiveCreateValidator to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a DirectiveEditValidator. */
            interface IDirectiveEditValidator {

                /** DirectiveEditValidator validatorAddress */
                validatorAddress?: (string|null);

                /** DirectiveEditValidator description */
                description?: (TW.Harmony.Proto.IDescription|null);

                /** DirectiveEditValidator commissionRate */
                commissionRate?: (TW.Harmony.Proto.IDecimal|null);

                /** DirectiveEditValidator minSelfDelegation */
                minSelfDelegation?: (Uint8Array|null);

                /** DirectiveEditValidator maxTotalDelegation */
                maxTotalDelegation?: (Uint8Array|null);

                /** DirectiveEditValidator slotKeyToRemove */
                slotKeyToRemove?: (Uint8Array|null);

                /** DirectiveEditValidator slotKeyToAdd */
                slotKeyToAdd?: (Uint8Array|null);

                /** DirectiveEditValidator slotKeyToAddSig */
                slotKeyToAddSig?: (Uint8Array|null);

                /** DirectiveEditValidator active */
                active?: (Uint8Array|null);
            }

            /** Represents a DirectiveEditValidator. */
            class DirectiveEditValidator implements IDirectiveEditValidator {

                /**
                 * Constructs a new DirectiveEditValidator.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Harmony.Proto.IDirectiveEditValidator);

                /** DirectiveEditValidator validatorAddress. */
                public validatorAddress: string;

                /** DirectiveEditValidator description. */
                public description?: (TW.Harmony.Proto.IDescription|null);

                /** DirectiveEditValidator commissionRate. */
                public commissionRate?: (TW.Harmony.Proto.IDecimal|null);

                /** DirectiveEditValidator minSelfDelegation. */
                public minSelfDelegation: Uint8Array;

                /** DirectiveEditValidator maxTotalDelegation. */
                public maxTotalDelegation: Uint8Array;

                /** DirectiveEditValidator slotKeyToRemove. */
                public slotKeyToRemove: Uint8Array;

                /** DirectiveEditValidator slotKeyToAdd. */
                public slotKeyToAdd: Uint8Array;

                /** DirectiveEditValidator slotKeyToAddSig. */
                public slotKeyToAddSig: Uint8Array;

                /** DirectiveEditValidator active. */
                public active: Uint8Array;

                /**
                 * Creates a new DirectiveEditValidator instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns DirectiveEditValidator instance
                 */
                public static create(properties?: TW.Harmony.Proto.IDirectiveEditValidator): TW.Harmony.Proto.DirectiveEditValidator;

                /**
                 * Encodes the specified DirectiveEditValidator message. Does not implicitly {@link TW.Harmony.Proto.DirectiveEditValidator.verify|verify} messages.
                 * @param message DirectiveEditValidator message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Harmony.Proto.IDirectiveEditValidator, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a DirectiveEditValidator message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns DirectiveEditValidator
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Harmony.Proto.DirectiveEditValidator;

                /**
                 * Verifies a DirectiveEditValidator message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a DirectiveEditValidator message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns DirectiveEditValidator
                 */
                public static fromObject(object: { [k: string]: any }): TW.Harmony.Proto.DirectiveEditValidator;

                /**
                 * Creates a plain object from a DirectiveEditValidator message. Also converts values to other types if specified.
                 * @param message DirectiveEditValidator
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Harmony.Proto.DirectiveEditValidator, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this DirectiveEditValidator to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a DirectiveDelegate. */
            interface IDirectiveDelegate {

                /** DirectiveDelegate delegatorAddress */
                delegatorAddress?: (string|null);

                /** DirectiveDelegate validatorAddress */
                validatorAddress?: (string|null);

                /** DirectiveDelegate amount */
                amount?: (Uint8Array|null);
            }

            /** Represents a DirectiveDelegate. */
            class DirectiveDelegate implements IDirectiveDelegate {

                /**
                 * Constructs a new DirectiveDelegate.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Harmony.Proto.IDirectiveDelegate);

                /** DirectiveDelegate delegatorAddress. */
                public delegatorAddress: string;

                /** DirectiveDelegate validatorAddress. */
                public validatorAddress: string;

                /** DirectiveDelegate amount. */
                public amount: Uint8Array;

                /**
                 * Creates a new DirectiveDelegate instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns DirectiveDelegate instance
                 */
                public static create(properties?: TW.Harmony.Proto.IDirectiveDelegate): TW.Harmony.Proto.DirectiveDelegate;

                /**
                 * Encodes the specified DirectiveDelegate message. Does not implicitly {@link TW.Harmony.Proto.DirectiveDelegate.verify|verify} messages.
                 * @param message DirectiveDelegate message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Harmony.Proto.IDirectiveDelegate, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a DirectiveDelegate message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns DirectiveDelegate
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Harmony.Proto.DirectiveDelegate;

                /**
                 * Verifies a DirectiveDelegate message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a DirectiveDelegate message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns DirectiveDelegate
                 */
                public static fromObject(object: { [k: string]: any }): TW.Harmony.Proto.DirectiveDelegate;

                /**
                 * Creates a plain object from a DirectiveDelegate message. Also converts values to other types if specified.
                 * @param message DirectiveDelegate
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Harmony.Proto.DirectiveDelegate, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this DirectiveDelegate to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a DirectiveUndelegate. */
            interface IDirectiveUndelegate {

                /** DirectiveUndelegate delegatorAddress */
                delegatorAddress?: (string|null);

                /** DirectiveUndelegate validatorAddress */
                validatorAddress?: (string|null);

                /** DirectiveUndelegate amount */
                amount?: (Uint8Array|null);
            }

            /** Represents a DirectiveUndelegate. */
            class DirectiveUndelegate implements IDirectiveUndelegate {

                /**
                 * Constructs a new DirectiveUndelegate.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Harmony.Proto.IDirectiveUndelegate);

                /** DirectiveUndelegate delegatorAddress. */
                public delegatorAddress: string;

                /** DirectiveUndelegate validatorAddress. */
                public validatorAddress: string;

                /** DirectiveUndelegate amount. */
                public amount: Uint8Array;

                /**
                 * Creates a new DirectiveUndelegate instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns DirectiveUndelegate instance
                 */
                public static create(properties?: TW.Harmony.Proto.IDirectiveUndelegate): TW.Harmony.Proto.DirectiveUndelegate;

                /**
                 * Encodes the specified DirectiveUndelegate message. Does not implicitly {@link TW.Harmony.Proto.DirectiveUndelegate.verify|verify} messages.
                 * @param message DirectiveUndelegate message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Harmony.Proto.IDirectiveUndelegate, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a DirectiveUndelegate message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns DirectiveUndelegate
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Harmony.Proto.DirectiveUndelegate;

                /**
                 * Verifies a DirectiveUndelegate message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a DirectiveUndelegate message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns DirectiveUndelegate
                 */
                public static fromObject(object: { [k: string]: any }): TW.Harmony.Proto.DirectiveUndelegate;

                /**
                 * Creates a plain object from a DirectiveUndelegate message. Also converts values to other types if specified.
                 * @param message DirectiveUndelegate
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Harmony.Proto.DirectiveUndelegate, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this DirectiveUndelegate to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a DirectiveCollectRewards. */
            interface IDirectiveCollectRewards {

                /** DirectiveCollectRewards delegatorAddress */
                delegatorAddress?: (string|null);
            }

            /** Represents a DirectiveCollectRewards. */
            class DirectiveCollectRewards implements IDirectiveCollectRewards {

                /**
                 * Constructs a new DirectiveCollectRewards.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Harmony.Proto.IDirectiveCollectRewards);

                /** DirectiveCollectRewards delegatorAddress. */
                public delegatorAddress: string;

                /**
                 * Creates a new DirectiveCollectRewards instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns DirectiveCollectRewards instance
                 */
                public static create(properties?: TW.Harmony.Proto.IDirectiveCollectRewards): TW.Harmony.Proto.DirectiveCollectRewards;

                /**
                 * Encodes the specified DirectiveCollectRewards message. Does not implicitly {@link TW.Harmony.Proto.DirectiveCollectRewards.verify|verify} messages.
                 * @param message DirectiveCollectRewards message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Harmony.Proto.IDirectiveCollectRewards, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a DirectiveCollectRewards message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns DirectiveCollectRewards
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Harmony.Proto.DirectiveCollectRewards;

                /**
                 * Verifies a DirectiveCollectRewards message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a DirectiveCollectRewards message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns DirectiveCollectRewards
                 */
                public static fromObject(object: { [k: string]: any }): TW.Harmony.Proto.DirectiveCollectRewards;

                /**
                 * Creates a plain object from a DirectiveCollectRewards message. Also converts values to other types if specified.
                 * @param message DirectiveCollectRewards
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Harmony.Proto.DirectiveCollectRewards, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this DirectiveCollectRewards to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Icon. */
    namespace Icon {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput fromAddress */
                fromAddress?: (string|null);

                /** SigningInput toAddress */
                toAddress?: (string|null);

                /** SigningInput value */
                value?: (Uint8Array|null);

                /** SigningInput stepLimit */
                stepLimit?: (Uint8Array|null);

                /** SigningInput timestamp */
                timestamp?: (Long|null);

                /** SigningInput nonce */
                nonce?: (Uint8Array|null);

                /** SigningInput networkId */
                networkId?: (Uint8Array|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Icon.Proto.ISigningInput);

                /** SigningInput fromAddress. */
                public fromAddress: string;

                /** SigningInput toAddress. */
                public toAddress: string;

                /** SigningInput value. */
                public value: Uint8Array;

                /** SigningInput stepLimit. */
                public stepLimit: Uint8Array;

                /** SigningInput timestamp. */
                public timestamp: Long;

                /** SigningInput nonce. */
                public nonce: Uint8Array;

                /** SigningInput networkId. */
                public networkId: Uint8Array;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Icon.Proto.ISigningInput): TW.Icon.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Icon.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Icon.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Icon.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Icon.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Icon.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (string|null);

                /** SigningOutput signature */
                signature?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Icon.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: string;

                /** SigningOutput signature. */
                public signature: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Icon.Proto.ISigningOutput): TW.Icon.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Icon.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Icon.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Icon.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Icon.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Icon.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace IoTeX. */
    namespace IoTeX {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a Transfer. */
            interface ITransfer {

                /** Transfer amount */
                amount?: (string|null);

                /** Transfer recipient */
                recipient?: (string|null);

                /** Transfer payload */
                payload?: (Uint8Array|null);
            }

            /** Represents a Transfer. */
            class Transfer implements ITransfer {

                /**
                 * Constructs a new Transfer.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IoTeX.Proto.ITransfer);

                /** Transfer amount. */
                public amount: string;

                /** Transfer recipient. */
                public recipient: string;

                /** Transfer payload. */
                public payload: Uint8Array;

                /**
                 * Creates a new Transfer instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Transfer instance
                 */
                public static create(properties?: TW.IoTeX.Proto.ITransfer): TW.IoTeX.Proto.Transfer;

                /**
                 * Encodes the specified Transfer message. Does not implicitly {@link TW.IoTeX.Proto.Transfer.verify|verify} messages.
                 * @param message Transfer message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IoTeX.Proto.ITransfer, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Transfer message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Transfer
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.Transfer;

                /**
                 * Verifies a Transfer message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Transfer message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Transfer
                 */
                public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.Transfer;

                /**
                 * Creates a plain object from a Transfer message. Also converts values to other types if specified.
                 * @param message Transfer
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IoTeX.Proto.Transfer, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Transfer to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Staking. */
            interface IStaking {

                /** Staking stakeCreate */
                stakeCreate?: (TW.IoTeX.Proto.Staking.ICreate|null);

                /** Staking stakeUnstake */
                stakeUnstake?: (TW.IoTeX.Proto.Staking.IReclaim|null);

                /** Staking stakeWithdraw */
                stakeWithdraw?: (TW.IoTeX.Proto.Staking.IReclaim|null);

                /** Staking stakeAddDeposit */
                stakeAddDeposit?: (TW.IoTeX.Proto.Staking.IAddDeposit|null);

                /** Staking stakeRestake */
                stakeRestake?: (TW.IoTeX.Proto.Staking.IRestake|null);

                /** Staking stakeChangeCandidate */
                stakeChangeCandidate?: (TW.IoTeX.Proto.Staking.IChangeCandidate|null);

                /** Staking stakeTransferOwnership */
                stakeTransferOwnership?: (TW.IoTeX.Proto.Staking.ITransferOwnership|null);

                /** Staking candidateRegister */
                candidateRegister?: (TW.IoTeX.Proto.Staking.ICandidateRegister|null);

                /** Staking candidateUpdate */
                candidateUpdate?: (TW.IoTeX.Proto.Staking.ICandidateBasicInfo|null);
            }

            /** Represents a Staking. */
            class Staking implements IStaking {

                /**
                 * Constructs a new Staking.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IoTeX.Proto.IStaking);

                /** Staking stakeCreate. */
                public stakeCreate?: (TW.IoTeX.Proto.Staking.ICreate|null);

                /** Staking stakeUnstake. */
                public stakeUnstake?: (TW.IoTeX.Proto.Staking.IReclaim|null);

                /** Staking stakeWithdraw. */
                public stakeWithdraw?: (TW.IoTeX.Proto.Staking.IReclaim|null);

                /** Staking stakeAddDeposit. */
                public stakeAddDeposit?: (TW.IoTeX.Proto.Staking.IAddDeposit|null);

                /** Staking stakeRestake. */
                public stakeRestake?: (TW.IoTeX.Proto.Staking.IRestake|null);

                /** Staking stakeChangeCandidate. */
                public stakeChangeCandidate?: (TW.IoTeX.Proto.Staking.IChangeCandidate|null);

                /** Staking stakeTransferOwnership. */
                public stakeTransferOwnership?: (TW.IoTeX.Proto.Staking.ITransferOwnership|null);

                /** Staking candidateRegister. */
                public candidateRegister?: (TW.IoTeX.Proto.Staking.ICandidateRegister|null);

                /** Staking candidateUpdate. */
                public candidateUpdate?: (TW.IoTeX.Proto.Staking.ICandidateBasicInfo|null);

                /** Staking message. */
                public message?: ("stakeCreate"|"stakeUnstake"|"stakeWithdraw"|"stakeAddDeposit"|"stakeRestake"|"stakeChangeCandidate"|"stakeTransferOwnership"|"candidateRegister"|"candidateUpdate");

                /**
                 * Creates a new Staking instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Staking instance
                 */
                public static create(properties?: TW.IoTeX.Proto.IStaking): TW.IoTeX.Proto.Staking;

                /**
                 * Encodes the specified Staking message. Does not implicitly {@link TW.IoTeX.Proto.Staking.verify|verify} messages.
                 * @param message Staking message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IoTeX.Proto.IStaking, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Staking message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Staking
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.Staking;

                /**
                 * Verifies a Staking message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Staking message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Staking
                 */
                public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.Staking;

                /**
                 * Creates a plain object from a Staking message. Also converts values to other types if specified.
                 * @param message Staking
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IoTeX.Proto.Staking, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Staking to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            namespace Staking {

                /** Properties of a Create. */
                interface ICreate {

                    /** Create candidateName */
                    candidateName?: (string|null);

                    /** Create stakedAmount */
                    stakedAmount?: (string|null);

                    /** Create stakedDuration */
                    stakedDuration?: (number|null);

                    /** Create autoStake */
                    autoStake?: (boolean|null);

                    /** Create payload */
                    payload?: (Uint8Array|null);
                }

                /** Represents a Create. */
                class Create implements ICreate {

                    /**
                     * Constructs a new Create.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.IoTeX.Proto.Staking.ICreate);

                    /** Create candidateName. */
                    public candidateName: string;

                    /** Create stakedAmount. */
                    public stakedAmount: string;

                    /** Create stakedDuration. */
                    public stakedDuration: number;

                    /** Create autoStake. */
                    public autoStake: boolean;

                    /** Create payload. */
                    public payload: Uint8Array;

                    /**
                     * Creates a new Create instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Create instance
                     */
                    public static create(properties?: TW.IoTeX.Proto.Staking.ICreate): TW.IoTeX.Proto.Staking.Create;

                    /**
                     * Encodes the specified Create message. Does not implicitly {@link TW.IoTeX.Proto.Staking.Create.verify|verify} messages.
                     * @param message Create message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.IoTeX.Proto.Staking.ICreate, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Create message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Create
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.Staking.Create;

                    /**
                     * Verifies a Create message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Create message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Create
                     */
                    public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.Staking.Create;

                    /**
                     * Creates a plain object from a Create message. Also converts values to other types if specified.
                     * @param message Create
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.IoTeX.Proto.Staking.Create, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Create to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a Reclaim. */
                interface IReclaim {

                    /** Reclaim bucketIndex */
                    bucketIndex?: (Long|null);

                    /** Reclaim payload */
                    payload?: (Uint8Array|null);
                }

                /** Represents a Reclaim. */
                class Reclaim implements IReclaim {

                    /**
                     * Constructs a new Reclaim.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.IoTeX.Proto.Staking.IReclaim);

                    /** Reclaim bucketIndex. */
                    public bucketIndex: Long;

                    /** Reclaim payload. */
                    public payload: Uint8Array;

                    /**
                     * Creates a new Reclaim instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Reclaim instance
                     */
                    public static create(properties?: TW.IoTeX.Proto.Staking.IReclaim): TW.IoTeX.Proto.Staking.Reclaim;

                    /**
                     * Encodes the specified Reclaim message. Does not implicitly {@link TW.IoTeX.Proto.Staking.Reclaim.verify|verify} messages.
                     * @param message Reclaim message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.IoTeX.Proto.Staking.IReclaim, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Reclaim message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Reclaim
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.Staking.Reclaim;

                    /**
                     * Verifies a Reclaim message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Reclaim message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Reclaim
                     */
                    public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.Staking.Reclaim;

                    /**
                     * Creates a plain object from a Reclaim message. Also converts values to other types if specified.
                     * @param message Reclaim
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.IoTeX.Proto.Staking.Reclaim, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Reclaim to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of an AddDeposit. */
                interface IAddDeposit {

                    /** AddDeposit bucketIndex */
                    bucketIndex?: (Long|null);

                    /** AddDeposit amount */
                    amount?: (string|null);

                    /** AddDeposit payload */
                    payload?: (Uint8Array|null);
                }

                /** Represents an AddDeposit. */
                class AddDeposit implements IAddDeposit {

                    /**
                     * Constructs a new AddDeposit.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.IoTeX.Proto.Staking.IAddDeposit);

                    /** AddDeposit bucketIndex. */
                    public bucketIndex: Long;

                    /** AddDeposit amount. */
                    public amount: string;

                    /** AddDeposit payload. */
                    public payload: Uint8Array;

                    /**
                     * Creates a new AddDeposit instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns AddDeposit instance
                     */
                    public static create(properties?: TW.IoTeX.Proto.Staking.IAddDeposit): TW.IoTeX.Proto.Staking.AddDeposit;

                    /**
                     * Encodes the specified AddDeposit message. Does not implicitly {@link TW.IoTeX.Proto.Staking.AddDeposit.verify|verify} messages.
                     * @param message AddDeposit message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.IoTeX.Proto.Staking.IAddDeposit, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes an AddDeposit message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns AddDeposit
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.Staking.AddDeposit;

                    /**
                     * Verifies an AddDeposit message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates an AddDeposit message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns AddDeposit
                     */
                    public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.Staking.AddDeposit;

                    /**
                     * Creates a plain object from an AddDeposit message. Also converts values to other types if specified.
                     * @param message AddDeposit
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.IoTeX.Proto.Staking.AddDeposit, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this AddDeposit to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a Restake. */
                interface IRestake {

                    /** Restake bucketIndex */
                    bucketIndex?: (Long|null);

                    /** Restake stakedDuration */
                    stakedDuration?: (number|null);

                    /** Restake autoStake */
                    autoStake?: (boolean|null);

                    /** Restake payload */
                    payload?: (Uint8Array|null);
                }

                /** Represents a Restake. */
                class Restake implements IRestake {

                    /**
                     * Constructs a new Restake.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.IoTeX.Proto.Staking.IRestake);

                    /** Restake bucketIndex. */
                    public bucketIndex: Long;

                    /** Restake stakedDuration. */
                    public stakedDuration: number;

                    /** Restake autoStake. */
                    public autoStake: boolean;

                    /** Restake payload. */
                    public payload: Uint8Array;

                    /**
                     * Creates a new Restake instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Restake instance
                     */
                    public static create(properties?: TW.IoTeX.Proto.Staking.IRestake): TW.IoTeX.Proto.Staking.Restake;

                    /**
                     * Encodes the specified Restake message. Does not implicitly {@link TW.IoTeX.Proto.Staking.Restake.verify|verify} messages.
                     * @param message Restake message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.IoTeX.Proto.Staking.IRestake, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Restake message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Restake
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.Staking.Restake;

                    /**
                     * Verifies a Restake message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Restake message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Restake
                     */
                    public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.Staking.Restake;

                    /**
                     * Creates a plain object from a Restake message. Also converts values to other types if specified.
                     * @param message Restake
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.IoTeX.Proto.Staking.Restake, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Restake to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a ChangeCandidate. */
                interface IChangeCandidate {

                    /** ChangeCandidate bucketIndex */
                    bucketIndex?: (Long|null);

                    /** ChangeCandidate candidateName */
                    candidateName?: (string|null);

                    /** ChangeCandidate payload */
                    payload?: (Uint8Array|null);
                }

                /** Represents a ChangeCandidate. */
                class ChangeCandidate implements IChangeCandidate {

                    /**
                     * Constructs a new ChangeCandidate.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.IoTeX.Proto.Staking.IChangeCandidate);

                    /** ChangeCandidate bucketIndex. */
                    public bucketIndex: Long;

                    /** ChangeCandidate candidateName. */
                    public candidateName: string;

                    /** ChangeCandidate payload. */
                    public payload: Uint8Array;

                    /**
                     * Creates a new ChangeCandidate instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns ChangeCandidate instance
                     */
                    public static create(properties?: TW.IoTeX.Proto.Staking.IChangeCandidate): TW.IoTeX.Proto.Staking.ChangeCandidate;

                    /**
                     * Encodes the specified ChangeCandidate message. Does not implicitly {@link TW.IoTeX.Proto.Staking.ChangeCandidate.verify|verify} messages.
                     * @param message ChangeCandidate message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.IoTeX.Proto.Staking.IChangeCandidate, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a ChangeCandidate message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns ChangeCandidate
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.Staking.ChangeCandidate;

                    /**
                     * Verifies a ChangeCandidate message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a ChangeCandidate message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns ChangeCandidate
                     */
                    public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.Staking.ChangeCandidate;

                    /**
                     * Creates a plain object from a ChangeCandidate message. Also converts values to other types if specified.
                     * @param message ChangeCandidate
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.IoTeX.Proto.Staking.ChangeCandidate, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this ChangeCandidate to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a TransferOwnership. */
                interface ITransferOwnership {

                    /** TransferOwnership bucketIndex */
                    bucketIndex?: (Long|null);

                    /** TransferOwnership voterAddress */
                    voterAddress?: (string|null);

                    /** TransferOwnership payload */
                    payload?: (Uint8Array|null);
                }

                /** Represents a TransferOwnership. */
                class TransferOwnership implements ITransferOwnership {

                    /**
                     * Constructs a new TransferOwnership.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.IoTeX.Proto.Staking.ITransferOwnership);

                    /** TransferOwnership bucketIndex. */
                    public bucketIndex: Long;

                    /** TransferOwnership voterAddress. */
                    public voterAddress: string;

                    /** TransferOwnership payload. */
                    public payload: Uint8Array;

                    /**
                     * Creates a new TransferOwnership instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns TransferOwnership instance
                     */
                    public static create(properties?: TW.IoTeX.Proto.Staking.ITransferOwnership): TW.IoTeX.Proto.Staking.TransferOwnership;

                    /**
                     * Encodes the specified TransferOwnership message. Does not implicitly {@link TW.IoTeX.Proto.Staking.TransferOwnership.verify|verify} messages.
                     * @param message TransferOwnership message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.IoTeX.Proto.Staking.ITransferOwnership, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a TransferOwnership message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns TransferOwnership
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.Staking.TransferOwnership;

                    /**
                     * Verifies a TransferOwnership message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a TransferOwnership message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns TransferOwnership
                     */
                    public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.Staking.TransferOwnership;

                    /**
                     * Creates a plain object from a TransferOwnership message. Also converts values to other types if specified.
                     * @param message TransferOwnership
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.IoTeX.Proto.Staking.TransferOwnership, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this TransferOwnership to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a CandidateBasicInfo. */
                interface ICandidateBasicInfo {

                    /** CandidateBasicInfo name */
                    name?: (string|null);

                    /** CandidateBasicInfo operatorAddress */
                    operatorAddress?: (string|null);

                    /** CandidateBasicInfo rewardAddress */
                    rewardAddress?: (string|null);
                }

                /** Represents a CandidateBasicInfo. */
                class CandidateBasicInfo implements ICandidateBasicInfo {

                    /**
                     * Constructs a new CandidateBasicInfo.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.IoTeX.Proto.Staking.ICandidateBasicInfo);

                    /** CandidateBasicInfo name. */
                    public name: string;

                    /** CandidateBasicInfo operatorAddress. */
                    public operatorAddress: string;

                    /** CandidateBasicInfo rewardAddress. */
                    public rewardAddress: string;

                    /**
                     * Creates a new CandidateBasicInfo instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns CandidateBasicInfo instance
                     */
                    public static create(properties?: TW.IoTeX.Proto.Staking.ICandidateBasicInfo): TW.IoTeX.Proto.Staking.CandidateBasicInfo;

                    /**
                     * Encodes the specified CandidateBasicInfo message. Does not implicitly {@link TW.IoTeX.Proto.Staking.CandidateBasicInfo.verify|verify} messages.
                     * @param message CandidateBasicInfo message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.IoTeX.Proto.Staking.ICandidateBasicInfo, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a CandidateBasicInfo message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns CandidateBasicInfo
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.Staking.CandidateBasicInfo;

                    /**
                     * Verifies a CandidateBasicInfo message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a CandidateBasicInfo message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns CandidateBasicInfo
                     */
                    public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.Staking.CandidateBasicInfo;

                    /**
                     * Creates a plain object from a CandidateBasicInfo message. Also converts values to other types if specified.
                     * @param message CandidateBasicInfo
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.IoTeX.Proto.Staking.CandidateBasicInfo, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this CandidateBasicInfo to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a CandidateRegister. */
                interface ICandidateRegister {

                    /** CandidateRegister candidate */
                    candidate?: (TW.IoTeX.Proto.Staking.ICandidateBasicInfo|null);

                    /** CandidateRegister stakedAmount */
                    stakedAmount?: (string|null);

                    /** CandidateRegister stakedDuration */
                    stakedDuration?: (number|null);

                    /** CandidateRegister autoStake */
                    autoStake?: (boolean|null);

                    /** CandidateRegister ownerAddress */
                    ownerAddress?: (string|null);

                    /** CandidateRegister payload */
                    payload?: (Uint8Array|null);
                }

                /** Represents a CandidateRegister. */
                class CandidateRegister implements ICandidateRegister {

                    /**
                     * Constructs a new CandidateRegister.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.IoTeX.Proto.Staking.ICandidateRegister);

                    /** CandidateRegister candidate. */
                    public candidate?: (TW.IoTeX.Proto.Staking.ICandidateBasicInfo|null);

                    /** CandidateRegister stakedAmount. */
                    public stakedAmount: string;

                    /** CandidateRegister stakedDuration. */
                    public stakedDuration: number;

                    /** CandidateRegister autoStake. */
                    public autoStake: boolean;

                    /** CandidateRegister ownerAddress. */
                    public ownerAddress: string;

                    /** CandidateRegister payload. */
                    public payload: Uint8Array;

                    /**
                     * Creates a new CandidateRegister instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns CandidateRegister instance
                     */
                    public static create(properties?: TW.IoTeX.Proto.Staking.ICandidateRegister): TW.IoTeX.Proto.Staking.CandidateRegister;

                    /**
                     * Encodes the specified CandidateRegister message. Does not implicitly {@link TW.IoTeX.Proto.Staking.CandidateRegister.verify|verify} messages.
                     * @param message CandidateRegister message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.IoTeX.Proto.Staking.ICandidateRegister, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a CandidateRegister message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns CandidateRegister
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.Staking.CandidateRegister;

                    /**
                     * Verifies a CandidateRegister message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a CandidateRegister message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns CandidateRegister
                     */
                    public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.Staking.CandidateRegister;

                    /**
                     * Creates a plain object from a CandidateRegister message. Also converts values to other types if specified.
                     * @param message CandidateRegister
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.IoTeX.Proto.Staking.CandidateRegister, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this CandidateRegister to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }
            }

            /** Properties of a ContractCall. */
            interface IContractCall {

                /** ContractCall amount */
                amount?: (string|null);

                /** ContractCall contract */
                contract?: (string|null);

                /** ContractCall data */
                data?: (Uint8Array|null);
            }

            /** Represents a ContractCall. */
            class ContractCall implements IContractCall {

                /**
                 * Constructs a new ContractCall.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IoTeX.Proto.IContractCall);

                /** ContractCall amount. */
                public amount: string;

                /** ContractCall contract. */
                public contract: string;

                /** ContractCall data. */
                public data: Uint8Array;

                /**
                 * Creates a new ContractCall instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns ContractCall instance
                 */
                public static create(properties?: TW.IoTeX.Proto.IContractCall): TW.IoTeX.Proto.ContractCall;

                /**
                 * Encodes the specified ContractCall message. Does not implicitly {@link TW.IoTeX.Proto.ContractCall.verify|verify} messages.
                 * @param message ContractCall message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IoTeX.Proto.IContractCall, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a ContractCall message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns ContractCall
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.ContractCall;

                /**
                 * Verifies a ContractCall message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a ContractCall message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns ContractCall
                 */
                public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.ContractCall;

                /**
                 * Creates a plain object from a ContractCall message. Also converts values to other types if specified.
                 * @param message ContractCall
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IoTeX.Proto.ContractCall, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this ContractCall to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput version */
                version?: (number|null);

                /** SigningInput nonce */
                nonce?: (Long|null);

                /** SigningInput gasLimit */
                gasLimit?: (Long|null);

                /** SigningInput gasPrice */
                gasPrice?: (string|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput transfer */
                transfer?: (TW.IoTeX.Proto.ITransfer|null);

                /** SigningInput call */
                call?: (TW.IoTeX.Proto.IContractCall|null);

                /** SigningInput stakeCreate */
                stakeCreate?: (TW.IoTeX.Proto.Staking.ICreate|null);

                /** SigningInput stakeUnstake */
                stakeUnstake?: (TW.IoTeX.Proto.Staking.IReclaim|null);

                /** SigningInput stakeWithdraw */
                stakeWithdraw?: (TW.IoTeX.Proto.Staking.IReclaim|null);

                /** SigningInput stakeAddDeposit */
                stakeAddDeposit?: (TW.IoTeX.Proto.Staking.IAddDeposit|null);

                /** SigningInput stakeRestake */
                stakeRestake?: (TW.IoTeX.Proto.Staking.IRestake|null);

                /** SigningInput stakeChangeCandidate */
                stakeChangeCandidate?: (TW.IoTeX.Proto.Staking.IChangeCandidate|null);

                /** SigningInput stakeTransferOwnership */
                stakeTransferOwnership?: (TW.IoTeX.Proto.Staking.ITransferOwnership|null);

                /** SigningInput candidateRegister */
                candidateRegister?: (TW.IoTeX.Proto.Staking.ICandidateRegister|null);

                /** SigningInput candidateUpdate */
                candidateUpdate?: (TW.IoTeX.Proto.Staking.ICandidateBasicInfo|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IoTeX.Proto.ISigningInput);

                /** SigningInput version. */
                public version: number;

                /** SigningInput nonce. */
                public nonce: Long;

                /** SigningInput gasLimit. */
                public gasLimit: Long;

                /** SigningInput gasPrice. */
                public gasPrice: string;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput transfer. */
                public transfer?: (TW.IoTeX.Proto.ITransfer|null);

                /** SigningInput call. */
                public call?: (TW.IoTeX.Proto.IContractCall|null);

                /** SigningInput stakeCreate. */
                public stakeCreate?: (TW.IoTeX.Proto.Staking.ICreate|null);

                /** SigningInput stakeUnstake. */
                public stakeUnstake?: (TW.IoTeX.Proto.Staking.IReclaim|null);

                /** SigningInput stakeWithdraw. */
                public stakeWithdraw?: (TW.IoTeX.Proto.Staking.IReclaim|null);

                /** SigningInput stakeAddDeposit. */
                public stakeAddDeposit?: (TW.IoTeX.Proto.Staking.IAddDeposit|null);

                /** SigningInput stakeRestake. */
                public stakeRestake?: (TW.IoTeX.Proto.Staking.IRestake|null);

                /** SigningInput stakeChangeCandidate. */
                public stakeChangeCandidate?: (TW.IoTeX.Proto.Staking.IChangeCandidate|null);

                /** SigningInput stakeTransferOwnership. */
                public stakeTransferOwnership?: (TW.IoTeX.Proto.Staking.ITransferOwnership|null);

                /** SigningInput candidateRegister. */
                public candidateRegister?: (TW.IoTeX.Proto.Staking.ICandidateRegister|null);

                /** SigningInput candidateUpdate. */
                public candidateUpdate?: (TW.IoTeX.Proto.Staking.ICandidateBasicInfo|null);

                /** SigningInput action. */
                public action?: ("transfer"|"call"|"stakeCreate"|"stakeUnstake"|"stakeWithdraw"|"stakeAddDeposit"|"stakeRestake"|"stakeChangeCandidate"|"stakeTransferOwnership"|"candidateRegister"|"candidateUpdate");

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.IoTeX.Proto.ISigningInput): TW.IoTeX.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.IoTeX.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IoTeX.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IoTeX.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);

                /** SigningOutput hash */
                hash?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IoTeX.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /** SigningOutput hash. */
                public hash: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.IoTeX.Proto.ISigningOutput): TW.IoTeX.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.IoTeX.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IoTeX.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IoTeX.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an ActionCore. */
            interface IActionCore {

                /** ActionCore version */
                version?: (number|null);

                /** ActionCore nonce */
                nonce?: (Long|null);

                /** ActionCore gasLimit */
                gasLimit?: (Long|null);

                /** ActionCore gasPrice */
                gasPrice?: (string|null);

                /** ActionCore transfer */
                transfer?: (TW.IoTeX.Proto.ITransfer|null);

                /** ActionCore execution */
                execution?: (TW.IoTeX.Proto.IContractCall|null);

                /** ActionCore stakeCreate */
                stakeCreate?: (TW.IoTeX.Proto.Staking.ICreate|null);

                /** ActionCore stakeUnstake */
                stakeUnstake?: (TW.IoTeX.Proto.Staking.IReclaim|null);

                /** ActionCore stakeWithdraw */
                stakeWithdraw?: (TW.IoTeX.Proto.Staking.IReclaim|null);

                /** ActionCore stakeAddDeposit */
                stakeAddDeposit?: (TW.IoTeX.Proto.Staking.IAddDeposit|null);

                /** ActionCore stakeRestake */
                stakeRestake?: (TW.IoTeX.Proto.Staking.IRestake|null);

                /** ActionCore stakeChangeCandidate */
                stakeChangeCandidate?: (TW.IoTeX.Proto.Staking.IChangeCandidate|null);

                /** ActionCore stakeTransferOwnership */
                stakeTransferOwnership?: (TW.IoTeX.Proto.Staking.ITransferOwnership|null);

                /** ActionCore candidateRegister */
                candidateRegister?: (TW.IoTeX.Proto.Staking.ICandidateRegister|null);

                /** ActionCore candidateUpdate */
                candidateUpdate?: (TW.IoTeX.Proto.Staking.ICandidateBasicInfo|null);
            }

            /** Represents an ActionCore. */
            class ActionCore implements IActionCore {

                /**
                 * Constructs a new ActionCore.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IoTeX.Proto.IActionCore);

                /** ActionCore version. */
                public version: number;

                /** ActionCore nonce. */
                public nonce: Long;

                /** ActionCore gasLimit. */
                public gasLimit: Long;

                /** ActionCore gasPrice. */
                public gasPrice: string;

                /** ActionCore transfer. */
                public transfer?: (TW.IoTeX.Proto.ITransfer|null);

                /** ActionCore execution. */
                public execution?: (TW.IoTeX.Proto.IContractCall|null);

                /** ActionCore stakeCreate. */
                public stakeCreate?: (TW.IoTeX.Proto.Staking.ICreate|null);

                /** ActionCore stakeUnstake. */
                public stakeUnstake?: (TW.IoTeX.Proto.Staking.IReclaim|null);

                /** ActionCore stakeWithdraw. */
                public stakeWithdraw?: (TW.IoTeX.Proto.Staking.IReclaim|null);

                /** ActionCore stakeAddDeposit. */
                public stakeAddDeposit?: (TW.IoTeX.Proto.Staking.IAddDeposit|null);

                /** ActionCore stakeRestake. */
                public stakeRestake?: (TW.IoTeX.Proto.Staking.IRestake|null);

                /** ActionCore stakeChangeCandidate. */
                public stakeChangeCandidate?: (TW.IoTeX.Proto.Staking.IChangeCandidate|null);

                /** ActionCore stakeTransferOwnership. */
                public stakeTransferOwnership?: (TW.IoTeX.Proto.Staking.ITransferOwnership|null);

                /** ActionCore candidateRegister. */
                public candidateRegister?: (TW.IoTeX.Proto.Staking.ICandidateRegister|null);

                /** ActionCore candidateUpdate. */
                public candidateUpdate?: (TW.IoTeX.Proto.Staking.ICandidateBasicInfo|null);

                /** ActionCore action. */
                public action?: ("transfer"|"execution"|"stakeCreate"|"stakeUnstake"|"stakeWithdraw"|"stakeAddDeposit"|"stakeRestake"|"stakeChangeCandidate"|"stakeTransferOwnership"|"candidateRegister"|"candidateUpdate");

                /**
                 * Creates a new ActionCore instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns ActionCore instance
                 */
                public static create(properties?: TW.IoTeX.Proto.IActionCore): TW.IoTeX.Proto.ActionCore;

                /**
                 * Encodes the specified ActionCore message. Does not implicitly {@link TW.IoTeX.Proto.ActionCore.verify|verify} messages.
                 * @param message ActionCore message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IoTeX.Proto.IActionCore, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an ActionCore message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns ActionCore
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.ActionCore;

                /**
                 * Verifies an ActionCore message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an ActionCore message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns ActionCore
                 */
                public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.ActionCore;

                /**
                 * Creates a plain object from an ActionCore message. Also converts values to other types if specified.
                 * @param message ActionCore
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IoTeX.Proto.ActionCore, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this ActionCore to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an Action. */
            interface IAction {

                /** Action core */
                core?: (TW.IoTeX.Proto.IActionCore|null);

                /** Action senderPubKey */
                senderPubKey?: (Uint8Array|null);

                /** Action signature */
                signature?: (Uint8Array|null);
            }

            /** Represents an Action. */
            class Action implements IAction {

                /**
                 * Constructs a new Action.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IoTeX.Proto.IAction);

                /** Action core. */
                public core?: (TW.IoTeX.Proto.IActionCore|null);

                /** Action senderPubKey. */
                public senderPubKey: Uint8Array;

                /** Action signature. */
                public signature: Uint8Array;

                /**
                 * Creates a new Action instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Action instance
                 */
                public static create(properties?: TW.IoTeX.Proto.IAction): TW.IoTeX.Proto.Action;

                /**
                 * Encodes the specified Action message. Does not implicitly {@link TW.IoTeX.Proto.Action.verify|verify} messages.
                 * @param message Action message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IoTeX.Proto.IAction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an Action message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Action
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.Action;

                /**
                 * Verifies an Action message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an Action message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Action
                 */
                public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.Action;

                /**
                 * Creates a plain object from an Action message. Also converts values to other types if specified.
                 * @param message Action
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IoTeX.Proto.Action, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Action to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Nano. */
    namespace Nano {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput parentBlock */
                parentBlock?: (Uint8Array|null);

                /** SigningInput linkBlock */
                linkBlock?: (Uint8Array|null);

                /** SigningInput linkRecipient */
                linkRecipient?: (string|null);

                /** SigningInput representative */
                representative?: (string|null);

                /** SigningInput balance */
                balance?: (string|null);

                /** SigningInput work */
                work?: (string|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Nano.Proto.ISigningInput);

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput parentBlock. */
                public parentBlock: Uint8Array;

                /** SigningInput linkBlock. */
                public linkBlock: Uint8Array;

                /** SigningInput linkRecipient. */
                public linkRecipient: string;

                /** SigningInput representative. */
                public representative: string;

                /** SigningInput balance. */
                public balance: string;

                /** SigningInput work. */
                public work: string;

                /** SigningInput linkOneof. */
                public linkOneof?: ("linkBlock"|"linkRecipient");

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Nano.Proto.ISigningInput): TW.Nano.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Nano.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Nano.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Nano.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Nano.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Nano.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput signature */
                signature?: (Uint8Array|null);

                /** SigningOutput blockHash */
                blockHash?: (Uint8Array|null);

                /** SigningOutput json */
                json?: (string|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Nano.Proto.ISigningOutput);

                /** SigningOutput signature. */
                public signature: Uint8Array;

                /** SigningOutput blockHash. */
                public blockHash: Uint8Array;

                /** SigningOutput json. */
                public json: string;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Nano.Proto.ISigningOutput): TW.Nano.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Nano.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Nano.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Nano.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Nano.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Nano.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace NEAR. */
    namespace NEAR {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a PublicKey. */
            interface IPublicKey {

                /** PublicKey keyType */
                keyType?: (number|null);

                /** PublicKey data */
                data?: (Uint8Array|null);
            }

            /** Represents a PublicKey. */
            class PublicKey implements IPublicKey {

                /**
                 * Constructs a new PublicKey.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.IPublicKey);

                /** PublicKey keyType. */
                public keyType: number;

                /** PublicKey data. */
                public data: Uint8Array;

                /**
                 * Creates a new PublicKey instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns PublicKey instance
                 */
                public static create(properties?: TW.NEAR.Proto.IPublicKey): TW.NEAR.Proto.PublicKey;

                /**
                 * Encodes the specified PublicKey message. Does not implicitly {@link TW.NEAR.Proto.PublicKey.verify|verify} messages.
                 * @param message PublicKey message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.IPublicKey, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a PublicKey message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns PublicKey
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.PublicKey;

                /**
                 * Verifies a PublicKey message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a PublicKey message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns PublicKey
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.PublicKey;

                /**
                 * Creates a plain object from a PublicKey message. Also converts values to other types if specified.
                 * @param message PublicKey
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.PublicKey, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this PublicKey to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a FunctionCallPermission. */
            interface IFunctionCallPermission {

                /** FunctionCallPermission allowance */
                allowance?: (Uint8Array|null);

                /** FunctionCallPermission receiverId */
                receiverId?: (string|null);
            }

            /** Represents a FunctionCallPermission. */
            class FunctionCallPermission implements IFunctionCallPermission {

                /**
                 * Constructs a new FunctionCallPermission.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.IFunctionCallPermission);

                /** FunctionCallPermission allowance. */
                public allowance: Uint8Array;

                /** FunctionCallPermission receiverId. */
                public receiverId: string;

                /**
                 * Creates a new FunctionCallPermission instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns FunctionCallPermission instance
                 */
                public static create(properties?: TW.NEAR.Proto.IFunctionCallPermission): TW.NEAR.Proto.FunctionCallPermission;

                /**
                 * Encodes the specified FunctionCallPermission message. Does not implicitly {@link TW.NEAR.Proto.FunctionCallPermission.verify|verify} messages.
                 * @param message FunctionCallPermission message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.IFunctionCallPermission, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a FunctionCallPermission message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns FunctionCallPermission
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.FunctionCallPermission;

                /**
                 * Verifies a FunctionCallPermission message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a FunctionCallPermission message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns FunctionCallPermission
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.FunctionCallPermission;

                /**
                 * Creates a plain object from a FunctionCallPermission message. Also converts values to other types if specified.
                 * @param message FunctionCallPermission
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.FunctionCallPermission, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this FunctionCallPermission to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a FullAccessPermission. */
            interface IFullAccessPermission {
            }

            /** Represents a FullAccessPermission. */
            class FullAccessPermission implements IFullAccessPermission {

                /**
                 * Constructs a new FullAccessPermission.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.IFullAccessPermission);

                /**
                 * Creates a new FullAccessPermission instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns FullAccessPermission instance
                 */
                public static create(properties?: TW.NEAR.Proto.IFullAccessPermission): TW.NEAR.Proto.FullAccessPermission;

                /**
                 * Encodes the specified FullAccessPermission message. Does not implicitly {@link TW.NEAR.Proto.FullAccessPermission.verify|verify} messages.
                 * @param message FullAccessPermission message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.IFullAccessPermission, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a FullAccessPermission message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns FullAccessPermission
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.FullAccessPermission;

                /**
                 * Verifies a FullAccessPermission message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a FullAccessPermission message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns FullAccessPermission
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.FullAccessPermission;

                /**
                 * Creates a plain object from a FullAccessPermission message. Also converts values to other types if specified.
                 * @param message FullAccessPermission
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.FullAccessPermission, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this FullAccessPermission to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an AccessKey. */
            interface IAccessKey {

                /** AccessKey nonce */
                nonce?: (Long|null);

                /** AccessKey functionCall */
                functionCall?: (TW.NEAR.Proto.IFunctionCallPermission|null);

                /** AccessKey fullAccess */
                fullAccess?: (TW.NEAR.Proto.IFullAccessPermission|null);
            }

            /** Represents an AccessKey. */
            class AccessKey implements IAccessKey {

                /**
                 * Constructs a new AccessKey.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.IAccessKey);

                /** AccessKey nonce. */
                public nonce: Long;

                /** AccessKey functionCall. */
                public functionCall?: (TW.NEAR.Proto.IFunctionCallPermission|null);

                /** AccessKey fullAccess. */
                public fullAccess?: (TW.NEAR.Proto.IFullAccessPermission|null);

                /** AccessKey permission. */
                public permission?: ("functionCall"|"fullAccess");

                /**
                 * Creates a new AccessKey instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns AccessKey instance
                 */
                public static create(properties?: TW.NEAR.Proto.IAccessKey): TW.NEAR.Proto.AccessKey;

                /**
                 * Encodes the specified AccessKey message. Does not implicitly {@link TW.NEAR.Proto.AccessKey.verify|verify} messages.
                 * @param message AccessKey message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.IAccessKey, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an AccessKey message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns AccessKey
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.AccessKey;

                /**
                 * Verifies an AccessKey message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an AccessKey message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns AccessKey
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.AccessKey;

                /**
                 * Creates a plain object from an AccessKey message. Also converts values to other types if specified.
                 * @param message AccessKey
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.AccessKey, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this AccessKey to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a CreateAccount. */
            interface ICreateAccount {
            }

            /** Represents a CreateAccount. */
            class CreateAccount implements ICreateAccount {

                /**
                 * Constructs a new CreateAccount.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.ICreateAccount);

                /**
                 * Creates a new CreateAccount instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns CreateAccount instance
                 */
                public static create(properties?: TW.NEAR.Proto.ICreateAccount): TW.NEAR.Proto.CreateAccount;

                /**
                 * Encodes the specified CreateAccount message. Does not implicitly {@link TW.NEAR.Proto.CreateAccount.verify|verify} messages.
                 * @param message CreateAccount message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.ICreateAccount, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a CreateAccount message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns CreateAccount
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.CreateAccount;

                /**
                 * Verifies a CreateAccount message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a CreateAccount message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns CreateAccount
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.CreateAccount;

                /**
                 * Creates a plain object from a CreateAccount message. Also converts values to other types if specified.
                 * @param message CreateAccount
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.CreateAccount, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this CreateAccount to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a DeployContract. */
            interface IDeployContract {

                /** DeployContract code */
                code?: (Uint8Array|null);
            }

            /** Represents a DeployContract. */
            class DeployContract implements IDeployContract {

                /**
                 * Constructs a new DeployContract.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.IDeployContract);

                /** DeployContract code. */
                public code: Uint8Array;

                /**
                 * Creates a new DeployContract instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns DeployContract instance
                 */
                public static create(properties?: TW.NEAR.Proto.IDeployContract): TW.NEAR.Proto.DeployContract;

                /**
                 * Encodes the specified DeployContract message. Does not implicitly {@link TW.NEAR.Proto.DeployContract.verify|verify} messages.
                 * @param message DeployContract message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.IDeployContract, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a DeployContract message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns DeployContract
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.DeployContract;

                /**
                 * Verifies a DeployContract message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a DeployContract message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns DeployContract
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.DeployContract;

                /**
                 * Creates a plain object from a DeployContract message. Also converts values to other types if specified.
                 * @param message DeployContract
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.DeployContract, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this DeployContract to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a FunctionCall. */
            interface IFunctionCall {

                /** FunctionCall methodName */
                methodName?: (Uint8Array|null);

                /** FunctionCall args */
                args?: (Uint8Array|null);

                /** FunctionCall gas */
                gas?: (Long|null);

                /** FunctionCall deposit */
                deposit?: (Uint8Array|null);
            }

            /** Represents a FunctionCall. */
            class FunctionCall implements IFunctionCall {

                /**
                 * Constructs a new FunctionCall.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.IFunctionCall);

                /** FunctionCall methodName. */
                public methodName: Uint8Array;

                /** FunctionCall args. */
                public args: Uint8Array;

                /** FunctionCall gas. */
                public gas: Long;

                /** FunctionCall deposit. */
                public deposit: Uint8Array;

                /**
                 * Creates a new FunctionCall instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns FunctionCall instance
                 */
                public static create(properties?: TW.NEAR.Proto.IFunctionCall): TW.NEAR.Proto.FunctionCall;

                /**
                 * Encodes the specified FunctionCall message. Does not implicitly {@link TW.NEAR.Proto.FunctionCall.verify|verify} messages.
                 * @param message FunctionCall message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.IFunctionCall, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a FunctionCall message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns FunctionCall
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.FunctionCall;

                /**
                 * Verifies a FunctionCall message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a FunctionCall message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns FunctionCall
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.FunctionCall;

                /**
                 * Creates a plain object from a FunctionCall message. Also converts values to other types if specified.
                 * @param message FunctionCall
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.FunctionCall, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this FunctionCall to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Transfer. */
            interface ITransfer {

                /** Transfer deposit */
                deposit?: (Uint8Array|null);
            }

            /** Represents a Transfer. */
            class Transfer implements ITransfer {

                /**
                 * Constructs a new Transfer.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.ITransfer);

                /** Transfer deposit. */
                public deposit: Uint8Array;

                /**
                 * Creates a new Transfer instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Transfer instance
                 */
                public static create(properties?: TW.NEAR.Proto.ITransfer): TW.NEAR.Proto.Transfer;

                /**
                 * Encodes the specified Transfer message. Does not implicitly {@link TW.NEAR.Proto.Transfer.verify|verify} messages.
                 * @param message Transfer message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.ITransfer, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Transfer message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Transfer
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.Transfer;

                /**
                 * Verifies a Transfer message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Transfer message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Transfer
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.Transfer;

                /**
                 * Creates a plain object from a Transfer message. Also converts values to other types if specified.
                 * @param message Transfer
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.Transfer, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Transfer to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Stake. */
            interface IStake {

                /** Stake stake */
                stake?: (Uint8Array|null);

                /** Stake publicKey */
                publicKey?: (string|null);
            }

            /** Represents a Stake. */
            class Stake implements IStake {

                /**
                 * Constructs a new Stake.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.IStake);

                /** Stake stake. */
                public stake: Uint8Array;

                /** Stake publicKey. */
                public publicKey: string;

                /**
                 * Creates a new Stake instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Stake instance
                 */
                public static create(properties?: TW.NEAR.Proto.IStake): TW.NEAR.Proto.Stake;

                /**
                 * Encodes the specified Stake message. Does not implicitly {@link TW.NEAR.Proto.Stake.verify|verify} messages.
                 * @param message Stake message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.IStake, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Stake message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Stake
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.Stake;

                /**
                 * Verifies a Stake message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Stake message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Stake
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.Stake;

                /**
                 * Creates a plain object from a Stake message. Also converts values to other types if specified.
                 * @param message Stake
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.Stake, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Stake to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an AddKey. */
            interface IAddKey {

                /** AddKey publicKey */
                publicKey?: (TW.NEAR.Proto.IPublicKey|null);

                /** AddKey accessKey */
                accessKey?: (TW.NEAR.Proto.IAccessKey|null);
            }

            /** Represents an AddKey. */
            class AddKey implements IAddKey {

                /**
                 * Constructs a new AddKey.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.IAddKey);

                /** AddKey publicKey. */
                public publicKey?: (TW.NEAR.Proto.IPublicKey|null);

                /** AddKey accessKey. */
                public accessKey?: (TW.NEAR.Proto.IAccessKey|null);

                /**
                 * Creates a new AddKey instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns AddKey instance
                 */
                public static create(properties?: TW.NEAR.Proto.IAddKey): TW.NEAR.Proto.AddKey;

                /**
                 * Encodes the specified AddKey message. Does not implicitly {@link TW.NEAR.Proto.AddKey.verify|verify} messages.
                 * @param message AddKey message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.IAddKey, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an AddKey message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns AddKey
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.AddKey;

                /**
                 * Verifies an AddKey message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an AddKey message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns AddKey
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.AddKey;

                /**
                 * Creates a plain object from an AddKey message. Also converts values to other types if specified.
                 * @param message AddKey
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.AddKey, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this AddKey to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a DeleteKey. */
            interface IDeleteKey {

                /** DeleteKey publicKey */
                publicKey?: (TW.NEAR.Proto.IPublicKey|null);
            }

            /** Represents a DeleteKey. */
            class DeleteKey implements IDeleteKey {

                /**
                 * Constructs a new DeleteKey.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.IDeleteKey);

                /** DeleteKey publicKey. */
                public publicKey?: (TW.NEAR.Proto.IPublicKey|null);

                /**
                 * Creates a new DeleteKey instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns DeleteKey instance
                 */
                public static create(properties?: TW.NEAR.Proto.IDeleteKey): TW.NEAR.Proto.DeleteKey;

                /**
                 * Encodes the specified DeleteKey message. Does not implicitly {@link TW.NEAR.Proto.DeleteKey.verify|verify} messages.
                 * @param message DeleteKey message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.IDeleteKey, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a DeleteKey message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns DeleteKey
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.DeleteKey;

                /**
                 * Verifies a DeleteKey message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a DeleteKey message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns DeleteKey
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.DeleteKey;

                /**
                 * Creates a plain object from a DeleteKey message. Also converts values to other types if specified.
                 * @param message DeleteKey
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.DeleteKey, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this DeleteKey to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a DeleteAccount. */
            interface IDeleteAccount {

                /** DeleteAccount beneficiaryId */
                beneficiaryId?: (string|null);
            }

            /** Represents a DeleteAccount. */
            class DeleteAccount implements IDeleteAccount {

                /**
                 * Constructs a new DeleteAccount.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.IDeleteAccount);

                /** DeleteAccount beneficiaryId. */
                public beneficiaryId: string;

                /**
                 * Creates a new DeleteAccount instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns DeleteAccount instance
                 */
                public static create(properties?: TW.NEAR.Proto.IDeleteAccount): TW.NEAR.Proto.DeleteAccount;

                /**
                 * Encodes the specified DeleteAccount message. Does not implicitly {@link TW.NEAR.Proto.DeleteAccount.verify|verify} messages.
                 * @param message DeleteAccount message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.IDeleteAccount, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a DeleteAccount message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns DeleteAccount
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.DeleteAccount;

                /**
                 * Verifies a DeleteAccount message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a DeleteAccount message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns DeleteAccount
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.DeleteAccount;

                /**
                 * Creates a plain object from a DeleteAccount message. Also converts values to other types if specified.
                 * @param message DeleteAccount
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.DeleteAccount, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this DeleteAccount to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an Action. */
            interface IAction {

                /** Action createAccount */
                createAccount?: (TW.NEAR.Proto.ICreateAccount|null);

                /** Action deployContract */
                deployContract?: (TW.NEAR.Proto.IDeployContract|null);

                /** Action functionCall */
                functionCall?: (TW.NEAR.Proto.IFunctionCall|null);

                /** Action transfer */
                transfer?: (TW.NEAR.Proto.ITransfer|null);

                /** Action stake */
                stake?: (TW.NEAR.Proto.IStake|null);

                /** Action addKey */
                addKey?: (TW.NEAR.Proto.IAddKey|null);

                /** Action deleteKey */
                deleteKey?: (TW.NEAR.Proto.IDeleteKey|null);

                /** Action deleteAccount */
                deleteAccount?: (TW.NEAR.Proto.IDeleteAccount|null);
            }

            /** Represents an Action. */
            class Action implements IAction {

                /**
                 * Constructs a new Action.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.IAction);

                /** Action createAccount. */
                public createAccount?: (TW.NEAR.Proto.ICreateAccount|null);

                /** Action deployContract. */
                public deployContract?: (TW.NEAR.Proto.IDeployContract|null);

                /** Action functionCall. */
                public functionCall?: (TW.NEAR.Proto.IFunctionCall|null);

                /** Action transfer. */
                public transfer?: (TW.NEAR.Proto.ITransfer|null);

                /** Action stake. */
                public stake?: (TW.NEAR.Proto.IStake|null);

                /** Action addKey. */
                public addKey?: (TW.NEAR.Proto.IAddKey|null);

                /** Action deleteKey. */
                public deleteKey?: (TW.NEAR.Proto.IDeleteKey|null);

                /** Action deleteAccount. */
                public deleteAccount?: (TW.NEAR.Proto.IDeleteAccount|null);

                /** Action payload. */
                public payload?: ("createAccount"|"deployContract"|"functionCall"|"transfer"|"stake"|"addKey"|"deleteKey"|"deleteAccount");

                /**
                 * Creates a new Action instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Action instance
                 */
                public static create(properties?: TW.NEAR.Proto.IAction): TW.NEAR.Proto.Action;

                /**
                 * Encodes the specified Action message. Does not implicitly {@link TW.NEAR.Proto.Action.verify|verify} messages.
                 * @param message Action message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.IAction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an Action message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Action
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.Action;

                /**
                 * Verifies an Action message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an Action message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Action
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.Action;

                /**
                 * Creates a plain object from an Action message. Also converts values to other types if specified.
                 * @param message Action
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.Action, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Action to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput signerId */
                signerId?: (string|null);

                /** SigningInput nonce */
                nonce?: (Long|null);

                /** SigningInput receiverId */
                receiverId?: (string|null);

                /** SigningInput blockHash */
                blockHash?: (Uint8Array|null);

                /** SigningInput actions */
                actions?: (TW.NEAR.Proto.IAction[]|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.ISigningInput);

                /** SigningInput signerId. */
                public signerId: string;

                /** SigningInput nonce. */
                public nonce: Long;

                /** SigningInput receiverId. */
                public receiverId: string;

                /** SigningInput blockHash. */
                public blockHash: Uint8Array;

                /** SigningInput actions. */
                public actions: TW.NEAR.Proto.IAction[];

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.NEAR.Proto.ISigningInput): TW.NEAR.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.NEAR.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput signedTransaction */
                signedTransaction?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEAR.Proto.ISigningOutput);

                /** SigningOutput signedTransaction. */
                public signedTransaction: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.NEAR.Proto.ISigningOutput): TW.NEAR.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.NEAR.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEAR.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEAR.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEAR.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEAR.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Nebulas. */
    namespace Nebulas {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput fromAddress */
                fromAddress?: (string|null);

                /** SigningInput chainId */
                chainId?: (Uint8Array|null);

                /** SigningInput nonce */
                nonce?: (Uint8Array|null);

                /** SigningInput gasPrice */
                gasPrice?: (Uint8Array|null);

                /** SigningInput gasLimit */
                gasLimit?: (Uint8Array|null);

                /** SigningInput toAddress */
                toAddress?: (string|null);

                /** SigningInput amount */
                amount?: (Uint8Array|null);

                /** SigningInput timestamp */
                timestamp?: (Uint8Array|null);

                /** SigningInput payload */
                payload?: (string|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Nebulas.Proto.ISigningInput);

                /** SigningInput fromAddress. */
                public fromAddress: string;

                /** SigningInput chainId. */
                public chainId: Uint8Array;

                /** SigningInput nonce. */
                public nonce: Uint8Array;

                /** SigningInput gasPrice. */
                public gasPrice: Uint8Array;

                /** SigningInput gasLimit. */
                public gasLimit: Uint8Array;

                /** SigningInput toAddress. */
                public toAddress: string;

                /** SigningInput amount. */
                public amount: Uint8Array;

                /** SigningInput timestamp. */
                public timestamp: Uint8Array;

                /** SigningInput payload. */
                public payload: string;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Nebulas.Proto.ISigningInput): TW.Nebulas.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Nebulas.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Nebulas.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Nebulas.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Nebulas.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Nebulas.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput algorithm */
                algorithm?: (number|null);

                /** SigningOutput signature */
                signature?: (Uint8Array|null);

                /** SigningOutput raw */
                raw?: (string|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Nebulas.Proto.ISigningOutput);

                /** SigningOutput algorithm. */
                public algorithm: number;

                /** SigningOutput signature. */
                public signature: Uint8Array;

                /** SigningOutput raw. */
                public raw: string;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Nebulas.Proto.ISigningOutput): TW.Nebulas.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Nebulas.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Nebulas.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Nebulas.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Nebulas.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Nebulas.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Data. */
            interface IData {

                /** Data type */
                type?: (string|null);

                /** Data payload */
                payload?: (Uint8Array|null);
            }

            /** Represents a Data. */
            class Data implements IData {

                /**
                 * Constructs a new Data.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Nebulas.Proto.IData);

                /** Data type. */
                public type: string;

                /** Data payload. */
                public payload: Uint8Array;

                /**
                 * Creates a new Data instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Data instance
                 */
                public static create(properties?: TW.Nebulas.Proto.IData): TW.Nebulas.Proto.Data;

                /**
                 * Encodes the specified Data message. Does not implicitly {@link TW.Nebulas.Proto.Data.verify|verify} messages.
                 * @param message Data message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Nebulas.Proto.IData, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Data message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Data
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Nebulas.Proto.Data;

                /**
                 * Verifies a Data message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Data message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Data
                 */
                public static fromObject(object: { [k: string]: any }): TW.Nebulas.Proto.Data;

                /**
                 * Creates a plain object from a Data message. Also converts values to other types if specified.
                 * @param message Data
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Nebulas.Proto.Data, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Data to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a RawTransaction. */
            interface IRawTransaction {

                /** RawTransaction hash */
                hash?: (Uint8Array|null);

                /** RawTransaction from */
                from?: (Uint8Array|null);

                /** RawTransaction to */
                to?: (Uint8Array|null);

                /** RawTransaction value */
                value?: (Uint8Array|null);

                /** RawTransaction nonce */
                nonce?: (Long|null);

                /** RawTransaction timestamp */
                timestamp?: (Long|null);

                /** RawTransaction data */
                data?: (TW.Nebulas.Proto.IData|null);

                /** RawTransaction chainId */
                chainId?: (number|null);

                /** RawTransaction gasPrice */
                gasPrice?: (Uint8Array|null);

                /** RawTransaction gasLimit */
                gasLimit?: (Uint8Array|null);

                /** RawTransaction alg */
                alg?: (number|null);

                /** RawTransaction sign */
                sign?: (Uint8Array|null);
            }

            /** Represents a RawTransaction. */
            class RawTransaction implements IRawTransaction {

                /**
                 * Constructs a new RawTransaction.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Nebulas.Proto.IRawTransaction);

                /** RawTransaction hash. */
                public hash: Uint8Array;

                /** RawTransaction from. */
                public from: Uint8Array;

                /** RawTransaction to. */
                public to: Uint8Array;

                /** RawTransaction value. */
                public value: Uint8Array;

                /** RawTransaction nonce. */
                public nonce: Long;

                /** RawTransaction timestamp. */
                public timestamp: Long;

                /** RawTransaction data. */
                public data?: (TW.Nebulas.Proto.IData|null);

                /** RawTransaction chainId. */
                public chainId: number;

                /** RawTransaction gasPrice. */
                public gasPrice: Uint8Array;

                /** RawTransaction gasLimit. */
                public gasLimit: Uint8Array;

                /** RawTransaction alg. */
                public alg: number;

                /** RawTransaction sign. */
                public sign: Uint8Array;

                /**
                 * Creates a new RawTransaction instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns RawTransaction instance
                 */
                public static create(properties?: TW.Nebulas.Proto.IRawTransaction): TW.Nebulas.Proto.RawTransaction;

                /**
                 * Encodes the specified RawTransaction message. Does not implicitly {@link TW.Nebulas.Proto.RawTransaction.verify|verify} messages.
                 * @param message RawTransaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Nebulas.Proto.IRawTransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a RawTransaction message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns RawTransaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Nebulas.Proto.RawTransaction;

                /**
                 * Verifies a RawTransaction message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a RawTransaction message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns RawTransaction
                 */
                public static fromObject(object: { [k: string]: any }): TW.Nebulas.Proto.RawTransaction;

                /**
                 * Creates a plain object from a RawTransaction message. Also converts values to other types if specified.
                 * @param message RawTransaction
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Nebulas.Proto.RawTransaction, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this RawTransaction to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace NEO. */
    namespace NEO {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a TransactionInput. */
            interface ITransactionInput {

                /** TransactionInput prevHash */
                prevHash?: (Uint8Array|null);

                /** TransactionInput prevIndex */
                prevIndex?: (number|null);

                /** TransactionInput value */
                value?: (Long|null);

                /** TransactionInput assetId */
                assetId?: (string|null);
            }

            /** Represents a TransactionInput. */
            class TransactionInput implements ITransactionInput {

                /**
                 * Constructs a new TransactionInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEO.Proto.ITransactionInput);

                /** TransactionInput prevHash. */
                public prevHash: Uint8Array;

                /** TransactionInput prevIndex. */
                public prevIndex: number;

                /** TransactionInput value. */
                public value: Long;

                /** TransactionInput assetId. */
                public assetId: string;

                /**
                 * Creates a new TransactionInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionInput instance
                 */
                public static create(properties?: TW.NEO.Proto.ITransactionInput): TW.NEO.Proto.TransactionInput;

                /**
                 * Encodes the specified TransactionInput message. Does not implicitly {@link TW.NEO.Proto.TransactionInput.verify|verify} messages.
                 * @param message TransactionInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEO.Proto.ITransactionInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEO.Proto.TransactionInput;

                /**
                 * Verifies a TransactionInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEO.Proto.TransactionInput;

                /**
                 * Creates a plain object from a TransactionInput message. Also converts values to other types if specified.
                 * @param message TransactionInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEO.Proto.TransactionInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransactionOutput. */
            interface ITransactionOutput {

                /** TransactionOutput assetId */
                assetId?: (string|null);

                /** TransactionOutput amount */
                amount?: (Long|null);

                /** TransactionOutput toAddress */
                toAddress?: (string|null);

                /** TransactionOutput changeAddress */
                changeAddress?: (string|null);
            }

            /** Represents a TransactionOutput. */
            class TransactionOutput implements ITransactionOutput {

                /**
                 * Constructs a new TransactionOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEO.Proto.ITransactionOutput);

                /** TransactionOutput assetId. */
                public assetId: string;

                /** TransactionOutput amount. */
                public amount: Long;

                /** TransactionOutput toAddress. */
                public toAddress: string;

                /** TransactionOutput changeAddress. */
                public changeAddress: string;

                /**
                 * Creates a new TransactionOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionOutput instance
                 */
                public static create(properties?: TW.NEO.Proto.ITransactionOutput): TW.NEO.Proto.TransactionOutput;

                /**
                 * Encodes the specified TransactionOutput message. Does not implicitly {@link TW.NEO.Proto.TransactionOutput.verify|verify} messages.
                 * @param message TransactionOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEO.Proto.ITransactionOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEO.Proto.TransactionOutput;

                /**
                 * Verifies a TransactionOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEO.Proto.TransactionOutput;

                /**
                 * Creates a plain object from a TransactionOutput message. Also converts values to other types if specified.
                 * @param message TransactionOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEO.Proto.TransactionOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput inputs */
                inputs?: (TW.NEO.Proto.ITransactionInput[]|null);

                /** SigningInput outputs */
                outputs?: (TW.NEO.Proto.ITransactionOutput[]|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput fee */
                fee?: (Long|null);

                /** SigningInput gasAssetId */
                gasAssetId?: (string|null);

                /** SigningInput gasChangeAddress */
                gasChangeAddress?: (string|null);

                /** SigningInput plan */
                plan?: (TW.NEO.Proto.ITransactionPlan|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEO.Proto.ISigningInput);

                /** SigningInput inputs. */
                public inputs: TW.NEO.Proto.ITransactionInput[];

                /** SigningInput outputs. */
                public outputs: TW.NEO.Proto.ITransactionOutput[];

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput fee. */
                public fee: Long;

                /** SigningInput gasAssetId. */
                public gasAssetId: string;

                /** SigningInput gasChangeAddress. */
                public gasChangeAddress: string;

                /** SigningInput plan. */
                public plan?: (TW.NEO.Proto.ITransactionPlan|null);

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.NEO.Proto.ISigningInput): TW.NEO.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.NEO.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEO.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEO.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEO.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEO.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);

                /** SigningOutput error */
                error?: (TW.Common.Proto.SigningError|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEO.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /** SigningOutput error. */
                public error: TW.Common.Proto.SigningError;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.NEO.Proto.ISigningOutput): TW.NEO.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.NEO.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEO.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEO.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEO.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEO.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransactionOutputPlan. */
            interface ITransactionOutputPlan {

                /** TransactionOutputPlan amount */
                amount?: (Long|null);

                /** TransactionOutputPlan availableAmount */
                availableAmount?: (Long|null);

                /** TransactionOutputPlan change */
                change?: (Long|null);

                /** TransactionOutputPlan assetId */
                assetId?: (string|null);

                /** TransactionOutputPlan toAddress */
                toAddress?: (string|null);

                /** TransactionOutputPlan changeAddress */
                changeAddress?: (string|null);
            }

            /** Represents a TransactionOutputPlan. */
            class TransactionOutputPlan implements ITransactionOutputPlan {

                /**
                 * Constructs a new TransactionOutputPlan.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEO.Proto.ITransactionOutputPlan);

                /** TransactionOutputPlan amount. */
                public amount: Long;

                /** TransactionOutputPlan availableAmount. */
                public availableAmount: Long;

                /** TransactionOutputPlan change. */
                public change: Long;

                /** TransactionOutputPlan assetId. */
                public assetId: string;

                /** TransactionOutputPlan toAddress. */
                public toAddress: string;

                /** TransactionOutputPlan changeAddress. */
                public changeAddress: string;

                /**
                 * Creates a new TransactionOutputPlan instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionOutputPlan instance
                 */
                public static create(properties?: TW.NEO.Proto.ITransactionOutputPlan): TW.NEO.Proto.TransactionOutputPlan;

                /**
                 * Encodes the specified TransactionOutputPlan message. Does not implicitly {@link TW.NEO.Proto.TransactionOutputPlan.verify|verify} messages.
                 * @param message TransactionOutputPlan message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEO.Proto.ITransactionOutputPlan, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionOutputPlan message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionOutputPlan
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEO.Proto.TransactionOutputPlan;

                /**
                 * Verifies a TransactionOutputPlan message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionOutputPlan message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionOutputPlan
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEO.Proto.TransactionOutputPlan;

                /**
                 * Creates a plain object from a TransactionOutputPlan message. Also converts values to other types if specified.
                 * @param message TransactionOutputPlan
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEO.Proto.TransactionOutputPlan, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionOutputPlan to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransactionPlan. */
            interface ITransactionPlan {

                /** TransactionPlan outputs */
                outputs?: (TW.NEO.Proto.ITransactionOutputPlan[]|null);

                /** TransactionPlan inputs */
                inputs?: (TW.NEO.Proto.ITransactionInput[]|null);

                /** TransactionPlan fee */
                fee?: (Long|null);

                /** TransactionPlan error */
                error?: (TW.Common.Proto.SigningError|null);
            }

            /** Represents a TransactionPlan. */
            class TransactionPlan implements ITransactionPlan {

                /**
                 * Constructs a new TransactionPlan.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NEO.Proto.ITransactionPlan);

                /** TransactionPlan outputs. */
                public outputs: TW.NEO.Proto.ITransactionOutputPlan[];

                /** TransactionPlan inputs. */
                public inputs: TW.NEO.Proto.ITransactionInput[];

                /** TransactionPlan fee. */
                public fee: Long;

                /** TransactionPlan error. */
                public error: TW.Common.Proto.SigningError;

                /**
                 * Creates a new TransactionPlan instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionPlan instance
                 */
                public static create(properties?: TW.NEO.Proto.ITransactionPlan): TW.NEO.Proto.TransactionPlan;

                /**
                 * Encodes the specified TransactionPlan message. Does not implicitly {@link TW.NEO.Proto.TransactionPlan.verify|verify} messages.
                 * @param message TransactionPlan message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NEO.Proto.ITransactionPlan, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionPlan message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionPlan
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NEO.Proto.TransactionPlan;

                /**
                 * Verifies a TransactionPlan message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionPlan message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionPlan
                 */
                public static fromObject(object: { [k: string]: any }): TW.NEO.Proto.TransactionPlan;

                /**
                 * Creates a plain object from a TransactionPlan message. Also converts values to other types if specified.
                 * @param message TransactionPlan
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NEO.Proto.TransactionPlan, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionPlan to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Nimiq. */
    namespace Nimiq {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput destination */
                destination?: (string|null);

                /** SigningInput value */
                value?: (Long|null);

                /** SigningInput fee */
                fee?: (Long|null);

                /** SigningInput validityStartHeight */
                validityStartHeight?: (number|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Nimiq.Proto.ISigningInput);

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput destination. */
                public destination: string;

                /** SigningInput value. */
                public value: Long;

                /** SigningInput fee. */
                public fee: Long;

                /** SigningInput validityStartHeight. */
                public validityStartHeight: number;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Nimiq.Proto.ISigningInput): TW.Nimiq.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Nimiq.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Nimiq.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Nimiq.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Nimiq.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Nimiq.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Nimiq.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Nimiq.Proto.ISigningOutput): TW.Nimiq.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Nimiq.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Nimiq.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Nimiq.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Nimiq.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Nimiq.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace NULS. */
    namespace NULS {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a TransactionCoinFrom. */
            interface ITransactionCoinFrom {

                /** TransactionCoinFrom fromAddress */
                fromAddress?: (string|null);

                /** TransactionCoinFrom assetsChainid */
                assetsChainid?: (number|null);

                /** TransactionCoinFrom assetsId */
                assetsId?: (number|null);

                /** TransactionCoinFrom idAmount */
                idAmount?: (Uint8Array|null);

                /** TransactionCoinFrom nonce */
                nonce?: (Uint8Array|null);

                /** TransactionCoinFrom locked */
                locked?: (number|null);
            }

            /** Represents a TransactionCoinFrom. */
            class TransactionCoinFrom implements ITransactionCoinFrom {

                /**
                 * Constructs a new TransactionCoinFrom.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NULS.Proto.ITransactionCoinFrom);

                /** TransactionCoinFrom fromAddress. */
                public fromAddress: string;

                /** TransactionCoinFrom assetsChainid. */
                public assetsChainid: number;

                /** TransactionCoinFrom assetsId. */
                public assetsId: number;

                /** TransactionCoinFrom idAmount. */
                public idAmount: Uint8Array;

                /** TransactionCoinFrom nonce. */
                public nonce: Uint8Array;

                /** TransactionCoinFrom locked. */
                public locked: number;

                /**
                 * Creates a new TransactionCoinFrom instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionCoinFrom instance
                 */
                public static create(properties?: TW.NULS.Proto.ITransactionCoinFrom): TW.NULS.Proto.TransactionCoinFrom;

                /**
                 * Encodes the specified TransactionCoinFrom message. Does not implicitly {@link TW.NULS.Proto.TransactionCoinFrom.verify|verify} messages.
                 * @param message TransactionCoinFrom message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NULS.Proto.ITransactionCoinFrom, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionCoinFrom message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionCoinFrom
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NULS.Proto.TransactionCoinFrom;

                /**
                 * Verifies a TransactionCoinFrom message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionCoinFrom message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionCoinFrom
                 */
                public static fromObject(object: { [k: string]: any }): TW.NULS.Proto.TransactionCoinFrom;

                /**
                 * Creates a plain object from a TransactionCoinFrom message. Also converts values to other types if specified.
                 * @param message TransactionCoinFrom
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NULS.Proto.TransactionCoinFrom, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionCoinFrom to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransactionCoinTo. */
            interface ITransactionCoinTo {

                /** TransactionCoinTo toAddress */
                toAddress?: (string|null);

                /** TransactionCoinTo assetsChainid */
                assetsChainid?: (number|null);

                /** TransactionCoinTo assetsId */
                assetsId?: (number|null);

                /** TransactionCoinTo idAmount */
                idAmount?: (Uint8Array|null);

                /** TransactionCoinTo lockTime */
                lockTime?: (number|null);
            }

            /** Represents a TransactionCoinTo. */
            class TransactionCoinTo implements ITransactionCoinTo {

                /**
                 * Constructs a new TransactionCoinTo.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NULS.Proto.ITransactionCoinTo);

                /** TransactionCoinTo toAddress. */
                public toAddress: string;

                /** TransactionCoinTo assetsChainid. */
                public assetsChainid: number;

                /** TransactionCoinTo assetsId. */
                public assetsId: number;

                /** TransactionCoinTo idAmount. */
                public idAmount: Uint8Array;

                /** TransactionCoinTo lockTime. */
                public lockTime: number;

                /**
                 * Creates a new TransactionCoinTo instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionCoinTo instance
                 */
                public static create(properties?: TW.NULS.Proto.ITransactionCoinTo): TW.NULS.Proto.TransactionCoinTo;

                /**
                 * Encodes the specified TransactionCoinTo message. Does not implicitly {@link TW.NULS.Proto.TransactionCoinTo.verify|verify} messages.
                 * @param message TransactionCoinTo message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NULS.Proto.ITransactionCoinTo, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionCoinTo message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionCoinTo
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NULS.Proto.TransactionCoinTo;

                /**
                 * Verifies a TransactionCoinTo message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionCoinTo message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionCoinTo
                 */
                public static fromObject(object: { [k: string]: any }): TW.NULS.Proto.TransactionCoinTo;

                /**
                 * Creates a plain object from a TransactionCoinTo message. Also converts values to other types if specified.
                 * @param message TransactionCoinTo
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NULS.Proto.TransactionCoinTo, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionCoinTo to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Signature. */
            interface ISignature {

                /** Signature pkeyLen */
                pkeyLen?: (number|null);

                /** Signature publicKey */
                publicKey?: (Uint8Array|null);

                /** Signature sigLen */
                sigLen?: (number|null);

                /** Signature signature */
                signature?: (Uint8Array|null);
            }

            /** Represents a Signature. */
            class Signature implements ISignature {

                /**
                 * Constructs a new Signature.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NULS.Proto.ISignature);

                /** Signature pkeyLen. */
                public pkeyLen: number;

                /** Signature publicKey. */
                public publicKey: Uint8Array;

                /** Signature sigLen. */
                public sigLen: number;

                /** Signature signature. */
                public signature: Uint8Array;

                /**
                 * Creates a new Signature instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Signature instance
                 */
                public static create(properties?: TW.NULS.Proto.ISignature): TW.NULS.Proto.Signature;

                /**
                 * Encodes the specified Signature message. Does not implicitly {@link TW.NULS.Proto.Signature.verify|verify} messages.
                 * @param message Signature message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NULS.Proto.ISignature, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Signature message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Signature
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NULS.Proto.Signature;

                /**
                 * Verifies a Signature message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Signature message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Signature
                 */
                public static fromObject(object: { [k: string]: any }): TW.NULS.Proto.Signature;

                /**
                 * Creates a plain object from a Signature message. Also converts values to other types if specified.
                 * @param message Signature
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NULS.Proto.Signature, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Signature to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Transaction. */
            interface ITransaction {

                /** Transaction type */
                type?: (number|null);

                /** Transaction timestamp */
                timestamp?: (number|null);

                /** Transaction remark */
                remark?: (string|null);

                /** Transaction txData */
                txData?: (Uint8Array|null);

                /** Transaction input */
                input?: (TW.NULS.Proto.ITransactionCoinFrom|null);

                /** Transaction output */
                output?: (TW.NULS.Proto.ITransactionCoinTo|null);

                /** Transaction txSigs */
                txSigs?: (TW.NULS.Proto.ISignature|null);

                /** Transaction hash */
                hash?: (number|null);
            }

            /** Represents a Transaction. */
            class Transaction implements ITransaction {

                /**
                 * Constructs a new Transaction.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NULS.Proto.ITransaction);

                /** Transaction type. */
                public type: number;

                /** Transaction timestamp. */
                public timestamp: number;

                /** Transaction remark. */
                public remark: string;

                /** Transaction txData. */
                public txData: Uint8Array;

                /** Transaction input. */
                public input?: (TW.NULS.Proto.ITransactionCoinFrom|null);

                /** Transaction output. */
                public output?: (TW.NULS.Proto.ITransactionCoinTo|null);

                /** Transaction txSigs. */
                public txSigs?: (TW.NULS.Proto.ISignature|null);

                /** Transaction hash. */
                public hash: number;

                /**
                 * Creates a new Transaction instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Transaction instance
                 */
                public static create(properties?: TW.NULS.Proto.ITransaction): TW.NULS.Proto.Transaction;

                /**
                 * Encodes the specified Transaction message. Does not implicitly {@link TW.NULS.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NULS.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Transaction message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NULS.Proto.Transaction;

                /**
                 * Verifies a Transaction message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Transaction message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Transaction
                 */
                public static fromObject(object: { [k: string]: any }): TW.NULS.Proto.Transaction;

                /**
                 * Creates a plain object from a Transaction message. Also converts values to other types if specified.
                 * @param message Transaction
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NULS.Proto.Transaction, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Transaction to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput from */
                from?: (string|null);

                /** SigningInput to */
                to?: (string|null);

                /** SigningInput amount */
                amount?: (Uint8Array|null);

                /** SigningInput chainId */
                chainId?: (number|null);

                /** SigningInput idassetsId */
                idassetsId?: (number|null);

                /** SigningInput nonce */
                nonce?: (Uint8Array|null);

                /** SigningInput remark */
                remark?: (string|null);

                /** SigningInput balance */
                balance?: (Uint8Array|null);

                /** SigningInput timestamp */
                timestamp?: (number|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NULS.Proto.ISigningInput);

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput from. */
                public from: string;

                /** SigningInput to. */
                public to: string;

                /** SigningInput amount. */
                public amount: Uint8Array;

                /** SigningInput chainId. */
                public chainId: number;

                /** SigningInput idassetsId. */
                public idassetsId: number;

                /** SigningInput nonce. */
                public nonce: Uint8Array;

                /** SigningInput remark. */
                public remark: string;

                /** SigningInput balance. */
                public balance: Uint8Array;

                /** SigningInput timestamp. */
                public timestamp: number;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.NULS.Proto.ISigningInput): TW.NULS.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.NULS.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NULS.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NULS.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.NULS.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NULS.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NULS.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.NULS.Proto.ISigningOutput): TW.NULS.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.NULS.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NULS.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NULS.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.NULS.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NULS.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Oasis. */
    namespace Oasis {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a TransferMessage. */
            interface ITransferMessage {

                /** TransferMessage to */
                to?: (string|null);

                /** TransferMessage gasPrice */
                gasPrice?: (Long|null);

                /** TransferMessage gasAmount */
                gasAmount?: (string|null);

                /** TransferMessage amount */
                amount?: (string|null);

                /** TransferMessage nonce */
                nonce?: (Long|null);

                /** TransferMessage context */
                context?: (string|null);
            }

            /** Represents a TransferMessage. */
            class TransferMessage implements ITransferMessage {

                /**
                 * Constructs a new TransferMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Oasis.Proto.ITransferMessage);

                /** TransferMessage to. */
                public to: string;

                /** TransferMessage gasPrice. */
                public gasPrice: Long;

                /** TransferMessage gasAmount. */
                public gasAmount: string;

                /** TransferMessage amount. */
                public amount: string;

                /** TransferMessage nonce. */
                public nonce: Long;

                /** TransferMessage context. */
                public context: string;

                /**
                 * Creates a new TransferMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransferMessage instance
                 */
                public static create(properties?: TW.Oasis.Proto.ITransferMessage): TW.Oasis.Proto.TransferMessage;

                /**
                 * Encodes the specified TransferMessage message. Does not implicitly {@link TW.Oasis.Proto.TransferMessage.verify|verify} messages.
                 * @param message TransferMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Oasis.Proto.ITransferMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransferMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransferMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Oasis.Proto.TransferMessage;

                /**
                 * Verifies a TransferMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransferMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransferMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Oasis.Proto.TransferMessage;

                /**
                 * Creates a plain object from a TransferMessage message. Also converts values to other types if specified.
                 * @param message TransferMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Oasis.Proto.TransferMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransferMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput transfer */
                transfer?: (TW.Oasis.Proto.ITransferMessage|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Oasis.Proto.ISigningInput);

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput transfer. */
                public transfer?: (TW.Oasis.Proto.ITransferMessage|null);

                /** SigningInput message. */
                public message?: "transfer";

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Oasis.Proto.ISigningInput): TW.Oasis.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Oasis.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Oasis.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Oasis.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Oasis.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Oasis.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Oasis.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Oasis.Proto.ISigningOutput): TW.Oasis.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Oasis.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Oasis.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Oasis.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Oasis.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Oasis.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Ontology. */
    namespace Ontology {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput contract */
                contract?: (string|null);

                /** SigningInput method */
                method?: (string|null);

                /** SigningInput ownerPrivateKey */
                ownerPrivateKey?: (Uint8Array|null);

                /** SigningInput toAddress */
                toAddress?: (string|null);

                /** SigningInput amount */
                amount?: (Long|null);

                /** SigningInput payerPrivateKey */
                payerPrivateKey?: (Uint8Array|null);

                /** SigningInput gasPrice */
                gasPrice?: (Long|null);

                /** SigningInput gasLimit */
                gasLimit?: (Long|null);

                /** SigningInput queryAddress */
                queryAddress?: (string|null);

                /** SigningInput nonce */
                nonce?: (number|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Ontology.Proto.ISigningInput);

                /** SigningInput contract. */
                public contract: string;

                /** SigningInput method. */
                public method: string;

                /** SigningInput ownerPrivateKey. */
                public ownerPrivateKey: Uint8Array;

                /** SigningInput toAddress. */
                public toAddress: string;

                /** SigningInput amount. */
                public amount: Long;

                /** SigningInput payerPrivateKey. */
                public payerPrivateKey: Uint8Array;

                /** SigningInput gasPrice. */
                public gasPrice: Long;

                /** SigningInput gasLimit. */
                public gasLimit: Long;

                /** SigningInput queryAddress. */
                public queryAddress: string;

                /** SigningInput nonce. */
                public nonce: number;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Ontology.Proto.ISigningInput): TW.Ontology.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Ontology.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Ontology.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Ontology.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Ontology.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Ontology.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Ontology.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Ontology.Proto.ISigningOutput): TW.Ontology.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Ontology.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Ontology.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Ontology.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Ontology.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Ontology.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Polkadot. */
    namespace Polkadot {

        /** Namespace Proto. */
        namespace Proto {

            /** Network enum. */
            enum Network {
                POLKADOT = 0,
                KUSAMA = 2
            }

            /** RewardDestination enum. */
            enum RewardDestination {
                STAKED = 0,
                STASH = 1,
                CONTROLLER = 2
            }

            /** Properties of an Era. */
            interface IEra {

                /** Era blockNumber */
                blockNumber?: (Long|null);

                /** Era period */
                period?: (Long|null);
            }

            /** Represents an Era. */
            class Era implements IEra {

                /**
                 * Constructs a new Era.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Polkadot.Proto.IEra);

                /** Era blockNumber. */
                public blockNumber: Long;

                /** Era period. */
                public period: Long;

                /**
                 * Creates a new Era instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Era instance
                 */
                public static create(properties?: TW.Polkadot.Proto.IEra): TW.Polkadot.Proto.Era;

                /**
                 * Encodes the specified Era message. Does not implicitly {@link TW.Polkadot.Proto.Era.verify|verify} messages.
                 * @param message Era message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Polkadot.Proto.IEra, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an Era message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Era
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Polkadot.Proto.Era;

                /**
                 * Verifies an Era message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an Era message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Era
                 */
                public static fromObject(object: { [k: string]: any }): TW.Polkadot.Proto.Era;

                /**
                 * Creates a plain object from an Era message. Also converts values to other types if specified.
                 * @param message Era
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Polkadot.Proto.Era, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Era to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Balance. */
            interface IBalance {

                /** Balance transfer */
                transfer?: (TW.Polkadot.Proto.Balance.ITransfer|null);
            }

            /** Represents a Balance. */
            class Balance implements IBalance {

                /**
                 * Constructs a new Balance.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Polkadot.Proto.IBalance);

                /** Balance transfer. */
                public transfer?: (TW.Polkadot.Proto.Balance.ITransfer|null);

                /** Balance messageOneof. */
                public messageOneof?: "transfer";

                /**
                 * Creates a new Balance instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Balance instance
                 */
                public static create(properties?: TW.Polkadot.Proto.IBalance): TW.Polkadot.Proto.Balance;

                /**
                 * Encodes the specified Balance message. Does not implicitly {@link TW.Polkadot.Proto.Balance.verify|verify} messages.
                 * @param message Balance message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Polkadot.Proto.IBalance, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Balance message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Balance
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Polkadot.Proto.Balance;

                /**
                 * Verifies a Balance message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Balance message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Balance
                 */
                public static fromObject(object: { [k: string]: any }): TW.Polkadot.Proto.Balance;

                /**
                 * Creates a plain object from a Balance message. Also converts values to other types if specified.
                 * @param message Balance
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Polkadot.Proto.Balance, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Balance to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            namespace Balance {

                /** Properties of a Transfer. */
                interface ITransfer {

                    /** Transfer toAddress */
                    toAddress?: (string|null);

                    /** Transfer value */
                    value?: (Uint8Array|null);
                }

                /** Represents a Transfer. */
                class Transfer implements ITransfer {

                    /**
                     * Constructs a new Transfer.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Polkadot.Proto.Balance.ITransfer);

                    /** Transfer toAddress. */
                    public toAddress: string;

                    /** Transfer value. */
                    public value: Uint8Array;

                    /**
                     * Creates a new Transfer instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Transfer instance
                     */
                    public static create(properties?: TW.Polkadot.Proto.Balance.ITransfer): TW.Polkadot.Proto.Balance.Transfer;

                    /**
                     * Encodes the specified Transfer message. Does not implicitly {@link TW.Polkadot.Proto.Balance.Transfer.verify|verify} messages.
                     * @param message Transfer message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Polkadot.Proto.Balance.ITransfer, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Transfer message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Transfer
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Polkadot.Proto.Balance.Transfer;

                    /**
                     * Verifies a Transfer message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Transfer message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Transfer
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Polkadot.Proto.Balance.Transfer;

                    /**
                     * Creates a plain object from a Transfer message. Also converts values to other types if specified.
                     * @param message Transfer
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Polkadot.Proto.Balance.Transfer, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Transfer to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }
            }

            /** Properties of a Staking. */
            interface IStaking {

                /** Staking bond */
                bond?: (TW.Polkadot.Proto.Staking.IBond|null);

                /** Staking bondAndNominate */
                bondAndNominate?: (TW.Polkadot.Proto.Staking.IBondAndNominate|null);

                /** Staking bondExtra */
                bondExtra?: (TW.Polkadot.Proto.Staking.IBondExtra|null);

                /** Staking unbond */
                unbond?: (TW.Polkadot.Proto.Staking.IUnbond|null);

                /** Staking withdrawUnbonded */
                withdrawUnbonded?: (TW.Polkadot.Proto.Staking.IWithdrawUnbonded|null);

                /** Staking nominate */
                nominate?: (TW.Polkadot.Proto.Staking.INominate|null);

                /** Staking chill */
                chill?: (TW.Polkadot.Proto.Staking.IChill|null);
            }

            /** Represents a Staking. */
            class Staking implements IStaking {

                /**
                 * Constructs a new Staking.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Polkadot.Proto.IStaking);

                /** Staking bond. */
                public bond?: (TW.Polkadot.Proto.Staking.IBond|null);

                /** Staking bondAndNominate. */
                public bondAndNominate?: (TW.Polkadot.Proto.Staking.IBondAndNominate|null);

                /** Staking bondExtra. */
                public bondExtra?: (TW.Polkadot.Proto.Staking.IBondExtra|null);

                /** Staking unbond. */
                public unbond?: (TW.Polkadot.Proto.Staking.IUnbond|null);

                /** Staking withdrawUnbonded. */
                public withdrawUnbonded?: (TW.Polkadot.Proto.Staking.IWithdrawUnbonded|null);

                /** Staking nominate. */
                public nominate?: (TW.Polkadot.Proto.Staking.INominate|null);

                /** Staking chill. */
                public chill?: (TW.Polkadot.Proto.Staking.IChill|null);

                /** Staking messageOneof. */
                public messageOneof?: ("bond"|"bondAndNominate"|"bondExtra"|"unbond"|"withdrawUnbonded"|"nominate"|"chill");

                /**
                 * Creates a new Staking instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Staking instance
                 */
                public static create(properties?: TW.Polkadot.Proto.IStaking): TW.Polkadot.Proto.Staking;

                /**
                 * Encodes the specified Staking message. Does not implicitly {@link TW.Polkadot.Proto.Staking.verify|verify} messages.
                 * @param message Staking message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Polkadot.Proto.IStaking, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Staking message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Staking
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Polkadot.Proto.Staking;

                /**
                 * Verifies a Staking message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Staking message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Staking
                 */
                public static fromObject(object: { [k: string]: any }): TW.Polkadot.Proto.Staking;

                /**
                 * Creates a plain object from a Staking message. Also converts values to other types if specified.
                 * @param message Staking
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Polkadot.Proto.Staking, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Staking to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            namespace Staking {

                /** Properties of a Bond. */
                interface IBond {

                    /** Bond controller */
                    controller?: (string|null);

                    /** Bond value */
                    value?: (Uint8Array|null);

                    /** Bond rewardDestination */
                    rewardDestination?: (TW.Polkadot.Proto.RewardDestination|null);
                }

                /** Represents a Bond. */
                class Bond implements IBond {

                    /**
                     * Constructs a new Bond.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Polkadot.Proto.Staking.IBond);

                    /** Bond controller. */
                    public controller: string;

                    /** Bond value. */
                    public value: Uint8Array;

                    /** Bond rewardDestination. */
                    public rewardDestination: TW.Polkadot.Proto.RewardDestination;

                    /**
                     * Creates a new Bond instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Bond instance
                     */
                    public static create(properties?: TW.Polkadot.Proto.Staking.IBond): TW.Polkadot.Proto.Staking.Bond;

                    /**
                     * Encodes the specified Bond message. Does not implicitly {@link TW.Polkadot.Proto.Staking.Bond.verify|verify} messages.
                     * @param message Bond message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Polkadot.Proto.Staking.IBond, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Bond message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Bond
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Polkadot.Proto.Staking.Bond;

                    /**
                     * Verifies a Bond message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Bond message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Bond
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Polkadot.Proto.Staking.Bond;

                    /**
                     * Creates a plain object from a Bond message. Also converts values to other types if specified.
                     * @param message Bond
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Polkadot.Proto.Staking.Bond, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Bond to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a BondAndNominate. */
                interface IBondAndNominate {

                    /** BondAndNominate controller */
                    controller?: (string|null);

                    /** BondAndNominate value */
                    value?: (Uint8Array|null);

                    /** BondAndNominate rewardDestination */
                    rewardDestination?: (TW.Polkadot.Proto.RewardDestination|null);

                    /** BondAndNominate nominators */
                    nominators?: (string[]|null);
                }

                /** Represents a BondAndNominate. */
                class BondAndNominate implements IBondAndNominate {

                    /**
                     * Constructs a new BondAndNominate.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Polkadot.Proto.Staking.IBondAndNominate);

                    /** BondAndNominate controller. */
                    public controller: string;

                    /** BondAndNominate value. */
                    public value: Uint8Array;

                    /** BondAndNominate rewardDestination. */
                    public rewardDestination: TW.Polkadot.Proto.RewardDestination;

                    /** BondAndNominate nominators. */
                    public nominators: string[];

                    /**
                     * Creates a new BondAndNominate instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns BondAndNominate instance
                     */
                    public static create(properties?: TW.Polkadot.Proto.Staking.IBondAndNominate): TW.Polkadot.Proto.Staking.BondAndNominate;

                    /**
                     * Encodes the specified BondAndNominate message. Does not implicitly {@link TW.Polkadot.Proto.Staking.BondAndNominate.verify|verify} messages.
                     * @param message BondAndNominate message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Polkadot.Proto.Staking.IBondAndNominate, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a BondAndNominate message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns BondAndNominate
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Polkadot.Proto.Staking.BondAndNominate;

                    /**
                     * Verifies a BondAndNominate message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a BondAndNominate message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns BondAndNominate
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Polkadot.Proto.Staking.BondAndNominate;

                    /**
                     * Creates a plain object from a BondAndNominate message. Also converts values to other types if specified.
                     * @param message BondAndNominate
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Polkadot.Proto.Staking.BondAndNominate, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this BondAndNominate to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a BondExtra. */
                interface IBondExtra {

                    /** BondExtra value */
                    value?: (Uint8Array|null);
                }

                /** Represents a BondExtra. */
                class BondExtra implements IBondExtra {

                    /**
                     * Constructs a new BondExtra.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Polkadot.Proto.Staking.IBondExtra);

                    /** BondExtra value. */
                    public value: Uint8Array;

                    /**
                     * Creates a new BondExtra instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns BondExtra instance
                     */
                    public static create(properties?: TW.Polkadot.Proto.Staking.IBondExtra): TW.Polkadot.Proto.Staking.BondExtra;

                    /**
                     * Encodes the specified BondExtra message. Does not implicitly {@link TW.Polkadot.Proto.Staking.BondExtra.verify|verify} messages.
                     * @param message BondExtra message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Polkadot.Proto.Staking.IBondExtra, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a BondExtra message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns BondExtra
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Polkadot.Proto.Staking.BondExtra;

                    /**
                     * Verifies a BondExtra message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a BondExtra message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns BondExtra
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Polkadot.Proto.Staking.BondExtra;

                    /**
                     * Creates a plain object from a BondExtra message. Also converts values to other types if specified.
                     * @param message BondExtra
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Polkadot.Proto.Staking.BondExtra, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this BondExtra to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of an Unbond. */
                interface IUnbond {

                    /** Unbond value */
                    value?: (Uint8Array|null);
                }

                /** Represents an Unbond. */
                class Unbond implements IUnbond {

                    /**
                     * Constructs a new Unbond.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Polkadot.Proto.Staking.IUnbond);

                    /** Unbond value. */
                    public value: Uint8Array;

                    /**
                     * Creates a new Unbond instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Unbond instance
                     */
                    public static create(properties?: TW.Polkadot.Proto.Staking.IUnbond): TW.Polkadot.Proto.Staking.Unbond;

                    /**
                     * Encodes the specified Unbond message. Does not implicitly {@link TW.Polkadot.Proto.Staking.Unbond.verify|verify} messages.
                     * @param message Unbond message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Polkadot.Proto.Staking.IUnbond, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes an Unbond message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Unbond
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Polkadot.Proto.Staking.Unbond;

                    /**
                     * Verifies an Unbond message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates an Unbond message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Unbond
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Polkadot.Proto.Staking.Unbond;

                    /**
                     * Creates a plain object from an Unbond message. Also converts values to other types if specified.
                     * @param message Unbond
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Polkadot.Proto.Staking.Unbond, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Unbond to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a WithdrawUnbonded. */
                interface IWithdrawUnbonded {

                    /** WithdrawUnbonded slashingSpans */
                    slashingSpans?: (number|null);
                }

                /** Represents a WithdrawUnbonded. */
                class WithdrawUnbonded implements IWithdrawUnbonded {

                    /**
                     * Constructs a new WithdrawUnbonded.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Polkadot.Proto.Staking.IWithdrawUnbonded);

                    /** WithdrawUnbonded slashingSpans. */
                    public slashingSpans: number;

                    /**
                     * Creates a new WithdrawUnbonded instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns WithdrawUnbonded instance
                     */
                    public static create(properties?: TW.Polkadot.Proto.Staking.IWithdrawUnbonded): TW.Polkadot.Proto.Staking.WithdrawUnbonded;

                    /**
                     * Encodes the specified WithdrawUnbonded message. Does not implicitly {@link TW.Polkadot.Proto.Staking.WithdrawUnbonded.verify|verify} messages.
                     * @param message WithdrawUnbonded message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Polkadot.Proto.Staking.IWithdrawUnbonded, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a WithdrawUnbonded message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns WithdrawUnbonded
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Polkadot.Proto.Staking.WithdrawUnbonded;

                    /**
                     * Verifies a WithdrawUnbonded message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a WithdrawUnbonded message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns WithdrawUnbonded
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Polkadot.Proto.Staking.WithdrawUnbonded;

                    /**
                     * Creates a plain object from a WithdrawUnbonded message. Also converts values to other types if specified.
                     * @param message WithdrawUnbonded
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Polkadot.Proto.Staking.WithdrawUnbonded, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this WithdrawUnbonded to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a Nominate. */
                interface INominate {

                    /** Nominate nominators */
                    nominators?: (string[]|null);
                }

                /** Represents a Nominate. */
                class Nominate implements INominate {

                    /**
                     * Constructs a new Nominate.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Polkadot.Proto.Staking.INominate);

                    /** Nominate nominators. */
                    public nominators: string[];

                    /**
                     * Creates a new Nominate instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Nominate instance
                     */
                    public static create(properties?: TW.Polkadot.Proto.Staking.INominate): TW.Polkadot.Proto.Staking.Nominate;

                    /**
                     * Encodes the specified Nominate message. Does not implicitly {@link TW.Polkadot.Proto.Staking.Nominate.verify|verify} messages.
                     * @param message Nominate message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Polkadot.Proto.Staking.INominate, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Nominate message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Nominate
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Polkadot.Proto.Staking.Nominate;

                    /**
                     * Verifies a Nominate message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Nominate message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Nominate
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Polkadot.Proto.Staking.Nominate;

                    /**
                     * Creates a plain object from a Nominate message. Also converts values to other types if specified.
                     * @param message Nominate
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Polkadot.Proto.Staking.Nominate, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Nominate to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a Chill. */
                interface IChill {
                }

                /** Represents a Chill. */
                class Chill implements IChill {

                    /**
                     * Constructs a new Chill.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Polkadot.Proto.Staking.IChill);

                    /**
                     * Creates a new Chill instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Chill instance
                     */
                    public static create(properties?: TW.Polkadot.Proto.Staking.IChill): TW.Polkadot.Proto.Staking.Chill;

                    /**
                     * Encodes the specified Chill message. Does not implicitly {@link TW.Polkadot.Proto.Staking.Chill.verify|verify} messages.
                     * @param message Chill message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Polkadot.Proto.Staking.IChill, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Chill message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Chill
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Polkadot.Proto.Staking.Chill;

                    /**
                     * Verifies a Chill message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Chill message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Chill
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Polkadot.Proto.Staking.Chill;

                    /**
                     * Creates a plain object from a Chill message. Also converts values to other types if specified.
                     * @param message Chill
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Polkadot.Proto.Staking.Chill, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Chill to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput blockHash */
                blockHash?: (Uint8Array|null);

                /** SigningInput genesisHash */
                genesisHash?: (Uint8Array|null);

                /** SigningInput nonce */
                nonce?: (Long|null);

                /** SigningInput specVersion */
                specVersion?: (number|null);

                /** SigningInput transactionVersion */
                transactionVersion?: (number|null);

                /** SigningInput tip */
                tip?: (Uint8Array|null);

                /** SigningInput era */
                era?: (TW.Polkadot.Proto.IEra|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput network */
                network?: (TW.Polkadot.Proto.Network|null);

                /** SigningInput balanceCall */
                balanceCall?: (TW.Polkadot.Proto.IBalance|null);

                /** SigningInput stakingCall */
                stakingCall?: (TW.Polkadot.Proto.IStaking|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Polkadot.Proto.ISigningInput);

                /** SigningInput blockHash. */
                public blockHash: Uint8Array;

                /** SigningInput genesisHash. */
                public genesisHash: Uint8Array;

                /** SigningInput nonce. */
                public nonce: Long;

                /** SigningInput specVersion. */
                public specVersion: number;

                /** SigningInput transactionVersion. */
                public transactionVersion: number;

                /** SigningInput tip. */
                public tip: Uint8Array;

                /** SigningInput era. */
                public era?: (TW.Polkadot.Proto.IEra|null);

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput network. */
                public network: TW.Polkadot.Proto.Network;

                /** SigningInput balanceCall. */
                public balanceCall?: (TW.Polkadot.Proto.IBalance|null);

                /** SigningInput stakingCall. */
                public stakingCall?: (TW.Polkadot.Proto.IStaking|null);

                /** SigningInput messageOneof. */
                public messageOneof?: ("balanceCall"|"stakingCall");

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Polkadot.Proto.ISigningInput): TW.Polkadot.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Polkadot.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Polkadot.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Polkadot.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Polkadot.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Polkadot.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Polkadot.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Polkadot.Proto.ISigningOutput): TW.Polkadot.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Polkadot.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Polkadot.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Polkadot.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Polkadot.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Polkadot.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Ripple. */
    namespace Ripple {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput amount */
                amount?: (Long|null);

                /** SigningInput fee */
                fee?: (Long|null);

                /** SigningInput sequence */
                sequence?: (number|null);

                /** SigningInput lastLedgerSequence */
                lastLedgerSequence?: (number|null);

                /** SigningInput account */
                account?: (string|null);

                /** SigningInput destination */
                destination?: (string|null);

                /** SigningInput destinationTag */
                destinationTag?: (Long|null);

                /** SigningInput flags */
                flags?: (Long|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Ripple.Proto.ISigningInput);

                /** SigningInput amount. */
                public amount: Long;

                /** SigningInput fee. */
                public fee: Long;

                /** SigningInput sequence. */
                public sequence: number;

                /** SigningInput lastLedgerSequence. */
                public lastLedgerSequence: number;

                /** SigningInput account. */
                public account: string;

                /** SigningInput destination. */
                public destination: string;

                /** SigningInput destinationTag. */
                public destinationTag: Long;

                /** SigningInput flags. */
                public flags: Long;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Ripple.Proto.ISigningInput): TW.Ripple.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Ripple.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Ripple.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Ripple.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Ripple.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Ripple.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Ripple.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Ripple.Proto.ISigningOutput): TW.Ripple.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Ripple.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Ripple.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Ripple.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Ripple.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Ripple.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Solana. */
    namespace Solana {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a Transfer. */
            interface ITransfer {

                /** Transfer recipient */
                recipient?: (string|null);

                /** Transfer value */
                value?: (Long|null);
            }

            /** Represents a Transfer. */
            class Transfer implements ITransfer {

                /**
                 * Constructs a new Transfer.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Solana.Proto.ITransfer);

                /** Transfer recipient. */
                public recipient: string;

                /** Transfer value. */
                public value: Long;

                /**
                 * Creates a new Transfer instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Transfer instance
                 */
                public static create(properties?: TW.Solana.Proto.ITransfer): TW.Solana.Proto.Transfer;

                /**
                 * Encodes the specified Transfer message. Does not implicitly {@link TW.Solana.Proto.Transfer.verify|verify} messages.
                 * @param message Transfer message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Solana.Proto.ITransfer, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Transfer message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Transfer
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Solana.Proto.Transfer;

                /**
                 * Verifies a Transfer message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Transfer message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Transfer
                 */
                public static fromObject(object: { [k: string]: any }): TW.Solana.Proto.Transfer;

                /**
                 * Creates a plain object from a Transfer message. Also converts values to other types if specified.
                 * @param message Transfer
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Solana.Proto.Transfer, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Transfer to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Stake. */
            interface IStake {

                /** Stake validatorPubkey */
                validatorPubkey?: (string|null);

                /** Stake value */
                value?: (Long|null);
            }

            /** Represents a Stake. */
            class Stake implements IStake {

                /**
                 * Constructs a new Stake.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Solana.Proto.IStake);

                /** Stake validatorPubkey. */
                public validatorPubkey: string;

                /** Stake value. */
                public value: Long;

                /**
                 * Creates a new Stake instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Stake instance
                 */
                public static create(properties?: TW.Solana.Proto.IStake): TW.Solana.Proto.Stake;

                /**
                 * Encodes the specified Stake message. Does not implicitly {@link TW.Solana.Proto.Stake.verify|verify} messages.
                 * @param message Stake message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Solana.Proto.IStake, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Stake message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Stake
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Solana.Proto.Stake;

                /**
                 * Verifies a Stake message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Stake message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Stake
                 */
                public static fromObject(object: { [k: string]: any }): TW.Solana.Proto.Stake;

                /**
                 * Creates a plain object from a Stake message. Also converts values to other types if specified.
                 * @param message Stake
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Solana.Proto.Stake, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Stake to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a DeactivateStake. */
            interface IDeactivateStake {

                /** DeactivateStake validatorPubkey */
                validatorPubkey?: (string|null);
            }

            /** Represents a DeactivateStake. */
            class DeactivateStake implements IDeactivateStake {

                /**
                 * Constructs a new DeactivateStake.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Solana.Proto.IDeactivateStake);

                /** DeactivateStake validatorPubkey. */
                public validatorPubkey: string;

                /**
                 * Creates a new DeactivateStake instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns DeactivateStake instance
                 */
                public static create(properties?: TW.Solana.Proto.IDeactivateStake): TW.Solana.Proto.DeactivateStake;

                /**
                 * Encodes the specified DeactivateStake message. Does not implicitly {@link TW.Solana.Proto.DeactivateStake.verify|verify} messages.
                 * @param message DeactivateStake message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Solana.Proto.IDeactivateStake, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a DeactivateStake message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns DeactivateStake
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Solana.Proto.DeactivateStake;

                /**
                 * Verifies a DeactivateStake message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a DeactivateStake message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns DeactivateStake
                 */
                public static fromObject(object: { [k: string]: any }): TW.Solana.Proto.DeactivateStake;

                /**
                 * Creates a plain object from a DeactivateStake message. Also converts values to other types if specified.
                 * @param message DeactivateStake
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Solana.Proto.DeactivateStake, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this DeactivateStake to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a WithdrawStake. */
            interface IWithdrawStake {

                /** WithdrawStake validatorPubkey */
                validatorPubkey?: (string|null);

                /** WithdrawStake value */
                value?: (Long|null);
            }

            /** Represents a WithdrawStake. */
            class WithdrawStake implements IWithdrawStake {

                /**
                 * Constructs a new WithdrawStake.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Solana.Proto.IWithdrawStake);

                /** WithdrawStake validatorPubkey. */
                public validatorPubkey: string;

                /** WithdrawStake value. */
                public value: Long;

                /**
                 * Creates a new WithdrawStake instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns WithdrawStake instance
                 */
                public static create(properties?: TW.Solana.Proto.IWithdrawStake): TW.Solana.Proto.WithdrawStake;

                /**
                 * Encodes the specified WithdrawStake message. Does not implicitly {@link TW.Solana.Proto.WithdrawStake.verify|verify} messages.
                 * @param message WithdrawStake message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Solana.Proto.IWithdrawStake, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a WithdrawStake message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns WithdrawStake
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Solana.Proto.WithdrawStake;

                /**
                 * Verifies a WithdrawStake message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a WithdrawStake message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns WithdrawStake
                 */
                public static fromObject(object: { [k: string]: any }): TW.Solana.Proto.WithdrawStake;

                /**
                 * Creates a plain object from a WithdrawStake message. Also converts values to other types if specified.
                 * @param message WithdrawStake
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Solana.Proto.WithdrawStake, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this WithdrawStake to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a CreateTokenAccount. */
            interface ICreateTokenAccount {

                /** CreateTokenAccount mainAddress */
                mainAddress?: (string|null);

                /** CreateTokenAccount tokenMintAddress */
                tokenMintAddress?: (string|null);

                /** CreateTokenAccount tokenAddress */
                tokenAddress?: (string|null);
            }

            /** Represents a CreateTokenAccount. */
            class CreateTokenAccount implements ICreateTokenAccount {

                /**
                 * Constructs a new CreateTokenAccount.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Solana.Proto.ICreateTokenAccount);

                /** CreateTokenAccount mainAddress. */
                public mainAddress: string;

                /** CreateTokenAccount tokenMintAddress. */
                public tokenMintAddress: string;

                /** CreateTokenAccount tokenAddress. */
                public tokenAddress: string;

                /**
                 * Creates a new CreateTokenAccount instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns CreateTokenAccount instance
                 */
                public static create(properties?: TW.Solana.Proto.ICreateTokenAccount): TW.Solana.Proto.CreateTokenAccount;

                /**
                 * Encodes the specified CreateTokenAccount message. Does not implicitly {@link TW.Solana.Proto.CreateTokenAccount.verify|verify} messages.
                 * @param message CreateTokenAccount message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Solana.Proto.ICreateTokenAccount, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a CreateTokenAccount message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns CreateTokenAccount
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Solana.Proto.CreateTokenAccount;

                /**
                 * Verifies a CreateTokenAccount message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a CreateTokenAccount message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns CreateTokenAccount
                 */
                public static fromObject(object: { [k: string]: any }): TW.Solana.Proto.CreateTokenAccount;

                /**
                 * Creates a plain object from a CreateTokenAccount message. Also converts values to other types if specified.
                 * @param message CreateTokenAccount
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Solana.Proto.CreateTokenAccount, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this CreateTokenAccount to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TokenTransfer. */
            interface ITokenTransfer {

                /** TokenTransfer tokenMintAddress */
                tokenMintAddress?: (string|null);

                /** TokenTransfer senderTokenAddress */
                senderTokenAddress?: (string|null);

                /** TokenTransfer recipientTokenAddress */
                recipientTokenAddress?: (string|null);

                /** TokenTransfer amount */
                amount?: (Long|null);

                /** TokenTransfer decimals */
                decimals?: (number|null);
            }

            /** Represents a TokenTransfer. */
            class TokenTransfer implements ITokenTransfer {

                /**
                 * Constructs a new TokenTransfer.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Solana.Proto.ITokenTransfer);

                /** TokenTransfer tokenMintAddress. */
                public tokenMintAddress: string;

                /** TokenTransfer senderTokenAddress. */
                public senderTokenAddress: string;

                /** TokenTransfer recipientTokenAddress. */
                public recipientTokenAddress: string;

                /** TokenTransfer amount. */
                public amount: Long;

                /** TokenTransfer decimals. */
                public decimals: number;

                /**
                 * Creates a new TokenTransfer instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TokenTransfer instance
                 */
                public static create(properties?: TW.Solana.Proto.ITokenTransfer): TW.Solana.Proto.TokenTransfer;

                /**
                 * Encodes the specified TokenTransfer message. Does not implicitly {@link TW.Solana.Proto.TokenTransfer.verify|verify} messages.
                 * @param message TokenTransfer message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Solana.Proto.ITokenTransfer, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TokenTransfer message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TokenTransfer
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Solana.Proto.TokenTransfer;

                /**
                 * Verifies a TokenTransfer message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TokenTransfer message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TokenTransfer
                 */
                public static fromObject(object: { [k: string]: any }): TW.Solana.Proto.TokenTransfer;

                /**
                 * Creates a plain object from a TokenTransfer message. Also converts values to other types if specified.
                 * @param message TokenTransfer
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Solana.Proto.TokenTransfer, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TokenTransfer to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a CreateAndTransferToken. */
            interface ICreateAndTransferToken {

                /** CreateAndTransferToken recipientMainAddress */
                recipientMainAddress?: (string|null);

                /** CreateAndTransferToken tokenMintAddress */
                tokenMintAddress?: (string|null);

                /** CreateAndTransferToken recipientTokenAddress */
                recipientTokenAddress?: (string|null);

                /** CreateAndTransferToken senderTokenAddress */
                senderTokenAddress?: (string|null);

                /** CreateAndTransferToken amount */
                amount?: (Long|null);

                /** CreateAndTransferToken decimals */
                decimals?: (number|null);
            }

            /** Represents a CreateAndTransferToken. */
            class CreateAndTransferToken implements ICreateAndTransferToken {

                /**
                 * Constructs a new CreateAndTransferToken.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Solana.Proto.ICreateAndTransferToken);

                /** CreateAndTransferToken recipientMainAddress. */
                public recipientMainAddress: string;

                /** CreateAndTransferToken tokenMintAddress. */
                public tokenMintAddress: string;

                /** CreateAndTransferToken recipientTokenAddress. */
                public recipientTokenAddress: string;

                /** CreateAndTransferToken senderTokenAddress. */
                public senderTokenAddress: string;

                /** CreateAndTransferToken amount. */
                public amount: Long;

                /** CreateAndTransferToken decimals. */
                public decimals: number;

                /**
                 * Creates a new CreateAndTransferToken instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns CreateAndTransferToken instance
                 */
                public static create(properties?: TW.Solana.Proto.ICreateAndTransferToken): TW.Solana.Proto.CreateAndTransferToken;

                /**
                 * Encodes the specified CreateAndTransferToken message. Does not implicitly {@link TW.Solana.Proto.CreateAndTransferToken.verify|verify} messages.
                 * @param message CreateAndTransferToken message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Solana.Proto.ICreateAndTransferToken, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a CreateAndTransferToken message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns CreateAndTransferToken
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Solana.Proto.CreateAndTransferToken;

                /**
                 * Verifies a CreateAndTransferToken message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a CreateAndTransferToken message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns CreateAndTransferToken
                 */
                public static fromObject(object: { [k: string]: any }): TW.Solana.Proto.CreateAndTransferToken;

                /**
                 * Creates a plain object from a CreateAndTransferToken message. Also converts values to other types if specified.
                 * @param message CreateAndTransferToken
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Solana.Proto.CreateAndTransferToken, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this CreateAndTransferToken to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput recentBlockhash */
                recentBlockhash?: (string|null);

                /** SigningInput transferTransaction */
                transferTransaction?: (TW.Solana.Proto.ITransfer|null);

                /** SigningInput stakeTransaction */
                stakeTransaction?: (TW.Solana.Proto.IStake|null);

                /** SigningInput deactivateStakeTransaction */
                deactivateStakeTransaction?: (TW.Solana.Proto.IDeactivateStake|null);

                /** SigningInput withdrawTransaction */
                withdrawTransaction?: (TW.Solana.Proto.IWithdrawStake|null);

                /** SigningInput createTokenAccountTransaction */
                createTokenAccountTransaction?: (TW.Solana.Proto.ICreateTokenAccount|null);

                /** SigningInput tokenTransferTransaction */
                tokenTransferTransaction?: (TW.Solana.Proto.ITokenTransfer|null);

                /** SigningInput createAndTransferTokenTransaction */
                createAndTransferTokenTransaction?: (TW.Solana.Proto.ICreateAndTransferToken|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Solana.Proto.ISigningInput);

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput recentBlockhash. */
                public recentBlockhash: string;

                /** SigningInput transferTransaction. */
                public transferTransaction?: (TW.Solana.Proto.ITransfer|null);

                /** SigningInput stakeTransaction. */
                public stakeTransaction?: (TW.Solana.Proto.IStake|null);

                /** SigningInput deactivateStakeTransaction. */
                public deactivateStakeTransaction?: (TW.Solana.Proto.IDeactivateStake|null);

                /** SigningInput withdrawTransaction. */
                public withdrawTransaction?: (TW.Solana.Proto.IWithdrawStake|null);

                /** SigningInput createTokenAccountTransaction. */
                public createTokenAccountTransaction?: (TW.Solana.Proto.ICreateTokenAccount|null);

                /** SigningInput tokenTransferTransaction. */
                public tokenTransferTransaction?: (TW.Solana.Proto.ITokenTransfer|null);

                /** SigningInput createAndTransferTokenTransaction. */
                public createAndTransferTokenTransaction?: (TW.Solana.Proto.ICreateAndTransferToken|null);

                /** SigningInput transactionType. */
                public transactionType?: ("transferTransaction"|"stakeTransaction"|"deactivateStakeTransaction"|"withdrawTransaction"|"createTokenAccountTransaction"|"tokenTransferTransaction"|"createAndTransferTokenTransaction");

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Solana.Proto.ISigningInput): TW.Solana.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Solana.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Solana.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Solana.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Solana.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Solana.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (string|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Solana.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: string;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Solana.Proto.ISigningOutput): TW.Solana.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Solana.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Solana.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Solana.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Solana.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Solana.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Stellar. */
    namespace Stellar {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of an Asset. */
            interface IAsset {

                /** Asset issuer */
                issuer?: (string|null);

                /** Asset alphanum4 */
                alphanum4?: (string|null);
            }

            /** Represents an Asset. */
            class Asset implements IAsset {

                /**
                 * Constructs a new Asset.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Stellar.Proto.IAsset);

                /** Asset issuer. */
                public issuer: string;

                /** Asset alphanum4. */
                public alphanum4: string;

                /**
                 * Creates a new Asset instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Asset instance
                 */
                public static create(properties?: TW.Stellar.Proto.IAsset): TW.Stellar.Proto.Asset;

                /**
                 * Encodes the specified Asset message. Does not implicitly {@link TW.Stellar.Proto.Asset.verify|verify} messages.
                 * @param message Asset message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Stellar.Proto.IAsset, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an Asset message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Asset
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Stellar.Proto.Asset;

                /**
                 * Verifies an Asset message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an Asset message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Asset
                 */
                public static fromObject(object: { [k: string]: any }): TW.Stellar.Proto.Asset;

                /**
                 * Creates a plain object from an Asset message. Also converts values to other types if specified.
                 * @param message Asset
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Stellar.Proto.Asset, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Asset to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an OperationCreateAccount. */
            interface IOperationCreateAccount {

                /** OperationCreateAccount destination */
                destination?: (string|null);

                /** OperationCreateAccount amount */
                amount?: (Long|null);
            }

            /** Represents an OperationCreateAccount. */
            class OperationCreateAccount implements IOperationCreateAccount {

                /**
                 * Constructs a new OperationCreateAccount.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Stellar.Proto.IOperationCreateAccount);

                /** OperationCreateAccount destination. */
                public destination: string;

                /** OperationCreateAccount amount. */
                public amount: Long;

                /**
                 * Creates a new OperationCreateAccount instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns OperationCreateAccount instance
                 */
                public static create(properties?: TW.Stellar.Proto.IOperationCreateAccount): TW.Stellar.Proto.OperationCreateAccount;

                /**
                 * Encodes the specified OperationCreateAccount message. Does not implicitly {@link TW.Stellar.Proto.OperationCreateAccount.verify|verify} messages.
                 * @param message OperationCreateAccount message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Stellar.Proto.IOperationCreateAccount, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an OperationCreateAccount message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns OperationCreateAccount
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Stellar.Proto.OperationCreateAccount;

                /**
                 * Verifies an OperationCreateAccount message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an OperationCreateAccount message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns OperationCreateAccount
                 */
                public static fromObject(object: { [k: string]: any }): TW.Stellar.Proto.OperationCreateAccount;

                /**
                 * Creates a plain object from an OperationCreateAccount message. Also converts values to other types if specified.
                 * @param message OperationCreateAccount
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Stellar.Proto.OperationCreateAccount, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this OperationCreateAccount to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an OperationPayment. */
            interface IOperationPayment {

                /** OperationPayment destination */
                destination?: (string|null);

                /** OperationPayment asset */
                asset?: (TW.Stellar.Proto.IAsset|null);

                /** OperationPayment amount */
                amount?: (Long|null);
            }

            /** Represents an OperationPayment. */
            class OperationPayment implements IOperationPayment {

                /**
                 * Constructs a new OperationPayment.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Stellar.Proto.IOperationPayment);

                /** OperationPayment destination. */
                public destination: string;

                /** OperationPayment asset. */
                public asset?: (TW.Stellar.Proto.IAsset|null);

                /** OperationPayment amount. */
                public amount: Long;

                /**
                 * Creates a new OperationPayment instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns OperationPayment instance
                 */
                public static create(properties?: TW.Stellar.Proto.IOperationPayment): TW.Stellar.Proto.OperationPayment;

                /**
                 * Encodes the specified OperationPayment message. Does not implicitly {@link TW.Stellar.Proto.OperationPayment.verify|verify} messages.
                 * @param message OperationPayment message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Stellar.Proto.IOperationPayment, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an OperationPayment message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns OperationPayment
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Stellar.Proto.OperationPayment;

                /**
                 * Verifies an OperationPayment message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an OperationPayment message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns OperationPayment
                 */
                public static fromObject(object: { [k: string]: any }): TW.Stellar.Proto.OperationPayment;

                /**
                 * Creates a plain object from an OperationPayment message. Also converts values to other types if specified.
                 * @param message OperationPayment
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Stellar.Proto.OperationPayment, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this OperationPayment to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an OperationChangeTrust. */
            interface IOperationChangeTrust {

                /** OperationChangeTrust asset */
                asset?: (TW.Stellar.Proto.IAsset|null);

                /** OperationChangeTrust validBefore */
                validBefore?: (Long|null);
            }

            /** Represents an OperationChangeTrust. */
            class OperationChangeTrust implements IOperationChangeTrust {

                /**
                 * Constructs a new OperationChangeTrust.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Stellar.Proto.IOperationChangeTrust);

                /** OperationChangeTrust asset. */
                public asset?: (TW.Stellar.Proto.IAsset|null);

                /** OperationChangeTrust validBefore. */
                public validBefore: Long;

                /**
                 * Creates a new OperationChangeTrust instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns OperationChangeTrust instance
                 */
                public static create(properties?: TW.Stellar.Proto.IOperationChangeTrust): TW.Stellar.Proto.OperationChangeTrust;

                /**
                 * Encodes the specified OperationChangeTrust message. Does not implicitly {@link TW.Stellar.Proto.OperationChangeTrust.verify|verify} messages.
                 * @param message OperationChangeTrust message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Stellar.Proto.IOperationChangeTrust, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an OperationChangeTrust message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns OperationChangeTrust
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Stellar.Proto.OperationChangeTrust;

                /**
                 * Verifies an OperationChangeTrust message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an OperationChangeTrust message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns OperationChangeTrust
                 */
                public static fromObject(object: { [k: string]: any }): TW.Stellar.Proto.OperationChangeTrust;

                /**
                 * Creates a plain object from an OperationChangeTrust message. Also converts values to other types if specified.
                 * @param message OperationChangeTrust
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Stellar.Proto.OperationChangeTrust, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this OperationChangeTrust to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a MemoVoid. */
            interface IMemoVoid {
            }

            /** Represents a MemoVoid. */
            class MemoVoid implements IMemoVoid {

                /**
                 * Constructs a new MemoVoid.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Stellar.Proto.IMemoVoid);

                /**
                 * Creates a new MemoVoid instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns MemoVoid instance
                 */
                public static create(properties?: TW.Stellar.Proto.IMemoVoid): TW.Stellar.Proto.MemoVoid;

                /**
                 * Encodes the specified MemoVoid message. Does not implicitly {@link TW.Stellar.Proto.MemoVoid.verify|verify} messages.
                 * @param message MemoVoid message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Stellar.Proto.IMemoVoid, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a MemoVoid message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns MemoVoid
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Stellar.Proto.MemoVoid;

                /**
                 * Verifies a MemoVoid message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a MemoVoid message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns MemoVoid
                 */
                public static fromObject(object: { [k: string]: any }): TW.Stellar.Proto.MemoVoid;

                /**
                 * Creates a plain object from a MemoVoid message. Also converts values to other types if specified.
                 * @param message MemoVoid
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Stellar.Proto.MemoVoid, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this MemoVoid to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a MemoText. */
            interface IMemoText {

                /** MemoText text */
                text?: (string|null);
            }

            /** Represents a MemoText. */
            class MemoText implements IMemoText {

                /**
                 * Constructs a new MemoText.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Stellar.Proto.IMemoText);

                /** MemoText text. */
                public text: string;

                /**
                 * Creates a new MemoText instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns MemoText instance
                 */
                public static create(properties?: TW.Stellar.Proto.IMemoText): TW.Stellar.Proto.MemoText;

                /**
                 * Encodes the specified MemoText message. Does not implicitly {@link TW.Stellar.Proto.MemoText.verify|verify} messages.
                 * @param message MemoText message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Stellar.Proto.IMemoText, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a MemoText message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns MemoText
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Stellar.Proto.MemoText;

                /**
                 * Verifies a MemoText message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a MemoText message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns MemoText
                 */
                public static fromObject(object: { [k: string]: any }): TW.Stellar.Proto.MemoText;

                /**
                 * Creates a plain object from a MemoText message. Also converts values to other types if specified.
                 * @param message MemoText
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Stellar.Proto.MemoText, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this MemoText to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a MemoId. */
            interface IMemoId {

                /** MemoId id */
                id?: (Long|null);
            }

            /** Represents a MemoId. */
            class MemoId implements IMemoId {

                /**
                 * Constructs a new MemoId.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Stellar.Proto.IMemoId);

                /** MemoId id. */
                public id: Long;

                /**
                 * Creates a new MemoId instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns MemoId instance
                 */
                public static create(properties?: TW.Stellar.Proto.IMemoId): TW.Stellar.Proto.MemoId;

                /**
                 * Encodes the specified MemoId message. Does not implicitly {@link TW.Stellar.Proto.MemoId.verify|verify} messages.
                 * @param message MemoId message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Stellar.Proto.IMemoId, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a MemoId message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns MemoId
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Stellar.Proto.MemoId;

                /**
                 * Verifies a MemoId message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a MemoId message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns MemoId
                 */
                public static fromObject(object: { [k: string]: any }): TW.Stellar.Proto.MemoId;

                /**
                 * Creates a plain object from a MemoId message. Also converts values to other types if specified.
                 * @param message MemoId
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Stellar.Proto.MemoId, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this MemoId to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a MemoHash. */
            interface IMemoHash {

                /** MemoHash hash */
                hash?: (Uint8Array|null);
            }

            /** Represents a MemoHash. */
            class MemoHash implements IMemoHash {

                /**
                 * Constructs a new MemoHash.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Stellar.Proto.IMemoHash);

                /** MemoHash hash. */
                public hash: Uint8Array;

                /**
                 * Creates a new MemoHash instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns MemoHash instance
                 */
                public static create(properties?: TW.Stellar.Proto.IMemoHash): TW.Stellar.Proto.MemoHash;

                /**
                 * Encodes the specified MemoHash message. Does not implicitly {@link TW.Stellar.Proto.MemoHash.verify|verify} messages.
                 * @param message MemoHash message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Stellar.Proto.IMemoHash, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a MemoHash message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns MemoHash
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Stellar.Proto.MemoHash;

                /**
                 * Verifies a MemoHash message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a MemoHash message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns MemoHash
                 */
                public static fromObject(object: { [k: string]: any }): TW.Stellar.Proto.MemoHash;

                /**
                 * Creates a plain object from a MemoHash message. Also converts values to other types if specified.
                 * @param message MemoHash
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Stellar.Proto.MemoHash, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this MemoHash to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput fee */
                fee?: (number|null);

                /** SigningInput sequence */
                sequence?: (Long|null);

                /** SigningInput account */
                account?: (string|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput passphrase */
                passphrase?: (string|null);

                /** SigningInput opCreateAccount */
                opCreateAccount?: (TW.Stellar.Proto.IOperationCreateAccount|null);

                /** SigningInput opPayment */
                opPayment?: (TW.Stellar.Proto.IOperationPayment|null);

                /** SigningInput opChangeTrust */
                opChangeTrust?: (TW.Stellar.Proto.IOperationChangeTrust|null);

                /** SigningInput memoVoid */
                memoVoid?: (TW.Stellar.Proto.IMemoVoid|null);

                /** SigningInput memoText */
                memoText?: (TW.Stellar.Proto.IMemoText|null);

                /** SigningInput memoId */
                memoId?: (TW.Stellar.Proto.IMemoId|null);

                /** SigningInput memoHash */
                memoHash?: (TW.Stellar.Proto.IMemoHash|null);

                /** SigningInput memoReturnHash */
                memoReturnHash?: (TW.Stellar.Proto.IMemoHash|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Stellar.Proto.ISigningInput);

                /** SigningInput fee. */
                public fee: number;

                /** SigningInput sequence. */
                public sequence: Long;

                /** SigningInput account. */
                public account: string;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput passphrase. */
                public passphrase: string;

                /** SigningInput opCreateAccount. */
                public opCreateAccount?: (TW.Stellar.Proto.IOperationCreateAccount|null);

                /** SigningInput opPayment. */
                public opPayment?: (TW.Stellar.Proto.IOperationPayment|null);

                /** SigningInput opChangeTrust. */
                public opChangeTrust?: (TW.Stellar.Proto.IOperationChangeTrust|null);

                /** SigningInput memoVoid. */
                public memoVoid?: (TW.Stellar.Proto.IMemoVoid|null);

                /** SigningInput memoText. */
                public memoText?: (TW.Stellar.Proto.IMemoText|null);

                /** SigningInput memoId. */
                public memoId?: (TW.Stellar.Proto.IMemoId|null);

                /** SigningInput memoHash. */
                public memoHash?: (TW.Stellar.Proto.IMemoHash|null);

                /** SigningInput memoReturnHash. */
                public memoReturnHash?: (TW.Stellar.Proto.IMemoHash|null);

                /** SigningInput operationOneof. */
                public operationOneof?: ("opCreateAccount"|"opPayment"|"opChangeTrust");

                /** SigningInput memoTypeOneof. */
                public memoTypeOneof?: ("memoVoid"|"memoText"|"memoId"|"memoHash"|"memoReturnHash");

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Stellar.Proto.ISigningInput): TW.Stellar.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Stellar.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Stellar.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Stellar.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Stellar.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Stellar.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput signature */
                signature?: (string|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Stellar.Proto.ISigningOutput);

                /** SigningOutput signature. */
                public signature: string;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Stellar.Proto.ISigningOutput): TW.Stellar.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Stellar.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Stellar.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Stellar.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Stellar.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Stellar.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Tezos. */
    namespace Tezos {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput operationList */
                operationList?: (TW.Tezos.Proto.IOperationList|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tezos.Proto.ISigningInput);

                /** SigningInput operationList. */
                public operationList?: (TW.Tezos.Proto.IOperationList|null);

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Tezos.Proto.ISigningInput): TW.Tezos.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Tezos.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tezos.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tezos.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tezos.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tezos.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tezos.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Tezos.Proto.ISigningOutput): TW.Tezos.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Tezos.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tezos.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tezos.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tezos.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tezos.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an OperationList. */
            interface IOperationList {

                /** OperationList branch */
                branch?: (string|null);

                /** OperationList operations */
                operations?: (TW.Tezos.Proto.IOperation[]|null);
            }

            /** Represents an OperationList. */
            class OperationList implements IOperationList {

                /**
                 * Constructs a new OperationList.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tezos.Proto.IOperationList);

                /** OperationList branch. */
                public branch: string;

                /** OperationList operations. */
                public operations: TW.Tezos.Proto.IOperation[];

                /**
                 * Creates a new OperationList instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns OperationList instance
                 */
                public static create(properties?: TW.Tezos.Proto.IOperationList): TW.Tezos.Proto.OperationList;

                /**
                 * Encodes the specified OperationList message. Does not implicitly {@link TW.Tezos.Proto.OperationList.verify|verify} messages.
                 * @param message OperationList message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tezos.Proto.IOperationList, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an OperationList message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns OperationList
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tezos.Proto.OperationList;

                /**
                 * Verifies an OperationList message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an OperationList message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns OperationList
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tezos.Proto.OperationList;

                /**
                 * Creates a plain object from an OperationList message. Also converts values to other types if specified.
                 * @param message OperationList
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tezos.Proto.OperationList, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this OperationList to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an Operation. */
            interface IOperation {

                /** Operation counter */
                counter?: (Long|null);

                /** Operation source */
                source?: (string|null);

                /** Operation fee */
                fee?: (Long|null);

                /** Operation gasLimit */
                gasLimit?: (Long|null);

                /** Operation storageLimit */
                storageLimit?: (Long|null);

                /** Operation kind */
                kind?: (TW.Tezos.Proto.Operation.OperationKind|null);

                /** Operation revealOperationData */
                revealOperationData?: (TW.Tezos.Proto.IRevealOperationData|null);

                /** Operation transactionOperationData */
                transactionOperationData?: (TW.Tezos.Proto.ITransactionOperationData|null);

                /** Operation delegationOperationData */
                delegationOperationData?: (TW.Tezos.Proto.IDelegationOperationData|null);
            }

            /** Represents an Operation. */
            class Operation implements IOperation {

                /**
                 * Constructs a new Operation.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tezos.Proto.IOperation);

                /** Operation counter. */
                public counter: Long;

                /** Operation source. */
                public source: string;

                /** Operation fee. */
                public fee: Long;

                /** Operation gasLimit. */
                public gasLimit: Long;

                /** Operation storageLimit. */
                public storageLimit: Long;

                /** Operation kind. */
                public kind: TW.Tezos.Proto.Operation.OperationKind;

                /** Operation revealOperationData. */
                public revealOperationData?: (TW.Tezos.Proto.IRevealOperationData|null);

                /** Operation transactionOperationData. */
                public transactionOperationData?: (TW.Tezos.Proto.ITransactionOperationData|null);

                /** Operation delegationOperationData. */
                public delegationOperationData?: (TW.Tezos.Proto.IDelegationOperationData|null);

                /** Operation operationData. */
                public operationData?: ("revealOperationData"|"transactionOperationData"|"delegationOperationData");

                /**
                 * Creates a new Operation instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Operation instance
                 */
                public static create(properties?: TW.Tezos.Proto.IOperation): TW.Tezos.Proto.Operation;

                /**
                 * Encodes the specified Operation message. Does not implicitly {@link TW.Tezos.Proto.Operation.verify|verify} messages.
                 * @param message Operation message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tezos.Proto.IOperation, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an Operation message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Operation
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tezos.Proto.Operation;

                /**
                 * Verifies an Operation message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an Operation message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Operation
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tezos.Proto.Operation;

                /**
                 * Creates a plain object from an Operation message. Also converts values to other types if specified.
                 * @param message Operation
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tezos.Proto.Operation, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Operation to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            namespace Operation {

                /** OperationKind enum. */
                enum OperationKind {
                    ENDORSEMENT = 0,
                    REVEAL = 107,
                    TRANSACTION = 108,
                    DELEGATION = 110
                }
            }

            /** Properties of a TransactionOperationData. */
            interface ITransactionOperationData {

                /** TransactionOperationData destination */
                destination?: (string|null);

                /** TransactionOperationData amount */
                amount?: (Long|null);
            }

            /** Represents a TransactionOperationData. */
            class TransactionOperationData implements ITransactionOperationData {

                /**
                 * Constructs a new TransactionOperationData.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tezos.Proto.ITransactionOperationData);

                /** TransactionOperationData destination. */
                public destination: string;

                /** TransactionOperationData amount. */
                public amount: Long;

                /**
                 * Creates a new TransactionOperationData instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionOperationData instance
                 */
                public static create(properties?: TW.Tezos.Proto.ITransactionOperationData): TW.Tezos.Proto.TransactionOperationData;

                /**
                 * Encodes the specified TransactionOperationData message. Does not implicitly {@link TW.Tezos.Proto.TransactionOperationData.verify|verify} messages.
                 * @param message TransactionOperationData message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tezos.Proto.ITransactionOperationData, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionOperationData message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionOperationData
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tezos.Proto.TransactionOperationData;

                /**
                 * Verifies a TransactionOperationData message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionOperationData message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionOperationData
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tezos.Proto.TransactionOperationData;

                /**
                 * Creates a plain object from a TransactionOperationData message. Also converts values to other types if specified.
                 * @param message TransactionOperationData
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tezos.Proto.TransactionOperationData, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionOperationData to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a RevealOperationData. */
            interface IRevealOperationData {

                /** RevealOperationData publicKey */
                publicKey?: (Uint8Array|null);
            }

            /** Represents a RevealOperationData. */
            class RevealOperationData implements IRevealOperationData {

                /**
                 * Constructs a new RevealOperationData.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tezos.Proto.IRevealOperationData);

                /** RevealOperationData publicKey. */
                public publicKey: Uint8Array;

                /**
                 * Creates a new RevealOperationData instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns RevealOperationData instance
                 */
                public static create(properties?: TW.Tezos.Proto.IRevealOperationData): TW.Tezos.Proto.RevealOperationData;

                /**
                 * Encodes the specified RevealOperationData message. Does not implicitly {@link TW.Tezos.Proto.RevealOperationData.verify|verify} messages.
                 * @param message RevealOperationData message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tezos.Proto.IRevealOperationData, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a RevealOperationData message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns RevealOperationData
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tezos.Proto.RevealOperationData;

                /**
                 * Verifies a RevealOperationData message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a RevealOperationData message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns RevealOperationData
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tezos.Proto.RevealOperationData;

                /**
                 * Creates a plain object from a RevealOperationData message. Also converts values to other types if specified.
                 * @param message RevealOperationData
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tezos.Proto.RevealOperationData, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this RevealOperationData to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a DelegationOperationData. */
            interface IDelegationOperationData {

                /** DelegationOperationData delegate */
                delegate?: (string|null);
            }

            /** Represents a DelegationOperationData. */
            class DelegationOperationData implements IDelegationOperationData {

                /**
                 * Constructs a new DelegationOperationData.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tezos.Proto.IDelegationOperationData);

                /** DelegationOperationData delegate. */
                public delegate: string;

                /**
                 * Creates a new DelegationOperationData instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns DelegationOperationData instance
                 */
                public static create(properties?: TW.Tezos.Proto.IDelegationOperationData): TW.Tezos.Proto.DelegationOperationData;

                /**
                 * Encodes the specified DelegationOperationData message. Does not implicitly {@link TW.Tezos.Proto.DelegationOperationData.verify|verify} messages.
                 * @param message DelegationOperationData message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tezos.Proto.IDelegationOperationData, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a DelegationOperationData message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns DelegationOperationData
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tezos.Proto.DelegationOperationData;

                /**
                 * Verifies a DelegationOperationData message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a DelegationOperationData message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns DelegationOperationData
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tezos.Proto.DelegationOperationData;

                /**
                 * Creates a plain object from a DelegationOperationData message. Also converts values to other types if specified.
                 * @param message DelegationOperationData
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tezos.Proto.DelegationOperationData, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this DelegationOperationData to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Theta. */
    namespace Theta {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** Chain ID string, mainnet, testnet and privatenet */
                chainId?: (string|null);

                /** Recipient address */
                toAddress?: (string|null);

                /** Theta token amount to send in wei (256-bit number) */
                thetaAmount?: (Uint8Array|null);

                /** TFuel token amount to send in wei (256-bit number) */
                tfuelAmount?: (Uint8Array|null);

                /** Sequence number of the transaction for the sender address */
                sequence?: (Long|null);

                /** Fee amount in TFuel wei for the transaction (256-bit number) */
                fee?: (Uint8Array|null);

                /** Private key */
                privateKey?: (Uint8Array|null);
            }

            /** Input data necessary to create a signed transaction */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Theta.Proto.ISigningInput);

                /** Chain ID string, mainnet, testnet and privatenet */
                public chainId: string;

                /** Recipient address */
                public toAddress: string;

                /** Theta token amount to send in wei (256-bit number) */
                public thetaAmount: Uint8Array;

                /** TFuel token amount to send in wei (256-bit number) */
                public tfuelAmount: Uint8Array;

                /** Sequence number of the transaction for the sender address */
                public sequence: Long;

                /** Fee amount in TFuel wei for the transaction (256-bit number) */
                public fee: Uint8Array;

                /** Private key */
                public privateKey: Uint8Array;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Theta.Proto.ISigningInput): TW.Theta.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Theta.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Theta.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Theta.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Theta.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Theta.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** Signed and encoded transaction bytes */
                encoded?: (Uint8Array|null);

                /** Signature */
                signature?: (Uint8Array|null);
            }

            /** Transaction signing output */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Theta.Proto.ISigningOutput);

                /** Signed and encoded transaction bytes */
                public encoded: Uint8Array;

                /** Signature */
                public signature: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Theta.Proto.ISigningOutput): TW.Theta.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Theta.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Theta.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Theta.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Theta.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Theta.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Tron. */
    namespace Tron {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a TransferContract. */
            interface ITransferContract {

                /** TransferContract ownerAddress */
                ownerAddress?: (string|null);

                /** TransferContract toAddress */
                toAddress?: (string|null);

                /** TransferContract amount */
                amount?: (Long|null);
            }

            /** Represents a TransferContract. */
            class TransferContract implements ITransferContract {

                /**
                 * Constructs a new TransferContract.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.ITransferContract);

                /** TransferContract ownerAddress. */
                public ownerAddress: string;

                /** TransferContract toAddress. */
                public toAddress: string;

                /** TransferContract amount. */
                public amount: Long;

                /**
                 * Creates a new TransferContract instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransferContract instance
                 */
                public static create(properties?: TW.Tron.Proto.ITransferContract): TW.Tron.Proto.TransferContract;

                /**
                 * Encodes the specified TransferContract message. Does not implicitly {@link TW.Tron.Proto.TransferContract.verify|verify} messages.
                 * @param message TransferContract message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.ITransferContract, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransferContract message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransferContract
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.TransferContract;

                /**
                 * Verifies a TransferContract message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransferContract message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransferContract
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.TransferContract;

                /**
                 * Creates a plain object from a TransferContract message. Also converts values to other types if specified.
                 * @param message TransferContract
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.TransferContract, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransferContract to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransferAssetContract. */
            interface ITransferAssetContract {

                /** TransferAssetContract assetName */
                assetName?: (string|null);

                /** TransferAssetContract ownerAddress */
                ownerAddress?: (string|null);

                /** TransferAssetContract toAddress */
                toAddress?: (string|null);

                /** TransferAssetContract amount */
                amount?: (Long|null);
            }

            /** Represents a TransferAssetContract. */
            class TransferAssetContract implements ITransferAssetContract {

                /**
                 * Constructs a new TransferAssetContract.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.ITransferAssetContract);

                /** TransferAssetContract assetName. */
                public assetName: string;

                /** TransferAssetContract ownerAddress. */
                public ownerAddress: string;

                /** TransferAssetContract toAddress. */
                public toAddress: string;

                /** TransferAssetContract amount. */
                public amount: Long;

                /**
                 * Creates a new TransferAssetContract instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransferAssetContract instance
                 */
                public static create(properties?: TW.Tron.Proto.ITransferAssetContract): TW.Tron.Proto.TransferAssetContract;

                /**
                 * Encodes the specified TransferAssetContract message. Does not implicitly {@link TW.Tron.Proto.TransferAssetContract.verify|verify} messages.
                 * @param message TransferAssetContract message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.ITransferAssetContract, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransferAssetContract message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransferAssetContract
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.TransferAssetContract;

                /**
                 * Verifies a TransferAssetContract message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransferAssetContract message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransferAssetContract
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.TransferAssetContract;

                /**
                 * Creates a plain object from a TransferAssetContract message. Also converts values to other types if specified.
                 * @param message TransferAssetContract
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.TransferAssetContract, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransferAssetContract to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransferTRC20Contract. */
            interface ITransferTRC20Contract {

                /** TransferTRC20Contract contractAddress */
                contractAddress?: (string|null);

                /** TransferTRC20Contract ownerAddress */
                ownerAddress?: (string|null);

                /** TransferTRC20Contract toAddress */
                toAddress?: (string|null);

                /** TransferTRC20Contract amount */
                amount?: (Uint8Array|null);
            }

            /** Represents a TransferTRC20Contract. */
            class TransferTRC20Contract implements ITransferTRC20Contract {

                /**
                 * Constructs a new TransferTRC20Contract.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.ITransferTRC20Contract);

                /** TransferTRC20Contract contractAddress. */
                public contractAddress: string;

                /** TransferTRC20Contract ownerAddress. */
                public ownerAddress: string;

                /** TransferTRC20Contract toAddress. */
                public toAddress: string;

                /** TransferTRC20Contract amount. */
                public amount: Uint8Array;

                /**
                 * Creates a new TransferTRC20Contract instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransferTRC20Contract instance
                 */
                public static create(properties?: TW.Tron.Proto.ITransferTRC20Contract): TW.Tron.Proto.TransferTRC20Contract;

                /**
                 * Encodes the specified TransferTRC20Contract message. Does not implicitly {@link TW.Tron.Proto.TransferTRC20Contract.verify|verify} messages.
                 * @param message TransferTRC20Contract message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.ITransferTRC20Contract, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransferTRC20Contract message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransferTRC20Contract
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.TransferTRC20Contract;

                /**
                 * Verifies a TransferTRC20Contract message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransferTRC20Contract message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransferTRC20Contract
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.TransferTRC20Contract;

                /**
                 * Creates a plain object from a TransferTRC20Contract message. Also converts values to other types if specified.
                 * @param message TransferTRC20Contract
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.TransferTRC20Contract, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransferTRC20Contract to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a FreezeBalanceContract. */
            interface IFreezeBalanceContract {

                /** FreezeBalanceContract ownerAddress */
                ownerAddress?: (string|null);

                /** FreezeBalanceContract frozenBalance */
                frozenBalance?: (Long|null);

                /** FreezeBalanceContract frozenDuration */
                frozenDuration?: (Long|null);

                /** FreezeBalanceContract resource */
                resource?: (string|null);

                /** FreezeBalanceContract receiverAddress */
                receiverAddress?: (string|null);
            }

            /** Represents a FreezeBalanceContract. */
            class FreezeBalanceContract implements IFreezeBalanceContract {

                /**
                 * Constructs a new FreezeBalanceContract.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.IFreezeBalanceContract);

                /** FreezeBalanceContract ownerAddress. */
                public ownerAddress: string;

                /** FreezeBalanceContract frozenBalance. */
                public frozenBalance: Long;

                /** FreezeBalanceContract frozenDuration. */
                public frozenDuration: Long;

                /** FreezeBalanceContract resource. */
                public resource: string;

                /** FreezeBalanceContract receiverAddress. */
                public receiverAddress: string;

                /**
                 * Creates a new FreezeBalanceContract instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns FreezeBalanceContract instance
                 */
                public static create(properties?: TW.Tron.Proto.IFreezeBalanceContract): TW.Tron.Proto.FreezeBalanceContract;

                /**
                 * Encodes the specified FreezeBalanceContract message. Does not implicitly {@link TW.Tron.Proto.FreezeBalanceContract.verify|verify} messages.
                 * @param message FreezeBalanceContract message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.IFreezeBalanceContract, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a FreezeBalanceContract message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns FreezeBalanceContract
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.FreezeBalanceContract;

                /**
                 * Verifies a FreezeBalanceContract message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a FreezeBalanceContract message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns FreezeBalanceContract
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.FreezeBalanceContract;

                /**
                 * Creates a plain object from a FreezeBalanceContract message. Also converts values to other types if specified.
                 * @param message FreezeBalanceContract
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.FreezeBalanceContract, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this FreezeBalanceContract to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an UnfreezeBalanceContract. */
            interface IUnfreezeBalanceContract {

                /** UnfreezeBalanceContract ownerAddress */
                ownerAddress?: (string|null);

                /** UnfreezeBalanceContract resource */
                resource?: (string|null);

                /** UnfreezeBalanceContract receiverAddress */
                receiverAddress?: (string|null);
            }

            /** Represents an UnfreezeBalanceContract. */
            class UnfreezeBalanceContract implements IUnfreezeBalanceContract {

                /**
                 * Constructs a new UnfreezeBalanceContract.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.IUnfreezeBalanceContract);

                /** UnfreezeBalanceContract ownerAddress. */
                public ownerAddress: string;

                /** UnfreezeBalanceContract resource. */
                public resource: string;

                /** UnfreezeBalanceContract receiverAddress. */
                public receiverAddress: string;

                /**
                 * Creates a new UnfreezeBalanceContract instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns UnfreezeBalanceContract instance
                 */
                public static create(properties?: TW.Tron.Proto.IUnfreezeBalanceContract): TW.Tron.Proto.UnfreezeBalanceContract;

                /**
                 * Encodes the specified UnfreezeBalanceContract message. Does not implicitly {@link TW.Tron.Proto.UnfreezeBalanceContract.verify|verify} messages.
                 * @param message UnfreezeBalanceContract message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.IUnfreezeBalanceContract, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an UnfreezeBalanceContract message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns UnfreezeBalanceContract
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.UnfreezeBalanceContract;

                /**
                 * Verifies an UnfreezeBalanceContract message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an UnfreezeBalanceContract message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns UnfreezeBalanceContract
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.UnfreezeBalanceContract;

                /**
                 * Creates a plain object from an UnfreezeBalanceContract message. Also converts values to other types if specified.
                 * @param message UnfreezeBalanceContract
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.UnfreezeBalanceContract, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this UnfreezeBalanceContract to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an UnfreezeAssetContract. */
            interface IUnfreezeAssetContract {

                /** UnfreezeAssetContract ownerAddress */
                ownerAddress?: (string|null);
            }

            /** Represents an UnfreezeAssetContract. */
            class UnfreezeAssetContract implements IUnfreezeAssetContract {

                /**
                 * Constructs a new UnfreezeAssetContract.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.IUnfreezeAssetContract);

                /** UnfreezeAssetContract ownerAddress. */
                public ownerAddress: string;

                /**
                 * Creates a new UnfreezeAssetContract instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns UnfreezeAssetContract instance
                 */
                public static create(properties?: TW.Tron.Proto.IUnfreezeAssetContract): TW.Tron.Proto.UnfreezeAssetContract;

                /**
                 * Encodes the specified UnfreezeAssetContract message. Does not implicitly {@link TW.Tron.Proto.UnfreezeAssetContract.verify|verify} messages.
                 * @param message UnfreezeAssetContract message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.IUnfreezeAssetContract, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an UnfreezeAssetContract message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns UnfreezeAssetContract
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.UnfreezeAssetContract;

                /**
                 * Verifies an UnfreezeAssetContract message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an UnfreezeAssetContract message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns UnfreezeAssetContract
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.UnfreezeAssetContract;

                /**
                 * Creates a plain object from an UnfreezeAssetContract message. Also converts values to other types if specified.
                 * @param message UnfreezeAssetContract
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.UnfreezeAssetContract, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this UnfreezeAssetContract to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a VoteAssetContract. */
            interface IVoteAssetContract {

                /** VoteAssetContract ownerAddress */
                ownerAddress?: (string|null);

                /** VoteAssetContract voteAddress */
                voteAddress?: (string[]|null);

                /** VoteAssetContract support */
                support?: (boolean|null);

                /** VoteAssetContract count */
                count?: (number|null);
            }

            /** Represents a VoteAssetContract. */
            class VoteAssetContract implements IVoteAssetContract {

                /**
                 * Constructs a new VoteAssetContract.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.IVoteAssetContract);

                /** VoteAssetContract ownerAddress. */
                public ownerAddress: string;

                /** VoteAssetContract voteAddress. */
                public voteAddress: string[];

                /** VoteAssetContract support. */
                public support: boolean;

                /** VoteAssetContract count. */
                public count: number;

                /**
                 * Creates a new VoteAssetContract instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns VoteAssetContract instance
                 */
                public static create(properties?: TW.Tron.Proto.IVoteAssetContract): TW.Tron.Proto.VoteAssetContract;

                /**
                 * Encodes the specified VoteAssetContract message. Does not implicitly {@link TW.Tron.Proto.VoteAssetContract.verify|verify} messages.
                 * @param message VoteAssetContract message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.IVoteAssetContract, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a VoteAssetContract message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns VoteAssetContract
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.VoteAssetContract;

                /**
                 * Verifies a VoteAssetContract message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a VoteAssetContract message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns VoteAssetContract
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.VoteAssetContract;

                /**
                 * Creates a plain object from a VoteAssetContract message. Also converts values to other types if specified.
                 * @param message VoteAssetContract
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.VoteAssetContract, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this VoteAssetContract to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a VoteWitnessContract. */
            interface IVoteWitnessContract {

                /** VoteWitnessContract ownerAddress */
                ownerAddress?: (string|null);

                /** VoteWitnessContract votes */
                votes?: (TW.Tron.Proto.VoteWitnessContract.IVote[]|null);

                /** VoteWitnessContract support */
                support?: (boolean|null);
            }

            /** Represents a VoteWitnessContract. */
            class VoteWitnessContract implements IVoteWitnessContract {

                /**
                 * Constructs a new VoteWitnessContract.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.IVoteWitnessContract);

                /** VoteWitnessContract ownerAddress. */
                public ownerAddress: string;

                /** VoteWitnessContract votes. */
                public votes: TW.Tron.Proto.VoteWitnessContract.IVote[];

                /** VoteWitnessContract support. */
                public support: boolean;

                /**
                 * Creates a new VoteWitnessContract instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns VoteWitnessContract instance
                 */
                public static create(properties?: TW.Tron.Proto.IVoteWitnessContract): TW.Tron.Proto.VoteWitnessContract;

                /**
                 * Encodes the specified VoteWitnessContract message. Does not implicitly {@link TW.Tron.Proto.VoteWitnessContract.verify|verify} messages.
                 * @param message VoteWitnessContract message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.IVoteWitnessContract, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a VoteWitnessContract message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns VoteWitnessContract
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.VoteWitnessContract;

                /**
                 * Verifies a VoteWitnessContract message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a VoteWitnessContract message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns VoteWitnessContract
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.VoteWitnessContract;

                /**
                 * Creates a plain object from a VoteWitnessContract message. Also converts values to other types if specified.
                 * @param message VoteWitnessContract
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.VoteWitnessContract, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this VoteWitnessContract to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            namespace VoteWitnessContract {

                /** Properties of a Vote. */
                interface IVote {

                    /** Vote voteAddress */
                    voteAddress?: (string|null);

                    /** Vote voteCount */
                    voteCount?: (Long|null);
                }

                /** Represents a Vote. */
                class Vote implements IVote {

                    /**
                     * Constructs a new Vote.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Tron.Proto.VoteWitnessContract.IVote);

                    /** Vote voteAddress. */
                    public voteAddress: string;

                    /** Vote voteCount. */
                    public voteCount: Long;

                    /**
                     * Creates a new Vote instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Vote instance
                     */
                    public static create(properties?: TW.Tron.Proto.VoteWitnessContract.IVote): TW.Tron.Proto.VoteWitnessContract.Vote;

                    /**
                     * Encodes the specified Vote message. Does not implicitly {@link TW.Tron.Proto.VoteWitnessContract.Vote.verify|verify} messages.
                     * @param message Vote message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Tron.Proto.VoteWitnessContract.IVote, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Vote message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Vote
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.VoteWitnessContract.Vote;

                    /**
                     * Verifies a Vote message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Vote message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Vote
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.VoteWitnessContract.Vote;

                    /**
                     * Creates a plain object from a Vote message. Also converts values to other types if specified.
                     * @param message Vote
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Tron.Proto.VoteWitnessContract.Vote, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Vote to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }
            }

            /** Properties of a WithdrawBalanceContract. */
            interface IWithdrawBalanceContract {

                /** WithdrawBalanceContract ownerAddress */
                ownerAddress?: (string|null);
            }

            /** Represents a WithdrawBalanceContract. */
            class WithdrawBalanceContract implements IWithdrawBalanceContract {

                /**
                 * Constructs a new WithdrawBalanceContract.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.IWithdrawBalanceContract);

                /** WithdrawBalanceContract ownerAddress. */
                public ownerAddress: string;

                /**
                 * Creates a new WithdrawBalanceContract instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns WithdrawBalanceContract instance
                 */
                public static create(properties?: TW.Tron.Proto.IWithdrawBalanceContract): TW.Tron.Proto.WithdrawBalanceContract;

                /**
                 * Encodes the specified WithdrawBalanceContract message. Does not implicitly {@link TW.Tron.Proto.WithdrawBalanceContract.verify|verify} messages.
                 * @param message WithdrawBalanceContract message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.IWithdrawBalanceContract, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a WithdrawBalanceContract message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns WithdrawBalanceContract
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.WithdrawBalanceContract;

                /**
                 * Verifies a WithdrawBalanceContract message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a WithdrawBalanceContract message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns WithdrawBalanceContract
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.WithdrawBalanceContract;

                /**
                 * Creates a plain object from a WithdrawBalanceContract message. Also converts values to other types if specified.
                 * @param message WithdrawBalanceContract
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.WithdrawBalanceContract, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this WithdrawBalanceContract to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TriggerSmartContract. */
            interface ITriggerSmartContract {

                /** TriggerSmartContract ownerAddress */
                ownerAddress?: (string|null);

                /** TriggerSmartContract contractAddress */
                contractAddress?: (string|null);

                /** TriggerSmartContract callValue */
                callValue?: (Long|null);

                /** TriggerSmartContract data */
                data?: (Uint8Array|null);

                /** TriggerSmartContract callTokenValue */
                callTokenValue?: (Long|null);

                /** TriggerSmartContract tokenId */
                tokenId?: (Long|null);
            }

            /** Represents a TriggerSmartContract. */
            class TriggerSmartContract implements ITriggerSmartContract {

                /**
                 * Constructs a new TriggerSmartContract.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.ITriggerSmartContract);

                /** TriggerSmartContract ownerAddress. */
                public ownerAddress: string;

                /** TriggerSmartContract contractAddress. */
                public contractAddress: string;

                /** TriggerSmartContract callValue. */
                public callValue: Long;

                /** TriggerSmartContract data. */
                public data: Uint8Array;

                /** TriggerSmartContract callTokenValue. */
                public callTokenValue: Long;

                /** TriggerSmartContract tokenId. */
                public tokenId: Long;

                /**
                 * Creates a new TriggerSmartContract instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TriggerSmartContract instance
                 */
                public static create(properties?: TW.Tron.Proto.ITriggerSmartContract): TW.Tron.Proto.TriggerSmartContract;

                /**
                 * Encodes the specified TriggerSmartContract message. Does not implicitly {@link TW.Tron.Proto.TriggerSmartContract.verify|verify} messages.
                 * @param message TriggerSmartContract message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.ITriggerSmartContract, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TriggerSmartContract message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TriggerSmartContract
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.TriggerSmartContract;

                /**
                 * Verifies a TriggerSmartContract message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TriggerSmartContract message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TriggerSmartContract
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.TriggerSmartContract;

                /**
                 * Creates a plain object from a TriggerSmartContract message. Also converts values to other types if specified.
                 * @param message TriggerSmartContract
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.TriggerSmartContract, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TriggerSmartContract to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a BlockHeader. */
            interface IBlockHeader {

                /** BlockHeader timestamp */
                timestamp?: (Long|null);

                /** BlockHeader txTrieRoot */
                txTrieRoot?: (Uint8Array|null);

                /** BlockHeader parentHash */
                parentHash?: (Uint8Array|null);

                /** BlockHeader number */
                number?: (Long|null);

                /** BlockHeader witnessAddress */
                witnessAddress?: (Uint8Array|null);

                /** BlockHeader version */
                version?: (number|null);
            }

            /** Represents a BlockHeader. */
            class BlockHeader implements IBlockHeader {

                /**
                 * Constructs a new BlockHeader.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.IBlockHeader);

                /** BlockHeader timestamp. */
                public timestamp: Long;

                /** BlockHeader txTrieRoot. */
                public txTrieRoot: Uint8Array;

                /** BlockHeader parentHash. */
                public parentHash: Uint8Array;

                /** BlockHeader number. */
                public number: Long;

                /** BlockHeader witnessAddress. */
                public witnessAddress: Uint8Array;

                /** BlockHeader version. */
                public version: number;

                /**
                 * Creates a new BlockHeader instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns BlockHeader instance
                 */
                public static create(properties?: TW.Tron.Proto.IBlockHeader): TW.Tron.Proto.BlockHeader;

                /**
                 * Encodes the specified BlockHeader message. Does not implicitly {@link TW.Tron.Proto.BlockHeader.verify|verify} messages.
                 * @param message BlockHeader message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.IBlockHeader, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a BlockHeader message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns BlockHeader
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.BlockHeader;

                /**
                 * Verifies a BlockHeader message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a BlockHeader message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns BlockHeader
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.BlockHeader;

                /**
                 * Creates a plain object from a BlockHeader message. Also converts values to other types if specified.
                 * @param message BlockHeader
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.BlockHeader, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this BlockHeader to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Transaction. */
            interface ITransaction {

                /** Transaction timestamp */
                timestamp?: (Long|null);

                /** Transaction expiration */
                expiration?: (Long|null);

                /** Transaction blockHeader */
                blockHeader?: (TW.Tron.Proto.IBlockHeader|null);

                /** Transaction feeLimit */
                feeLimit?: (Long|null);

                /** Transaction transfer */
                transfer?: (TW.Tron.Proto.ITransferContract|null);

                /** Transaction transferAsset */
                transferAsset?: (TW.Tron.Proto.ITransferAssetContract|null);

                /** Transaction freezeBalance */
                freezeBalance?: (TW.Tron.Proto.IFreezeBalanceContract|null);

                /** Transaction unfreezeBalance */
                unfreezeBalance?: (TW.Tron.Proto.IUnfreezeBalanceContract|null);

                /** Transaction unfreezeAsset */
                unfreezeAsset?: (TW.Tron.Proto.IUnfreezeAssetContract|null);

                /** Transaction withdrawBalance */
                withdrawBalance?: (TW.Tron.Proto.IWithdrawBalanceContract|null);

                /** Transaction voteAsset */
                voteAsset?: (TW.Tron.Proto.IVoteAssetContract|null);

                /** Transaction voteWitness */
                voteWitness?: (TW.Tron.Proto.IVoteWitnessContract|null);

                /** Transaction triggerSmartContract */
                triggerSmartContract?: (TW.Tron.Proto.ITriggerSmartContract|null);

                /** Transaction transferTrc20Contract */
                transferTrc20Contract?: (TW.Tron.Proto.ITransferTRC20Contract|null);
            }

            /** Represents a Transaction. */
            class Transaction implements ITransaction {

                /**
                 * Constructs a new Transaction.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.ITransaction);

                /** Transaction timestamp. */
                public timestamp: Long;

                /** Transaction expiration. */
                public expiration: Long;

                /** Transaction blockHeader. */
                public blockHeader?: (TW.Tron.Proto.IBlockHeader|null);

                /** Transaction feeLimit. */
                public feeLimit: Long;

                /** Transaction transfer. */
                public transfer?: (TW.Tron.Proto.ITransferContract|null);

                /** Transaction transferAsset. */
                public transferAsset?: (TW.Tron.Proto.ITransferAssetContract|null);

                /** Transaction freezeBalance. */
                public freezeBalance?: (TW.Tron.Proto.IFreezeBalanceContract|null);

                /** Transaction unfreezeBalance. */
                public unfreezeBalance?: (TW.Tron.Proto.IUnfreezeBalanceContract|null);

                /** Transaction unfreezeAsset. */
                public unfreezeAsset?: (TW.Tron.Proto.IUnfreezeAssetContract|null);

                /** Transaction withdrawBalance. */
                public withdrawBalance?: (TW.Tron.Proto.IWithdrawBalanceContract|null);

                /** Transaction voteAsset. */
                public voteAsset?: (TW.Tron.Proto.IVoteAssetContract|null);

                /** Transaction voteWitness. */
                public voteWitness?: (TW.Tron.Proto.IVoteWitnessContract|null);

                /** Transaction triggerSmartContract. */
                public triggerSmartContract?: (TW.Tron.Proto.ITriggerSmartContract|null);

                /** Transaction transferTrc20Contract. */
                public transferTrc20Contract?: (TW.Tron.Proto.ITransferTRC20Contract|null);

                /** Transaction contractOneof. */
                public contractOneof?: ("transfer"|"transferAsset"|"freezeBalance"|"unfreezeBalance"|"unfreezeAsset"|"withdrawBalance"|"voteAsset"|"voteWitness"|"triggerSmartContract"|"transferTrc20Contract");

                /**
                 * Creates a new Transaction instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Transaction instance
                 */
                public static create(properties?: TW.Tron.Proto.ITransaction): TW.Tron.Proto.Transaction;

                /**
                 * Encodes the specified Transaction message. Does not implicitly {@link TW.Tron.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Transaction message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.Transaction;

                /**
                 * Verifies a Transaction message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Transaction message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Transaction
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.Transaction;

                /**
                 * Creates a plain object from a Transaction message. Also converts values to other types if specified.
                 * @param message Transaction
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.Transaction, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Transaction to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput transaction */
                transaction?: (TW.Tron.Proto.ITransaction|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.ISigningInput);

                /** SigningInput transaction. */
                public transaction?: (TW.Tron.Proto.ITransaction|null);

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Tron.Proto.ISigningInput): TW.Tron.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Tron.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput id */
                id?: (Uint8Array|null);

                /** SigningOutput signature */
                signature?: (Uint8Array|null);

                /** SigningOutput refBlockBytes */
                refBlockBytes?: (Uint8Array|null);

                /** SigningOutput refBlockHash */
                refBlockHash?: (Uint8Array|null);

                /** SigningOutput json */
                json?: (string|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tron.Proto.ISigningOutput);

                /** SigningOutput id. */
                public id: Uint8Array;

                /** SigningOutput signature. */
                public signature: Uint8Array;

                /** SigningOutput refBlockBytes. */
                public refBlockBytes: Uint8Array;

                /** SigningOutput refBlockHash. */
                public refBlockHash: Uint8Array;

                /** SigningOutput json. */
                public json: string;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Tron.Proto.ISigningOutput): TW.Tron.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Tron.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tron.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tron.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tron.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tron.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace VeChain. */
    namespace VeChain {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a Clause. */
            interface IClause {

                /** Recipient address. */
                to?: (string|null);

                /** Transaction amount. */
                value?: (Uint8Array|null);

                /** Payload data. */
                data?: (Uint8Array|null);
            }

            /** Represents a Clause. */
            class Clause implements IClause {

                /**
                 * Constructs a new Clause.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.VeChain.Proto.IClause);

                /** Recipient address. */
                public to: string;

                /** Transaction amount. */
                public value: Uint8Array;

                /** Payload data. */
                public data: Uint8Array;

                /**
                 * Creates a new Clause instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Clause instance
                 */
                public static create(properties?: TW.VeChain.Proto.IClause): TW.VeChain.Proto.Clause;

                /**
                 * Encodes the specified Clause message. Does not implicitly {@link TW.VeChain.Proto.Clause.verify|verify} messages.
                 * @param message Clause message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.VeChain.Proto.IClause, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Clause message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Clause
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.VeChain.Proto.Clause;

                /**
                 * Verifies a Clause message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Clause message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Clause
                 */
                public static fromObject(object: { [k: string]: any }): TW.VeChain.Proto.Clause;

                /**
                 * Creates a plain object from a Clause message. Also converts values to other types if specified.
                 * @param message Clause
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.VeChain.Proto.Clause, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Clause to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** Last byte of the genesis block ID which is used to identify a blockchain to prevent the cross-chain replay attack. */
                chainTag?: (number|null);

                /** Reference to a specific block. */
                blockRef?: (Long|null);

                /** How long, in terms of the number of blocks, the transaction will be allowed to be mined in VeChainThor. */
                expiration?: (number|null);

                /** by the transaction sender. */
                clauses?: (TW.VeChain.Proto.IClause[]|null);

                /** Coefficient used to calculate the gas price for the transaction. */
                gasPriceCoef?: (number|null);

                /** Maximum amount of gas allowed to pay for the transaction. */
                gas?: (Long|null);

                /** ID of the transaction on which the current transaction depends. */
                dependsOn?: (Uint8Array|null);

                /** Number set by user. */
                nonce?: (Long|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.VeChain.Proto.ISigningInput);

                /** Last byte of the genesis block ID which is used to identify a blockchain to prevent the cross-chain replay attack. */
                public chainTag: number;

                /** Reference to a specific block. */
                public blockRef: Long;

                /** How long, in terms of the number of blocks, the transaction will be allowed to be mined in VeChainThor. */
                public expiration: number;

                /** by the transaction sender. */
                public clauses: TW.VeChain.Proto.IClause[];

                /** Coefficient used to calculate the gas price for the transaction. */
                public gasPriceCoef: number;

                /** Maximum amount of gas allowed to pay for the transaction. */
                public gas: Long;

                /** ID of the transaction on which the current transaction depends. */
                public dependsOn: Uint8Array;

                /** Number set by user. */
                public nonce: Long;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.VeChain.Proto.ISigningInput): TW.VeChain.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.VeChain.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.VeChain.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.VeChain.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.VeChain.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.VeChain.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput encoded */
                encoded?: (Uint8Array|null);

                /** SigningOutput signature */
                signature?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.VeChain.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /** SigningOutput signature. */
                public signature: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.VeChain.Proto.ISigningOutput): TW.VeChain.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.VeChain.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.VeChain.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.VeChain.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.VeChain.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.VeChain.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Waves. */
    namespace Waves {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a TransferMessage. */
            interface ITransferMessage {

                /** TransferMessage amount */
                amount?: (Long|null);

                /** TransferMessage asset */
                asset?: (string|null);

                /** TransferMessage fee */
                fee?: (Long|null);

                /** TransferMessage feeAsset */
                feeAsset?: (string|null);

                /** TransferMessage to */
                to?: (string|null);

                /** TransferMessage attachment */
                attachment?: (Uint8Array|null);
            }

            /** Represents a TransferMessage. */
            class TransferMessage implements ITransferMessage {

                /**
                 * Constructs a new TransferMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Waves.Proto.ITransferMessage);

                /** TransferMessage amount. */
                public amount: Long;

                /** TransferMessage asset. */
                public asset: string;

                /** TransferMessage fee. */
                public fee: Long;

                /** TransferMessage feeAsset. */
                public feeAsset: string;

                /** TransferMessage to. */
                public to: string;

                /** TransferMessage attachment. */
                public attachment: Uint8Array;

                /**
                 * Creates a new TransferMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransferMessage instance
                 */
                public static create(properties?: TW.Waves.Proto.ITransferMessage): TW.Waves.Proto.TransferMessage;

                /**
                 * Encodes the specified TransferMessage message. Does not implicitly {@link TW.Waves.Proto.TransferMessage.verify|verify} messages.
                 * @param message TransferMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Waves.Proto.ITransferMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransferMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransferMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Waves.Proto.TransferMessage;

                /**
                 * Verifies a TransferMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransferMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransferMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Waves.Proto.TransferMessage;

                /**
                 * Creates a plain object from a TransferMessage message. Also converts values to other types if specified.
                 * @param message TransferMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Waves.Proto.TransferMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransferMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a LeaseMessage. */
            interface ILeaseMessage {

                /** LeaseMessage amount */
                amount?: (Long|null);

                /** LeaseMessage to */
                to?: (string|null);

                /** LeaseMessage fee */
                fee?: (Long|null);
            }

            /** Represents a LeaseMessage. */
            class LeaseMessage implements ILeaseMessage {

                /**
                 * Constructs a new LeaseMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Waves.Proto.ILeaseMessage);

                /** LeaseMessage amount. */
                public amount: Long;

                /** LeaseMessage to. */
                public to: string;

                /** LeaseMessage fee. */
                public fee: Long;

                /**
                 * Creates a new LeaseMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns LeaseMessage instance
                 */
                public static create(properties?: TW.Waves.Proto.ILeaseMessage): TW.Waves.Proto.LeaseMessage;

                /**
                 * Encodes the specified LeaseMessage message. Does not implicitly {@link TW.Waves.Proto.LeaseMessage.verify|verify} messages.
                 * @param message LeaseMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Waves.Proto.ILeaseMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a LeaseMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns LeaseMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Waves.Proto.LeaseMessage;

                /**
                 * Verifies a LeaseMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a LeaseMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns LeaseMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Waves.Proto.LeaseMessage;

                /**
                 * Creates a plain object from a LeaseMessage message. Also converts values to other types if specified.
                 * @param message LeaseMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Waves.Proto.LeaseMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this LeaseMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a CancelLeaseMessage. */
            interface ICancelLeaseMessage {

                /** CancelLeaseMessage leaseId */
                leaseId?: (string|null);

                /** CancelLeaseMessage fee */
                fee?: (Long|null);
            }

            /** Represents a CancelLeaseMessage. */
            class CancelLeaseMessage implements ICancelLeaseMessage {

                /**
                 * Constructs a new CancelLeaseMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Waves.Proto.ICancelLeaseMessage);

                /** CancelLeaseMessage leaseId. */
                public leaseId: string;

                /** CancelLeaseMessage fee. */
                public fee: Long;

                /**
                 * Creates a new CancelLeaseMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns CancelLeaseMessage instance
                 */
                public static create(properties?: TW.Waves.Proto.ICancelLeaseMessage): TW.Waves.Proto.CancelLeaseMessage;

                /**
                 * Encodes the specified CancelLeaseMessage message. Does not implicitly {@link TW.Waves.Proto.CancelLeaseMessage.verify|verify} messages.
                 * @param message CancelLeaseMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Waves.Proto.ICancelLeaseMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a CancelLeaseMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns CancelLeaseMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Waves.Proto.CancelLeaseMessage;

                /**
                 * Verifies a CancelLeaseMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a CancelLeaseMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns CancelLeaseMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Waves.Proto.CancelLeaseMessage;

                /**
                 * Creates a plain object from a CancelLeaseMessage message. Also converts values to other types if specified.
                 * @param message CancelLeaseMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Waves.Proto.CancelLeaseMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this CancelLeaseMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput timestamp */
                timestamp?: (Long|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput transferMessage */
                transferMessage?: (TW.Waves.Proto.ITransferMessage|null);

                /** SigningInput leaseMessage */
                leaseMessage?: (TW.Waves.Proto.ILeaseMessage|null);

                /** SigningInput cancelLeaseMessage */
                cancelLeaseMessage?: (TW.Waves.Proto.ICancelLeaseMessage|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Waves.Proto.ISigningInput);

                /** SigningInput timestamp. */
                public timestamp: Long;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput transferMessage. */
                public transferMessage?: (TW.Waves.Proto.ITransferMessage|null);

                /** SigningInput leaseMessage. */
                public leaseMessage?: (TW.Waves.Proto.ILeaseMessage|null);

                /** SigningInput cancelLeaseMessage. */
                public cancelLeaseMessage?: (TW.Waves.Proto.ICancelLeaseMessage|null);

                /** SigningInput messageOneof. */
                public messageOneof?: ("transferMessage"|"leaseMessage"|"cancelLeaseMessage");

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Waves.Proto.ISigningInput): TW.Waves.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Waves.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Waves.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Waves.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Waves.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Waves.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput signature */
                signature?: (Uint8Array|null);

                /** SigningOutput json */
                json?: (string|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Waves.Proto.ISigningOutput);

                /** SigningOutput signature. */
                public signature: Uint8Array;

                /** SigningOutput json. */
                public json: string;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Waves.Proto.ISigningOutput): TW.Waves.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Waves.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Waves.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Waves.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Waves.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Waves.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Zilliqa. */
    namespace Zilliqa {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a Transaction. */
            interface ITransaction {

                /** Transaction transfer */
                transfer?: (TW.Zilliqa.Proto.Transaction.ITransfer|null);

                /** Transaction rawTransaction */
                rawTransaction?: (TW.Zilliqa.Proto.Transaction.IRaw|null);
            }

            /** Represents a Transaction. */
            class Transaction implements ITransaction {

                /**
                 * Constructs a new Transaction.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Zilliqa.Proto.ITransaction);

                /** Transaction transfer. */
                public transfer?: (TW.Zilliqa.Proto.Transaction.ITransfer|null);

                /** Transaction rawTransaction. */
                public rawTransaction?: (TW.Zilliqa.Proto.Transaction.IRaw|null);

                /** Transaction messageOneof. */
                public messageOneof?: ("transfer"|"rawTransaction");

                /**
                 * Creates a new Transaction instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Transaction instance
                 */
                public static create(properties?: TW.Zilliqa.Proto.ITransaction): TW.Zilliqa.Proto.Transaction;

                /**
                 * Encodes the specified Transaction message. Does not implicitly {@link TW.Zilliqa.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Zilliqa.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Transaction message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Zilliqa.Proto.Transaction;

                /**
                 * Verifies a Transaction message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a Transaction message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Transaction
                 */
                public static fromObject(object: { [k: string]: any }): TW.Zilliqa.Proto.Transaction;

                /**
                 * Creates a plain object from a Transaction message. Also converts values to other types if specified.
                 * @param message Transaction
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Zilliqa.Proto.Transaction, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Transaction to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            namespace Transaction {

                /** Properties of a Transfer. */
                interface ITransfer {

                    /** Transfer amount */
                    amount?: (Uint8Array|null);
                }

                /** Represents a Transfer. */
                class Transfer implements ITransfer {

                    /**
                     * Constructs a new Transfer.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Zilliqa.Proto.Transaction.ITransfer);

                    /** Transfer amount. */
                    public amount: Uint8Array;

                    /**
                     * Creates a new Transfer instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Transfer instance
                     */
                    public static create(properties?: TW.Zilliqa.Proto.Transaction.ITransfer): TW.Zilliqa.Proto.Transaction.Transfer;

                    /**
                     * Encodes the specified Transfer message. Does not implicitly {@link TW.Zilliqa.Proto.Transaction.Transfer.verify|verify} messages.
                     * @param message Transfer message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Zilliqa.Proto.Transaction.ITransfer, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Transfer message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Transfer
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Zilliqa.Proto.Transaction.Transfer;

                    /**
                     * Verifies a Transfer message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Transfer message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Transfer
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Zilliqa.Proto.Transaction.Transfer;

                    /**
                     * Creates a plain object from a Transfer message. Also converts values to other types if specified.
                     * @param message Transfer
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Zilliqa.Proto.Transaction.Transfer, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Transfer to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }

                /** Properties of a Raw. */
                interface IRaw {

                    /** Raw amount */
                    amount?: (Uint8Array|null);

                    /** Raw code */
                    code?: (Uint8Array|null);

                    /** Raw data */
                    data?: (Uint8Array|null);
                }

                /** Represents a Raw. */
                class Raw implements IRaw {

                    /**
                     * Constructs a new Raw.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Zilliqa.Proto.Transaction.IRaw);

                    /** Raw amount. */
                    public amount: Uint8Array;

                    /** Raw code. */
                    public code: Uint8Array;

                    /** Raw data. */
                    public data: Uint8Array;

                    /**
                     * Creates a new Raw instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Raw instance
                     */
                    public static create(properties?: TW.Zilliqa.Proto.Transaction.IRaw): TW.Zilliqa.Proto.Transaction.Raw;

                    /**
                     * Encodes the specified Raw message. Does not implicitly {@link TW.Zilliqa.Proto.Transaction.Raw.verify|verify} messages.
                     * @param message Raw message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Zilliqa.Proto.Transaction.IRaw, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes a Raw message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Raw
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Zilliqa.Proto.Transaction.Raw;

                    /**
                     * Verifies a Raw message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates a Raw message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Raw
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Zilliqa.Proto.Transaction.Raw;

                    /**
                     * Creates a plain object from a Raw message. Also converts values to other types if specified.
                     * @param message Raw
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Zilliqa.Proto.Transaction.Raw, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Raw to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput version */
                version?: (number|null);

                /** SigningInput nonce */
                nonce?: (Long|null);

                /** SigningInput to */
                to?: (string|null);

                /** SigningInput gasPrice */
                gasPrice?: (Uint8Array|null);

                /** SigningInput gasLimit */
                gasLimit?: (Long|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput transaction */
                transaction?: (TW.Zilliqa.Proto.ITransaction|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Zilliqa.Proto.ISigningInput);

                /** SigningInput version. */
                public version: number;

                /** SigningInput nonce. */
                public nonce: Long;

                /** SigningInput to. */
                public to: string;

                /** SigningInput gasPrice. */
                public gasPrice: Uint8Array;

                /** SigningInput gasLimit. */
                public gasLimit: Long;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput transaction. */
                public transaction?: (TW.Zilliqa.Proto.ITransaction|null);

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Zilliqa.Proto.ISigningInput): TW.Zilliqa.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Zilliqa.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Zilliqa.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Zilliqa.Proto.SigningInput;

                /**
                 * Verifies a SigningInput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningInput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningInput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Zilliqa.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Zilliqa.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput signature */
                signature?: (Uint8Array|null);

                /** SigningOutput json */
                json?: (string|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Zilliqa.Proto.ISigningOutput);

                /** SigningOutput signature. */
                public signature: Uint8Array;

                /** SigningOutput json. */
                public json: string;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Zilliqa.Proto.ISigningOutput): TW.Zilliqa.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Zilliqa.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Zilliqa.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Zilliqa.Proto.SigningOutput;

                /**
                 * Verifies a SigningOutput message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SigningOutput message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SigningOutput
                 */
                public static fromObject(object: { [k: string]: any }): TW.Zilliqa.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Zilliqa.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }
}
