# TON Blockchain Support, Examples

## Smart Contract

The TON network utilizes smart contract, defined in an own VM langugage.
Wallets and light clients do not need to understand smart contract language, but they have to deal with them to some extent.

## Cell Representation

In the TON network data is organized in Slice/Cell structures.  They are just containers for different type of data.  Specifically, non-byte bundary bit streams are a special feautre.  Cells can be be organized recursively, can be hashed, serialized and deserialzed.

## Cell Examples

Sample slice usage:

    Slice s1 = Slice::createFromHex("123456")); // or createFromData(parse_hex("123456"));
    std::cout << "slice, size: " << s1.size() << " data: " << s1.asBytesStr() << " hash: " << hex(s1.hash()) << std::endl; 

Outputs:

    slice, size: 3 data: 123456 hash:
    bf7cbe09d71a1bcc373ab9a764917f730a6ed951ffa1a7399b7abd8f8fd73cb4

Sample Cell usage, bytes slice with no children:

    Cell cell;
    cell.setSliceBytesStr("123456");
    ASSERT_EQ("09a6e1fb711077014e7cae82826707cace55a493501a16144cfd83fca0c8e6d6", hex(cell.hash())); 

Fift equivalent:
    <b x{123456} s, b> dup ."Data: " <s csr. cr ."Data hash: " hash x. cr

outputs:
    Data: x{123456}
    Data hash: 9a6e1fb711077014e7cae82826707cace55a493501a16144cfd83fca0c8e6d6

Sample usage, bits slice with  no children:

    Cell cell;
    cell.setSliceBitsStr("31", 5); // 5 bits are 00110, the last 3 are not used
    ASSERT_EQ("34", cell.getSlice()->asBytesStr()); // the 5 bits padded with 100
    ASSERT_EQ("ea23ba20e2f88a07af38948bfaef741741f5a464df43e87067c901e537d1c44f", hex(cell.hash()));
    
Fift equivalent:
    <b b{00110} s, b> dup ."Data: " <s csr. cr ."Data hash: " hash x. cr
 
outputs:
    Data: x{34_}
    Data hash: ea23ba20e2f88a07af38948bfaef741741f5a464df43e87067c901e537d1c44f

Sample usage, no value and one child:

    auto c1 = std::make_shared<Cell>();
    c1->setSliceBytesStr("123456");
    Cell c;
    c.addCell(c1);
    ASSERT_EQ("993e6d53d70fb05f052ce45ac751e24abd7d43e358b297694cfc19bbc796141c", hex(c.hash()));

Fift equivalent:
    <b x{123456} s, b>
    <b swap ref, b>
    dup ."Data: " <s csr. cr ."Data hash: " hash x. cr

outputs:
    Data: x{}
     x{123456}
    Data hash: 993e6d53d70fb05f052ce45ac751e24abd7d43e358b297694cfc19bbc796141c

Sample usage, StateInit cell:

    Cell c;
    c.setSliceBitsStr("30", 5);
    auto ccode = std::make_shared<Cell>();
    ccode->setSliceBytesStr("FF0020DDA4F260810200D71820D70B1FED44D0D7091FD709FFD15112BAF2A122F901541044F910F2A2F80001D7091F3120D74A97D70907D402FB00DED1A4C8CB1FCBFFC9ED54");
    auto cdata = std::make_shared<Cell>();
    cdata->setSliceBytesStr("00000000F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3");
    c.addCell(ccode);
    c.addCell(cdata);
    ASSERT_EQ("60c04141c6a7b96d68615e7a91d265ad0f3a9a922e9ae9c901d4fa83f5d3c0d0", hex(c.hash()));

Fift equivalent:
    <b x{FF0020DDA4F260810200D71820D70B1FED44D0D7091FD709FFD15112BAF2A122F901541044F910F2A2F80001D7091F3120D74A97D70907D402FB00DED1A4C8CB1FCBFFC9ED54} s, b>
    <b 0 32 u, x{F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3} s, b>
    <b b{00110} s, rot ref, swap ref, b>
    dup ."Data: " <s csr. cr ."Data hash: " hash x. cr

outputs:
    Data: x{34_}
     x{FF0020DDA4F260810200D71820D70B1FED44D0D7091FD709FFD15112BAF2A122F901541044F910F2A2F80001D7091F3120D74A97D70907D402FB00DED1A4C8CB1FCBFFC9ED54}
     x{00000000F61CF0BC8E891AD7636E0CD35229D579323AA2DA827EB85D8071407464DC2FA3}
    Data hash: 60c04141c6a7b96d68615e7a91d265ad0f3a9a922e9ae9c901d4fa83f5d3c0d0
