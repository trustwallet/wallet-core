import * as $protobuf from "protobufjs";
/** Namespace TW. */
export namespace TW {

    /** Namespace ARK. */
    namespace ARK {

        /** Namespace Proto. */
        namespace Proto {

            /** TransactionType enum. */
            enum TransactionType {
                Transfer = 0
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput type */
                type?: (TW.ARK.Proto.TransactionType|null);

                /** SigningInput amount */
                amount?: (Long|null);

                /** SigningInput fee */
                fee?: (Long|null);

                /** SigningInput toAddress */
                toAddress?: (string|null);

                /** SigningInput timestamp */
                timestamp?: (number|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.ARK.Proto.ISigningInput);

                /** SigningInput type. */
                public type: TW.ARK.Proto.TransactionType;

                /** SigningInput amount. */
                public amount: Long;

                /** SigningInput fee. */
                public fee: Long;

                /** SigningInput toAddress. */
                public toAddress: string;

                /** SigningInput timestamp. */
                public timestamp: number;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.ARK.Proto.ISigningInput): TW.ARK.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.ARK.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.ARK.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.ARK.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.ARK.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.ARK.Proto.SigningInput;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.ARK.Proto.SigningInput;

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
                public static fromObject(object: { [k: string]: any }): TW.ARK.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.ARK.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

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
                constructor(properties?: TW.ARK.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: string;

                /** SigningOutput signature. */
                public signature: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.ARK.Proto.ISigningOutput): TW.ARK.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.ARK.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.ARK.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.ARK.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.ARK.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.ARK.Proto.SigningOutput;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.ARK.Proto.SigningOutput;

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
                public static fromObject(object: { [k: string]: any }): TW.ARK.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.ARK.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Aeternity.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Aeternity.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Aeternity.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Aeternity.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Aeternity.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Aeternity.Proto.SigningOutput;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Aion.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Aion.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Aion.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Aion.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Aion.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Aion.Proto.SigningOutput;

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

    /** Namespace Any. */
    namespace Any {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput coinType */
                coinType?: (number|null);

                /** SigningInput transaction */
                transaction?: (string|null);

                /** SigningInput privateKey */
                privateKey?: (string|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Any.Proto.ISigningInput);

                /** SigningInput coinType. */
                public coinType: number;

                /** SigningInput transaction. */
                public transaction: string;

                /** SigningInput privateKey. */
                public privateKey: string;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Any.Proto.ISigningInput): TW.Any.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Any.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Any.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Any.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Any.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Any.Proto.SigningInput;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Any.Proto.SigningInput;

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
                public static fromObject(object: { [k: string]: any }): TW.Any.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Any.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput error */
                error?: (TW.Any.Proto.SigningOutput.IError|null);

                /** SigningOutput success */
                success?: (boolean|null);

                /** SigningOutput output */
                output?: (string|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Any.Proto.ISigningOutput);

                /** SigningOutput error. */
                public error?: (TW.Any.Proto.SigningOutput.IError|null);

                /** SigningOutput success. */
                public success: boolean;

                /** SigningOutput output. */
                public output: string;

