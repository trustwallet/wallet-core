// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.28.0
// 	protoc        v3.19.2
// source: Common.proto

package common

import (
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
	sync "sync"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

type SigningError int32

const (
	SigningError_OK SigningError = 0 // OK
	// chain-generic, generic
	SigningError_Error_general  SigningError = 1
	SigningError_Error_internal SigningError = 2
	// chain-generic, input
	SigningError_Error_low_balance           SigningError = 3
	SigningError_Error_zero_amount_requested SigningError = 4 // Requested amount is zero
	SigningError_Error_missing_private_key   SigningError = 5
	SigningError_Error_invalid_private_key   SigningError = 15
	SigningError_Error_invalid_address       SigningError = 16
	SigningError_Error_invalid_utxo          SigningError = 17
	SigningError_Error_invalid_utxo_amount   SigningError = 18
	// chain-generic, fee
	SigningError_Error_wrong_fee SigningError = 6
	// chain-generic, signing
	SigningError_Error_signing    SigningError = 7
	SigningError_Error_tx_too_big SigningError = 8 // [NEO] Transaction too big, fee in GAS needed or try send by parts
	// UTXO-chain specific, inputs
	SigningError_Error_missing_input_utxos SigningError = 9  // No UTXOs provided [BTC]
	SigningError_Error_not_enough_utxos    SigningError = 10 // Not enough non-dust input UTXOs to cover requested amount (dust UTXOs are filtered out) [BTC]
	// UTXO-chain specific, script
	SigningError_Error_script_redeem          SigningError = 11 // [BTC] Missing redeem script
	SigningError_Error_script_output          SigningError = 12 // [BTC] Invalid output script
	SigningError_Error_script_witness_program SigningError = 13 // [BTC] Unrecognized witness program
	SigningError_Error_invalid_memo           SigningError = 14 // e.g. [XRP] Invalid tag
	SigningError_Error_input_parse            SigningError = 19 // e.g. Invalid input data
	SigningError_Error_no_support_n2n         SigningError = 20 // e.g. Not support n2n transaction
	SigningError_Error_signatures_count       SigningError = 21 // Incorrect count of signatures passed to compile
)

// Enum value maps for SigningError.
var (
	SigningError_name = map[int32]string{
		0:  "OK",
		1:  "Error_general",
		2:  "Error_internal",
		3:  "Error_low_balance",
		4:  "Error_zero_amount_requested",
		5:  "Error_missing_private_key",
		15: "Error_invalid_private_key",
		16: "Error_invalid_address",
		17: "Error_invalid_utxo",
		18: "Error_invalid_utxo_amount",
		6:  "Error_wrong_fee",
		7:  "Error_signing",
		8:  "Error_tx_too_big",
		9:  "Error_missing_input_utxos",
		10: "Error_not_enough_utxos",
		11: "Error_script_redeem",
		12: "Error_script_output",
		13: "Error_script_witness_program",
		14: "Error_invalid_memo",
		19: "Error_input_parse",
		20: "Error_no_support_n2n",
		21: "Error_signatures_count",
	}
	SigningError_value = map[string]int32{
		"OK":                           0,
		"Error_general":                1,
		"Error_internal":               2,
		"Error_low_balance":            3,
		"Error_zero_amount_requested":  4,
		"Error_missing_private_key":    5,
		"Error_invalid_private_key":    15,
		"Error_invalid_address":        16,
		"Error_invalid_utxo":           17,
		"Error_invalid_utxo_amount":    18,
		"Error_wrong_fee":              6,
		"Error_signing":                7,
		"Error_tx_too_big":             8,
		"Error_missing_input_utxos":    9,
		"Error_not_enough_utxos":       10,
		"Error_script_redeem":          11,
		"Error_script_output":          12,
		"Error_script_witness_program": 13,
		"Error_invalid_memo":           14,
		"Error_input_parse":            19,
		"Error_no_support_n2n":         20,
		"Error_signatures_count":       21,
	}
)

func (x SigningError) Enum() *SigningError {
	p := new(SigningError)
	*p = x
	return p
}

func (x SigningError) String() string {
	return protoimpl.X.EnumStringOf(x.Descriptor(), protoreflect.EnumNumber(x))
}

func (SigningError) Descriptor() protoreflect.EnumDescriptor {
	return file_Common_proto_enumTypes[0].Descriptor()
}

func (SigningError) Type() protoreflect.EnumType {
	return &file_Common_proto_enumTypes[0]
}

func (x SigningError) Number() protoreflect.EnumNumber {
	return protoreflect.EnumNumber(x)
}

// Deprecated: Use SigningError.Descriptor instead.
func (SigningError) EnumDescriptor() ([]byte, []int) {
	return file_Common_proto_rawDescGZIP(), []int{0}
}

var File_Common_proto protoreflect.FileDescriptor

var file_Common_proto_rawDesc = []byte{
	0x0a, 0x0c, 0x43, 0x6f, 0x6d, 0x6d, 0x6f, 0x6e, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x0f,
	0x54, 0x57, 0x2e, 0x43, 0x6f, 0x6d, 0x6d, 0x6f, 0x6e, 0x2e, 0x50, 0x72, 0x6f, 0x74, 0x6f, 0x2a,
	0xb7, 0x04, 0x0a, 0x0c, 0x53, 0x69, 0x67, 0x6e, 0x69, 0x6e, 0x67, 0x45, 0x72, 0x72, 0x6f, 0x72,
	0x12, 0x06, 0x0a, 0x02, 0x4f, 0x4b, 0x10, 0x00, 0x12, 0x11, 0x0a, 0x0d, 0x45, 0x72, 0x72, 0x6f,
	0x72, 0x5f, 0x67, 0x65, 0x6e, 0x65, 0x72, 0x61, 0x6c, 0x10, 0x01, 0x12, 0x12, 0x0a, 0x0e, 0x45,
	0x72, 0x72, 0x6f, 0x72, 0x5f, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x61, 0x6c, 0x10, 0x02, 0x12,
	0x15, 0x0a, 0x11, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f, 0x6c, 0x6f, 0x77, 0x5f, 0x62, 0x61, 0x6c,
	0x61, 0x6e, 0x63, 0x65, 0x10, 0x03, 0x12, 0x1f, 0x0a, 0x1b, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f,
	0x7a, 0x65, 0x72, 0x6f, 0x5f, 0x61, 0x6d, 0x6f, 0x75, 0x6e, 0x74, 0x5f, 0x72, 0x65, 0x71, 0x75,
	0x65, 0x73, 0x74, 0x65, 0x64, 0x10, 0x04, 0x12, 0x1d, 0x0a, 0x19, 0x45, 0x72, 0x72, 0x6f, 0x72,
	0x5f, 0x6d, 0x69, 0x73, 0x73, 0x69, 0x6e, 0x67, 0x5f, 0x70, 0x72, 0x69, 0x76, 0x61, 0x74, 0x65,
	0x5f, 0x6b, 0x65, 0x79, 0x10, 0x05, 0x12, 0x1d, 0x0a, 0x19, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f,
	0x69, 0x6e, 0x76, 0x61, 0x6c, 0x69, 0x64, 0x5f, 0x70, 0x72, 0x69, 0x76, 0x61, 0x74, 0x65, 0x5f,
	0x6b, 0x65, 0x79, 0x10, 0x0f, 0x12, 0x19, 0x0a, 0x15, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f, 0x69,
	0x6e, 0x76, 0x61, 0x6c, 0x69, 0x64, 0x5f, 0x61, 0x64, 0x64, 0x72, 0x65, 0x73, 0x73, 0x10, 0x10,
	0x12, 0x16, 0x0a, 0x12, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f, 0x69, 0x6e, 0x76, 0x61, 0x6c, 0x69,
	0x64, 0x5f, 0x75, 0x74, 0x78, 0x6f, 0x10, 0x11, 0x12, 0x1d, 0x0a, 0x19, 0x45, 0x72, 0x72, 0x6f,
	0x72, 0x5f, 0x69, 0x6e, 0x76, 0x61, 0x6c, 0x69, 0x64, 0x5f, 0x75, 0x74, 0x78, 0x6f, 0x5f, 0x61,
	0x6d, 0x6f, 0x75, 0x6e, 0x74, 0x10, 0x12, 0x12, 0x13, 0x0a, 0x0f, 0x45, 0x72, 0x72, 0x6f, 0x72,
	0x5f, 0x77, 0x72, 0x6f, 0x6e, 0x67, 0x5f, 0x66, 0x65, 0x65, 0x10, 0x06, 0x12, 0x11, 0x0a, 0x0d,
	0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f, 0x73, 0x69, 0x67, 0x6e, 0x69, 0x6e, 0x67, 0x10, 0x07, 0x12,
	0x14, 0x0a, 0x10, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f, 0x74, 0x78, 0x5f, 0x74, 0x6f, 0x6f, 0x5f,
	0x62, 0x69, 0x67, 0x10, 0x08, 0x12, 0x1d, 0x0a, 0x19, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f, 0x6d,
	0x69, 0x73, 0x73, 0x69, 0x6e, 0x67, 0x5f, 0x69, 0x6e, 0x70, 0x75, 0x74, 0x5f, 0x75, 0x74, 0x78,
	0x6f, 0x73, 0x10, 0x09, 0x12, 0x1a, 0x0a, 0x16, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f, 0x6e, 0x6f,
	0x74, 0x5f, 0x65, 0x6e, 0x6f, 0x75, 0x67, 0x68, 0x5f, 0x75, 0x74, 0x78, 0x6f, 0x73, 0x10, 0x0a,
	0x12, 0x17, 0x0a, 0x13, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f, 0x73, 0x63, 0x72, 0x69, 0x70, 0x74,
	0x5f, 0x72, 0x65, 0x64, 0x65, 0x65, 0x6d, 0x10, 0x0b, 0x12, 0x17, 0x0a, 0x13, 0x45, 0x72, 0x72,
	0x6f, 0x72, 0x5f, 0x73, 0x63, 0x72, 0x69, 0x70, 0x74, 0x5f, 0x6f, 0x75, 0x74, 0x70, 0x75, 0x74,
	0x10, 0x0c, 0x12, 0x20, 0x0a, 0x1c, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f, 0x73, 0x63, 0x72, 0x69,
	0x70, 0x74, 0x5f, 0x77, 0x69, 0x74, 0x6e, 0x65, 0x73, 0x73, 0x5f, 0x70, 0x72, 0x6f, 0x67, 0x72,
	0x61, 0x6d, 0x10, 0x0d, 0x12, 0x16, 0x0a, 0x12, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f, 0x69, 0x6e,
	0x76, 0x61, 0x6c, 0x69, 0x64, 0x5f, 0x6d, 0x65, 0x6d, 0x6f, 0x10, 0x0e, 0x12, 0x15, 0x0a, 0x11,
	0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f, 0x69, 0x6e, 0x70, 0x75, 0x74, 0x5f, 0x70, 0x61, 0x72, 0x73,
	0x65, 0x10, 0x13, 0x12, 0x18, 0x0a, 0x14, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f, 0x6e, 0x6f, 0x5f,
	0x73, 0x75, 0x70, 0x70, 0x6f, 0x72, 0x74, 0x5f, 0x6e, 0x32, 0x6e, 0x10, 0x14, 0x12, 0x1a, 0x0a,
	0x16, 0x45, 0x72, 0x72, 0x6f, 0x72, 0x5f, 0x73, 0x69, 0x67, 0x6e, 0x61, 0x74, 0x75, 0x72, 0x65,
	0x73, 0x5f, 0x63, 0x6f, 0x75, 0x6e, 0x74, 0x10, 0x15, 0x42, 0x55, 0x0a, 0x15, 0x77, 0x61, 0x6c,
	0x6c, 0x65, 0x74, 0x2e, 0x63, 0x6f, 0x72, 0x65, 0x2e, 0x6a, 0x6e, 0x69, 0x2e, 0x70, 0x72, 0x6f,
	0x74, 0x6f, 0x5a, 0x3c, 0x67, 0x69, 0x74, 0x68, 0x75, 0x62, 0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x62,
	0x69, 0x6e, 0x61, 0x6e, 0x63, 0x65, 0x2d, 0x63, 0x68, 0x61, 0x69, 0x6e, 0x2f, 0x63, 0x68, 0x61,
	0x69, 0x6e, 0x2d, 0x69, 0x6e, 0x74, 0x65, 0x67, 0x72, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x62,
	0x6c, 0x6f, 0x63, 0x6b, 0x63, 0x68, 0x61, 0x69, 0x6e, 0x2f, 0x63, 0x6f, 0x6d, 0x6d, 0x6f, 0x6e,
	0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var (
	file_Common_proto_rawDescOnce sync.Once
	file_Common_proto_rawDescData = file_Common_proto_rawDesc
)

func file_Common_proto_rawDescGZIP() []byte {
	file_Common_proto_rawDescOnce.Do(func() {
		file_Common_proto_rawDescData = protoimpl.X.CompressGZIP(file_Common_proto_rawDescData)
	})
	return file_Common_proto_rawDescData
}

var file_Common_proto_enumTypes = make([]protoimpl.EnumInfo, 1)
var file_Common_proto_goTypes = []interface{}{
	(SigningError)(0), // 0: TW.Common.Proto.SigningError
}
var file_Common_proto_depIdxs = []int32{
	0, // [0:0] is the sub-list for method output_type
	0, // [0:0] is the sub-list for method input_type
	0, // [0:0] is the sub-list for extension type_name
	0, // [0:0] is the sub-list for extension extendee
	0, // [0:0] is the sub-list for field type_name
}

func init() { file_Common_proto_init() }
func file_Common_proto_init() {
	if File_Common_proto != nil {
		return
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_Common_proto_rawDesc,
			NumEnums:      1,
			NumMessages:   0,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_Common_proto_goTypes,
		DependencyIndexes: file_Common_proto_depIdxs,
		EnumInfos:         file_Common_proto_enumTypes,
	}.Build()
	File_Common_proto = out.File
	file_Common_proto_rawDesc = nil
	file_Common_proto_goTypes = nil
	file_Common_proto_depIdxs = nil
}
