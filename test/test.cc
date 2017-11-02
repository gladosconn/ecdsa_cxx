#include <openssl/sha.h>

#include <cstdint>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "key.h"

const char *HASH_STRING = "Hello world!";

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

TEST(Signature, Sign) {
  std::tie(signature, succ) = key.Sign(Hash(HASH_STRING));
  EXPECT_TRUE(succ);
}

TEST(Signature, Verify) {
  auto pub_key = key.CreatePubKey();
  EXPECT_TRUE(pub_key.Verify(Hash(HASH_STRING), signature));
}
