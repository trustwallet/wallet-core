package core

// #cgo CFLAGS: -I../../../include
// #include <TrustWalletCore/TWCurve.h>
import "C"

type CurveType uint32

const (
	CurveSECP256k1              CurveType = C.TWCurveSECP256k1
	CurveED25519                CurveType = C.TWCurveED25519
	CurveCurve25519             CurveType = C.TWCurveCurve25519
	CurveED25519ExtendedCardano CurveType = C.TWCurveED25519ExtendedCardano
	CurveNone                   CurveType = C.TWCurveNone
)
