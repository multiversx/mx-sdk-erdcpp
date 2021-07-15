#include "gtest/gtest.h"

#include "utils/hex.h"
#include "utils/errors.h"
#include "filehandler/pemreader.h"
#include "filehandler/keyfilereader.h"

class PemFileReaderConstructorFixture : public ::testing::Test
{
public:
    template <typename T>
    void expectException(std::string const &filePath, errorMessage const &errMsg)
    {
        EXPECT_THROW({
                         try
                         {
                             PemFileReader pemHandler(filePath);
                         }
                         catch(const T &e)
                         {
                             std::string const err = e.what();
                             EXPECT_TRUE(err.find(errMsg) != std::string::npos );
                             throw;
                         }
                     }, T );
    }

};

TEST_F(PemFileReaderConstructorFixture, validFile)
{
    EXPECT_NO_THROW(PemFileReader pemHandler("..//..//testData//keysValid1.pem"));
}

TEST_F(PemFileReaderConstructorFixture, invalidFile_notEnoughBytes)
{
    expectException<std::length_error>("..//..//testData//keysNotEnoughBytes.pem",ERROR_MSG_KEY_BYTES_SIZE);
}

TEST_F(PemFileReaderConstructorFixture, invalidFile_invalidFileExtension)
{
    expectException<std::invalid_argument>("..//..//testData//keysInvalidExtension.pme",ERROR_MSG_FILE_EXTENSION_INVALID);
}

TEST_F(PemFileReaderConstructorFixture, invalidFile_emptyFile)
{
    expectException<std::invalid_argument>("..//..//testData//keysEmptyFile.pem",ERROR_MSG_FILE_EMPTY);
}

TEST_F(PemFileReaderConstructorFixture, invalidFile_notExisting)
{
    expectException<std::invalid_argument>("..//..//testData//thisFileDoesNotExist.pem",ERROR_MSG_FILE_DOES_NOT_EXIST);
}

struct pemData
{
    std::string filePath;
    std::string seed;
    std::string publicKey;
    std::string bech32Address;
};

class PemFileReaderParametrized : public ::testing::TestWithParam<pemData> {};

INSTANTIATE_TEST_CASE_P (
        ValidPemFiles,
        PemFileReaderParametrized,
        ::testing::Values
        (pemData {"..//..//testData//keysValid2.pem",                                   // File path
                  "413f42575f7f26fad3317a778771212fdb80245850981e48b58a4f25e344e8f9",   // Seed
                  "0139472eff6886771a982f3083da5d421f24c29181e63888228dc81ca60d69e1",   // Public key
                  "erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"},    // Bech32 address

         pemData {"..//..//testData//keysValid3.pem",                                   // File path
                  "b8ca6f8203fb4b545a8e83c5384da033c415db155b53fb5b8eba7ff5a039d639",   // Seed
                  "8049d639e5a6980d1cd2392abcce41029cda74a1563523a202f09641cc2618f8",   // Public key
                  "erd1spyavw0956vq68xj8y4tenjpq2wd5a9p2c6j8gsz7ztyrnpxrruqzu66jx"},    // Bech32 address

         pemData {"..//..//testData//keysValid4.pem",                                   // File path
                  "e253a571ca153dc2aee845819f74bcc9773b0586edead15a94cb7235a5027436",   // Seed
                  "b2a11555ce521e4944e09ab17549d85b487dcd26c84b5017a39e31a3670889ba",   // Public key
                  "erd1k2s324ww2g0yj38qn2ch2jwctdy8mnfxep94q9arncc6xecg3xaq6mjse8"}));  // Bech32 address

TEST_P(PemFileReaderParametrized, getSeed_getPublicKey_getBech32Address)
{
    pemData const& currParam = GetParam();

    PemFileReader const pemReader(currParam.filePath);

    bytes const pemSeed = pemReader.getSeed();
    bytes const pemPubKey = pemReader.getAddress().getPublicKey();
    std::string const pemBech32Address = pemReader.getAddress().getBech32Address();

    EXPECT_EQ(pemSeed, util::hexToBytes(currParam.seed));
    EXPECT_EQ(pemPubKey, util::hexToBytes(currParam.publicKey));
    EXPECT_EQ(pemBech32Address, currParam.bech32Address);
}

TEST(KeyFileReader, someTest)
{
    KeyFileReader keys("..//..//testData//keyFile.json", "12345678Qq!");
    std::cerr<<keys.getAddress().getBech32Address();
}