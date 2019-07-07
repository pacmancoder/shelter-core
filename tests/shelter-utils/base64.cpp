#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <vector>

#include <shelter-utils/private/base64.hpp>

using namespace Shelter::Utils;

TEST_CASE("Estimated encoded Base64 size is correct", "[shelter-utils]")
{
    REQUIRE(GetBase64EncodedOutputSize(0) == 0);
    REQUIRE(GetBase64EncodedOutputSize(1) == 4);
    REQUIRE(GetBase64EncodedOutputSize(2) == 4);
    REQUIRE(GetBase64EncodedOutputSize(3) == 4);
    REQUIRE(GetBase64EncodedOutputSize(4) == 8);
    REQUIRE(GetBase64EncodedOutputSize(5) == 8);
    REQUIRE(GetBase64EncodedOutputSize(6) == 8);
    REQUIRE(GetBase64EncodedOutputSize(7) == 12);
}

TEST_CASE("Estimated decoded Base64 size is correct", "[shelter-utils]")
{
    REQUIRE(GetBase64DecodedOutputSize(0) == 0);
    REQUIRE(GetBase64DecodedOutputSize(1) == 0);
    REQUIRE(GetBase64DecodedOutputSize(2) == 0);
    REQUIRE(GetBase64DecodedOutputSize(3) == 0);
    REQUIRE(GetBase64DecodedOutputSize(4) == 3);
    REQUIRE(GetBase64DecodedOutputSize(5) == 3);
    REQUIRE(GetBase64DecodedOutputSize(6) == 3);
    REQUIRE(GetBase64DecodedOutputSize(7) == 3);
    REQUIRE(GetBase64DecodedOutputSize(8) == 6);
    REQUIRE(GetBase64DecodedOutputSize(9) == 6);
}


TEST_CASE("Encoded Base64 is correct", "[shelter-utils]")
{
    auto assertValid = [](const char* expected, const std::vector<uint8_t>& input)
    {
        std::string expectedString(expected);
        std::vector<char> buffer(GetBase64EncodedOutputSize(input.size()) + 1);

        auto encodedSize = EncodeBase64(input.data(), input.size(), buffer.data());

        REQUIRE(encodedSize == expectedString.size());
        REQUIRE(expectedString == buffer.data());
    };

    assertValid("Qg==", std::vector<uint8_t> { 0x42 });
    assertValid("VWY=", std::vector<uint8_t> { 0x55, 0x66 });
    assertValid("AQID", std::vector<uint8_t> { 0x01, 0x02, 0x03 });
    assertValid("diNVEg==", std::vector<uint8_t> { 0x76, 0x23, 0x55, 0x12 });
    assertValid(
        "aGVsbG8gd29ybGQ=",
        std::vector<uint8_t> {
            0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64
        }
    );
}

TEST_CASE("Decoded Base64 is correct", "[shelter-utils]")
{
    auto assertValid = [](const std::vector<uint8_t> expected, const std::string& input)
    {
        std::vector<uint8_t> buffer(GetBase64DecodedOutputSize(input.size()));
        auto decodedSize = DecodeBase64(input.data(), input.size(), buffer.data());
        buffer.resize(decodedSize);

        REQUIRE(expected == buffer);
    };

    assertValid(std::vector<uint8_t> { 0x42 }, "Qg==");
    assertValid(std::vector<uint8_t> { 0x55, 0x66 }, "VWY=");
    assertValid(std::vector<uint8_t> { 0x01, 0x02, 0x03 }, "AQID");
    assertValid(std::vector<uint8_t> { 0x76, 0x23, 0x55, 0x12 }, "diNVEg==");
    assertValid(
        std::vector<uint8_t> {
            0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64
        },
        "aGVsbG8gd29ybGQ="
    );
}