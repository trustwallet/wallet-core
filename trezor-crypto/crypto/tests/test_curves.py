#!/usr/bin/py.test
import binascii
import ctypes as c
import hashlib
import os
import random

import curve25519
import ecdsa
import pytest


def bytes2num(s):
    res = 0
    for i, b in enumerate(reversed(bytearray(s))):
        res += b << (i * 8)
    return res


curves = {"secp256k1": ecdsa.curves.SECP256k1}


class Point:
    def __init__(self, name, x, y):
        self.curve = name
        self.x = x
        self.y = y


points = [
    Point(
        "secp256k1",
        0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798,
        0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8,
    ),
    Point(
        "secp256k1",
        0x1,
        0x4218f20ae6c646b363db68605822fb14264ca8d2587fdd6fbc750d587e76a7ee,
    ),
    Point(
        "secp256k1",
        0x2,
        0x66fbe727b2ba09e09f5a98d70a5efce8424c5fa425bbda1c511f860657b8535e,
    ),
    Point(
        "secp256k1",
        0x1b,
        0x1adcea1cf831b0ad1653e769d1a229091d0cc68d4b0328691b9caacc76e37c90,
    ),
]

random_iters = int(os.environ.get("ITERS", 1))

DIR = os.path.abspath(os.path.dirname(__file__))
lib = c.cdll.LoadLibrary(os.path.join(DIR, "libtrezor-crypto.so"))


class curve_info(c.Structure):
    _fields_ = [("bip32_name", c.c_char_p), ("params", c.c_void_p)]


lib.get_curve_by_name.restype = c.POINTER(curve_info)

BIGNUM = c.c_uint32 * 9


class Random(random.Random):
    def randbytes(self, n):
        buf = (c.c_uint8 * n)()
        for i in range(n):
            buf[i] = self.randrange(0, 256)
        return buf

    def randpoint(self, curve):
        k = self.randrange(0, curve.order)
        return k * curve.generator


def int2bn(x, bn_type=BIGNUM):
    b = bn_type()
    b._int = x
    for i in range(len(b)):
        b[i] = x % (1 << 30)
        x = x >> 30
    return b


def bn2int(b):
    x = 0
    for i in range(len(b)):
        x += b[i] << (30 * i)
    return x


@pytest.fixture(params=range(random_iters))
def r(request):
    seed = request.param
    return Random(seed + int(os.environ.get("SEED", 0)))


@pytest.fixture(params=list(sorted(curves)))
def curve(request):
    name = request.param
    curve_ptr = lib.get_curve_by_name(bytes(name, "ascii")).contents.params
    assert curve_ptr, "curve {} not found".format(name)
    curve_obj = curves[name]
    curve_obj.ptr = c.c_void_p(curve_ptr)
    curve_obj.p = curve_obj.curve.p()  # shorthand
    return curve_obj


@pytest.fixture(params=points)
def point(request):
    name = request.param.curve
    curve_ptr = lib.get_curve_by_name(bytes(name, "ascii")).contents.params
    assert curve_ptr, "curve {} not found".format(name)
    curve_obj = curves[name]
    curve_obj.ptr = c.c_void_p(curve_ptr)
    curve_obj.p = ecdsa.ellipticcurve.Point(
        curve_obj.curve, request.param.x, request.param.y
    )
    return curve_obj


def test_inverse(curve, r):
    x = r.randrange(1, curve.p)
    y = int2bn(x)
    lib.bn_inverse(y, int2bn(curve.p))
    y = bn2int(y)
    y_ = ecdsa.numbertheory.inverse_mod(x, curve.p)
    assert y == y_


def test_is_less(curve, r):
    x = r.randrange(0, curve.p)
    y = r.randrange(0, curve.p)
    x_ = int2bn(x)
    y_ = int2bn(y)

    res = lib.bn_is_less(x_, y_)
    assert res == (x < y)

    res = lib.bn_is_less(y_, x_)
    assert res == (y < x)


