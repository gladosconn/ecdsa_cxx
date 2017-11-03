#include <openssl/sha.h>

#include <cstdint>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "base58.h"
#include "key.h"

const char *STRING = "Hello world!";

/**
 * @brief Hash data with SHA512 algorithm.
 *
 * @param str Input string data.
 *
 * @return Hash value.
 */
std::vector<uint8_t> Hash(const std::string &str) {
  SHA512_CTX ctx;
  SHA512_Init(&ctx);
  SHA512_Update(&ctx, str.c_str(), str.size());
  std::vector<uint8_t> md(SHA512_DIGEST_LENGTH);
  SHA512_Final(md.data(), &ctx);
  return md;
}

ecdsa::Key key;
std::vector<uint8_t> signature;
bool succ;

TEST(NewKey_Signature, Signing) {
  std::tie(signature, succ) = key.Sign(Hash(STRING));
  EXPECT_TRUE(succ);
}

TEST(NewKey_Signature, Verifying) {
  auto pub_key = key.CreatePubKey();
  EXPECT_TRUE(pub_key.Verify(Hash(STRING), signature));
}

const char *g_signature_base58 =
    "381yXZUXk9uxm4wh8M6zH6ZoJspiBUUVhvF2C6k1uXfu8ADkrjALXscmYs73yPGqA6oijHDmdz"
    "pjeSgmkfMm265hpuAXjSBB";
const char *g_priv_key_base58 = "DNymwn5s2gRgXGUxxvP3GqeBhMU3eLw14TGmuKbvpgZ7";

TEST(Import_Signature, Signing) {
  std::vector<uint8_t> priv_key_data;
  EXPECT_TRUE(
      base58::DecodeBase58(std::string(g_priv_key_base58), priv_key_data));
  ecdsa::Key key(priv_key_data);
  std::tie(signature, succ) = key.Sign(Hash(STRING));
  EXPECT_TRUE(succ);
  EXPECT_EQ(base58::EncodeBase58(signature), g_signature_base58);
}

const char *g_pub_key_base58 = "gnN4RacqEnr7929vVX6Le1HKPrudgaWW9DfQYAf9q7LG";

TEST(Import_Signature, Verifying) {
  std::vector<uint8_t> pub_key_data, signature;
  EXPECT_TRUE(base58::DecodeBase58(g_pub_key_base58, pub_key_data));
  ecdsa::PubKey pub_key(pub_key_data);
  EXPECT_TRUE(base58::DecodeBase58(g_signature_base58, signature));
  pub_key.Verify(Hash(STRING), signature);
}
