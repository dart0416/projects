#include <iostream>
#include <cmath>
#include <cstring>

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int is_prime(int num) {
    if (num < 2) {
        return 0;
    }
    for (int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int mod_inverse(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    if (m == 1) {
        return 0;
    }

    while (a > 1) {
        q = a / m;
        t = m;

        m = a % m;
        a = t;
        t = x0;

        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) {
        x1 += m0;
    }

    return x1;
}

int generate_prime_number() {
    int num;
    while (1) {
        num = rand() % 400 + 100;  // Adjust the range for larger prime numbers
        if (is_prime(num)) {
            return num;
        }
    }
}

int* generate_keypair(int p, int q) {
    int n = p * q;
    int phi = (p - 1) * (q - 1);

    static int keys[4];

    int e = 2;
    while (gcd(e, phi) != 1) {
        e++;
    }

    int d = mod_inverse(e, phi);

    keys[0] = e;
    keys[1] = n;
    keys[2] = d;
    keys[3] = n;

    return keys;
}

char* encrypt(int* public_key, char* plaintext) {
    int e = public_key[0];
    int n = public_key[1];
    int i = 0;
    char* cipher = new char[100];

    while (plaintext[i] != '\0') {
        int val = plaintext[i];
        int encrypted = fmod(pow(val, e), n);
        sprintf(&cipher[i * 4], "%d ", encrypted);
        i++;
    }

    return cipher;
}

char* decrypt(char* ciphertext, int d, int n) {
    char manual_d;
    std::cout << "Do you want to manually insert private key (d, n) for decryption? (Y/N): ";
    std::cin >> manual_d;

    if (manual_d == 'Y' || manual_d == 'y') {
        std::cout << "Enter private key 'd': ";
        std::cin >> d;
        std::cout << "Enter modulus 'n': ";
        std::cin >> n;
    } else {
        std::cout << "Private key insertion declined. Cannot decrypt without private key." << std::endl;
        return "";
    }

    int i = 0;
    char* decrypted = new char[100];
    char* token = strtok(ciphertext, " ");
    while (token != NULL) {
        int val = atoi(token);
        int decrypted_val = fmod(pow(val, d), n);
        decrypted[i] = decrypted_val;
        i++;
        token = strtok(NULL, " ");
    }
    decrypted[i] = '\0';

    return decrypted;
}

void display_keys(int* public_key, int* private_key) {
    std::cout << "Public Key (e, n): (" << public_key[0] << ", " << public_key[1] << ")" << std::endl;
    std::cout << "Private Key (d, n): (" << private_key[0] << ", " << private_key[1] << ")" << std::endl;
}

int main() {
    std::cout << "RSA Algorithm Implementation" << std::endl;

    while (1) {
        std::cout << "\nChoose an option:\n";
        std::cout << "1. Generate Key Pair\n";
        std::cout << "2. Encrypt\n";
        std::cout << "3. Decrypt\n";
        std::cout << "4. Display Keys\n";
        std::cout << "5. Exit\n";

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        static int* public_key;
        static int* private_key;
        static char* ciphertext;

        switch (choice) {
            case 1: {
                int p = generate_prime_number();
                int q = generate_prime_number();
                std::cout << "Generated prime numbers:\np = " << p << "\nq = " << q << std::endl;
                public_key = generate_keypair(p, q);
                private_key = new int[2];
                private_key[0] = public_key[2];
                private_key[1] = public_key[3];
                std::cout << "Keys generated successfully!" << std::endl;
                display_keys(public_key, private_key);
                break;
            }
            case 2: {
                if (!public_key) {
                    std::cout << "Please generate keys first!" << std::endl;
                    break;
                }
                std::cin.ignore(); // Clear buffer
                char plaintext[100];
                std::cout << "Enter the message to encrypt: ";
                std::cin.getline(plaintext, 100);
                ciphertext = encrypt(public_key, plaintext);
                std::cout << "Encrypted message: " << ciphertext << std::endl;
                break;
            }
            case 3: {
                if (!private_key) {
                    std::cout << "Please generate keys first!" << std::endl;
                    break;
                }
                if (!ciphertext) {
                    std::cout << "Please encrypt a message first!" << std::endl;
                    break;
                }
                std::cout << "Enter the ciphertext to decrypt: ";
                char decrypt_input[100];
                std::cin >> decrypt_input;
                char* decrypted = decrypt(decrypt_input, private_key[0], private_key[1]);
                std::cout << "Decrypted message: " << decrypted << std::endl;
                break;
            }
            case 4: {
                if (!public_key || !private_key) {
                    std::cout << "Keys not generated yet!" << std::endl;
                } else {
                    display_keys(public_key, private_key);
                }
                break;
            }
            case 5:
                std::cout << "Exiting..." << std::endl;
                exit(0);
            default:
                std::cout << "Invalid choice. Please enter a valid option." << std::endl;
                break;
        }
    }

    return 0;
}

