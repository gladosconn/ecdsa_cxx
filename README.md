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

# Test cases