                /** SigningOutput result. */
                public result?: ("error"|"success");

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Any.Proto.ISigningOutput): TW.Any.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Any.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Any.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Any.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Any.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Any.Proto.SigningOutput;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Any.Proto.SigningOutput;

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
                public static fromObject(object: { [k: string]: any }): TW.Any.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Any.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            namespace SigningOutput {

                /** Properties of an Error. */
                interface IError {

                    /** Error code */
                    code?: (number|null);

                    /** Error description */
                    description?: (string|null);
                }

                /** Represents an Error. */
                class Error implements IError {

                    /**
                     * Constructs a new Error.
                     * @param [properties] Properties to set
                     */
                    constructor(properties?: TW.Any.Proto.SigningOutput.IError);

                    /** Error code. */
                    public code: number;

                    /** Error description. */
                    public description: string;

                    /**
                     * Creates a new Error instance using the specified properties.
                     * @param [properties] Properties to set
                     * @returns Error instance
                     */
                    public static create(properties?: TW.Any.Proto.SigningOutput.IError): TW.Any.Proto.SigningOutput.Error;

                    /**
                     * Encodes the specified Error message. Does not implicitly {@link TW.Any.Proto.SigningOutput.Error.verify|verify} messages.
                     * @param message Error message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encode(message: TW.Any.Proto.SigningOutput.IError, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Encodes the specified Error message, length delimited. Does not implicitly {@link TW.Any.Proto.SigningOutput.Error.verify|verify} messages.
                     * @param message Error message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encodeDelimited(message: TW.Any.Proto.SigningOutput.IError, writer?: $protobuf.Writer): $protobuf.Writer;

                    /**
                     * Decodes an Error message from the specified reader or buffer.
                     * @param reader Reader or buffer to decode from
                     * @param [length] Message length if known beforehand
                     * @returns Error
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Any.Proto.SigningOutput.Error;

                    /**
                     * Decodes an Error message from the specified reader or buffer, length delimited.
                     * @param reader Reader or buffer to decode from
                     * @returns Error
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Any.Proto.SigningOutput.Error;

                    /**
                     * Verifies an Error message.
                     * @param message Plain object to verify
                     * @returns `null` if valid, otherwise the reason why it is not
                     */
                    public static verify(message: { [k: string]: any }): (string|null);

                    /**
                     * Creates an Error message from a plain object. Also converts values to their respective internal types.
                     * @param object Plain object
                     * @returns Error
                     */
                    public static fromObject(object: { [k: string]: any }): TW.Any.Proto.SigningOutput.Error;

                    /**
                     * Creates a plain object from an Error message. Also converts values to other types if specified.
                     * @param message Error
                     * @param [options] Conversion options
                     * @returns Plain object
                     */
                    public static toObject(message: TW.Any.Proto.SigningOutput.Error, options?: $protobuf.IConversionOptions): { [k: string]: any };

                    /**
                     * Converts this Error to JSON.
                     * @returns JSON object
                     */
                    public toJSON(): { [k: string]: any };
                }
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
                 * Encodes the specified Transaction message, length delimited. Does not implicitly {@link TW.Binance.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Binance.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a Transaction message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Binance.Proto.Transaction;

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
                 * Encodes the specified Signature message, length delimited. Does not implicitly {@link TW.Binance.Proto.Signature.verify|verify} messages.
                 * @param message Signature message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Binance.Proto.ISignature, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a Signature message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Signature
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Binance.Proto.Signature;

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
                     * Encodes the specified PubKey message, length delimited. Does not implicitly {@link TW.Binance.Proto.Signature.PubKey.verify|verify} messages.
                     * @param message PubKey message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encodeDelimited(message: TW.Binance.Proto.Signature.IPubKey, writer?: $protobuf.Writer): $protobuf.Writer;

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
                     * Decodes a PubKey message from the specified reader or buffer, length delimited.
                     * @param reader Reader or buffer to decode from
                     * @returns PubKey
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Binance.Proto.Signature.PubKey;

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
                 * Encodes the specified TradeOrder message, length delimited. Does not implicitly {@link TW.Binance.Proto.TradeOrder.verify|verify} messages.
                 * @param message TradeOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Binance.Proto.ITradeOrder, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a TradeOrder message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TradeOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Binance.Proto.TradeOrder;

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
                 * Encodes the specified CancelTradeOrder message, length delimited. Does not implicitly {@link TW.Binance.Proto.CancelTradeOrder.verify|verify} messages.
                 * @param message CancelTradeOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Binance.Proto.ICancelTradeOrder, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a CancelTradeOrder message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns CancelTradeOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Binance.Proto.CancelTradeOrder;

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
                 * Encodes the specified SendOrder message, length delimited. Does not implicitly {@link TW.Binance.Proto.SendOrder.verify|verify} messages.
                 * @param message SendOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Binance.Proto.ISendOrder, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SendOrder message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SendOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Binance.Proto.SendOrder;

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
                     * Encodes the specified Token message, length delimited. Does not implicitly {@link TW.Binance.Proto.SendOrder.Token.verify|verify} messages.
                     * @param message Token message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encodeDelimited(message: TW.Binance.Proto.SendOrder.IToken, writer?: $protobuf.Writer): $protobuf.Writer;

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
                     * Decodes a Token message from the specified reader or buffer, length delimited.
                     * @param reader Reader or buffer to decode from
                     * @returns Token
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Binance.Proto.SendOrder.Token;

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
                     * Encodes the specified Input message, length delimited. Does not implicitly {@link TW.Binance.Proto.SendOrder.Input.verify|verify} messages.
                     * @param message Input message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encodeDelimited(message: TW.Binance.Proto.SendOrder.IInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                     * Decodes an Input message from the specified reader or buffer, length delimited.
                     * @param reader Reader or buffer to decode from
                     * @returns Input
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Binance.Proto.SendOrder.Input;

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
                     * Encodes the specified Output message, length delimited. Does not implicitly {@link TW.Binance.Proto.SendOrder.Output.verify|verify} messages.
                     * @param message Output message or plain object to encode
                     * @param [writer] Writer to encode to
                     * @returns Writer
                     */
                    public static encodeDelimited(message: TW.Binance.Proto.SendOrder.IOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                     * Decodes an Output message from the specified reader or buffer, length delimited.
                     * @param reader Reader or buffer to decode from
                     * @returns Output
                     * @throws {Error} If the payload is not a reader or valid buffer
                     * @throws {$protobuf.util.ProtocolError} If required fields are missing
                     */
                    public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Binance.Proto.SendOrder.Output;

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
                 * Encodes the specified TokenFreezeOrder message, length delimited. Does not implicitly {@link TW.Binance.Proto.TokenFreezeOrder.verify|verify} messages.
                 * @param message TokenFreezeOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Binance.Proto.ITokenFreezeOrder, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a TokenFreezeOrder message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TokenFreezeOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Binance.Proto.TokenFreezeOrder;

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
                 * Encodes the specified TokenUnfreezeOrder message, length delimited. Does not implicitly {@link TW.Binance.Proto.TokenUnfreezeOrder.verify|verify} messages.
                 * @param message TokenUnfreezeOrder message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Binance.Proto.ITokenUnfreezeOrder, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a TokenUnfreezeOrder message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TokenUnfreezeOrder
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Binance.Proto.TokenUnfreezeOrder;

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

                /** SigningInput orderOneof. */
                public orderOneof?: ("tradeOrder"|"cancelTradeOrder"|"sendOrder"|"freezeOrder"|"unfreezeOrder");

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Binance.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Binance.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Binance.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Binance.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Binance.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Binance.Proto.SigningOutput;

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
                 * Encodes the specified Transaction message, length delimited. Does not implicitly {@link TW.Bitcoin.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Bitcoin.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a Transaction message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Bitcoin.Proto.Transaction;

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
                 * Encodes the specified TransactionInput message, length delimited. Does not implicitly {@link TW.Bitcoin.Proto.TransactionInput.verify|verify} messages.
                 * @param message TransactionInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Bitcoin.Proto.ITransactionInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a TransactionInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TransactionInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Bitcoin.Proto.TransactionInput;

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
                 * Encodes the specified OutPoint message, length delimited. Does not implicitly {@link TW.Bitcoin.Proto.OutPoint.verify|verify} messages.
                 * @param message OutPoint message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Bitcoin.Proto.IOutPoint, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes an OutPoint message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns OutPoint
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Bitcoin.Proto.OutPoint;

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
                 * Encodes the specified TransactionOutput message, length delimited. Does not implicitly {@link TW.Bitcoin.Proto.TransactionOutput.verify|verify} messages.
                 * @param message TransactionOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Bitcoin.Proto.ITransactionOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a TransactionOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TransactionOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Bitcoin.Proto.TransactionOutput;

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
                 * Encodes the specified UnspentTransaction message, length delimited. Does not implicitly {@link TW.Bitcoin.Proto.UnspentTransaction.verify|verify} messages.
                 * @param message UnspentTransaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Bitcoin.Proto.IUnspentTransaction, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes an UnspentTransaction message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns UnspentTransaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Bitcoin.Proto.UnspentTransaction;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Bitcoin.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Bitcoin.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Bitcoin.Proto.SigningInput;

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
                 * Encodes the specified TransactionPlan message, length delimited. Does not implicitly {@link TW.Bitcoin.Proto.TransactionPlan.verify|verify} messages.
                 * @param message TransactionPlan message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Bitcoin.Proto.ITransactionPlan, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a TransactionPlan message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TransactionPlan
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Bitcoin.Proto.TransactionPlan;

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

                /** SigningOutput fee */
                fee?: (Long|null);

                /** SigningOutput maxAmount */
                maxAmount?: (Long|null);

                /** SigningOutput transactionId */
                transactionId?: (string|null);
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

                /** SigningOutput fee. */
                public fee: Long;

                /** SigningOutput maxAmount. */
                public maxAmount: Long;

                /** SigningOutput transactionId. */
                public transactionId: string;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Bitcoin.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Bitcoin.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Bitcoin.Proto.SigningOutput;

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

    /** Namespace Bravo. */
    namespace Bravo {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput chainId */
                chainId?: (Uint8Array|null);

                /** SigningInput sender */
                sender?: (string|null);

                /** SigningInput recipient */
                recipient?: (string|null);

                /** SigningInput amount */
                amount?: (number|null);

                /** SigningInput testnet */
                testnet?: (boolean|null);

                /** SigningInput memo */
                memo?: (string|null);

                /** SigningInput referenceBlockId */
                referenceBlockId?: (Uint8Array|null);

                /** SigningInput referenceBlockTime */
                referenceBlockTime?: (number|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Bravo.Proto.ISigningInput);

                /** SigningInput chainId. */
                public chainId: Uint8Array;

                /** SigningInput sender. */
                public sender: string;

                /** SigningInput recipient. */
                public recipient: string;

                /** SigningInput amount. */
                public amount: number;

                /** SigningInput testnet. */
                public testnet: boolean;

                /** SigningInput memo. */
                public memo: string;

                /** SigningInput referenceBlockId. */
                public referenceBlockId: Uint8Array;

                /** SigningInput referenceBlockTime. */
                public referenceBlockTime: number;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Bravo.Proto.ISigningInput): TW.Bravo.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Bravo.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Bravo.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Bravo.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Bravo.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Bravo.Proto.SigningInput;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Bravo.Proto.SigningInput;

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
                public static fromObject(object: { [k: string]: any }): TW.Bravo.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Bravo.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

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
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Bravo.Proto.ISigningOutput);

                /** SigningOutput jsonEncoded. */
                public jsonEncoded: string;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Bravo.Proto.ISigningOutput): TW.Bravo.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Bravo.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Bravo.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Bravo.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Bravo.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Bravo.Proto.SigningOutput;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Bravo.Proto.SigningOutput;

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
                public static fromObject(object: { [k: string]: any }): TW.Bravo.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Bravo.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Coinex. */
    namespace Coinex {

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
                constructor(properties?: TW.Coinex.Proto.IAmount);

                /** Amount denom. */
                public denom: string;

                /** Amount amount. */
                public amount: Long;

                /**
                 * Creates a new Amount instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Amount instance
                 */
                public static create(properties?: TW.Coinex.Proto.IAmount): TW.Coinex.Proto.Amount;

                /**
                 * Encodes the specified Amount message. Does not implicitly {@link TW.Coinex.Proto.Amount.verify|verify} messages.
                 * @param message Amount message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Coinex.Proto.IAmount, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified Amount message, length delimited. Does not implicitly {@link TW.Coinex.Proto.Amount.verify|verify} messages.
                 * @param message Amount message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Coinex.Proto.IAmount, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an Amount message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Amount
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Coinex.Proto.Amount;

                /**
                 * Decodes an Amount message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Amount
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Coinex.Proto.Amount;

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
                public static fromObject(object: { [k: string]: any }): TW.Coinex.Proto.Amount;

                /**
                 * Creates a plain object from an Amount message. Also converts values to other types if specified.
                 * @param message Amount
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Coinex.Proto.Amount, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Amount to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Fee. */
            interface IFee {

                /** Fee amounts */
                amounts?: (TW.Coinex.Proto.IAmount[]|null);

                /** Fee gas */
                gas?: (Long|null);
            }

            /** Represents a Fee. */
            class Fee implements IFee {

                /**
                 * Constructs a new Fee.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Coinex.Proto.IFee);

                /** Fee amounts. */
                public amounts: TW.Coinex.Proto.IAmount[];

                /** Fee gas. */
                public gas: Long;

                /**
                 * Creates a new Fee instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Fee instance
                 */
                public static create(properties?: TW.Coinex.Proto.IFee): TW.Coinex.Proto.Fee;

                /**
                 * Encodes the specified Fee message. Does not implicitly {@link TW.Coinex.Proto.Fee.verify|verify} messages.
                 * @param message Fee message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Coinex.Proto.IFee, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified Fee message, length delimited. Does not implicitly {@link TW.Coinex.Proto.Fee.verify|verify} messages.
                 * @param message Fee message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Coinex.Proto.IFee, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Fee message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Fee
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Coinex.Proto.Fee;

                /**
                 * Decodes a Fee message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Fee
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Coinex.Proto.Fee;

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
                public static fromObject(object: { [k: string]: any }): TW.Coinex.Proto.Fee;

                /**
                 * Creates a plain object from a Fee message. Also converts values to other types if specified.
                 * @param message Fee
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Coinex.Proto.Fee, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Fee to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SendCoinsMessage. */
            interface ISendCoinsMessage {

                /** SendCoinsMessage fromAddress */
                fromAddress?: (string|null);

                /** SendCoinsMessage toAddress */
                toAddress?: (string|null);

                /** SendCoinsMessage amounts */
                amounts?: (TW.Coinex.Proto.IAmount[]|null);

                /** SendCoinsMessage unlockTime */
                unlockTime?: (Long|null);

                /** SendCoinsMessage typePrefix */
                typePrefix?: (string|null);
            }

            /** Represents a SendCoinsMessage. */
            class SendCoinsMessage implements ISendCoinsMessage {

                /**
                 * Constructs a new SendCoinsMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Coinex.Proto.ISendCoinsMessage);

                /** SendCoinsMessage fromAddress. */
                public fromAddress: string;

                /** SendCoinsMessage toAddress. */
                public toAddress: string;

                /** SendCoinsMessage amounts. */
                public amounts: TW.Coinex.Proto.IAmount[];

                /** SendCoinsMessage unlockTime. */
                public unlockTime: Long;

                /** SendCoinsMessage typePrefix. */
                public typePrefix: string;

                /**
                 * Creates a new SendCoinsMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SendCoinsMessage instance
                 */
                public static create(properties?: TW.Coinex.Proto.ISendCoinsMessage): TW.Coinex.Proto.SendCoinsMessage;

                /**
                 * Encodes the specified SendCoinsMessage message. Does not implicitly {@link TW.Coinex.Proto.SendCoinsMessage.verify|verify} messages.
                 * @param message SendCoinsMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Coinex.Proto.ISendCoinsMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SendCoinsMessage message, length delimited. Does not implicitly {@link TW.Coinex.Proto.SendCoinsMessage.verify|verify} messages.
                 * @param message SendCoinsMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Coinex.Proto.ISendCoinsMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SendCoinsMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SendCoinsMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Coinex.Proto.SendCoinsMessage;

                /**
                 * Decodes a SendCoinsMessage message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SendCoinsMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Coinex.Proto.SendCoinsMessage;

                /**
                 * Verifies a SendCoinsMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SendCoinsMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SendCoinsMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Coinex.Proto.SendCoinsMessage;

                /**
                 * Creates a plain object from a SendCoinsMessage message. Also converts values to other types if specified.
                 * @param message SendCoinsMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Coinex.Proto.SendCoinsMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SendCoinsMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a StakeMessage. */
            interface IStakeMessage {

                /** StakeMessage delegatorAddress */
                delegatorAddress?: (string|null);

                /** StakeMessage validatorAddress */
                validatorAddress?: (string|null);

                /** StakeMessage amount */
                amount?: (TW.Coinex.Proto.IAmount|null);

                /** StakeMessage typePrefix */
                typePrefix?: (string|null);
            }

            /** Represents a StakeMessage. */
            class StakeMessage implements IStakeMessage {

                /**
                 * Constructs a new StakeMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Coinex.Proto.IStakeMessage);

                /** StakeMessage delegatorAddress. */
                public delegatorAddress: string;

                /** StakeMessage validatorAddress. */
                public validatorAddress: string;

                /** StakeMessage amount. */
                public amount?: (TW.Coinex.Proto.IAmount|null);

                /** StakeMessage typePrefix. */
                public typePrefix: string;

                /**
                 * Creates a new StakeMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns StakeMessage instance
                 */
                public static create(properties?: TW.Coinex.Proto.IStakeMessage): TW.Coinex.Proto.StakeMessage;

                /**
                 * Encodes the specified StakeMessage message. Does not implicitly {@link TW.Coinex.Proto.StakeMessage.verify|verify} messages.
                 * @param message StakeMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Coinex.Proto.IStakeMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified StakeMessage message, length delimited. Does not implicitly {@link TW.Coinex.Proto.StakeMessage.verify|verify} messages.
                 * @param message StakeMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Coinex.Proto.IStakeMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a StakeMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns StakeMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Coinex.Proto.StakeMessage;

                /**
                 * Decodes a StakeMessage message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns StakeMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Coinex.Proto.StakeMessage;

                /**
                 * Verifies a StakeMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a StakeMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns StakeMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Coinex.Proto.StakeMessage;

                /**
                 * Creates a plain object from a StakeMessage message. Also converts values to other types if specified.
                 * @param message StakeMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Coinex.Proto.StakeMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this StakeMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Signature. */
            interface ISignature {

                /** Signature publicKey */
                publicKey?: (Uint8Array|null);

                /** Signature signature */
                signature?: (Uint8Array|null);
            }

            /** Represents a Signature. */
            class Signature implements ISignature {

                /**
                 * Constructs a new Signature.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Coinex.Proto.ISignature);

                /** Signature publicKey. */
                public publicKey: Uint8Array;

                /** Signature signature. */
                public signature: Uint8Array;

                /**
                 * Creates a new Signature instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Signature instance
                 */
                public static create(properties?: TW.Coinex.Proto.ISignature): TW.Coinex.Proto.Signature;

                /**
                 * Encodes the specified Signature message. Does not implicitly {@link TW.Coinex.Proto.Signature.verify|verify} messages.
                 * @param message Signature message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Coinex.Proto.ISignature, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified Signature message, length delimited. Does not implicitly {@link TW.Coinex.Proto.Signature.verify|verify} messages.
                 * @param message Signature message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Coinex.Proto.ISignature, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Signature message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Signature
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Coinex.Proto.Signature;

                /**
                 * Decodes a Signature message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Signature
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Coinex.Proto.Signature;

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
                public static fromObject(object: { [k: string]: any }): TW.Coinex.Proto.Signature;

                /**
                 * Creates a plain object from a Signature message. Also converts values to other types if specified.
                 * @param message Signature
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Coinex.Proto.Signature, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Signature to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a WithdrawStakeRewardMessage. */
            interface IWithdrawStakeRewardMessage {

                /** WithdrawStakeRewardMessage delegatorAddress */
                delegatorAddress?: (string|null);

                /** WithdrawStakeRewardMessage validatorAddress */
                validatorAddress?: (string|null);

                /** WithdrawStakeRewardMessage typePrefix */
                typePrefix?: (string|null);
            }

            /** Represents a WithdrawStakeRewardMessage. */
            class WithdrawStakeRewardMessage implements IWithdrawStakeRewardMessage {

                /**
                 * Constructs a new WithdrawStakeRewardMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Coinex.Proto.IWithdrawStakeRewardMessage);

                /** WithdrawStakeRewardMessage delegatorAddress. */
                public delegatorAddress: string;

                /** WithdrawStakeRewardMessage validatorAddress. */
                public validatorAddress: string;

                /** WithdrawStakeRewardMessage typePrefix. */
                public typePrefix: string;

                /**
                 * Creates a new WithdrawStakeRewardMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns WithdrawStakeRewardMessage instance
                 */
                public static create(properties?: TW.Coinex.Proto.IWithdrawStakeRewardMessage): TW.Coinex.Proto.WithdrawStakeRewardMessage;

                /**
                 * Encodes the specified WithdrawStakeRewardMessage message. Does not implicitly {@link TW.Coinex.Proto.WithdrawStakeRewardMessage.verify|verify} messages.
                 * @param message WithdrawStakeRewardMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Coinex.Proto.IWithdrawStakeRewardMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified WithdrawStakeRewardMessage message, length delimited. Does not implicitly {@link TW.Coinex.Proto.WithdrawStakeRewardMessage.verify|verify} messages.
                 * @param message WithdrawStakeRewardMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Coinex.Proto.IWithdrawStakeRewardMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a WithdrawStakeRewardMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns WithdrawStakeRewardMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Coinex.Proto.WithdrawStakeRewardMessage;

                /**
                 * Decodes a WithdrawStakeRewardMessage message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns WithdrawStakeRewardMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Coinex.Proto.WithdrawStakeRewardMessage;

                /**
                 * Verifies a WithdrawStakeRewardMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a WithdrawStakeRewardMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns WithdrawStakeRewardMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Coinex.Proto.WithdrawStakeRewardMessage;

                /**
                 * Creates a plain object from a WithdrawStakeRewardMessage message. Also converts values to other types if specified.
                 * @param message WithdrawStakeRewardMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Coinex.Proto.WithdrawStakeRewardMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this WithdrawStakeRewardMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SetAliasMessage. */
            interface ISetAliasMessage {

                /** SetAliasMessage owner */
                owner?: (string|null);

                /** SetAliasMessage alias */
                alias?: (string|null);

                /** SetAliasMessage isAdd */
                isAdd?: (boolean|null);

                /** SetAliasMessage asDefault */
                asDefault?: (boolean|null);

                /** SetAliasMessage typePrefix */
                typePrefix?: (string|null);
            }

            /** Represents a SetAliasMessage. */
            class SetAliasMessage implements ISetAliasMessage {

                /**
                 * Constructs a new SetAliasMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Coinex.Proto.ISetAliasMessage);

                /** SetAliasMessage owner. */
                public owner: string;

                /** SetAliasMessage alias. */
                public alias: string;

                /** SetAliasMessage isAdd. */
                public isAdd: boolean;

                /** SetAliasMessage asDefault. */
                public asDefault: boolean;

                /** SetAliasMessage typePrefix. */
                public typePrefix: string;

                /**
                 * Creates a new SetAliasMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SetAliasMessage instance
                 */
                public static create(properties?: TW.Coinex.Proto.ISetAliasMessage): TW.Coinex.Proto.SetAliasMessage;

                /**
                 * Encodes the specified SetAliasMessage message. Does not implicitly {@link TW.Coinex.Proto.SetAliasMessage.verify|verify} messages.
                 * @param message SetAliasMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Coinex.Proto.ISetAliasMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SetAliasMessage message, length delimited. Does not implicitly {@link TW.Coinex.Proto.SetAliasMessage.verify|verify} messages.
                 * @param message SetAliasMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Coinex.Proto.ISetAliasMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SetAliasMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SetAliasMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Coinex.Proto.SetAliasMessage;

                /**
                 * Decodes a SetAliasMessage message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SetAliasMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Coinex.Proto.SetAliasMessage;

                /**
                 * Verifies a SetAliasMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SetAliasMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SetAliasMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Coinex.Proto.SetAliasMessage;

                /**
                 * Creates a plain object from a SetAliasMessage message. Also converts values to other types if specified.
                 * @param message SetAliasMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Coinex.Proto.SetAliasMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SetAliasMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a CreateOrderMessage. */
            interface ICreateOrderMessage {

                /** CreateOrderMessage sender */
                sender?: (string|null);

                /** CreateOrderMessage sequence */
                sequence?: (string|null);

                /** CreateOrderMessage tradingPair */
                tradingPair?: (string|null);

                /** CreateOrderMessage orderType */
                orderType?: (Long|null);

                /** CreateOrderMessage pricePrecision */
                pricePrecision?: (Long|null);

                /** CreateOrderMessage price */
                price?: (string|null);

                /** CreateOrderMessage quantity */
                quantity?: (string|null);

                /** CreateOrderMessage side */
                side?: (Long|null);

                /** CreateOrderMessage timeInForce */
                timeInForce?: (string|null);

                /** CreateOrderMessage existBlocks */
                existBlocks?: (string|null);

                /** CreateOrderMessage typePrefix */
                typePrefix?: (string|null);
            }

            /** Represents a CreateOrderMessage. */
            class CreateOrderMessage implements ICreateOrderMessage {

                /**
                 * Constructs a new CreateOrderMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Coinex.Proto.ICreateOrderMessage);

                /** CreateOrderMessage sender. */
                public sender: string;

                /** CreateOrderMessage sequence. */
                public sequence: string;

                /** CreateOrderMessage tradingPair. */
                public tradingPair: string;

                /** CreateOrderMessage orderType. */
                public orderType: Long;

                /** CreateOrderMessage pricePrecision. */
                public pricePrecision: Long;

                /** CreateOrderMessage price. */
                public price: string;

                /** CreateOrderMessage quantity. */
                public quantity: string;

                /** CreateOrderMessage side. */
                public side: Long;

                /** CreateOrderMessage timeInForce. */
                public timeInForce: string;

                /** CreateOrderMessage existBlocks. */
                public existBlocks: string;

                /** CreateOrderMessage typePrefix. */
                public typePrefix: string;

                /**
                 * Creates a new CreateOrderMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns CreateOrderMessage instance
                 */
                public static create(properties?: TW.Coinex.Proto.ICreateOrderMessage): TW.Coinex.Proto.CreateOrderMessage;

                /**
                 * Encodes the specified CreateOrderMessage message. Does not implicitly {@link TW.Coinex.Proto.CreateOrderMessage.verify|verify} messages.
                 * @param message CreateOrderMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Coinex.Proto.ICreateOrderMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified CreateOrderMessage message, length delimited. Does not implicitly {@link TW.Coinex.Proto.CreateOrderMessage.verify|verify} messages.
                 * @param message CreateOrderMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Coinex.Proto.ICreateOrderMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a CreateOrderMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns CreateOrderMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Coinex.Proto.CreateOrderMessage;

                /**
                 * Decodes a CreateOrderMessage message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns CreateOrderMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Coinex.Proto.CreateOrderMessage;

                /**
                 * Verifies a CreateOrderMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a CreateOrderMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns CreateOrderMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Coinex.Proto.CreateOrderMessage;

                /**
                 * Creates a plain object from a CreateOrderMessage message. Also converts values to other types if specified.
                 * @param message CreateOrderMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Coinex.Proto.CreateOrderMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this CreateOrderMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a CancelOrderMessage. */
            interface ICancelOrderMessage {

                /** CancelOrderMessage orderId */
                orderId?: (string|null);

                /** CancelOrderMessage sender */
                sender?: (string|null);

                /** CancelOrderMessage typePrefix */
                typePrefix?: (string|null);
            }

            /** Represents a CancelOrderMessage. */
            class CancelOrderMessage implements ICancelOrderMessage {

                /**
                 * Constructs a new CancelOrderMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Coinex.Proto.ICancelOrderMessage);

                /** CancelOrderMessage orderId. */
                public orderId: string;

                /** CancelOrderMessage sender. */
                public sender: string;

                /** CancelOrderMessage typePrefix. */
                public typePrefix: string;

                /**
                 * Creates a new CancelOrderMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns CancelOrderMessage instance
                 */
                public static create(properties?: TW.Coinex.Proto.ICancelOrderMessage): TW.Coinex.Proto.CancelOrderMessage;

                /**
                 * Encodes the specified CancelOrderMessage message. Does not implicitly {@link TW.Coinex.Proto.CancelOrderMessage.verify|verify} messages.
                 * @param message CancelOrderMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Coinex.Proto.ICancelOrderMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified CancelOrderMessage message, length delimited. Does not implicitly {@link TW.Coinex.Proto.CancelOrderMessage.verify|verify} messages.
                 * @param message CancelOrderMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Coinex.Proto.ICancelOrderMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a CancelOrderMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns CancelOrderMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Coinex.Proto.CancelOrderMessage;

                /**
                 * Decodes a CancelOrderMessage message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns CancelOrderMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Coinex.Proto.CancelOrderMessage;

                /**
                 * Verifies a CancelOrderMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a CancelOrderMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns CancelOrderMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Coinex.Proto.CancelOrderMessage;

                /**
                 * Creates a plain object from a CancelOrderMessage message. Also converts values to other types if specified.
                 * @param message CancelOrderMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Coinex.Proto.CancelOrderMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this CancelOrderMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Transaction. */
            interface ITransaction {

                /** Transaction fee */
                fee?: (TW.Coinex.Proto.IFee|null);

                /** Transaction memo */
                memo?: (string|null);

                /** Transaction signature */
                signature?: (TW.Coinex.Proto.ISignature|null);

                /** Transaction sendCoinsMessage */
                sendCoinsMessage?: (TW.Coinex.Proto.ISendCoinsMessage|null);

                /** Transaction stakeMessage */
                stakeMessage?: (TW.Coinex.Proto.IStakeMessage|null);

                /** Transaction unstakeMessage */
                unstakeMessage?: (TW.Coinex.Proto.IStakeMessage|null);

                /** Transaction withdrawStakeRewardMessage */
                withdrawStakeRewardMessage?: (TW.Coinex.Proto.IWithdrawStakeRewardMessage|null);

                /** Transaction setAliasMessage */
                setAliasMessage?: (TW.Coinex.Proto.ISetAliasMessage|null);

                /** Transaction createOrderMessage */
                createOrderMessage?: (TW.Coinex.Proto.ICreateOrderMessage|null);

                /** Transaction cancelOrderMessage */
                cancelOrderMessage?: (TW.Coinex.Proto.ICancelOrderMessage|null);
            }

            /** Represents a Transaction. */
            class Transaction implements ITransaction {

                /**
                 * Constructs a new Transaction.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Coinex.Proto.ITransaction);

                /** Transaction fee. */
                public fee?: (TW.Coinex.Proto.IFee|null);

                /** Transaction memo. */
                public memo: string;

                /** Transaction signature. */
                public signature?: (TW.Coinex.Proto.ISignature|null);

                /** Transaction sendCoinsMessage. */
                public sendCoinsMessage?: (TW.Coinex.Proto.ISendCoinsMessage|null);

                /** Transaction stakeMessage. */
                public stakeMessage?: (TW.Coinex.Proto.IStakeMessage|null);

                /** Transaction unstakeMessage. */
                public unstakeMessage?: (TW.Coinex.Proto.IStakeMessage|null);

                /** Transaction withdrawStakeRewardMessage. */
                public withdrawStakeRewardMessage?: (TW.Coinex.Proto.IWithdrawStakeRewardMessage|null);

                /** Transaction setAliasMessage. */
                public setAliasMessage?: (TW.Coinex.Proto.ISetAliasMessage|null);

                /** Transaction createOrderMessage. */
                public createOrderMessage?: (TW.Coinex.Proto.ICreateOrderMessage|null);

                /** Transaction cancelOrderMessage. */
                public cancelOrderMessage?: (TW.Coinex.Proto.ICancelOrderMessage|null);

                /** Transaction messageOneof. */
                public messageOneof?: ("sendCoinsMessage"|"stakeMessage"|"unstakeMessage"|"withdrawStakeRewardMessage"|"setAliasMessage"|"createOrderMessage"|"cancelOrderMessage");

                /**
                 * Creates a new Transaction instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Transaction instance
                 */
                public static create(properties?: TW.Coinex.Proto.ITransaction): TW.Coinex.Proto.Transaction;

                /**
                 * Encodes the specified Transaction message. Does not implicitly {@link TW.Coinex.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Coinex.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified Transaction message, length delimited. Does not implicitly {@link TW.Coinex.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Coinex.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Transaction message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Coinex.Proto.Transaction;

                /**
                 * Decodes a Transaction message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Coinex.Proto.Transaction;

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
                public static fromObject(object: { [k: string]: any }): TW.Coinex.Proto.Transaction;

                /**
                 * Creates a plain object from a Transaction message. Also converts values to other types if specified.
                 * @param message Transaction
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Coinex.Proto.Transaction, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Transaction to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput accountNumber */
                accountNumber?: (Long|null);

                /** SigningInput chainId */
                chainId?: (string|null);

                /** SigningInput fee */
                fee?: (TW.Coinex.Proto.IFee|null);

                /** SigningInput memo */
                memo?: (string|null);

                /** SigningInput sequence */
                sequence?: (Long|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput sendCoinsMessage */
                sendCoinsMessage?: (TW.Coinex.Proto.ISendCoinsMessage|null);

                /** SigningInput stakeMessage */
                stakeMessage?: (TW.Coinex.Proto.IStakeMessage|null);

                /** SigningInput unstakeMessage */
                unstakeMessage?: (TW.Coinex.Proto.IStakeMessage|null);

                /** SigningInput withdrawStakeRewardMessage */
                withdrawStakeRewardMessage?: (TW.Coinex.Proto.IWithdrawStakeRewardMessage|null);

                /** SigningInput setAliasMessage */
                setAliasMessage?: (TW.Coinex.Proto.ISetAliasMessage|null);

                /** SigningInput createOrderMessage */
                createOrderMessage?: (TW.Coinex.Proto.ICreateOrderMessage|null);

                /** SigningInput cancelOrderMessage */
                cancelOrderMessage?: (TW.Coinex.Proto.ICancelOrderMessage|null);

                /** SigningInput typePrefix */
                typePrefix?: (string|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Coinex.Proto.ISigningInput);

                /** SigningInput accountNumber. */
                public accountNumber: Long;

                /** SigningInput chainId. */
                public chainId: string;

                /** SigningInput fee. */
                public fee?: (TW.Coinex.Proto.IFee|null);

                /** SigningInput memo. */
                public memo: string;

                /** SigningInput sequence. */
                public sequence: Long;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput sendCoinsMessage. */
                public sendCoinsMessage?: (TW.Coinex.Proto.ISendCoinsMessage|null);

                /** SigningInput stakeMessage. */
                public stakeMessage?: (TW.Coinex.Proto.IStakeMessage|null);

                /** SigningInput unstakeMessage. */
                public unstakeMessage?: (TW.Coinex.Proto.IStakeMessage|null);

                /** SigningInput withdrawStakeRewardMessage. */
                public withdrawStakeRewardMessage?: (TW.Coinex.Proto.IWithdrawStakeRewardMessage|null);

                /** SigningInput setAliasMessage. */
                public setAliasMessage?: (TW.Coinex.Proto.ISetAliasMessage|null);

                /** SigningInput createOrderMessage. */
                public createOrderMessage?: (TW.Coinex.Proto.ICreateOrderMessage|null);

                /** SigningInput cancelOrderMessage. */
                public cancelOrderMessage?: (TW.Coinex.Proto.ICancelOrderMessage|null);

                /** SigningInput typePrefix. */
                public typePrefix: string;

                /** SigningInput messageOneof. */
                public messageOneof?: ("sendCoinsMessage"|"stakeMessage"|"unstakeMessage"|"withdrawStakeRewardMessage"|"setAliasMessage"|"createOrderMessage"|"cancelOrderMessage");

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Coinex.Proto.ISigningInput): TW.Coinex.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Coinex.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Coinex.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Coinex.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Coinex.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Coinex.Proto.SigningInput;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Coinex.Proto.SigningInput;

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
                public static fromObject(object: { [k: string]: any }): TW.Coinex.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Coinex.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

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
                constructor(properties?: TW.Coinex.Proto.ISigningOutput);

                /** SigningOutput signature. */
                public signature: Uint8Array;

                /** SigningOutput json. */
                public json: string;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Coinex.Proto.ISigningOutput): TW.Coinex.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Coinex.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Coinex.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Coinex.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Coinex.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Coinex.Proto.SigningOutput;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Coinex.Proto.SigningOutput;

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
                public static fromObject(object: { [k: string]: any }): TW.Coinex.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Coinex.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }
        }
    }

    /** Namespace Proto. */
    namespace Proto {

        /** Properties of a Result. */
        interface IResult {

            /** Result success */
            success?: (boolean|null);

            /** Result error */
            error?: (string|null);

            /** Result objects */
            objects?: (google.protobuf.IAny[]|null);
        }

        /** Represents a Result. */
        class Result implements IResult {

            /**
             * Constructs a new Result.
             * @param [properties] Properties to set
             */
            constructor(properties?: TW.Proto.IResult);

            /** Result success. */
            public success: boolean;

            /** Result error. */
            public error: string;

            /** Result objects. */
            public objects: google.protobuf.IAny[];

            /**
             * Creates a new Result instance using the specified properties.
             * @param [properties] Properties to set
             * @returns Result instance
             */
            public static create(properties?: TW.Proto.IResult): TW.Proto.Result;

            /**
             * Encodes the specified Result message. Does not implicitly {@link TW.Proto.Result.verify|verify} messages.
             * @param message Result message or plain object to encode
             * @param [writer] Writer to encode to
             * @returns Writer
             */
            public static encode(message: TW.Proto.IResult, writer?: $protobuf.Writer): $protobuf.Writer;

            /**
             * Encodes the specified Result message, length delimited. Does not implicitly {@link TW.Proto.Result.verify|verify} messages.
             * @param message Result message or plain object to encode
             * @param [writer] Writer to encode to
             * @returns Writer
             */
            public static encodeDelimited(message: TW.Proto.IResult, writer?: $protobuf.Writer): $protobuf.Writer;

            /**
             * Decodes a Result message from the specified reader or buffer.
             * @param reader Reader or buffer to decode from
             * @param [length] Message length if known beforehand
             * @returns Result
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Proto.Result;

            /**
             * Decodes a Result message from the specified reader or buffer, length delimited.
             * @param reader Reader or buffer to decode from
             * @returns Result
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Proto.Result;

            /**
             * Verifies a Result message.
             * @param message Plain object to verify
             * @returns `null` if valid, otherwise the reason why it is not
             */
            public static verify(message: { [k: string]: any }): (string|null);

            /**
             * Creates a Result message from a plain object. Also converts values to their respective internal types.
             * @param object Plain object
             * @returns Result
             */
            public static fromObject(object: { [k: string]: any }): TW.Proto.Result;

            /**
             * Creates a plain object from a Result message. Also converts values to other types if specified.
             * @param message Result
             * @param [options] Conversion options
             * @returns Plain object
             */
            public static toObject(message: TW.Proto.Result, options?: $protobuf.IConversionOptions): { [k: string]: any };

            /**
             * Converts this Result to JSON.
             * @returns JSON object
             */
            public toJSON(): { [k: string]: any };
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
                 * Encodes the specified Amount message, length delimited. Does not implicitly {@link TW.Cosmos.Proto.Amount.verify|verify} messages.
                 * @param message Amount message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Cosmos.Proto.IAmount, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes an Amount message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Amount
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Cosmos.Proto.Amount;

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
                 * Encodes the specified Fee message, length delimited. Does not implicitly {@link TW.Cosmos.Proto.Fee.verify|verify} messages.
                 * @param message Fee message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Cosmos.Proto.IFee, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a Fee message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Fee
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Cosmos.Proto.Fee;

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

            /** Properties of a SendCoinsMessage. */
            interface ISendCoinsMessage {

                /** SendCoinsMessage fromAddress */
                fromAddress?: (string|null);

                /** SendCoinsMessage toAddress */
                toAddress?: (string|null);

                /** SendCoinsMessage amounts */
                amounts?: (TW.Cosmos.Proto.IAmount[]|null);

                /** SendCoinsMessage typePrefix */
                typePrefix?: (string|null);
            }

            /** Represents a SendCoinsMessage. */
            class SendCoinsMessage implements ISendCoinsMessage {

                /**
                 * Constructs a new SendCoinsMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Cosmos.Proto.ISendCoinsMessage);

                /** SendCoinsMessage fromAddress. */
                public fromAddress: string;

                /** SendCoinsMessage toAddress. */
                public toAddress: string;

                /** SendCoinsMessage amounts. */
                public amounts: TW.Cosmos.Proto.IAmount[];

                /** SendCoinsMessage typePrefix. */
                public typePrefix: string;

                /**
                 * Creates a new SendCoinsMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SendCoinsMessage instance
                 */
                public static create(properties?: TW.Cosmos.Proto.ISendCoinsMessage): TW.Cosmos.Proto.SendCoinsMessage;

                /**
                 * Encodes the specified SendCoinsMessage message. Does not implicitly {@link TW.Cosmos.Proto.SendCoinsMessage.verify|verify} messages.
                 * @param message SendCoinsMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Cosmos.Proto.ISendCoinsMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SendCoinsMessage message, length delimited. Does not implicitly {@link TW.Cosmos.Proto.SendCoinsMessage.verify|verify} messages.
                 * @param message SendCoinsMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Cosmos.Proto.ISendCoinsMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SendCoinsMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SendCoinsMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.SendCoinsMessage;

                /**
                 * Decodes a SendCoinsMessage message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SendCoinsMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Cosmos.Proto.SendCoinsMessage;

                /**
                 * Verifies a SendCoinsMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a SendCoinsMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns SendCoinsMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.SendCoinsMessage;

                /**
                 * Creates a plain object from a SendCoinsMessage message. Also converts values to other types if specified.
                 * @param message SendCoinsMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Cosmos.Proto.SendCoinsMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SendCoinsMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a StakeMessage. */
            interface IStakeMessage {

                /** StakeMessage delegatorAddress */
                delegatorAddress?: (string|null);

                /** StakeMessage validatorAddress */
                validatorAddress?: (string|null);

                /** StakeMessage amount */
                amount?: (TW.Cosmos.Proto.IAmount|null);

                /** StakeMessage typePrefix */
                typePrefix?: (string|null);
            }

            /** Represents a StakeMessage. */
            class StakeMessage implements IStakeMessage {

                /**
                 * Constructs a new StakeMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Cosmos.Proto.IStakeMessage);

                /** StakeMessage delegatorAddress. */
                public delegatorAddress: string;

                /** StakeMessage validatorAddress. */
                public validatorAddress: string;

                /** StakeMessage amount. */
                public amount?: (TW.Cosmos.Proto.IAmount|null);

                /** StakeMessage typePrefix. */
                public typePrefix: string;

                /**
                 * Creates a new StakeMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns StakeMessage instance
                 */
                public static create(properties?: TW.Cosmos.Proto.IStakeMessage): TW.Cosmos.Proto.StakeMessage;

                /**
                 * Encodes the specified StakeMessage message. Does not implicitly {@link TW.Cosmos.Proto.StakeMessage.verify|verify} messages.
                 * @param message StakeMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Cosmos.Proto.IStakeMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified StakeMessage message, length delimited. Does not implicitly {@link TW.Cosmos.Proto.StakeMessage.verify|verify} messages.
                 * @param message StakeMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Cosmos.Proto.IStakeMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a StakeMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns StakeMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.StakeMessage;

                /**
                 * Decodes a StakeMessage message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns StakeMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Cosmos.Proto.StakeMessage;

                /**
                 * Verifies a StakeMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a StakeMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns StakeMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.StakeMessage;

                /**
                 * Creates a plain object from a StakeMessage message. Also converts values to other types if specified.
                 * @param message StakeMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Cosmos.Proto.StakeMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this StakeMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a WithdrawStakeRewardMessage. */
            interface IWithdrawStakeRewardMessage {

                /** WithdrawStakeRewardMessage delegatorAddress */
                delegatorAddress?: (string|null);

                /** WithdrawStakeRewardMessage validatorAddress */
                validatorAddress?: (string|null);

                /** WithdrawStakeRewardMessage typePrefix */
                typePrefix?: (string|null);
            }

            /** Represents a WithdrawStakeRewardMessage. */
            class WithdrawStakeRewardMessage implements IWithdrawStakeRewardMessage {

                /**
                 * Constructs a new WithdrawStakeRewardMessage.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Cosmos.Proto.IWithdrawStakeRewardMessage);

                /** WithdrawStakeRewardMessage delegatorAddress. */
                public delegatorAddress: string;

                /** WithdrawStakeRewardMessage validatorAddress. */
                public validatorAddress: string;

                /** WithdrawStakeRewardMessage typePrefix. */
                public typePrefix: string;

                /**
                 * Creates a new WithdrawStakeRewardMessage instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns WithdrawStakeRewardMessage instance
                 */
                public static create(properties?: TW.Cosmos.Proto.IWithdrawStakeRewardMessage): TW.Cosmos.Proto.WithdrawStakeRewardMessage;

                /**
                 * Encodes the specified WithdrawStakeRewardMessage message. Does not implicitly {@link TW.Cosmos.Proto.WithdrawStakeRewardMessage.verify|verify} messages.
                 * @param message WithdrawStakeRewardMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Cosmos.Proto.IWithdrawStakeRewardMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified WithdrawStakeRewardMessage message, length delimited. Does not implicitly {@link TW.Cosmos.Proto.WithdrawStakeRewardMessage.verify|verify} messages.
                 * @param message WithdrawStakeRewardMessage message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Cosmos.Proto.IWithdrawStakeRewardMessage, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a WithdrawStakeRewardMessage message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns WithdrawStakeRewardMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.WithdrawStakeRewardMessage;

                /**
                 * Decodes a WithdrawStakeRewardMessage message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns WithdrawStakeRewardMessage
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Cosmos.Proto.WithdrawStakeRewardMessage;

                /**
                 * Verifies a WithdrawStakeRewardMessage message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a WithdrawStakeRewardMessage message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns WithdrawStakeRewardMessage
                 */
                public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.WithdrawStakeRewardMessage;

                /**
                 * Creates a plain object from a WithdrawStakeRewardMessage message. Also converts values to other types if specified.
                 * @param message WithdrawStakeRewardMessage
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Cosmos.Proto.WithdrawStakeRewardMessage, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this WithdrawStakeRewardMessage to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Signature. */
            interface ISignature {

                /** Signature publicKey */
                publicKey?: (Uint8Array|null);

                /** Signature signature */
                signature?: (Uint8Array|null);
            }

            /** Represents a Signature. */
            class Signature implements ISignature {

                /**
                 * Constructs a new Signature.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Cosmos.Proto.ISignature);

                /** Signature publicKey. */
                public publicKey: Uint8Array;

                /** Signature signature. */
                public signature: Uint8Array;

                /**
                 * Creates a new Signature instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Signature instance
                 */
                public static create(properties?: TW.Cosmos.Proto.ISignature): TW.Cosmos.Proto.Signature;

                /**
                 * Encodes the specified Signature message. Does not implicitly {@link TW.Cosmos.Proto.Signature.verify|verify} messages.
                 * @param message Signature message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Cosmos.Proto.ISignature, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified Signature message, length delimited. Does not implicitly {@link TW.Cosmos.Proto.Signature.verify|verify} messages.
                 * @param message Signature message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Cosmos.Proto.ISignature, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Signature message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Signature
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.Signature;

                /**
                 * Decodes a Signature message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Signature
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Cosmos.Proto.Signature;

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
                public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.Signature;

                /**
                 * Creates a plain object from a Signature message. Also converts values to other types if specified.
                 * @param message Signature
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Cosmos.Proto.Signature, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Signature to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Transaction. */
            interface ITransaction {

                /** Transaction fee */
                fee?: (TW.Cosmos.Proto.IFee|null);

                /** Transaction memo */
                memo?: (string|null);

                /** Transaction signature */
                signature?: (TW.Cosmos.Proto.ISignature|null);

                /** Transaction sendCoinsMessage */
                sendCoinsMessage?: (TW.Cosmos.Proto.ISendCoinsMessage|null);

                /** Transaction stakeMessage */
                stakeMessage?: (TW.Cosmos.Proto.IStakeMessage|null);

                /** Transaction unstakeMessage */
                unstakeMessage?: (TW.Cosmos.Proto.IStakeMessage|null);

                /** Transaction withdrawStakeRewardMessage */
                withdrawStakeRewardMessage?: (TW.Cosmos.Proto.IWithdrawStakeRewardMessage|null);
            }

            /** Represents a Transaction. */
            class Transaction implements ITransaction {

                /**
                 * Constructs a new Transaction.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Cosmos.Proto.ITransaction);

                /** Transaction fee. */
                public fee?: (TW.Cosmos.Proto.IFee|null);

                /** Transaction memo. */
                public memo: string;

                /** Transaction signature. */
                public signature?: (TW.Cosmos.Proto.ISignature|null);

                /** Transaction sendCoinsMessage. */
                public sendCoinsMessage?: (TW.Cosmos.Proto.ISendCoinsMessage|null);

                /** Transaction stakeMessage. */
                public stakeMessage?: (TW.Cosmos.Proto.IStakeMessage|null);

                /** Transaction unstakeMessage. */
                public unstakeMessage?: (TW.Cosmos.Proto.IStakeMessage|null);

                /** Transaction withdrawStakeRewardMessage. */
                public withdrawStakeRewardMessage?: (TW.Cosmos.Proto.IWithdrawStakeRewardMessage|null);

                /** Transaction messageOneof. */
                public messageOneof?: ("sendCoinsMessage"|"stakeMessage"|"unstakeMessage"|"withdrawStakeRewardMessage");

                /**
                 * Creates a new Transaction instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Transaction instance
                 */
                public static create(properties?: TW.Cosmos.Proto.ITransaction): TW.Cosmos.Proto.Transaction;

                /**
                 * Encodes the specified Transaction message. Does not implicitly {@link TW.Cosmos.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Cosmos.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified Transaction message, length delimited. Does not implicitly {@link TW.Cosmos.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Cosmos.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Transaction message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Cosmos.Proto.Transaction;

                /**
                 * Decodes a Transaction message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Cosmos.Proto.Transaction;

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
                public static fromObject(object: { [k: string]: any }): TW.Cosmos.Proto.Transaction;

                /**
                 * Creates a plain object from a Transaction message. Also converts values to other types if specified.
                 * @param message Transaction
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Cosmos.Proto.Transaction, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Transaction to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
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

                /** SigningInput sendCoinsMessage */
                sendCoinsMessage?: (TW.Cosmos.Proto.ISendCoinsMessage|null);

                /** SigningInput stakeMessage */
                stakeMessage?: (TW.Cosmos.Proto.IStakeMessage|null);

                /** SigningInput unstakeMessage */
                unstakeMessage?: (TW.Cosmos.Proto.IStakeMessage|null);

                /** SigningInput withdrawStakeRewardMessage */
                withdrawStakeRewardMessage?: (TW.Cosmos.Proto.IWithdrawStakeRewardMessage|null);

                /** SigningInput typePrefix */
                typePrefix?: (string|null);
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

                /** SigningInput sendCoinsMessage. */
                public sendCoinsMessage?: (TW.Cosmos.Proto.ISendCoinsMessage|null);

                /** SigningInput stakeMessage. */
                public stakeMessage?: (TW.Cosmos.Proto.IStakeMessage|null);

                /** SigningInput unstakeMessage. */
                public unstakeMessage?: (TW.Cosmos.Proto.IStakeMessage|null);

                /** SigningInput withdrawStakeRewardMessage. */
                public withdrawStakeRewardMessage?: (TW.Cosmos.Proto.IWithdrawStakeRewardMessage|null);

                /** SigningInput typePrefix. */
                public typePrefix: string;

                /** SigningInput messageOneof. */
                public messageOneof?: ("sendCoinsMessage"|"stakeMessage"|"unstakeMessage"|"withdrawStakeRewardMessage");

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Cosmos.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Cosmos.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Cosmos.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Cosmos.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Cosmos.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Cosmos.Proto.SigningOutput;

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
                 * Encodes the specified Transaction message, length delimited. Does not implicitly {@link TW.Decred.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Decred.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a Transaction message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Decred.Proto.Transaction;

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
                 * Encodes the specified TransactionInput message, length delimited. Does not implicitly {@link TW.Decred.Proto.TransactionInput.verify|verify} messages.
                 * @param message TransactionInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Decred.Proto.ITransactionInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a TransactionInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TransactionInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Decred.Proto.TransactionInput;

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
                 * Encodes the specified TransactionOutput message, length delimited. Does not implicitly {@link TW.Decred.Proto.TransactionOutput.verify|verify} messages.
                 * @param message TransactionOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Decred.Proto.ITransactionOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a TransactionOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TransactionOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Decred.Proto.TransactionOutput;

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

                /** SigningOutput fee */
                fee?: (Long|null);

                /** SigningOutput maxAmount */
                maxAmount?: (Long|null);

                /** SigningOutput transactionId */
                transactionId?: (string|null);
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

                /** SigningOutput fee. */
                public fee: Long;

                /** SigningOutput maxAmount. */
                public maxAmount: Long;

                /** SigningOutput transactionId. */
                public transactionId: string;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Decred.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Decred.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Decred.Proto.SigningOutput;

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
                 * Encodes the specified Asset message, length delimited. Does not implicitly {@link TW.EOS.Proto.Asset.verify|verify} messages.
                 * @param message Asset message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.EOS.Proto.IAsset, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes an Asset message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Asset
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.EOS.Proto.Asset;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.EOS.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.EOS.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.EOS.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.EOS.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.EOS.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.EOS.Proto.SigningOutput;

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

                /** SigningInput amount */
                amount?: (Uint8Array|null);

                /** SigningInput payload */
                payload?: (Uint8Array|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);
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

                /** SigningInput amount. */
                public amount: Uint8Array;

                /** SigningInput payload. */
                public payload: Uint8Array;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Ethereum.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Ethereum.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Ethereum.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Ethereum.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Ethereum.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Ethereum.Proto.SigningOutput;

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

    /** Namespace IOST. */
    namespace IOST {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of an Action. */
            interface IAction {

                /** Action contract */
                contract?: (string|null);

                /** Action actionName */
                actionName?: (string|null);

                /** Action data */
                data?: (string|null);
            }

            /** Represents an Action. */
            class Action implements IAction {

                /**
                 * Constructs a new Action.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IOST.Proto.IAction);

                /** Action contract. */
                public contract: string;

                /** Action actionName. */
                public actionName: string;

                /** Action data. */
                public data: string;

                /**
                 * Creates a new Action instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Action instance
                 */
                public static create(properties?: TW.IOST.Proto.IAction): TW.IOST.Proto.Action;

                /**
                 * Encodes the specified Action message. Does not implicitly {@link TW.IOST.Proto.Action.verify|verify} messages.
                 * @param message Action message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IOST.Proto.IAction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified Action message, length delimited. Does not implicitly {@link TW.IOST.Proto.Action.verify|verify} messages.
                 * @param message Action message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.IOST.Proto.IAction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an Action message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Action
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IOST.Proto.Action;

                /**
                 * Decodes an Action message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Action
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.IOST.Proto.Action;

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
                public static fromObject(object: { [k: string]: any }): TW.IOST.Proto.Action;

                /**
                 * Creates a plain object from an Action message. Also converts values to other types if specified.
                 * @param message Action
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IOST.Proto.Action, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Action to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an AmountLimit. */
            interface IAmountLimit {

                /** AmountLimit token */
                token?: (string|null);

                /** AmountLimit value */
                value?: (string|null);
            }

            /** Represents an AmountLimit. */
            class AmountLimit implements IAmountLimit {

                /**
                 * Constructs a new AmountLimit.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IOST.Proto.IAmountLimit);

                /** AmountLimit token. */
                public token: string;

                /** AmountLimit value. */
                public value: string;

                /**
                 * Creates a new AmountLimit instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns AmountLimit instance
                 */
                public static create(properties?: TW.IOST.Proto.IAmountLimit): TW.IOST.Proto.AmountLimit;

                /**
                 * Encodes the specified AmountLimit message. Does not implicitly {@link TW.IOST.Proto.AmountLimit.verify|verify} messages.
                 * @param message AmountLimit message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IOST.Proto.IAmountLimit, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified AmountLimit message, length delimited. Does not implicitly {@link TW.IOST.Proto.AmountLimit.verify|verify} messages.
                 * @param message AmountLimit message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.IOST.Proto.IAmountLimit, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an AmountLimit message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns AmountLimit
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IOST.Proto.AmountLimit;

                /**
                 * Decodes an AmountLimit message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns AmountLimit
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.IOST.Proto.AmountLimit;

                /**
                 * Verifies an AmountLimit message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an AmountLimit message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns AmountLimit
                 */
                public static fromObject(object: { [k: string]: any }): TW.IOST.Proto.AmountLimit;

                /**
                 * Creates a plain object from an AmountLimit message. Also converts values to other types if specified.
                 * @param message AmountLimit
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IOST.Proto.AmountLimit, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this AmountLimit to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Algorithm enum. */
            enum Algorithm {
                UNKNOWN = 0,
                SECP256K1 = 1,
                ED25519 = 2
            }

            /** Properties of a Signature. */
            interface ISignature {

                /** Signature algorithm */
                algorithm?: (TW.IOST.Proto.Algorithm|null);

                /** Signature signature */
                signature?: (Uint8Array|null);

                /** Signature publicKey */
                publicKey?: (Uint8Array|null);
            }

            /** Represents a Signature. */
            class Signature implements ISignature {

                /**
                 * Constructs a new Signature.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IOST.Proto.ISignature);

                /** Signature algorithm. */
                public algorithm: TW.IOST.Proto.Algorithm;

                /** Signature signature. */
                public signature: Uint8Array;

                /** Signature publicKey. */
                public publicKey: Uint8Array;

                /**
                 * Creates a new Signature instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Signature instance
                 */
                public static create(properties?: TW.IOST.Proto.ISignature): TW.IOST.Proto.Signature;

                /**
                 * Encodes the specified Signature message. Does not implicitly {@link TW.IOST.Proto.Signature.verify|verify} messages.
                 * @param message Signature message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IOST.Proto.ISignature, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified Signature message, length delimited. Does not implicitly {@link TW.IOST.Proto.Signature.verify|verify} messages.
                 * @param message Signature message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.IOST.Proto.ISignature, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Signature message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Signature
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IOST.Proto.Signature;

                /**
                 * Decodes a Signature message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Signature
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.IOST.Proto.Signature;

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
                public static fromObject(object: { [k: string]: any }): TW.IOST.Proto.Signature;

                /**
                 * Creates a plain object from a Signature message. Also converts values to other types if specified.
                 * @param message Signature
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IOST.Proto.Signature, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Signature to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Transaction. */
            interface ITransaction {

                /** Transaction time */
                time?: (Long|null);

                /** Transaction expiration */
                expiration?: (Long|null);

                /** Transaction gasRatio */
                gasRatio?: (number|null);

                /** Transaction gasLimit */
                gasLimit?: (number|null);

                /** Transaction delay */
                delay?: (Long|null);

                /** Transaction chainId */
                chainId?: (number|null);

                /** Transaction actions */
                actions?: (TW.IOST.Proto.IAction[]|null);

                /** Transaction amountLimit */
                amountLimit?: (TW.IOST.Proto.IAmountLimit[]|null);

                /** Transaction signers */
                signers?: (string[]|null);

                /** Transaction signatures */
                signatures?: (TW.IOST.Proto.ISignature[]|null);

                /** Transaction publisher */
                publisher?: (string|null);

                /** Transaction publisherSigs */
                publisherSigs?: (TW.IOST.Proto.ISignature[]|null);
            }

            /** Represents a Transaction. */
            class Transaction implements ITransaction {

                /**
                 * Constructs a new Transaction.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IOST.Proto.ITransaction);

                /** Transaction time. */
                public time: Long;

                /** Transaction expiration. */
                public expiration: Long;

                /** Transaction gasRatio. */
                public gasRatio: number;

                /** Transaction gasLimit. */
                public gasLimit: number;

                /** Transaction delay. */
                public delay: Long;

                /** Transaction chainId. */
                public chainId: number;

                /** Transaction actions. */
                public actions: TW.IOST.Proto.IAction[];

                /** Transaction amountLimit. */
                public amountLimit: TW.IOST.Proto.IAmountLimit[];

                /** Transaction signers. */
                public signers: string[];

                /** Transaction signatures. */
                public signatures: TW.IOST.Proto.ISignature[];

                /** Transaction publisher. */
                public publisher: string;

                /** Transaction publisherSigs. */
                public publisherSigs: TW.IOST.Proto.ISignature[];

                /**
                 * Creates a new Transaction instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Transaction instance
                 */
                public static create(properties?: TW.IOST.Proto.ITransaction): TW.IOST.Proto.Transaction;

                /**
                 * Encodes the specified Transaction message. Does not implicitly {@link TW.IOST.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IOST.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified Transaction message, length delimited. Does not implicitly {@link TW.IOST.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.IOST.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a Transaction message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IOST.Proto.Transaction;

                /**
                 * Decodes a Transaction message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.IOST.Proto.Transaction;

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
                public static fromObject(object: { [k: string]: any }): TW.IOST.Proto.Transaction;

                /**
                 * Creates a plain object from a Transaction message. Also converts values to other types if specified.
                 * @param message Transaction
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IOST.Proto.Transaction, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Transaction to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of an AccountInfo. */
            interface IAccountInfo {

                /** AccountInfo name */
                name?: (string|null);

                /** AccountInfo activeKey */
                activeKey?: (Uint8Array|null);

                /** AccountInfo ownerKey */
                ownerKey?: (Uint8Array|null);
            }

            /** Represents an AccountInfo. */
            class AccountInfo implements IAccountInfo {

                /**
                 * Constructs a new AccountInfo.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IOST.Proto.IAccountInfo);

                /** AccountInfo name. */
                public name: string;

                /** AccountInfo activeKey. */
                public activeKey: Uint8Array;

                /** AccountInfo ownerKey. */
                public ownerKey: Uint8Array;

                /**
                 * Creates a new AccountInfo instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns AccountInfo instance
                 */
                public static create(properties?: TW.IOST.Proto.IAccountInfo): TW.IOST.Proto.AccountInfo;

                /**
                 * Encodes the specified AccountInfo message. Does not implicitly {@link TW.IOST.Proto.AccountInfo.verify|verify} messages.
                 * @param message AccountInfo message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IOST.Proto.IAccountInfo, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified AccountInfo message, length delimited. Does not implicitly {@link TW.IOST.Proto.AccountInfo.verify|verify} messages.
                 * @param message AccountInfo message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.IOST.Proto.IAccountInfo, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an AccountInfo message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns AccountInfo
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IOST.Proto.AccountInfo;

                /**
                 * Decodes an AccountInfo message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns AccountInfo
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.IOST.Proto.AccountInfo;

                /**
                 * Verifies an AccountInfo message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an AccountInfo message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns AccountInfo
                 */
                public static fromObject(object: { [k: string]: any }): TW.IOST.Proto.AccountInfo;

                /**
                 * Creates a plain object from an AccountInfo message. Also converts values to other types if specified.
                 * @param message AccountInfo
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IOST.Proto.AccountInfo, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this AccountInfo to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput account */
                account?: (TW.IOST.Proto.IAccountInfo|null);

                /** SigningInput transactionTemplate */
                transactionTemplate?: (TW.IOST.Proto.ITransaction|null);

                /** SigningInput transferDestination */
                transferDestination?: (string|null);

                /** SigningInput transferAmount */
                transferAmount?: (string|null);

                /** SigningInput transferMemo */
                transferMemo?: (string|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IOST.Proto.ISigningInput);

                /** SigningInput account. */
                public account?: (TW.IOST.Proto.IAccountInfo|null);

                /** SigningInput transactionTemplate. */
                public transactionTemplate?: (TW.IOST.Proto.ITransaction|null);

                /** SigningInput transferDestination. */
                public transferDestination: string;

                /** SigningInput transferAmount. */
                public transferAmount: string;

                /** SigningInput transferMemo. */
                public transferMemo: string;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.IOST.Proto.ISigningInput): TW.IOST.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.IOST.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IOST.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.IOST.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.IOST.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IOST.Proto.SigningInput;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.IOST.Proto.SigningInput;

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
                public static fromObject(object: { [k: string]: any }): TW.IOST.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IOST.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a SigningOutput. */
            interface ISigningOutput {

                /** SigningOutput transaction */
                transaction?: (TW.IOST.Proto.ITransaction|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IOST.Proto.ISigningOutput);

                /** SigningOutput transaction. */
                public transaction?: (TW.IOST.Proto.ITransaction|null);

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.IOST.Proto.ISigningOutput): TW.IOST.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.IOST.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IOST.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.IOST.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.IOST.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IOST.Proto.SigningOutput;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.IOST.Proto.SigningOutput;

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
                public static fromObject(object: { [k: string]: any }): TW.IOST.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IOST.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this SigningOutput to JSON.
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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Icon.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Icon.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Icon.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Icon.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Icon.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Icon.Proto.SigningOutput;

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
                 * Encodes the specified Transfer message, length delimited. Does not implicitly {@link TW.IoTeX.Proto.Transfer.verify|verify} messages.
                 * @param message Transfer message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.IoTeX.Proto.ITransfer, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a Transfer message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Transfer
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.IoTeX.Proto.Transfer;

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

            /** Properties of an Execution. */
            interface IExecution {

                /** Execution amount */
                amount?: (string|null);

                /** Execution contract */
                contract?: (string|null);

                /** Execution data */
                data?: (Uint8Array|null);
            }

            /** Represents an Execution. */
            class Execution implements IExecution {

                /**
                 * Constructs a new Execution.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.IoTeX.Proto.IExecution);

                /** Execution amount. */
                public amount: string;

                /** Execution contract. */
                public contract: string;

                /** Execution data. */
                public data: Uint8Array;

                /**
                 * Creates a new Execution instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns Execution instance
                 */
                public static create(properties?: TW.IoTeX.Proto.IExecution): TW.IoTeX.Proto.Execution;

                /**
                 * Encodes the specified Execution message. Does not implicitly {@link TW.IoTeX.Proto.Execution.verify|verify} messages.
                 * @param message Execution message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.IoTeX.Proto.IExecution, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified Execution message, length delimited. Does not implicitly {@link TW.IoTeX.Proto.Execution.verify|verify} messages.
                 * @param message Execution message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.IoTeX.Proto.IExecution, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an Execution message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns Execution
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.IoTeX.Proto.Execution;

                /**
                 * Decodes an Execution message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Execution
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.IoTeX.Proto.Execution;

                /**
                 * Verifies an Execution message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an Execution message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns Execution
                 */
                public static fromObject(object: { [k: string]: any }): TW.IoTeX.Proto.Execution;

                /**
                 * Creates a plain object from an Execution message. Also converts values to other types if specified.
                 * @param message Execution
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.IoTeX.Proto.Execution, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this Execution to JSON.
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
                execution?: (TW.IoTeX.Proto.IExecution|null);
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
                public execution?: (TW.IoTeX.Proto.IExecution|null);

                /** ActionCore action. */
                public action?: ("transfer"|"execution");

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
                 * Encodes the specified ActionCore message, length delimited. Does not implicitly {@link TW.IoTeX.Proto.ActionCore.verify|verify} messages.
                 * @param message ActionCore message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.IoTeX.Proto.IActionCore, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes an ActionCore message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns ActionCore
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.IoTeX.Proto.ActionCore;

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
                 * Encodes the specified Action message, length delimited. Does not implicitly {@link TW.IoTeX.Proto.Action.verify|verify} messages.
                 * @param message Action message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.IoTeX.Proto.IAction, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes an Action message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Action
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.IoTeX.Proto.Action;

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

                /** SigningInput execution */
                execution?: (TW.IoTeX.Proto.IExecution|null);
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

                /** SigningInput execution. */
                public execution?: (TW.IoTeX.Proto.IExecution|null);

                /** SigningInput action. */
                public action?: ("transfer"|"execution");

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.IoTeX.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.IoTeX.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.IoTeX.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.IoTeX.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.IoTeX.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.IoTeX.Proto.SigningOutput;

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
        }
    }

    /** Namespace NULS. */
    namespace NULS {

        /** Namespace Proto. */
        namespace Proto {

            /** Properties of a TransactionInput. */
            interface ITransactionInput {

                /** TransactionInput fromHash */
                fromHash?: (string|null);

                /** TransactionInput fromIndex */
                fromIndex?: (number|null);

                /** TransactionInput amount */
                amount?: (Long|null);

                /** TransactionInput lockTime */
                lockTime?: (Long|null);

                /** TransactionInput address */
                address?: (string|null);
            }

            /** Represents a TransactionInput. */
            class TransactionInput implements ITransactionInput {

                /**
                 * Constructs a new TransactionInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NULS.Proto.ITransactionInput);

                /** TransactionInput fromHash. */
                public fromHash: string;

                /** TransactionInput fromIndex. */
                public fromIndex: number;

                /** TransactionInput amount. */
                public amount: Long;

                /** TransactionInput lockTime. */
                public lockTime: Long;

                /** TransactionInput address. */
                public address: string;

                /**
                 * Creates a new TransactionInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionInput instance
                 */
                public static create(properties?: TW.NULS.Proto.ITransactionInput): TW.NULS.Proto.TransactionInput;

                /**
                 * Encodes the specified TransactionInput message. Does not implicitly {@link TW.NULS.Proto.TransactionInput.verify|verify} messages.
                 * @param message TransactionInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NULS.Proto.ITransactionInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified TransactionInput message, length delimited. Does not implicitly {@link TW.NULS.Proto.TransactionInput.verify|verify} messages.
                 * @param message TransactionInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.NULS.Proto.ITransactionInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NULS.Proto.TransactionInput;

                /**
                 * Decodes a TransactionInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TransactionInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.NULS.Proto.TransactionInput;

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
                public static fromObject(object: { [k: string]: any }): TW.NULS.Proto.TransactionInput;

                /**
                 * Creates a plain object from a TransactionInput message. Also converts values to other types if specified.
                 * @param message TransactionInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NULS.Proto.TransactionInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionInput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransactionOutput. */
            interface ITransactionOutput {

                /** TransactionOutput toAddress */
                toAddress?: (string|null);

                /** TransactionOutput amount */
                amount?: (Long|null);

                /** TransactionOutput lockTime */
                lockTime?: (Long|null);

                /** TransactionOutput index */
                index?: (number|null);
            }

            /** Represents a TransactionOutput. */
            class TransactionOutput implements ITransactionOutput {

                /**
                 * Constructs a new TransactionOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NULS.Proto.ITransactionOutput);

                /** TransactionOutput toAddress. */
                public toAddress: string;

                /** TransactionOutput amount. */
                public amount: Long;

                /** TransactionOutput lockTime. */
                public lockTime: Long;

                /** TransactionOutput index. */
                public index: number;

                /**
                 * Creates a new TransactionOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionOutput instance
                 */
                public static create(properties?: TW.NULS.Proto.ITransactionOutput): TW.NULS.Proto.TransactionOutput;

                /**
                 * Encodes the specified TransactionOutput message. Does not implicitly {@link TW.NULS.Proto.TransactionOutput.verify|verify} messages.
                 * @param message TransactionOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NULS.Proto.ITransactionOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified TransactionOutput message, length delimited. Does not implicitly {@link TW.NULS.Proto.TransactionOutput.verify|verify} messages.
                 * @param message TransactionOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.NULS.Proto.ITransactionOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NULS.Proto.TransactionOutput;

                /**
                 * Decodes a TransactionOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TransactionOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.NULS.Proto.TransactionOutput;

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
                public static fromObject(object: { [k: string]: any }): TW.NULS.Proto.TransactionOutput;

                /**
                 * Creates a plain object from a TransactionOutput message. Also converts values to other types if specified.
                 * @param message TransactionOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NULS.Proto.TransactionOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionOutput to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a Transaction. */
            interface ITransaction {

                /** Transaction fromAddress */
                fromAddress?: (string|null);

                /** Transaction toAddress */
                toAddress?: (string|null);

                /** Transaction amount */
                amount?: (Long|null);

                /** UTF-8 encode strings */
                remark?: (string|null);

                /** Transaction timestamp */
                timestamp?: (Long|null);

                /** Unspent input list */
                inputs?: (TW.NULS.Proto.ITransactionInput[]|null);

                /** Output list */
                outputs?: (TW.NULS.Proto.ITransactionOutput[]|null);
            }

            /** Represents a Transaction. */
            class Transaction implements ITransaction {

                /**
                 * Constructs a new Transaction.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NULS.Proto.ITransaction);

                /** Transaction fromAddress. */
                public fromAddress: string;

                /** Transaction toAddress. */
                public toAddress: string;

                /** Transaction amount. */
                public amount: Long;

                /** UTF-8 encode strings */
                public remark: string;

                /** Transaction timestamp. */
                public timestamp: Long;

                /** Unspent input list */
                public inputs: TW.NULS.Proto.ITransactionInput[];

                /** Output list */
                public outputs: TW.NULS.Proto.ITransactionOutput[];

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
                 * Encodes the specified Transaction message, length delimited. Does not implicitly {@link TW.NULS.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.NULS.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a Transaction message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.NULS.Proto.Transaction;

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

            /** Properties of a TransactionPurpose. */
            interface ITransactionPurpose {

                /** TransactionPurpose privateKey */
                privateKey?: (Uint8Array|null);

                /** TransactionPurpose fromAddress */
                fromAddress?: (string|null);

                /** TransactionPurpose toAddress */
                toAddress?: (string|null);

                /** TransactionPurpose amount */
                amount?: (Long|null);

                /** UTF-8 encode strings */
                remark?: (string|null);

                /** TransactionPurpose timestamp */
                timestamp?: (Long|null);

                /** Unspent input list */
                utxos?: (TW.NULS.Proto.ITransactionInput[]|null);

                /** TransactionPurpose useMaxAmount */
                useMaxAmount?: (boolean|null);
            }

            /** Represents a TransactionPurpose. */
            class TransactionPurpose implements ITransactionPurpose {

                /**
                 * Constructs a new TransactionPurpose.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NULS.Proto.ITransactionPurpose);

                /** TransactionPurpose privateKey. */
                public privateKey: Uint8Array;

                /** TransactionPurpose fromAddress. */
                public fromAddress: string;

                /** TransactionPurpose toAddress. */
                public toAddress: string;

                /** TransactionPurpose amount. */
                public amount: Long;

                /** UTF-8 encode strings */
                public remark: string;

                /** TransactionPurpose timestamp. */
                public timestamp: Long;

                /** Unspent input list */
                public utxos: TW.NULS.Proto.ITransactionInput[];

                /** TransactionPurpose useMaxAmount. */
                public useMaxAmount: boolean;

                /**
                 * Creates a new TransactionPurpose instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionPurpose instance
                 */
                public static create(properties?: TW.NULS.Proto.ITransactionPurpose): TW.NULS.Proto.TransactionPurpose;

                /**
                 * Encodes the specified TransactionPurpose message. Does not implicitly {@link TW.NULS.Proto.TransactionPurpose.verify|verify} messages.
                 * @param message TransactionPurpose message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NULS.Proto.ITransactionPurpose, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified TransactionPurpose message, length delimited. Does not implicitly {@link TW.NULS.Proto.TransactionPurpose.verify|verify} messages.
                 * @param message TransactionPurpose message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.NULS.Proto.ITransactionPurpose, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionPurpose message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionPurpose
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NULS.Proto.TransactionPurpose;

                /**
                 * Decodes a TransactionPurpose message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TransactionPurpose
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.NULS.Proto.TransactionPurpose;

                /**
                 * Verifies a TransactionPurpose message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates a TransactionPurpose message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns TransactionPurpose
                 */
                public static fromObject(object: { [k: string]: any }): TW.NULS.Proto.TransactionPurpose;

                /**
                 * Creates a plain object from a TransactionPurpose message. Also converts values to other types if specified.
                 * @param message TransactionPurpose
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NULS.Proto.TransactionPurpose, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionPurpose to JSON.
                 * @returns JSON object
                 */
                public toJSON(): { [k: string]: any };
            }

            /** Properties of a TransactionPlan. */
            interface ITransactionPlan {

                /** TransactionPlan privateKey */
                privateKey?: (Uint8Array|null);

                /** TransactionPlan fromAddress */
                fromAddress?: (string|null);

                /** TransactionPlan toAddress */
                toAddress?: (string|null);

                /** TransactionPlan amount */
                amount?: (Long|null);

                /** UTF-8 encode strings */
                remark?: (string|null);

                /** TransactionPlan timestamp */
                timestamp?: (Long|null);

                /** TransactionPlan fee */
                fee?: (Long|null);

                /** TransactionPlan change */
                change?: (Long|null);

                /** TransactionPlan availableAmount */
                availableAmount?: (Long|null);

                /** Selected Unspent input list */
                inputs?: (TW.NULS.Proto.ITransactionInput[]|null);

                /** TransactionPlan outputs */
                outputs?: (TW.NULS.Proto.ITransactionOutput[]|null);
            }

            /** Represents a TransactionPlan. */
            class TransactionPlan implements ITransactionPlan {

                /**
                 * Constructs a new TransactionPlan.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.NULS.Proto.ITransactionPlan);

                /** TransactionPlan privateKey. */
                public privateKey: Uint8Array;

                /** TransactionPlan fromAddress. */
                public fromAddress: string;

                /** TransactionPlan toAddress. */
                public toAddress: string;

                /** TransactionPlan amount. */
                public amount: Long;

                /** UTF-8 encode strings */
                public remark: string;

                /** TransactionPlan timestamp. */
                public timestamp: Long;

                /** TransactionPlan fee. */
                public fee: Long;

                /** TransactionPlan change. */
                public change: Long;

                /** TransactionPlan availableAmount. */
                public availableAmount: Long;

                /** Selected Unspent input list */
                public inputs: TW.NULS.Proto.ITransactionInput[];

                /** TransactionPlan outputs. */
                public outputs: TW.NULS.Proto.ITransactionOutput[];

                /**
                 * Creates a new TransactionPlan instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns TransactionPlan instance
                 */
                public static create(properties?: TW.NULS.Proto.ITransactionPlan): TW.NULS.Proto.TransactionPlan;

                /**
                 * Encodes the specified TransactionPlan message. Does not implicitly {@link TW.NULS.Proto.TransactionPlan.verify|verify} messages.
                 * @param message TransactionPlan message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.NULS.Proto.ITransactionPlan, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified TransactionPlan message, length delimited. Does not implicitly {@link TW.NULS.Proto.TransactionPlan.verify|verify} messages.
                 * @param message TransactionPlan message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.NULS.Proto.ITransactionPlan, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a TransactionPlan message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns TransactionPlan
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.NULS.Proto.TransactionPlan;

                /**
                 * Decodes a TransactionPlan message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TransactionPlan
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.NULS.Proto.TransactionPlan;

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
                public static fromObject(object: { [k: string]: any }): TW.NULS.Proto.TransactionPlan;

                /**
                 * Creates a plain object from a TransactionPlan message. Also converts values to other types if specified.
                 * @param message TransactionPlan
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.NULS.Proto.TransactionPlan, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this TransactionPlan to JSON.
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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.NULS.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.NULS.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.NULS.Proto.SigningOutput;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Nano.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Nano.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Nano.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Nano.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Nano.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Nano.Proto.SigningOutput;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Nebulas.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Nebulas.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Nebulas.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Nebulas.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Nebulas.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Nebulas.Proto.SigningOutput;

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
                 * Encodes the specified Data message, length delimited. Does not implicitly {@link TW.Nebulas.Proto.Data.verify|verify} messages.
                 * @param message Data message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Nebulas.Proto.IData, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a Data message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Data
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Nebulas.Proto.Data;

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
                 * Encodes the specified RawTransaction message, length delimited. Does not implicitly {@link TW.Nebulas.Proto.RawTransaction.verify|verify} messages.
                 * @param message RawTransaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Nebulas.Proto.IRawTransaction, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a RawTransaction message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns RawTransaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Nebulas.Proto.RawTransaction;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Nimiq.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Nimiq.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Nimiq.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Nimiq.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Nimiq.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Nimiq.Proto.SigningOutput;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Ontology.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Ontology.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Ontology.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Ontology.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Ontology.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Ontology.Proto.SigningOutput;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Ripple.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Ripple.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Ripple.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Ripple.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Ripple.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Ripple.Proto.SigningOutput;

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

    /** Namespace Semux. */
    namespace Semux {

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

                /** SigningInput nonce */
                nonce?: (Long|null);

                /** SigningInput timestamp */
                timestamp?: (Long|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Semux.Proto.ISigningInput);

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput destination. */
                public destination: string;

                /** SigningInput value. */
                public value: Long;

                /** SigningInput fee. */
                public fee: Long;

                /** SigningInput nonce. */
                public nonce: Long;

                /** SigningInput timestamp. */
                public timestamp: Long;

                /**
                 * Creates a new SigningInput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningInput instance
                 */
                public static create(properties?: TW.Semux.Proto.ISigningInput): TW.Semux.Proto.SigningInput;

                /**
                 * Encodes the specified SigningInput message. Does not implicitly {@link TW.Semux.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Semux.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Semux.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Semux.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Semux.Proto.SigningInput;

                /**
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Semux.Proto.SigningInput;

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
                public static fromObject(object: { [k: string]: any }): TW.Semux.Proto.SigningInput;

                /**
                 * Creates a plain object from a SigningInput message. Also converts values to other types if specified.
                 * @param message SigningInput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Semux.Proto.SigningInput, options?: $protobuf.IConversionOptions): { [k: string]: any };

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
                constructor(properties?: TW.Semux.Proto.ISigningOutput);

                /** SigningOutput encoded. */
                public encoded: Uint8Array;

                /**
                 * Creates a new SigningOutput instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns SigningOutput instance
                 */
                public static create(properties?: TW.Semux.Proto.ISigningOutput): TW.Semux.Proto.SigningOutput;

                /**
                 * Encodes the specified SigningOutput message. Does not implicitly {@link TW.Semux.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Semux.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Semux.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Semux.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Semux.Proto.SigningOutput;

                /**
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Semux.Proto.SigningOutput;

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
                public static fromObject(object: { [k: string]: any }): TW.Semux.Proto.SigningOutput;

                /**
                 * Creates a plain object from a SigningOutput message. Also converts values to other types if specified.
                 * @param message SigningOutput
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Semux.Proto.SigningOutput, options?: $protobuf.IConversionOptions): { [k: string]: any };

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
                 * Encodes the specified MemoVoid message, length delimited. Does not implicitly {@link TW.Stellar.Proto.MemoVoid.verify|verify} messages.
                 * @param message MemoVoid message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Stellar.Proto.IMemoVoid, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a MemoVoid message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns MemoVoid
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Stellar.Proto.MemoVoid;

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
                 * Encodes the specified MemoText message, length delimited. Does not implicitly {@link TW.Stellar.Proto.MemoText.verify|verify} messages.
                 * @param message MemoText message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Stellar.Proto.IMemoText, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a MemoText message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns MemoText
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Stellar.Proto.MemoText;

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
                 * Encodes the specified MemoId message, length delimited. Does not implicitly {@link TW.Stellar.Proto.MemoId.verify|verify} messages.
                 * @param message MemoId message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Stellar.Proto.IMemoId, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a MemoId message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns MemoId
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Stellar.Proto.MemoId;

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
                 * Encodes the specified MemoHash message, length delimited. Does not implicitly {@link TW.Stellar.Proto.MemoHash.verify|verify} messages.
                 * @param message MemoHash message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Stellar.Proto.IMemoHash, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a MemoHash message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns MemoHash
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Stellar.Proto.MemoHash;

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

                /** SigningInput amount */
                amount?: (Long|null);

                /** SigningInput fee */
                fee?: (number|null);

                /** SigningInput sequence */
                sequence?: (Long|null);

                /** SigningInput account */
                account?: (string|null);

                /** SigningInput destination */
                destination?: (string|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

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

                /** SigningInput operationType */
                operationType?: (TW.Stellar.Proto.SigningInput.OperationType|null);

                /** SigningInput passphrase */
                passphrase?: (string|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Stellar.Proto.ISigningInput);

                /** SigningInput amount. */
                public amount: Long;

                /** SigningInput fee. */
                public fee: number;

                /** SigningInput sequence. */
                public sequence: Long;

                /** SigningInput account. */
                public account: string;

                /** SigningInput destination. */
                public destination: string;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

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

                /** SigningInput operationType. */
                public operationType: TW.Stellar.Proto.SigningInput.OperationType;

                /** SigningInput passphrase. */
                public passphrase: string;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Stellar.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Stellar.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Stellar.Proto.SigningInput;

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

            namespace SigningInput {

                /** OperationType enum. */
                enum OperationType {
                    CREATE_ACCOUNT = 0,
                    PAYMENT = 1
                }
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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Stellar.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Stellar.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Stellar.Proto.SigningOutput;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Tezos.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tezos.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tezos.Proto.SigningInput;

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

                /** SigningOutput signedBytes */
                signedBytes?: (Uint8Array|null);
            }

            /** Represents a SigningOutput. */
            class SigningOutput implements ISigningOutput {

                /**
                 * Constructs a new SigningOutput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tezos.Proto.ISigningOutput);

                /** SigningOutput signedBytes. */
                public signedBytes: Uint8Array;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Tezos.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tezos.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tezos.Proto.SigningOutput;

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
                 * Encodes the specified OperationList message, length delimited. Does not implicitly {@link TW.Tezos.Proto.OperationList.verify|verify} messages.
                 * @param message OperationList message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tezos.Proto.IOperationList, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes an OperationList message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns OperationList
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tezos.Proto.OperationList;

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

                /** Operation originationOperationData */
                originationOperationData?: (TW.Tezos.Proto.IOriginationOperationData|null);

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

                /** Operation originationOperationData. */
                public originationOperationData?: (TW.Tezos.Proto.IOriginationOperationData|null);

                /** Operation delegationOperationData. */
                public delegationOperationData?: (TW.Tezos.Proto.IDelegationOperationData|null);

                /** Operation operationData. */
                public operationData?: ("revealOperationData"|"transactionOperationData"|"originationOperationData"|"delegationOperationData");

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
                 * Encodes the specified Operation message, length delimited. Does not implicitly {@link TW.Tezos.Proto.Operation.verify|verify} messages.
                 * @param message Operation message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tezos.Proto.IOperation, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes an Operation message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Operation
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tezos.Proto.Operation;

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
                    REVEAL = 7,
                    TRANSACTION = 8,
                    ORIGINATION = 9,
                    DELEGATION = 10
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
                 * Encodes the specified TransactionOperationData message, length delimited. Does not implicitly {@link TW.Tezos.Proto.TransactionOperationData.verify|verify} messages.
                 * @param message TransactionOperationData message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tezos.Proto.ITransactionOperationData, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a TransactionOperationData message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TransactionOperationData
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tezos.Proto.TransactionOperationData;

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
                 * Encodes the specified RevealOperationData message, length delimited. Does not implicitly {@link TW.Tezos.Proto.RevealOperationData.verify|verify} messages.
                 * @param message RevealOperationData message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tezos.Proto.IRevealOperationData, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a RevealOperationData message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns RevealOperationData
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tezos.Proto.RevealOperationData;

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

            /** Properties of an OriginationOperationData. */
            interface IOriginationOperationData {

                /** OriginationOperationData managerPubkey */
                managerPubkey?: (string|null);

                /** OriginationOperationData balance */
                balance?: (Long|null);
            }

            /** Represents an OriginationOperationData. */
            class OriginationOperationData implements IOriginationOperationData {

                /**
                 * Constructs a new OriginationOperationData.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Tezos.Proto.IOriginationOperationData);

                /** OriginationOperationData managerPubkey. */
                public managerPubkey: string;

                /** OriginationOperationData balance. */
                public balance: Long;

                /**
                 * Creates a new OriginationOperationData instance using the specified properties.
                 * @param [properties] Properties to set
                 * @returns OriginationOperationData instance
                 */
                public static create(properties?: TW.Tezos.Proto.IOriginationOperationData): TW.Tezos.Proto.OriginationOperationData;

                /**
                 * Encodes the specified OriginationOperationData message. Does not implicitly {@link TW.Tezos.Proto.OriginationOperationData.verify|verify} messages.
                 * @param message OriginationOperationData message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encode(message: TW.Tezos.Proto.IOriginationOperationData, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Encodes the specified OriginationOperationData message, length delimited. Does not implicitly {@link TW.Tezos.Proto.OriginationOperationData.verify|verify} messages.
                 * @param message OriginationOperationData message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tezos.Proto.IOriginationOperationData, writer?: $protobuf.Writer): $protobuf.Writer;

                /**
                 * Decodes an OriginationOperationData message from the specified reader or buffer.
                 * @param reader Reader or buffer to decode from
                 * @param [length] Message length if known beforehand
                 * @returns OriginationOperationData
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): TW.Tezos.Proto.OriginationOperationData;

                /**
                 * Decodes an OriginationOperationData message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns OriginationOperationData
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tezos.Proto.OriginationOperationData;

                /**
                 * Verifies an OriginationOperationData message.
                 * @param message Plain object to verify
                 * @returns `null` if valid, otherwise the reason why it is not
                 */
                public static verify(message: { [k: string]: any }): (string|null);

                /**
                 * Creates an OriginationOperationData message from a plain object. Also converts values to their respective internal types.
                 * @param object Plain object
                 * @returns OriginationOperationData
                 */
                public static fromObject(object: { [k: string]: any }): TW.Tezos.Proto.OriginationOperationData;

                /**
                 * Creates a plain object from an OriginationOperationData message. Also converts values to other types if specified.
                 * @param message OriginationOperationData
                 * @param [options] Conversion options
                 * @returns Plain object
                 */
                public static toObject(message: TW.Tezos.Proto.OriginationOperationData, options?: $protobuf.IConversionOptions): { [k: string]: any };

                /**
                 * Converts this OriginationOperationData to JSON.
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
                 * Encodes the specified DelegationOperationData message, length delimited. Does not implicitly {@link TW.Tezos.Proto.DelegationOperationData.verify|verify} messages.
                 * @param message DelegationOperationData message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tezos.Proto.IDelegationOperationData, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a DelegationOperationData message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns DelegationOperationData
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tezos.Proto.DelegationOperationData;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Theta.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Theta.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Theta.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Theta.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Theta.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Theta.Proto.SigningOutput;

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
                 * Encodes the specified TransferContract message, length delimited. Does not implicitly {@link TW.Tron.Proto.TransferContract.verify|verify} messages.
                 * @param message TransferContract message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tron.Proto.ITransferContract, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a TransferContract message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TransferContract
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tron.Proto.TransferContract;

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
                 * Encodes the specified TransferAssetContract message, length delimited. Does not implicitly {@link TW.Tron.Proto.TransferAssetContract.verify|verify} messages.
                 * @param message TransferAssetContract message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tron.Proto.ITransferAssetContract, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a TransferAssetContract message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns TransferAssetContract
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tron.Proto.TransferAssetContract;

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
                 * Encodes the specified BlockHeader message, length delimited. Does not implicitly {@link TW.Tron.Proto.BlockHeader.verify|verify} messages.
                 * @param message BlockHeader message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tron.Proto.IBlockHeader, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a BlockHeader message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns BlockHeader
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tron.Proto.BlockHeader;

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

                /** Transaction transfer */
                transfer?: (TW.Tron.Proto.ITransferContract|null);

                /** Transaction transferAsset */
                transferAsset?: (TW.Tron.Proto.ITransferAssetContract|null);
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

                /** Transaction transfer. */
                public transfer?: (TW.Tron.Proto.ITransferContract|null);

                /** Transaction transferAsset. */
                public transferAsset?: (TW.Tron.Proto.ITransferAssetContract|null);

                /** Transaction contractOneof. */
                public contractOneof?: ("transfer"|"transferAsset");

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
                 * Encodes the specified Transaction message, length delimited. Does not implicitly {@link TW.Tron.Proto.Transaction.verify|verify} messages.
                 * @param message Transaction message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tron.Proto.ITransaction, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a Transaction message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Transaction
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tron.Proto.Transaction;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Tron.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tron.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tron.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Tron.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Tron.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Tron.Proto.SigningOutput;

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
                 * Encodes the specified Clause message, length delimited. Does not implicitly {@link TW.VeChain.Proto.Clause.verify|verify} messages.
                 * @param message Clause message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.VeChain.Proto.IClause, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a Clause message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns Clause
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.VeChain.Proto.Clause;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.VeChain.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.VeChain.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.VeChain.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.VeChain.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.VeChain.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.VeChain.Proto.SigningOutput;

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

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput amount */
                amount?: (Long|null);

                /** SigningInput asset */
                asset?: (string|null);

                /** SigningInput fee */
                fee?: (Long|null);

                /** SigningInput feeAsset */
                feeAsset?: (string|null);

                /** SigningInput to */
                to?: (string|null);

                /** SigningInput attachment */
                attachment?: (Uint8Array|null);

                /** SigningInput timestamp */
                timestamp?: (Long|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);
            }

            /** Represents a SigningInput. */
            class SigningInput implements ISigningInput {

                /**
                 * Constructs a new SigningInput.
                 * @param [properties] Properties to set
                 */
                constructor(properties?: TW.Waves.Proto.ISigningInput);

                /** SigningInput amount. */
                public amount: Long;

                /** SigningInput asset. */
                public asset: string;

                /** SigningInput fee. */
                public fee: Long;

                /** SigningInput feeAsset. */
                public feeAsset: string;

                /** SigningInput to. */
                public to: string;

                /** SigningInput attachment. */
                public attachment: Uint8Array;

                /** SigningInput timestamp. */
                public timestamp: Long;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Waves.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Waves.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Waves.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Waves.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Waves.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Waves.Proto.SigningOutput;

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

            /** Properties of a SigningInput. */
            interface ISigningInput {

                /** SigningInput version */
                version?: (number|null);

                /** SigningInput nonce */
                nonce?: (Long|null);

                /** SigningInput toAddress */
                toAddress?: (string|null);

                /** SigningInput amount */
                amount?: (Uint8Array|null);

                /** SigningInput gasPrice */
                gasPrice?: (Uint8Array|null);

                /** SigningInput gasLimit */
                gasLimit?: (Long|null);

                /** SigningInput privateKey */
                privateKey?: (Uint8Array|null);

                /** SigningInput code */
                code?: (Uint8Array|null);

                /** SigningInput data */
                data?: (Uint8Array|null);
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

                /** SigningInput toAddress. */
                public toAddress: string;

                /** SigningInput amount. */
                public amount: Uint8Array;

                /** SigningInput gasPrice. */
                public gasPrice: Uint8Array;

                /** SigningInput gasLimit. */
                public gasLimit: Long;

                /** SigningInput privateKey. */
                public privateKey: Uint8Array;

                /** SigningInput code. */
                public code: Uint8Array;

                /** SigningInput data. */
                public data: Uint8Array;

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
                 * Encodes the specified SigningInput message, length delimited. Does not implicitly {@link TW.Zilliqa.Proto.SigningInput.verify|verify} messages.
                 * @param message SigningInput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Zilliqa.Proto.ISigningInput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningInput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningInput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Zilliqa.Proto.SigningInput;

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
                 * Encodes the specified SigningOutput message, length delimited. Does not implicitly {@link TW.Zilliqa.Proto.SigningOutput.verify|verify} messages.
                 * @param message SigningOutput message or plain object to encode
                 * @param [writer] Writer to encode to
                 * @returns Writer
                 */
                public static encodeDelimited(message: TW.Zilliqa.Proto.ISigningOutput, writer?: $protobuf.Writer): $protobuf.Writer;

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
                 * Decodes a SigningOutput message from the specified reader or buffer, length delimited.
                 * @param reader Reader or buffer to decode from
                 * @returns SigningOutput
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): TW.Zilliqa.Proto.SigningOutput;

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

/** Namespace google. */
export namespace google {

    /** Namespace protobuf. */
    namespace protobuf {

        /** Properties of an Any. */
        interface IAny {

            /** Any type_url */
            type_url?: (string|null);

            /** Any value */
            value?: (Uint8Array|null);
        }

        /** Represents an Any. */
        class Any implements IAny {

            /**
             * Constructs a new Any.
             * @param [properties] Properties to set
             */
            constructor(properties?: google.protobuf.IAny);

            /** Any type_url. */
            public type_url: string;

            /** Any value. */
            public value: Uint8Array;

            /**
             * Creates a new Any instance using the specified properties.
             * @param [properties] Properties to set
             * @returns Any instance
             */
            public static create(properties?: google.protobuf.IAny): google.protobuf.Any;

            /**
             * Encodes the specified Any message. Does not implicitly {@link google.protobuf.Any.verify|verify} messages.
             * @param message Any message or plain object to encode
             * @param [writer] Writer to encode to
             * @returns Writer
             */
            public static encode(message: google.protobuf.IAny, writer?: $protobuf.Writer): $protobuf.Writer;

            /**
             * Encodes the specified Any message, length delimited. Does not implicitly {@link google.protobuf.Any.verify|verify} messages.
             * @param message Any message or plain object to encode
             * @param [writer] Writer to encode to
             * @returns Writer
             */
            public static encodeDelimited(message: google.protobuf.IAny, writer?: $protobuf.Writer): $protobuf.Writer;

            /**
             * Decodes an Any message from the specified reader or buffer.
             * @param reader Reader or buffer to decode from
             * @param [length] Message length if known beforehand
             * @returns Any
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): google.protobuf.Any;

            /**
             * Decodes an Any message from the specified reader or buffer, length delimited.
             * @param reader Reader or buffer to decode from
             * @returns Any
             * @throws {Error} If the payload is not a reader or valid buffer
             * @throws {$protobuf.util.ProtocolError} If required fields are missing
             */
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): google.protobuf.Any;

            /**
             * Verifies an Any message.
             * @param message Plain object to verify
             * @returns `null` if valid, otherwise the reason why it is not
             */
            public static verify(message: { [k: string]: any }): (string|null);

            /**
             * Creates an Any message from a plain object. Also converts values to their respective internal types.
             * @param object Plain object
             * @returns Any
             */
            public static fromObject(object: { [k: string]: any }): google.protobuf.Any;

            /**
             * Creates a plain object from an Any message. Also converts values to other types if specified.
             * @param message Any
             * @param [options] Conversion options
             * @returns Plain object
             */
            public static toObject(message: google.protobuf.Any, options?: $protobuf.IConversionOptions): { [k: string]: any };

            /**
             * Converts this Any to JSON.
             * @returns JSON object
             */
            public toJSON(): { [k: string]: any };
        }
    }
}