def test_is_equal(curve, r):
    x = r.randrange(0, curve.p)
    y = r.randrange(0, curve.p)
    x_ = int2bn(x)
    y_ = int2bn(y)

    assert lib.bn_is_equal(x_, y_) == (x == y)
    assert lib.bn_is_equal(x_, x_) == 1
    assert lib.bn_is_equal(y_, y_) == 1


def test_is_zero(curve, r):
    x = r.randrange(0, curve.p)
    assert lib.bn_is_zero(int2bn(x)) == (not x)


def test_simple_comparisons():
    assert lib.bn_is_zero(int2bn(0)) == 1
    assert lib.bn_is_zero(int2bn(1)) == 0

    assert lib.bn_is_less(int2bn(0), int2bn(0)) == 0
    assert lib.bn_is_less(int2bn(1), int2bn(0)) == 0
    assert lib.bn_is_less(int2bn(0), int2bn(1)) == 1

    assert lib.bn_is_equal(int2bn(0), int2bn(0)) == 1
    assert lib.bn_is_equal(int2bn(1), int2bn(0)) == 0
    assert lib.bn_is_equal(int2bn(0), int2bn(1)) == 0


def test_mult_half(curve, r):
    x = r.randrange(0, 2 * curve.p)
    y = int2bn(x)
    lib.bn_mult_half(y, int2bn(curve.p))
    y = bn2int(y)
    if y >= curve.p:
        y -= curve.p
    half = ecdsa.numbertheory.inverse_mod(2, curve.p)
    assert y == (x * half) % curve.p


def test_subtractmod(curve, r):
    x = r.randrange(0, 2 ** 256)
    y = r.randrange(0, 2 ** 256)
    z = int2bn(0)
    lib.bn_subtractmod(int2bn(x), int2bn(y), z, int2bn(curve.p))
    z = bn2int(z)
    z_ = x + 2 * curve.p - y
    assert z == z_


def test_subtract2(r):
    x = r.randrange(0, 2 ** 256)
    y = r.randrange(0, 2 ** 256)
    x, y = max(x, y), min(x, y)
    z = int2bn(0)
    lib.bn_subtract(int2bn(x), int2bn(y), z)
    z = bn2int(z)
    z_ = x - y
    assert z == z_


def test_add(curve, r):
    x = r.randrange(0, 2 ** 256)
    y = r.randrange(0, 2 ** 256)
    z_ = x + y
    z = int2bn(x)
    lib.bn_add(z, int2bn(y))
    z = bn2int(z)

    assert z == z_


def test_addmod(curve, r):
    x = r.randrange(0, 2 ** 256)
    y = r.randrange(0, 2 ** 256)
    z_ = (x + y) % curve.p
    z = int2bn(x)
    lib.bn_addmod(z, int2bn(y), int2bn(curve.p))
    z = bn2int(z)
    if z >= curve.p:
        z = z - curve.p
    assert z == z_


def test_multiply(curve, r):
    k = r.randrange(0, 2 * curve.p)
    x = r.randrange(0, 2 * curve.p)
    z = (k * x) % curve.p
    k = int2bn(k)
    z_ = int2bn(x)
    p_ = int2bn(curve.p)
    lib.bn_multiply(k, z_, p_)
    z_ = bn2int(z_)
    assert z_ < 2 * curve.p
    if z_ >= curve.p:
        z_ = z_ - curve.p
    assert z_ == z


def test_multiply1(curve, r):
    k = r.randrange(0, 2 * curve.p)
    x = r.randrange(0, 2 * curve.p)
    kx = k * x
    res = int2bn(0, bn_type=(c.c_uint32 * 18))
    lib.bn_multiply_long(int2bn(k), int2bn(x), res)
    res = bn2int(res)
    assert res == kx


def test_multiply2(curve, r):
    x = int2bn(0)
    s = r.randrange(0, 2 ** 526)
    res = int2bn(s, bn_type=(c.c_uint32 * 18))
    prime = int2bn(curve.p)
    lib.bn_multiply_reduce(x, res, prime)

    x = bn2int(x) % curve.p
    x_ = s % curve.p

    assert x == x_


