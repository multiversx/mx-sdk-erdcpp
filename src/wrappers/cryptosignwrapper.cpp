#include "cryptosignwrapper.h"
#include "strchr.h"
#include "errors.h"

#include <sodium.h>
#include <stdexcept>
#include "aes_128_ctr/aes.hpp"

#if \
    (PUBLIC_KEY_LENGTH != crypto_sign_PUBLICKEYBYTES) || \
    (SECRET_KEY_LENGTH != crypto_sign_SECRETKEYBYTES) || \
    (SEED_LENGTH  != crypto_sign_SEEDBYTES) ||           \
    (SIGNATURE_LENGTH != crypto_sign_BYTES) ||           \
    (HMAC_SHA256_BYTES != crypto_auth_hmacsha256_BYTES)
#pragma message "Error. Libsodium library was updated. Update define parameters in the wrapper!"

#else

namespace wrapper
{
namespace crypto
{
std::string getSignature(bytes const &secretKey, std::string const &message)
{
    auto msg = reinterpret_cast<unsigned char const *>(message.data());
    auto sk = reinterpret_cast<unsigned char const *>(secretKey.data());

    unsigned char sig[SIGNATURE_LENGTH];
    unsigned long long sigLength;

    crypto_sign_detached(sig, &sigLength, msg, message.length(), sk);

    return util::uCharToStr(sig, sigLength);
}

bytes getSeed(bytes const &secretKey)
{
    auto sk = reinterpret_cast<const unsigned char*>(secretKey.data());

    unsigned char seed[SEED_LENGTH];

    crypto_sign_ed25519_sk_to_seed(seed, sk);

    return bytes(seed, seed + SEED_LENGTH);
}

bytes getSecretKey(bytes const &seed)
{
    auto sd = reinterpret_cast<const unsigned char*>(seed.data());

    unsigned char pk[PUBLIC_KEY_LENGTH];
    unsigned char sk[SECRET_KEY_LENGTH];

    crypto_sign_seed_keypair(pk, sk, sd);

    return bytes(sk, sk + SECRET_KEY_LENGTH);
}

bytes getPublicKey(bytes const &secretKey)
{
    auto sk = reinterpret_cast<const unsigned char*>(secretKey.data());

    unsigned char pk[PUBLIC_KEY_LENGTH];

    crypto_sign_ed25519_sk_to_pk(pk, sk);

    return bytes(pk, pk + PUBLIC_KEY_LENGTH);
}

bool verify(std::string const &signature, std::string const &message, bytes const &publicKey)
{
    auto sig = reinterpret_cast<const unsigned char*>(signature.data());
    auto msg = reinterpret_cast<const unsigned char*>(message.data());
    auto pk = reinterpret_cast<const unsigned char*>(publicKey.data());
    auto msgLen = message.size();

    int const res = crypto_sign_verify_detached(sig, msg, msgLen, pk);

    return res == 0;
}

bytes scryptsy(std::string const &password, KdfParams const &kdfParams)
{
    unsigned int const keyLength = kdfParams.dklen;
    unsigned char derivedKey[keyLength];

    auto passw = reinterpret_cast<const unsigned char*>(password.data());
    auto salt = reinterpret_cast<const unsigned char*>(kdfParams.salt.data());

    if (crypto_pwhash_scryptsalsa208sha256_ll
            (passw, password.size(),
             salt, kdfParams.salt.size(),
             kdfParams.n,
             kdfParams.r,
             kdfParams.p,
             derivedKey, keyLength) !=0)
    {
        throw std::runtime_error(ERROR_MSG_SCRYPTSY);
    }

    return bytes(derivedKey, derivedKey + keyLength);
}

std::string hmacsha256(bytes const &key, std::string const &cipherText)
{
    auto k = reinterpret_cast<const unsigned char*>(key.data());
    auto cipher = reinterpret_cast<const unsigned char*>(cipherText.data());

    unsigned char digest[HMAC_SHA256_BYTES];

    crypto_auth_hmacsha256_state state;

    crypto_auth_hmacsha256_init(&state, k, key.size());
    crypto_auth_hmacsha256_update(&state, cipher, cipherText.size());
    crypto_auth_hmacsha256_final(&state, digest);

    return std::string(digest, digest + HMAC_SHA256_BYTES);
}

bytes aes128ctrDecrypt(bytes const &key, std::string const &cipherText, std::string const &iv)
{
    auto k = reinterpret_cast<const unsigned char*>(key.data());
    auto initVector = reinterpret_cast<const unsigned char*>(iv.data());
    auto cipher = reinterpret_cast<unsigned char*>(const_cast<char*>(cipherText.data()));
    auto cipherSize = cipherText.size();

    AES_ctx ctx{};

    AES_init_ctx_iv(&ctx, k, initVector);
    AES_CTR_xcrypt_buffer(&ctx, cipher, cipherSize);

    return bytes(cipher, cipher + cipherSize);
}

}
}

#endif
