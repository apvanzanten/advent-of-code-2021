#include <numeric>
#include <algorithm>
#include <iostream>
#include <array>
#include <bitset>

using B = std::bitset<12>;

constexpr auto data = std::array{
    B{0b011110011110}, B{0b101101001111}, B{0b000000010101}, B{0b100111001010}, B{0b110000011010}, B{0b011101010101}, B{0b011110001100}, B{0b010111111001}, B{0b111011100101}, B{0b011110000110}, B{0b010100001111}, B{0b000001001100}, B{0b110001001000}, B{0b001000100101}, B{0b010100011110}, B{0b010111011110}, B{0b110101001111}, B{0b011010111011}, B{0b000011010101}, B{0b100111011100}, B{0b010010100011}, B{0b010010011001}, B{0b000111010011}, B{0b111001000111}, B{0b100101111111}, B{0b011001101110}, B{0b110010101100}, B{0b001000010001}, B{0b110101100011}, B{0b101110001011}, B{0b010100000110}, B{0b001110011110}, B{0b011101100110}, B{0b001111101101}, B{0b000101101000}, B{0b100110000111}, B{0b000001010100}, B{0b111100010110}, B{0b000100100100}, B{0b001000110001}, B{0b101000001011}, B{0b101000100110}, B{0b110000101010}, B{0b010111110000}, B{0b111110010110}, B{0b011011111111}, B{0b111010100000}, B{0b100001000111}, B{0b011000001010}, B{0b011000010001}, B{0b101101100001}, B{0b011001101011}, B{0b111001001010}, B{0b000010110101}, B{0b101110100010}, B{0b001111111101}, B{0b011110001001}, B{0b000111100011}, B{0b010101001001}, B{0b111110111111}, B{0b111000111010}, B{0b001001111100}, B{0b000011000001}, B{0b011100010110}, B{0b110010000111}, B{0b000010010101}, B{0b110000001110}, B{0b010011111011}, B{0b001010100100}, B{0b111001010101}, B{0b001101000011}, B{0b110100110011}, B{0b110010100101}, B{0b001000001011}, B{0b111110011000}, B{0b110111011011}, B{0b110110101001}, B{0b010010100100}, B{0b111100001100}, B{0b100100001100}, B{0b110001111010}, B{0b111011001011}, B{0b010000010001}, B{0b110010100000}, B{0b010100011100}, B{0b000110001001}, B{0b010011000001}, B{0b111011010001}, B{0b010011111101}, B{0b111101110100}, B{0b110110101100}, B{0b010100111110}, B{0b100011001101}, B{0b000010000010}, B{0b011111100001}, B{0b100000101100}, B{0b100001110011}, B{0b100101100100}, B{0b000100001001}, B{0b101000000011}, B{0b110111101100}, B{0b100100101001}, B{0b001001101001}, B{0b000001010000}, B{0b001001111110}, B{0b111011111010}, B{0b001000011110}, B{0b110000100111}, B{0b010010010101}, B{0b011010001011}, B{0b100010010101}, B{0b010110101010}, B{0b011111001011}, B{0b110010011100}, B{0b111110101010}, B{0b000001111010}, B{0b111101110101}, B{0b101010110111}, B{0b100010101111}, B{0b000110101110}, B{0b000000010000}, B{0b000010100110}, B{0b111110010001}, B{0b000100110000}, B{0b110010011011}, B{0b101001000010}, B{0b010110000101}, B{0b100011100100}, B{0b110000110100}, B{0b110011000001}, B{0b101110001000}, B{0b010100011111}, B{0b111000100111}, B{0b110001100010}, B{0b100001101000}, B{0b101001000111}, B{0b111110100000}, B{0b000011000010}, B{0b110000000001}, B{0b111010000111}, B{0b100100111011}, B{0b101101110001}, B{0b111100110100}, B{0b100110010000}, B{0b001001001110}, B{0b100010001001}, B{0b101000110100}, B{0b011100001111}, B{0b001110100110}, B{0b110100001100}, B{0b101101001011}, B{0b000011111110}, B{0b101101010100}, B{0b000110110001}, B{0b011100011001}, B{0b111000111111}, B{0b011010101100}, B{0b111000100101}, B{0b100111011110}, B{0b011010100110}, B{0b110000101111}, B{0b110010000001}, B{0b100001110101}, B{0b110101011111}, B{0b111111000011}, B{0b101010010010}, B{0b000111111111}, B{0b000001001111}, B{0b101101010010}, B{0b111111001000}, B{0b001001110000}, B{0b110010011110}, B{0b110011001111}, B{0b100111101011}, B{0b011111000111}, B{0b110010111100}, B{0b010110000000}, B{0b110001100011}, B{0b111000100001}, B{0b011000101000}, B{0b100110001100}, B{0b001101011011}, B{0b011001111110}, B{0b011101000110}, B{0b110110111001}, B{0b110101001010}, B{0b111000111011}, B{0b100111010001}, B{0b100110000011}, B{0b010001000100}, B{0b001001001000}, B{0b010010001010}, B{0b010101101101}, B{0b001110001001}, B{0b101110011111}, B{0b010111001111}, B{0b110111001110}, B{0b100000101011}, B{0b010000111111}, B{0b101000101011}, B{0b101011010111}, B{0b000101110001}, B{0b100100010010}, B{0b100111011101}, B{0b111100000100}, B{0b100110000001}, B{0b000001000101}, B{0b101100001111}, B{0b100000101001}, B{0b010101010110}, B{0b000110010111}, B{0b011101001100}, B{0b000111111011}, B{0b000001100101}, B{0b100001011011}, B{0b111000101110}, B{0b111101100111}, B{0b011010010100}, B{0b101001001010}, B{0b110100110001}, B{0b111101011101}, B{0b000011101110}, B{0b011000100100}, B{0b101110000111}, B{0b011101110110}, B{0b111001010001}, B{0b011110000100}, B{0b011110001101}, B{0b100111100001}, B{0b010011100001}, B{0b001001101101}, B{0b001111111010}, B{0b010101000101}, B{0b011100000110}, B{0b100010001101}, B{0b011011101011}, B{0b110111001010}, B{0b101000010010}, B{0b111111100011}, B{0b000101111011}, B{0b001101010011}, B{0b100101000001}, B{0b101101100011}, B{0b101110010011}, B{0b011001100000}, B{0b110001111111}, B{0b011100101100}, B{0b010010001110}, B{0b111110101101}, B{0b010110001000}, B{0b000011000110}, B{0b101010011011}, B{0b000100111011}, B{0b001000111101}, B{0b010011111111}, B{0b011101100111}, B{0b111111011111}, B{0b001100000000}, B{0b101100001110}, B{0b101100001000}, B{0b100001011111}, B{0b100001000101}, B{0b000101011001}, B{0b010100111100}, B{0b110000111011}, B{0b010000111100}, B{0b011100110000}, B{0b101111110010}, B{0b100011110101}, B{0b100100110101}, B{0b100001100000}, B{0b101000100101}, B{0b111000001100}, B{0b010000001011}, B{0b110101010111}, B{0b011011001000}, B{0b100111000101}, B{0b011000000100}, B{0b110110011000}, B{0b101101101101}, B{0b100101110010}, B{0b001100001010}, B{0b010111000011}, B{0b111111100001}, B{0b100010111011}, B{0b110011001001}, B{0b000011011101}, B{0b001100101101}, B{0b010110011001}, B{0b111100110010}, B{0b011111001110}, B{0b100110001111}, B{0b000111101111}, B{0b011000110001}, B{0b001010001010}, B{0b000111010100}, B{0b010011110101}, B{0b011111000010}, B{0b000101000010}, B{0b111101011010}, B{0b110011111100}, B{0b100000001000}, B{0b100001000110}, B{0b101010100001}, B{0b000101000101}, B{0b100100011111}, B{0b110000101110}, B{0b000110000101}, B{0b001001011111}, B{0b100010011100}, B{0b000001110001}, B{0b110000010100}, B{0b011111010100}, B{0b001000001111}, B{0b000111000001}, B{0b110010001000}, B{0b001001111101}, B{0b111000001001}, B{0b001011000001}, B{0b111110010100}, B{0b110010001100}, B{0b010110110000}, B{0b011100100010}, B{0b011000101011}, B{0b001001000001}, B{0b011001001110}, B{0b111000010100}, B{0b000110000110}, B{0b011011000100}, B{0b011000010111}, B{0b110001011000}, B{0b010111011000}, B{0b000101110100}, B{0b000000011111}, B{0b001100111100}, B{0b000010100101}, B{0b000111110000}, B{0b000110000001}, B{0b000100101001}, B{0b101111110100}, B{0b010010000001}, B{0b001100010001}, B{0b101111001101}, B{0b111011111101}, B{0b000011110011}, B{0b111001011100}, B{0b010110110110}, B{0b000000001101}, B{0b010010000000}, B{0b000001010011}, B{0b101000010000}, B{0b101000001110}, B{0b010001000111}, B{0b111010011011}, B{0b101001111110}, B{0b000110101100}, B{0b100110101001}, B{0b100111111111}, B{0b101001101001}, B{0b110010010011}, B{0b011001011111}, B{0b111111110110}, B{0b101110010110}, B{0b100100001101}, B{0b010001111110}, B{0b111111101000}, B{0b100010100001}, B{0b011110010100}, B{0b101000110011}, B{0b010110110011}, B{0b011111001000}, B{0b100010100101}, B{0b110010010110}, B{0b111011001000}, B{0b101110001101}, B{0b000010110011}, B{0b001110100100}, B{0b100000011101}, B{0b011001100001}, B{0b011001101010}, B{0b001100011101}, B{0b000111101100}, B{0b001010001001}, B{0b111001101110}, B{0b110110001111}, B{0b001111001010}, B{0b001001000011}, B{0b001000010000}, B{0b000001111110}, B{0b000101101111}, B{0b001001000110}, B{0b100001101110}, B{0b101111011100}, B{0b111110010010}, B{0b010001100011}, B{0b101000100111}, B{0b001101000111}, B{0b111011110011}, B{0b000011000101}, B{0b111101001110}, B{0b110111111111}, B{0b011100010100}, B{0b110011110001}, B{0b001011011001}, B{0b000011010111}, B{0b011000001110}, B{0b000101010111}, B{0b110101110001}, B{0b001100001111}, B{0b110111010111}, B{0b110010010101}, B{0b100111100111}, B{0b101000000111}, B{0b101111000100}, B{0b100011100010}, B{0b000100100101}, B{0b111011110110}, B{0b001111100110}, B{0b011011000010}, B{0b100010010100}, B{0b010111110010}, B{0b100010110011}, B{0b111111111000}, B{0b011010101111}, B{0b101110111011}, B{0b101110101010}, B{0b001011001000}, B{0b110110001011}, B{0b110100101111}, B{0b001011001011}, B{0b110011010000}, B{0b000000100000}, B{0b100000110000}, B{0b101110011011}, B{0b100101101111}, B{0b010110100010}, B{0b000010100011}, B{0b000110110000}, B{0b111101000001}, B{0b011001010011}, B{0b000000001110}, B{0b000100110101}, B{0b111111111110}, B{0b001010101001}, B{0b001101100111}, B{0b010100000101}, B{0b000111001001}, B{0b101100101110}, B{0b011011110111}, B{0b011010000011}, B{0b000100101011}, B{0b100001100110}, B{0b000111011101}, B{0b000100011100}, B{0b000101100101}, B{0b011111111110}, B{0b111010111011}, B{0b100000100101}, B{0b011001011110}, B{0b010101100010}, B{0b010101111101}, B{0b100001000001}, B{0b001000100001}, B{0b010010111110}, B{0b100011001011}, B{0b011011000111}, B{0b110011010101}, B{0b111101100100}, B{0b001110001010}, B{0b110011111101}, B{0b111011011101}, B{0b001101000001}, B{0b101011011011}, B{0b100000111010}, B{0b010101110101}, B{0b000101011100}, B{0b011000011001}, B{0b111001010011}, B{0b100100010110}, B{0b111101010001}, B{0b110110011011}, B{0b011110110100}, B{0b101000110101}, B{0b101110111101}, B{0b011011011110}, B{0b000100110110}, B{0b100011011111}, B{0b111010101001}, B{0b011111011000}, B{0b101000101110}, B{0b110010111001}, B{0b111010000010}, B{0b110011011001}, B{0b110110010100}, B{0b111010011010}, B{0b000010110001}, B{0b011101100010}, B{0b110010110111}, B{0b010101001111}, B{0b011011001101}, B{0b101111000110}, B{0b010111010010}, B{0b101111011011}, B{0b100111101100}, B{0b001000001101}, B{0b010110100000}, B{0b001010100110}, B{0b111111000111}, B{0b100101000100}, B{0b100101001100}, B{0b110111010110}, B{0b101000101000}, B{0b100001001111}, B{0b110011110010}, B{0b111111101111}, B{0b110001111000}, B{0b010001110110}, B{0b101100000010}, B{0b000110000111}, B{0b001011100101}, B{0b101010100101}, B{0b110110001001}, B{0b000011011111}, B{0b110110101110}, B{0b101100110010}, B{0b101110101110}, B{0b101010110101}, B{0b000111000011}, B{0b101000011111}, B{0b001100000111}, B{0b101001111111}, B{0b100100011101}, B{0b111100011011}, B{0b000010111010}, B{0b111010101010}, B{0b101001010010}, B{0b101100010011}, B{0b010000110110}, B{0b011110110011}, B{0b001101110111}, B{0b110111110110}, B{0b001011000100}, B{0b101111100010}, B{0b011010011100}, B{0b110100001010}, B{0b100100110111}, B{0b000011010011}, B{0b000110111010}, B{0b001100100000}, B{0b100111011011}, B{0b001101101011}, B{0b101011101111}, B{0b001101100100}, B{0b111010010111}, B{0b100111111100}, B{0b101101000100}, B{0b000011111100}, B{0b011111001101}, B{0b110101001101}, B{0b011100100110}, B{0b101011000000}, B{0b100001010011}, B{0b011110111110}, B{0b011011100110}, B{0b110001011010}, B{0b111111110101}, B{0b001111111000}, B{0b100010011011}, B{0b100001101100}, B{0b110011010100}, B{0b100011000001}, B{0b011110111001}, B{0b010111110110}, B{0b100110001010}, B{0b100000000011}, B{0b001001101111}, B{0b101111011010}, B{0b010000110100}, B{0b101001011110}, B{0b100100011000}, B{0b101101000111}, B{0b011101001001}, B{0b010101110010}, B{0b000110000000}, B{0b000100111000}, B{0b010110111010}, B{0b110000011111}, B{0b011001011100}, B{0b010100110110}, B{0b011111110011}, B{0b111011011010}, B{0b001000101011}, B{0b000010110100}, B{0b011100101000}, B{0b111111100110}, B{0b011001110100}, B{0b000100011000}, B{0b010101010111}, B{0b001011101000}, B{0b001000011100}, B{0b110101101100}, B{0b010001000010}, B{0b001010010000}, B{0b111000110001}, B{0b101111111111}, B{0b011101011110}, B{0b001001111010}, B{0b001000100110}, B{0b000011011001}, B{0b100101100001}, B{0b110111000000}, B{0b100111011111}, B{0b111000110000}, B{0b110010111111}, B{0b000010010011}, B{0b100001100111}, B{0b010110100100}, B{0b001101001010}, B{0b101001100110}, B{0b010001110101}, B{0b001011000010}, B{0b010111110101}, B{0b000001001110}, B{0b010010100110}, B{0b111011001110}, B{0b101011111001}, B{0b000100000000}, B{0b111101001100}, B{0b100111011010}, B{0b000010010001}, B{0b100000010000}, B{0b101101000010}, B{0b110100010100}, B{0b111011111000}, B{0b101001100100}, B{0b001110101011}, B{0b000111001010}, B{0b111110000011}, B{0b001110100101}, B{0b101010000101}, B{0b010101010011}, B{0b110000111100}, B{0b101011001010}, B{0b010001011011}, B{0b010000011010}, B{0b000010101011}, B{0b100110110101}, B{0b100111011001}, B{0b100111010111}, B{0b000101100000}, B{0b101110000001}, B{0b100100001000}, B{0b010111000010}, B{0b000101001010}, B{0b001110010010}, B{0b111011000111}, B{0b001110110010}, B{0b111010000001}, B{0b011000100101}, B{0b000111100101}, B{0b100000100111}, B{0b100110011011}, B{0b001011100001}, B{0b101010100111}, B{0b011001111000}, B{0b000011011010}, B{0b001011101100}, B{0b010011011001}, B{0b000111111100}, B{0b010001011110}, B{0b111111011010}, B{0b111010010101}, B{0b110000010011}, B{0b001001101010}, B{0b000101011011}, B{0b100111111110}, B{0b110010101001}, B{0b111000011000}, B{0b011100101110}, B{0b011100010001}, B{0b001101110010}, B{0b101000111111}, B{0b101011011001}, B{0b110111000110}, B{0b001000010100}, B{0b000111000100}, B{0b010000101110}, B{0b001100100001}, B{0b111100000011}, B{0b111011101010}, B{0b000001111000}, B{0b100100010000}, B{0b110110000110}, B{0b000010111100}, B{0b110110110111}, B{0b010000011011}, B{0b101110000100}, B{0b011100100100}, B{0b010111001011}, B{0b001011010000}, B{0b000111010001}, B{0b110010111000}, B{0b001011011110}, B{0b000001100111}, B{0b010000101111}, B{0b110001100100}, B{0b111000001101}, B{0b000110011001}, B{0b010000010110}, B{0b101010001110}, B{0b111000111000}, B{0b011111011100}, B{0b111001110110}, B{0b111100010001}, B{0b110101111001}, B{0b001001010011}, B{0b010001111001}, B{0b001101101110}, B{0b010011000010}, B{0b000100011011}, B{0b000001101000}, B{0b001110100001}, B{0b100011010011}, B{0b100000011100}, B{0b011001010111}, B{0b110011011010}, B{0b100110001011}, B{0b011001110111}, B{0b001010001100}, B{0b000111010111}, B{0b110111111101}, B{0b111001101010}, B{0b001110001000}, B{0b011100001010}, B{0b010011101010}, B{0b111011010011}, B{0b000011001010}, B{0b100101000110}, B{0b100101010111}, B{0b110010100010}, B{0b010100101111}, B{0b111011011100}, B{0b100011100101}, B{0b101111111101}, B{0b001001110011}, B{0b110111101001}, B{0b100100010101}, B{0b010101100001}, B{0b100011000011}, B{0b001010010111}, B{0b011001001001}, B{0b110001101001}, B{0b010011100010}, B{0b111000001111}, B{0b010101111111}, B{0b001001001011}, B{0b000111110111}, B{0b100110111101}, B{0b011011110000}, B{0b001001011110}, B{0b011010101101}, B{0b111011101011}, B{0b010001100111}, B{0b110100001111}, B{0b100010110111}, B{0b000000010100}, B{0b100000010001}, B{0b010011010101}, B{0b010011100110}, B{0b100011111011}, B{0b110110100001}, B{0b110000100110}, B{0b110101100000}, B{0b110001101011}, B{0b110110100110}, B{0b101111101110}, B{0b011010101011}, B{0b101001010101}, B{0b110110111101}, B{0b100111100010}, B{0b101100100010}, B{0b101011101101}, B{0b010011010011}, B{0b001111011100}, B{0b100101010100}, B{0b101010101010}, B{0b010000011101}, B{0b011100000010}, B{0b100111101000}, B{0b111111010101}, B{0b011010101000}, B{0b001010110010}, B{0b111110010011}, B{0b110111011010}, B{0b010010111111}, B{0b111001100111}, B{0b010101111000}, B{0b000111110011}, B{0b111101000110}, B{0b001011000000}, B{0b110100100110}, B{0b101001110010}, B{0b101111011001}, B{0b111110000001}, B{0b011101110010}, B{0b011000010110}, B{0b101011010010}, B{0b100110011101}, B{0b000010011101}, B{0b001111110100}, B{0b110011001100}, B{0b111110111100}, B{0b011001011000}, B{0b001110000011}, B{0b110001001101}, B{0b111110111011}, B{0b110000000000}, B{0b010000110001}, B{0b000100110001}, B{0b001000101101}, B{0b001100110111}, B{0b101011001111}, B{0b110011110011}, B{0b010011100101}, B{0b001001000000}, B{0b011110100000}, B{0b001010110111}, B{0b110101111101}, B{0b001111000000}, B{0b100011011001}, B{0b100000001001}, B{0b000010101010}, B{0b110000100000}, B{0b111111101001}, B{0b111010101100}, B{0b101100110111}, B{0b110100011000}, B{0b010011000111}, B{0b010010001000}, B{0b001011001111}, B{0b001001001100}, B{0b100010110110}, B{0b111100111011}, B{0b111100101100}, B{0b000111111101}, B{0b101110000011}, B{0b100001011101}, B{0b111101110011}, B{0b100100000011}, B{0b111110011001}, B{0b100010111101}, B{0b110001001010}, B{0b111111100100}, B{0b010000110101}, B{0b001100001101}, B{0b001001110111}, B{0b110110101010}, B{0b011110010010}, B{0b101011010000}, B{0b101101110010}, B{0b010010110000}, B{0b011110100111}, B{0b010111000100}, B{0b010100000010}, B{0b000011110100}, B{0b111001011101}, B{0b010111100101}, B{0b110010101101}, B{0b011100010010}, B{0b001110111100}, B{0b011111110110}, B{0b001110100010}, B{0b000001100001}, B{0b011011110010}, B{0b000010111101}, B{0b001011111110}, B{0b111110111010}, B{0b001010101101}, B{0b100011110010}, B{0b001111110010}, B{0b011001110010}, B{0b100110101100}, B{0b010111010011}, B{0b001000010011}, B{0b111110110010}, B{0b011000000110}, B{0b011111111100}, B{0b000111110110}, B{0b110001110100}, B{0b101110010101}, B{0b010000111101}, B{0b011011101110}, B{0b011010100111}, B{0b001010000010}, B{0b100001100011}, B{0b110001000000}, B{0b101111100111}, B{0b101111110011}, B{0b001101110011}, B{0b110001110101}, B{0b100101010010}, B{0b010101011110}, B{0b000011111000}, B{0b000100001000}, B{0b101001110011}, B{0b011100110001}, B{0b000000011100}, B{0b111110010111}, B{0b101011101100}, B{0b110011101100}, B{0b110101011000}, B{0b010111001000}, B{0b111100001010}, B{0b001000011111}, B{0b010100111011}, B{0b101010101011}, B{0b001110001110}, B{0b100011000110}, B{0b000101101011}, B{0b110011010111}, B{0b111001011110}, B{0b111010100110}, B{0b010001110001}, B{0b011010101110}, B{0b101011110000}, B{0b001101010010}, B{0b001010000011}, B{0b011001000111}, B{0b010001000110}, B{0b010110011111}, B{0b001111011000}, B{0b000010010111}, B{0b100011110111}, B{0b111100111100}, B{0b010101010001}, B{0b100011101111}, B{0b011101011001}, B{0b001011001100}, B{0b001111000101}, B{0b110111011000}, B{0b011001001010}, B{0b010000000100}, B{0b100110111010}, B{0b000000111110}, B{0b011100100011}, B{0b100100111110}, B{0b000010011010}, B{0b111000000100}, B{0b001110001111}, B{0b011000101010}, B{0b011100110010}, B{0b110101000111}, B{0b110110111111}, B{0b101110110010}, B{0b000110100000}, B{0b110100001101}, B{0b000111101110}, B{0b111000111001}, B{0b000111111010}, B{0b010110111011}, B{0b000101110111}, B{0b111000011011}, B{0b010110001010}, B{0b001011111011}, B{0b100100100011}, B{0b011110110010}, B{0b000111110101}, B{0b110110011101}, B{0b000011000011}, B{0b110011001010}, B{0b000100101100}, B{0b101100100011}, B{0b010111000001}, B{0b110101100001}, B{0b011110111010}, B{0b111100101000}, B{0b010000100010}, B{0b110011000111}, B{0b010011011100}, B{0b100110101011}, B{0b000100001011}, B{0b101111000010}, B{0b010000010000}, B{0b111110000100}, B{0b111100011110}, B{0b010100111001}, B{0b100000011111}, B{0b010111110100}, B{0b100100110011}, B{0b111101111001}, B{0b100010011000}, B{0b111110101111}, B{0b010111001010}, B{0b110000101000}, B{0b001111111111}, B{0b000111100110}, B{0b000011101100}, B{0b011010001000}, B{0b001011010011}, B{0b110011111011}};

constexpr size_t get_sum_of_bits_at_idx(size_t idx)
{
  return std::accumulate(data.begin(), data.end(), 0, [=](auto s, auto bits)
                         { return s + static_cast<size_t>(bits[idx]); });
}

int main()
{
  std::bitset<12> gamma_bits{};

  for(size_t i = 0; i < 12; i++) {
    gamma_bits.set(i, (get_sum_of_bits_at_idx(i) > 500));
  }

  const auto epsilon_bits = ~gamma_bits;
  
  const auto power_consumption = gamma_bits.to_ulong() * epsilon_bits.to_ulong();  
  
  std::cout << power_consumption << std::endl;
}