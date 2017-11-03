#ifndef __ECDSA_PUB_KEY_H__
#define __ECDSA_PUB_KEY_H__

#include <cstdint>
#include <vector>

#include "secp256k1.h"

namespace ecdsa {

class PubKey {
 public:
  PubKey(const PubKey &rhs) = delete;
  PubKey &operator=(const PubKey &rhs) = delete;

  PubKey(PubKey &&rhs);
  PubKey &operator=(PubKey &&rhs);

  /**
   * @brief Create public key object.
   *
   * @param pub_key_data Public key data.
   */
  explicit PubKey(const std::vector<uint8_t> &pub_key_data);

  /// Destructor.
  ~PubKey();

  /// Get public key data.
  const std::vector<uint8_t> &get_pub_key_data() const { return pub_key_data_; }

  /**
   * @brief Verify signature.
   *
   * @param hash Hash value.
   * @param sign Signature value.
   *
   * @return Returns true if the signature is valid, otherwise returns false.
   */
  bool Verify(const std::vector<uint8_t> &hash,
              const std::vector<uint8_t> &sign) const;

 private:
  std::vector<uint8_t> pub_key_data_;
  secp256k1_context_struct *ctx_ = nullptr;
};

}  // namespace ecdsa

#endif