def test_fast_mod(curve, r):
    x = r.randrange(0, 128 * curve.p)
    y = int2bn(x)
    lib.bn_fast_mod(y, int2bn(curve.p))
    y = bn2int(y)
    assert y < 2 * curve.p
    if y >= curve.p:
        y -= curve.p
    assert x % curve.p == y


def test_mod(curve, r):
    x = r.randrange(0, 2 * curve.p)
    y = int2bn(x)
    lib.bn_mod(y, int2bn(curve.p))
    assert bn2int(y) == x % curve.p


def test_mod_specific(curve):
    p = curve.p
    for x in [0, 1, 2, p - 2, p - 1, p, p + 1, p + 2, 2 * p - 2, 2 * p - 1]:
        y = int2bn(x)
        lib.bn_mod(y, int2bn(curve.p))
        assert bn2int(y) == x % p


POINT = BIGNUM * 2


def to_POINT(p):
    return POINT(int2bn(p.x()), int2bn(p.y()))


def from_POINT(p):
    return (bn2int(p[0]), bn2int(p[1]))


JACOBIAN = BIGNUM * 3


def to_JACOBIAN(jp):
    return JACOBIAN(int2bn(jp[0]), int2bn(jp[1]), int2bn(jp[2]))


def from_JACOBIAN(p):
    return (bn2int(p[0]), bn2int(p[1]), bn2int(p[2]))


def test_point_multiply(curve, r):
    p = r.randpoint(curve)
    k = r.randrange(0, 2 ** 256)
    kp = k * p
    res = POINT(int2bn(0), int2bn(0))
    lib.point_multiply(curve.ptr, int2bn(k), to_POINT(p), res)
    res = from_POINT(res)
    assert res == (kp.x(), kp.y())


def test_point_add(curve, r):
    p1 = r.randpoint(curve)
    p2 = r.randpoint(curve)
    # print '-' * 80
    q = p1 + p2
    q1 = to_POINT(p1)
    q2 = to_POINT(p2)
    lib.point_add(curve.ptr, q1, q2)
    q_ = from_POINT(q2)
    assert q_ == (q.x(), q.y())


def test_point_double(curve, r):
    p = r.randpoint(curve)
    q = p.double()
    q_ = to_POINT(p)
    lib.point_double(curve.ptr, q_)
    q_ = from_POINT(q_)
    assert q_ == (q.x(), q.y())


def test_point_to_jacobian(curve, r):
    p = r.randpoint(curve)
    jp = JACOBIAN()
    lib.curve_to_jacobian(to_POINT(p), jp, int2bn(curve.p))
    jx, jy, jz = from_JACOBIAN(jp)
    assert jx % curve.p == (p.x() * jz ** 2) % curve.p
    assert jy % curve.p == (p.y() * jz ** 3) % curve.p

    q = POINT()
    lib.jacobian_to_curve(jp, q, int2bn(curve.p))
    q = from_POINT(q)
    assert q == (p.x(), p.y())


def test_cond_negate(curve, r):
    x = r.randrange(0, curve.p)
    a = int2bn(x)
    lib.conditional_negate(0, a, int2bn(curve.p))
    assert bn2int(a) == x
    lib.conditional_negate(-1, a, int2bn(curve.p))
    assert bn2int(a) == 2 * curve.p - x


def test_jacobian_add(curve, r):
    p1 = r.randpoint(curve)
    p2 = r.randpoint(curve)
    prime = int2bn(curve.p)
    q = POINT()
    jp2 = JACOBIAN()
    lib.curve_to_jacobian(to_POINT(p2), jp2, prime)
    lib.point_jacobian_add(to_POINT(p1), jp2, curve.ptr)
    lib.jacobian_to_curve(jp2, q, prime)
    q = from_POINT(q)
    p_ = p1 + p2
    assert (p_.x(), p_.y()) == q


