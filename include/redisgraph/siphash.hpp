/**
* Copyright 2019 RRD Software Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/

#ifndef REDIS_GRAPH_SIPHASH_H

#include <cstdint>
#include <algorithm>
#include <random>
#define REDIS_GRAPH_SIPHASH_H

namespace redisgraph
{

    inline
        static uint64_t rotl(uint64_t x, uint64_t b)
        // Return the bits of the specified 'x' rotated to the left by the
        // specified 'b' number of bits.  Bits that are rotated off the end are
        // wrapped around to the beginning.
    {
        return (x << b) | (x >> (64 - b));
    }

    // in this case support just x86/x64  - little endian
    inline
        static uint64_t u8to64_le(const uint8_t* p)
    {
        return *reinterpret_cast<const uint64_t*>(p);  // Ignore alignment.
    }
    inline
        static void sipround(uint64_t& v0, uint64_t& v1, uint64_t& v2, uint64_t& v3)
        // Mix the specified 'v0', 'v1', 'v2', and 'v3' in a "Sip Round" as
        // described in the siphash algorithm
    {
        v0 += v1;
        v1 = rotl(v1, 13);
        v1 ^= v0;
        v0 = rotl(v0, 32);
        v2 += v3;
        v3 = rotl(v3, 16);
        v3 ^= v2;
        v0 += v3;
        v3 = rotl(v3, 21);
        v3 ^= v0;
        v2 += v1;
        v1 = rotl(v1, 17);
        v1 ^= v2;
        v2 = rotl(v2, 32);
    }

    class siphash
    {
    private:
        // DATA
        uint64_t d_v0;
        uint64_t d_v1;
        uint64_t d_v2;
        uint64_t d_v3;
        // Stores the intermediate state of the algorithm as values are
        // accumulated

        union {
            uint64_t d_alignment;
            // Provides alignment
            unsigned char d_buf[8];
            // Used to buffer data until we have enough to do a full round of
            // computation as specified by the algorithm.
        };

        size_t d_bufSize;
        // The length of the data currently stored in the buffer.

        size_t d_totalLength;
        // The total length of all data that has been passed into the
        // algorithm.


    public:
        siphash(const siphash& original) = delete;
        ~siphash() = default;

        // Do not allow copy construction.

        siphash operator=(const siphash& rhs) = delete;

        // TYPES
        typedef uint64_t result_type;
        // Typedef indicating the value type returned by this algorithm.

    // CONSTANTS
        enum { k_SEED_LENGTH = 16 }; // Seed length in bytes.

        // CREATORS
       
        explicit siphash(): d_v0(0x736f6d6570736575ULL)
                , d_v1(0x646f72616e646f6dULL)
                , d_v2(0x6c7967656e657261ULL)
                , d_v3(0x7465646279746573ULL)
                , d_bufSize(0)
                , d_totalLength(0)
        {
            std::uint32_t d_seed[4];
            const std::uint8_t r_seed[16] = { 0x8a, 0x8a, 0xeb, 0x65,
                                              0x5a, 0x39, 0xa5, 0x71,
                                              0xb3, 0x83, 0x6b, 0xd5,
                                              0x73, 0x71, 0xf8, 0xf1 };
            uint64_t k0 = redisgraph::u8to64_le(&r_seed[0]);
            uint64_t k1 = redisgraph::u8to64_le(&r_seed[k_SEED_LENGTH / 2]);
            d_v0 ^= k0;
            d_v1 ^= k1;
            d_v2 ^= k0;
            d_v3 ^= k1;
        }
        explicit siphash(const char* seed)
            : d_v0(0x736f6d6570736575ULL)
            , d_v1(0x646f72616e646f6dULL)
            , d_v2(0x6c7967656e657261ULL)
            , d_v3(0x7465646279746573ULL)
            , d_bufSize(0)
            , d_totalLength(0)
        {
           
            uint64_t k0 = redisgraph::u8to64_le(reinterpret_cast<const uint8_t*>(&seed[0]));
            uint64_t k1 = redisgraph::u8to64_le(reinterpret_cast<const uint8_t*>(&seed[k_SEED_LENGTH / 2]));

            d_v0 ^= k0;
            d_v1 ^= k1;
            d_v2 ^= k0;
            d_v3 ^= k1;
        }
        void set_data(const void* data, size_t numBytes)
        {
            uint8_t const* in = static_cast<const uint8_t*>(data);

            d_totalLength += numBytes;
            if (d_bufSize + numBytes < 8)
            {
                std::copy(in, in + numBytes, d_buf + d_bufSize);
                d_bufSize += numBytes;
                return;                                                       // RETURN
            }
            if (d_bufSize > 0)
            {
                size_t t = 8 - d_bufSize;
                std::copy(in, in + t, d_buf + d_bufSize);
                uint64_t m = redisgraph::u8to64_le(d_buf);
                d_v3 ^= m;
                sipround(d_v0, d_v1, d_v2, d_v3);
                sipround(d_v0, d_v1, d_v2, d_v3);
                d_v0 ^= m;
                in += t;
                numBytes -= t;
            }
            d_bufSize = numBytes & 7;
            uint8_t const* const end = in + (numBytes - d_bufSize);
            for (; in != end; in += 8)
            {
                uint64_t m = redisgraph::u8to64_le(in);
                d_v3 ^= m;
                sipround(d_v0, d_v1, d_v2, d_v3);
                sipround(d_v0, d_v1, d_v2, d_v3);
                d_v0 ^= m;
            }
            std::copy(end, end + d_bufSize, d_buf);
        }

        result_type computeHash()
        {
            result_type b = static_cast<uint64_t>(d_totalLength) << 56;
            switch (d_bufSize)
            {
            case 7: b |= static_cast<uint64_t>(d_buf[6]) << 48; [[fallthrough]];
            case 6: b |= static_cast<uint64_t>(d_buf[5]) << 40; [[fallthrough]];
            case 5: b |= static_cast<uint64_t>(d_buf[4]) << 32; [[fallthrough]];
            case 4: b |= static_cast<uint64_t>(d_buf[3]) << 24; [[fallthrough]];
            case 3: b |= static_cast<uint64_t>(d_buf[2]) << 16; [[fallthrough]];
            case 2: b |= static_cast<uint64_t>(d_buf[1]) << 8; [[fallthrough]];
            case 1: b |= static_cast<uint64_t>(d_buf[0]); [[fallthrough]];
            case 0:                                                        break;
            }
            d_v3 ^= b;
            sipround(d_v0, d_v1, d_v2, d_v3);
            sipround(d_v0, d_v1, d_v2, d_v3);
            d_v0 ^= b;
            d_v2 ^= 0xff;
            sipround(d_v0, d_v1, d_v2, d_v3);
            sipround(d_v0, d_v1, d_v2, d_v3);
            sipround(d_v0, d_v1, d_v2, d_v3);
            sipround(d_v0, d_v1, d_v2, d_v3);
            b = d_v0 ^ d_v1 ^ d_v2 ^ d_v3;
            return b;
        }
    };
}
#endif