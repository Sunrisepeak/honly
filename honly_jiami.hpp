// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2025 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/honly
//

#ifndef JIAMI_HPP_HONLY
#define JIAMI_HPP_HONLY

#include <array>
#include <string>
#include <type_traits>

namespace honly {

using JiaMiPassword = std::array<char, 2048>; // default password size

template <std::size_t N, std::size_t M = 0>
struct JiaMiData {
    std::array<char, N> password; // encrypted password
    std::array<char, M> data; // encrypted data

    constexpr JiaMiData() : password{}, data{} {}
    constexpr JiaMiData(const std::array<char, N>& p, const std::array<char, M>& d) : password(p), data(d) {}
    constexpr JiaMiData(const JiaMiData& other) : password(other.password), data(other.data) {}

    JiaMiData& operator=(const JiaMiData&) = delete;
    JiaMiData& operator=(JiaMiData&&) = delete;
};

template<typename Encryptor>
struct JiaMi {
    // compile-time compiletime_encrypt interface
    // N includes null terminator, so need N - 1
    template<std::size_t N>
    static constexpr auto compiletime_encrypt(const char (&password)[N]) {
        JiaMiPassword data{};
        if constexpr (has_compiletime_encrypt<Encryptor>::value) {
            Encryptor::compiletime_encrypt(password, N - 1, data);
        } else {
            for (std::size_t i = 0; i < N - 1; ++i) {
                data[i] = Encryptor::apply(password[i]);
            }
        }
        return data;
    }

    template<std::size_t N, std::size_t M>
    static constexpr JiaMiData<2048, M - 1> compiletime_encrypt(const char (&password)[N], const char (&data)[M]) {
        JiaMiData<2048, M - 1> result{};
        // encrypt password
        static_assert(N > 1, "Password must be at least 2 characters long.");
        result.password = JiaMi::compiletime_encrypt<N>(password);

        // encrypt data
        static_assert(M > 1, "Data must be at least 2 characters long.");
        JiaMi::data_process(
            password, N - 1,
            data, M - 1,
            result.data
        );

        return result;
    }

    // runtime encrypt interface
    static auto runtime_encrypt(const std::string& input) {
        JiaMiPassword data{};
        if constexpr (has_compiletime_encrypt<Encryptor>::value) {
            Encryptor::compiletime_encrypt(input.c_str(), input.size(), &data);
        } else {
            for (std::size_t i = 0; i < input.size(); ++i) {
                data[i] = Encryptor::apply(input[i]);
            }
        }
        return data;
    }

    template <typename JiaMiDataType>
    static auto runtime_decrypt(const std::string& password, const JiaMiDataType &jmData) {
        if (password.empty() || jmData.data.empty()) {
            return std::string{ "password or data is empty..." };
        }

        if (!verify(password, jmData.password)) {
            return std::string{ "password verification failed..." };
        }

        std::string decrypted;
        decrypted.resize(jmData.data.size());
        JiaMi::data_process(
            password.c_str(),
            password.size(),
            jmData.data.data(),
            jmData.data.size(),
            decrypted
        );

        return decrypted;
    }

    static bool verify(const std::string& input, const JiaMiPassword& expected) {
        auto encrypted = runtime_encrypt(input);
        for (std::size_t i = 0; i < encrypted.size(); ++i) {
            if (encrypted[i] != expected[i]) {
                return false;
            }
        }
        return true;
    }

private:

    // check if Encryptor has a compile-time compiletime_encrypt method
    template<typename E, typename = void>
    struct has_compiletime_encrypt : std::false_type {};

    template<typename E>
    struct has_compiletime_encrypt<E, std::void_t<decltype(E::compiletime_encrypt(std::declval<const char *>(), std::declval<int>(), std::declval<JiaMiPassword &>()))>>
        : std::true_type {};

    // check if Encryptor has a derive_key method
    template<typename T>
    struct has_derive_key {
    private:
        template<typename U>
        static auto test(int) -> decltype(
            static_cast<char>(U::derive_key(std::declval<const char*>(), std::declval<std::size_t>(), std::declval<std::size_t>())),
            std::true_type{}
        );

        template<typename>
        static std::false_type test(...);

    public:
        static constexpr bool value = decltype(test<T>(0))::value;
    };

    template <typename JiaMiDataType> // n / m is real length
    static constexpr void data_process(const char *password, int n, const char *data, int m, JiaMiDataType &jmData) {
        for (std::size_t i = 0; i < m; ++i) {
            char key = 0;
            if constexpr (has_derive_key<Encryptor>::value) {
                key = Encryptor::derive_key(password, n, i);
            } else { // default derive_key implement
                key = JiaMi::derive_key(password, n, i);
            }
            jmData[i] = data[i] ^ key;
        }
    }

    // default derive_key implementation
    static constexpr char derive_key(const char* password, std::size_t size, std::size_t index) {
        return password[index % size] ^ 0x5A; // default XOR key
    }

};

//// Encryptor Implementations Example
//
// interface1: Encryptor::apply(char c) for character-based encryption
// interface2:
//     Encryptor::compiletime_encrypt(const char (&input)[N]) for compile-time encryption
// interface(optional):
//     Encryptor::derive_key(password, password.size(), index)
//

struct XorEncryptor { // for character-based XOR encryption
    static constexpr char key = 0x5A;

    static constexpr char apply(char c) {
        return c ^ key;
    }
};

struct FNA1AEncryptor { // for FNV-1a hash encryption

    static constexpr void compiletime_encrypt(const char *input, int len, JiaMiPassword &data) {
        auto hash = fnv1a_constexpr(input, static_cast<std::size_t>(len));
        for (int i = 0; i < len; ++i) {
            data[i] = hash & 0xFF; // store only the first byte of the hash
            hash >>= 8; // shift right to get the next byte
        }
    }

private:
    static constexpr unsigned int hashbase = 2166136261u; // FNV offset basis 32-bit
    static constexpr unsigned int prime = 16777619u;      // FNV prime 32-bit

    static constexpr unsigned int fnv1a_constexpr(const char* str, std::size_t len) {
        unsigned int hash = hashbase;
        for (std::size_t i = 0; i < len; ++i) {
            hash ^= static_cast<unsigned char>(str[i]);
            hash *= prime;
        }
        return hash;
    }
};

}

#endif