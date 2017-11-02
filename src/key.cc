#include "key.h"

#include "rnd_man.h"
#include "rnd_openssl.h"
#include "rnd_os.h"

namespace ecdsa {

const unsigned int PRIVATE_KEY_STORE_SIZE = 32;
const unsigned int PRIVATE_KEY_SIZE = 279;
const unsigned int PUBLIC_KEY_SIZE = 65;
const unsigned int SIGNATURE_SIZE = 72;

Key::Key() {
  // Create secp256k1 context.
  ctx_ = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);

  // Randomize private key.
  do {
    rnd::RandManager rnd_man(PRIVATE_KEY_STORE_SIZE);
    rnd_man.Begin();
    rnd_man.Rand<rnd::Rand_OpenSSL<128>>();
    rnd_man.Rand<rnd::Rand_OS>();
    priv_key_data_ = rnd_man.End();
  } while (!VerifyKey());

  // Calculate public key from private key.
  CalculatePublicKey(true);
}

Key::Key(const std::vector<uint8_t> &priv_key_data)
    : priv_key_data_(priv_key_data) {
  // Create secp256k1 context.
  ctx_ = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
}

Key::~Key() {
  if (ctx_) {
    secp256k1_context_destroy(ctx_);
    ctx_ = nullptr;
  }
}

bool Key::VerifyKey() const {
  return secp256k1_ec_seckey_verify(ctx_, priv_key_data_.data());
}

std::tuple<std::vector<uint8_t>, bool> Key::Sign(
    const std::vector<uint8_t> &hash) const {
  // Make signature.
  secp256k1_ecdsa_signature sig;
  int ret = secp256k1_ecdsa_sign(ctx_, &sig, hash.data(), priv_key_data_.data(),
                                 secp256k1_nonce_function_rfc6979, nullptr);
  if (ret != 1) {
    // Failed to sign.
    return std::make_tuple(std::vector<uint8_t>(), false);
  }

  // Serialize signature.
  std::vector<uint8_t> sig_out(72);
  size_t sig_out_size = 72;
  ret = secp256k1_ecdsa_signature_serialize_der(
      ctx_, (unsigned char *)sig_out.data(), &sig_out_size, &sig);
  if (ret != 1) {
    // Failed to serialize.
    return std::make_tuple(std::vector<uint8_t>(), false);
  }

  // Returns
  sig_out.resize(sig_out_size);
  return std::make_tuple(sig_out, true);
}

bool Key::CalculatePublicKey(bool compressed) {
  // Calculate public key.
  secp256k1_pubkey pubkey;
  int ret = secp256k1_ec_pubkey_create(ctx_, &pubkey, priv_key_data_.data());
  if (ret != 1) {
    return false;
  }

  // Serialize public key.
  size_t out_size = PUBLIC_KEY_SIZE;
  pub_key_data_.resize(out_size);
  secp256k1_ec_pubkey_serialize(
      ctx_, pub_key_data_.data(), &out_size, &pubkey,
      compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED);
  pub_key_data_.resize(out_size);

  // Succeed.
  return true;
}

}  // namespace ecdsa