def test_jacobian_add_double(curve, r):
    p1 = r.randpoint(curve)
    p2 = p1
    prime = int2bn(curve.p)
    q = POINT()
    jp2 = JACOBIAN()
    lib.curve_to_jacobian(to_POINT(p2), jp2, prime)
    lib.point_jacobian_add(to_POINT(p1), jp2, curve.ptr)
    lib.jacobian_to_curve(jp2, q, prime)
    q = from_POINT(q)
    p_ = p1 + p2
    assert (p_.x(), p_.y()) == q


def test_jacobian_double(curve, r):
    p = r.randpoint(curve)
    p2 = p.double()
    prime = int2bn(curve.p)
    q = POINT()
    jp = JACOBIAN()
    lib.curve_to_jacobian(to_POINT(p), jp, prime)
    lib.point_jacobian_double(jp, curve.ptr)
    lib.jacobian_to_curve(jp, q, prime)
    q = from_POINT(q)
    assert (p2.x(), p2.y()) == q


def sigdecode(sig, _):
    return map(bytes2num, [sig[:32], sig[32:]])


def test_sign(curve, r):
    priv = r.randbytes(32)
    digest = r.randbytes(32)
    sig = r.randbytes(64)

    lib.ecdsa_sign_digest(curve.ptr, priv, digest, sig, c.c_void_p(0), c.c_void_p(0))

    exp = bytes2num(priv)
    sk = ecdsa.SigningKey.from_secret_exponent(exp, curve, hashfunc=hashlib.sha256)
    vk = sk.get_verifying_key()

    sig_ref = sk.sign_digest_deterministic(
        digest, hashfunc=hashlib.sha256, sigencode=ecdsa.util.sigencode_string_canonize
    )
    assert binascii.hexlify(sig) == binascii.hexlify(sig_ref)

    assert vk.verify_digest(sig, digest, sigdecode)


def test_validate_pubkey(curve, r):
    p = r.randpoint(curve)
    assert lib.ecdsa_validate_pubkey(curve.ptr, to_POINT(p))


def test_validate_pubkey_direct(point):
    assert lib.ecdsa_validate_pubkey(point.ptr, to_POINT(point.p))


def test_curve25519(r):
    sec1 = bytes(bytearray(r.randbytes(32)))
    sec2 = bytes(bytearray(r.randbytes(32)))
    pub1 = curve25519.Private(sec1).get_public()
    pub2 = curve25519.Private(sec2).get_public()

    session1 = r.randbytes(32)
    lib.curve25519_scalarmult(session1, sec2, pub1.public)
    session2 = r.randbytes(32)
    lib.curve25519_scalarmult(session2, sec1, pub2.public)
    assert bytearray(session1) == bytearray(session2)

    shared1 = curve25519.Private(sec2).get_shared_key(pub1, hashfunc=lambda x: x)
    shared2 = curve25519.Private(sec1).get_shared_key(pub2, hashfunc=lambda x: x)
    assert shared1 == shared2
    assert bytearray(session1) == shared1
    assert bytearray(session2) == shared2


def test_curve25519_pubkey(r):
    sec = bytes(bytearray(r.randbytes(32)))
    pub = curve25519.Private(sec).get_public()
    res = r.randbytes(32)
    lib.curve25519_scalarmult_basepoint(res, sec)
    assert bytearray(res) == pub.public


def test_curve25519_scalarmult_from_gpg(r):
    sec = binascii.unhexlify(
        "4a1e76f133afb29dbc7860bcbc16d0e829009cc15c2f81ed26de1179b1d9c938"
    )
    pub = binascii.unhexlify(
        "5d6fc75c016e85b17f54e0128a216d5f9229f25bac1ec85cecab8daf48621b31"
    )
    res = r.randbytes(32)
    lib.curve25519_scalarmult(res, sec[::-1], pub[::-1])
    expected = "a93dbdb23e5c99da743e203bd391af79f2b83fb8d0fd6ec813371c71f08f2d4d"
    assert binascii.hexlify(bytearray(res)) == bytes(expected, "ascii")
