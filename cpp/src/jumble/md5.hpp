#ifndef JUMBLE_MD5_HPP_INCLUDED
#define JUMBLE_MD5_HPP_INCLUDED

#include "jumble/util/common.hpp"
#include <string>
#include <vector>
#include <cstdint>

JUMBLE_NAMESPACE_BEGIN

class MD5 {
public:
    using UInt32 = std::uint32_t;
    using UInt64 = std::uint64_t;

    // Forbid copy
    MD5(const MD5&) = delete;
    MD5& operator=(const MD5&) = delete;

    // Forbid move
    MD5(MD5&&) = delete;
    MD5& operator=(MD5&&) = delete;

    // Return the singleton instance
    static MD5* getInstance() {
        static MD5 instance;
        return &instance;
    }

    /*
    Hash a message.

    @param msg The message to be hashed
    @return    The hashed string result
    */
    std::string hash(const std::string& msg) {
        UInt32 a = 0x67452301, b = 0xefcdab89;
        UInt32 c = 0x98badcfe, d = 0x10325476;
        UInt32 groupNum = 0;
        UInt32 *str = pad(msg, groupNum);
        for (UInt32 i = 0; i < groupNum; ++i) {
            UInt32 group[16];  // Each group contains 16 32-bit intergers
            for (int j = 0; j < 16; ++j) {
                group[j] = str[(i << 4) + j];
            }
            loop(group, a, b, c, d);
        }
        delete str;
        return toHexStr(a) + toHexStr(b) + toHexStr(c) + toHexStr(d);
    }

private:
    // Default ctor
    MD5() : hex("0123456789abcdef"), s({
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    }), k({
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    }) {}

    /*
    Pad the string to make the length of the string satisfy: #(length) mod 512 = 0.
    The last 64 bits of the result is #(length of the original string) mod 2^64.

    @param str      The string to be filled up
    @param groupNum The number of the groups
    @return         The filled string stored in an array of 32-bit integer.
    */
    UInt32* pad(const std::string& str, UInt32& groupNum) {
        UInt32 strLen = (UInt32)str.length();

        // Each group has 64 bytes = 512 bits.
        // Add 8 bytes = 64 bits to store original length.
        groupNum = ((strLen + 8) >> 6) + 1;

        // Each group stores as 16 32-bit intergers
        UInt32 len = groupNum << 4;
        UInt32 *res = new UInt32[len];
        for (UInt32 i = 0; i < len; ++i) {
            res[i] = 0;
        }

        // One 32-bit integer stores 4 character(8 bytes)
        for (UInt32 i = 0; i < strLen; ++i) {
            res[i >> 2] |= str[i] << ((i % 4) << 3);
        }
        res[strLen >> 2] |= (1 << 7) << ((strLen % 4) << 3);  // Append one '1' at the end 

        // Append original length of the message in bits
        UInt64 originLenInBits = (UInt64)strLen << 3;
        res[len - 1] = originLenInBits >> 32;  // MSB
        res[len - 2] = originLenInBits & 0xffffffff;  // LSB
        return res;
    }

    /*
    Loop 64 times for each group and calculate a, b, c and d.

    @param group   The group to be processed
    @param a/b/c/d The four 32-bit integer of hashed text
    */
    void loop(const UInt32 *const group, UInt32& a, UInt32& b, UInt32& c, UInt32& d) noexcept {
        UInt32 f, g;
        UInt32 A = a, B = b, C = c, D = d;
        for (int i = 0; i < 64; ++i) {  // Loop for 4 turns, process 16 times in each turn
            if (i < 16) {
                f = (B & C) | ((~B) & D);
                g = i;
            } else if (i < 32) {
                f = (D & B) | ((~D) & C);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                f = B ^ C ^ D;
                g = (3 * i + 5) % 16;
            } else {
                f = C ^ (B | (~D));
                g = (7 * i) % 16;
            }
            UInt32 tmp = D;
            D = C;
            C = B;
            B = B + cycleShiftLeft((A + f + k[i] + group[g]), s[i]);
            A = tmp;
        }
        a += A, b += B, c += C, d += D;
    }

    /*
    Map a 32-bit integer to its hexadecimal presentation.

    @param num The 32-bit integer
    @return    A string that stores the hexadecimal presentation
    */
    std::string toHexStr(const UInt32 num) {
        std::string tmp, res = "";
        for (UInt32 i = 0; i < 4; ++i) {
            tmp = "";
            int byte = (num >> (i << 3)) % (1 << 8);
            for (UInt32 j = 0; j < 2; ++j) {
                tmp.insert(0, 1, hex[byte % 16]);
                byte >>= 4;
            }
            res += tmp;
        }
        return res;
    }

    /*
    Cycle shift left.

    @param num      The number to be shifted
    @param shiftCnt The shift count
    @return         The number after shifted
    */
    UInt32 cycleShiftLeft(const UInt32 num, const UInt32 shiftCnt) noexcept {
        return (num << shiftCnt) | (num >> (32 - shiftCnt));
    }

private:
    // Hexadecimal character
    const std::string hex;

    // Left shift number
    const std::vector<UInt32> s;

    // Constants in main loop
    const std::vector<UInt32> k;
};

JUMBLE_NAMESPACE_END

#endif
