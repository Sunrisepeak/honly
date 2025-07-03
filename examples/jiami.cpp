#include <iostream>

#include "honly_jiami.hpp"

/*
objdump -s -j .rodata a.out
strings a.out | grep -i "123456"
*/

// Custom Encryptor Example
struct SimpleEncryptor {
    // character-based encryption
    static constexpr char apply(char c) {
        return c + 1;
    }

    // use default derive_key
};

constexpr auto jiamiData = honly::JiaMi<SimpleEncryptor>::compiletime_encrypt("abcdef", R"(
a = 1
b = 2
print("Hello World")
print(a + b)
)");

struct CustomEncryptor {
    static constexpr void compiletime_encrypt(const char *input, int len, honly::JiaMiPassword &data) {
        for (int i = 0; i < len; i++) {
            data[i] = 'a';
        }
    }

    static constexpr char derive_key(const char* password, std::size_t size, std::size_t index) {
        return 0x20;
    }
};

constexpr auto jiamiData2 = honly::JiaMi<CustomEncryptor>::compiletime_encrypt("123456", "Hello World! -> abc");

int main() {

    std::string password;
    std::cout << "[0] Enter password: ";
    std::cin >> password;

    auto script = honly::JiaMi<SimpleEncryptor>::runtime_decrypt(password, jiamiData);

    std::cout << "[1] Decrypted Data(Python script):" << std::endl;
    std::cout << script << std::endl;

    std::cout << "[2] Running the script...\n" << std::endl;
    
    std::string command = "python -c \'" + script + "\'";
    auto ret = std::system(command.c_str());

    if (ret != 0) {
       std::cout << "[3] Try get tips..." << std::endl;
       std::cout << honly::JiaMi<SimpleEncryptor>::runtime_decrypt(password, jiamiData2) << std::endl;
    }

    return 0;
}