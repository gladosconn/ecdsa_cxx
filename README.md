# ECDSA++ ![build status](https://travis-ci.org/gladosconn/ecdsa_cxx.svg?branch=master)

**ECDSA++** is a ECDSA algorithm C++ wrap library base on libsecp256k1.

# Features

* ECDSA signing/verification and key generation.
* Only two classes `ecdsa::Key` and `ecdsa::PubKey`, very easy to use.

# Build steps

## Requirements

* C/C++ compiler which supports C++11, `clang 3+` or `gcc 4.7+`.
* OpenSSL library installed.
* CMake and Make toolset installed.

## Build instructions

### MacOS

1. Ensure you have installed Xcode command-line toolset.
```
xcode-select --install
```

2. You need install CMake and Make toolset, install them by using [brew](https://brew.sh) is a good idea.
```
brew install cmake

```

3. Clone ECDSA++ source code.
```
git clone https://github.com/gladosconn/ecdsa_cxx
```

4. Compile library secp256k1.
```
cd ecdsa_cxx/secp256k1
./autogen.sh && ./configure
make
```

5. Compile ECDSA++
```
cd ..
mkdir build
cd build
OPENSSL_ROOT_DIR=/usr/local/opt/openssl cmake ..
make
```

6. Run test
```
./ecdsa_test
```

General, test program will show results like below

```
Running main() from gtest_main.cc
[==========] Running 2 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 2 tests from Signature
[ RUN      ] Signature.Sign
[       OK ] Signature.Sign (0 ms)
[ RUN      ] Signature.Verify
[       OK ] Signature.Verify (19 ms)
[----------] 2 tests from Signature (19 ms total)
[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (19 ms total)
[  PASSED  ] 2 tests.
```

# How to use?

## Create a new key pair.

Create an object of class `ecdsa::Key` without any parameters will create a key pair, and you are ready to use this key pair to sign message.

```
#include "ecdsa/key.h"

int main() {
  // A key object created without any parameters will generate a random key
  // pair automatically.
  ecdsa::Key key; 

  // ...

  // Returns
  return 0;
}
```

## Sign a message and verify it.

To signing a message, you need do 2 things.

1. Hash your message and get the hash value.
2. Signing the hash value with key and get signature.

```
#include <openssl/sha.h>
#include "ecdsa/key.h"

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

const char *HASH_STRING = "Hello world!";

int main() {
  ecdsa::Key key;
  std::vector<uint8_t> signature;
  bool succ;
  std::tie(signature, succ) = key.Sign(Hash(HASH_STRING));
  if (succ) {
    // Signature created succeed, now create a public key object from key.
    auto pub_key = key.CreatePubKey();
    // Verify signature.
    bool verified = pub_key.Verify(Hash(HASH_STRING), signature);
    if (verified) {
      // TODO Signature verified
    }
  } else {
    // TODO FAILED!
  }
}
```

## Import private key.

Import private key data is easy. Just construct a object of `ecdsa::Key` with key data vector.

```
int main() {
  std::vector<uint8_t> priv_key_data;
  // TODO Reading private key data to priv_key_data, ...
  ecdsa::Key key(priv_key_data); // Construct key object with priv_key_data.
  // TODO Use key to signing
}
```

## Import public key.

If you import a public key, you can use `ecdsa::PubKey` object to verify a signature. Create a `ecdsa::PubKey` object with public key data will import public key to key object.

## Base58 support.

ECDSA++ includes base58 module (Which is import from bitcoin-core project), include "base58.h" to get base58 support. See "test/test.cc" for more information.

# Problems?

If you have any question, please fire an issue or if you have fixed any bug or make any improvement, pull request is very welcomed!

