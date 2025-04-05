#include "sha256.h"
#include "pch.h"

typedef unsigned char uint8;
typedef unsigned int uint32;

namespace {
    const uint32 roundConstants[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    inline uint32 rotateRight(uint32 value, uint32 shift) {
        return (value >> shift) | (value << (32 - shift));
    }

    inline uint32 choose(uint32 e, uint32 f, uint32 g) {
        return (e & f) ^ (~e & g);
    }

    inline uint32 majority(uint32 a, uint32 b, uint32 c) {
        return (a & b) ^ (a & c) ^ (b & c);
    }

    inline uint32 sigma0(uint32 x) {
        return rotateRight(x, 7) ^ rotateRight(x, 18) ^ (x >> 3);
    }

    inline uint32 sigma1(uint32 x) {
        return rotateRight(x, 17) ^ rotateRight(x, 19) ^ (x >> 10);
    }

    void processChunk(uint32* hashState, const uint8* chunk) {
        uint32 messageSchedule[64];
        for (int i = 0; i < 16; i++) {
            messageSchedule[i] = (chunk[i * 4] << 24) |
                (chunk[i * 4 + 1] << 16) |
                (chunk[i * 4 + 2] << 8) |
                (chunk[i * 4 + 3]);
        }
        for (int i = 16; i < 64; i++) {
            messageSchedule[i] = sigma1(messageSchedule[i - 2]) + messageSchedule[i - 7] + sigma0(messageSchedule[i - 15]) + messageSchedule[i - 16];
        }

        uint32 a = hashState[0], b = hashState[1], c = hashState[2], d = hashState[3];
        uint32 e = hashState[4], f = hashState[5], g = hashState[6], h = hashState[7];

        for (int i = 0; i < 64; i++) {
            uint32 t1 = h + (rotateRight(e, 6) ^ rotateRight(e, 11) ^ rotateRight(e, 25)) + choose(e, f, g) + roundConstants[i] + messageSchedule[i];
            uint32 t2 = (rotateRight(a, 2) ^ rotateRight(a, 13) ^ rotateRight(a, 22)) + majority(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }

        hashState[0] += a;
        hashState[1] += b;
        hashState[2] += c;
        hashState[3] += d;
        hashState[4] += e;
        hashState[5] += f;
        hashState[6] += g;
        hashState[7] += h;
    }
}

std::string sha256(const std::string& input) {
    uint32 hashState[8] = {
        0x6a09e667, 0xbb67ae85,
        0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c,
        0x1f83d9ab, 0x5be0cd19
    };

    std::vector<uint8> data(input.begin(), input.end());

    data.push_back(0x80);

    while ((data.size() % 64) != 56) {
        data.push_back(0x00);
    }

    uint64_t bitLength = input.size() * 8;
    for (int i = 7; i >= 0; i--) {
        data.push_back((bitLength >> (i * 8)) & 0xff);
    }

    for (size_t i = 0; i < data.size(); i += 64) {
        processChunk(hashState, &data[i]);
    }

    std::ostringstream out;
    for (int i = 0; i < 8; i++) {
        out << std::hex << std::setw(8) << std::setfill('0') << hashState[i];
    }

    return out.str();
}
