#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    uint64_t n;
    uint64_t e;
} PublicKey;

typedef struct {
    uint64_t n;
    uint64_t d;
} PrivateKey;

uint64_t gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// This is a very basic implementation and not a proper way to generate large primes.
uint64_t generate_prime() {
    uint64_t prime = rand() % 30 + 2; // generating small prime numbers for simplicity
    int isPrime = 0;

    while (!isPrime) {
        isPrime = 1;
        for (uint64_t i = 2; i <= prime / 2; i++) {
            if (prime % i == 0) {
                isPrime = 0;
                break;
            }
        }
        if (!isPrime) {
            prime++;
        }
    }

    return prime;
}

void generate_keys(PublicKey *pubKey, PrivateKey *privKey) {
    uint64_t p = generate_prime();
    uint64_t q = generate_prime();

    while (p == q) {
        q = generate_prime();
    }

    uint64_t n = p * q;
    uint64_t phi = (p - 1) * (q - 1);

    // Choose an integer e such that e and phi(n) are coprime
    uint64_t e = 3;
    while (e < phi) {
        if (gcd(e, phi) == 1) {
            break;
        } else {
            e++;
        }
    }

    // Choose d such that it satisfies the condition d*e = 1 mod phi
    uint64_t d = 1;
    while ((d * e) % phi != 1 || d == e) {
        d++;
    }

    pubKey->n = n;
    pubKey->e = e;
    privKey->n = n;
    privKey->d = d;
}

uint64_t encrypt(uint64_t plaintext, PublicKey key) {
    uint64_t ciphertext = 1;
    for (uint64_t i = 0; i < key.e; i++) {
        ciphertext = (ciphertext * plaintext) % key.n;
    }
    return ciphertext;
}

uint64_t decrypt(uint64_t ciphertext, PrivateKey key) {
    uint64_t plaintext = 1;
    for (uint64_t i = 0; i < key.d; i++) {
        plaintext = (plaintext * ciphertext) % key.n;
    }
    return plaintext;
}

void string_encrypt(const char *plaintext, const PublicKey key, uint64_t *encrypted) {
    size_t len = strlen(plaintext);
    for (size_t i = 0; i < len; i++) {
        encrypted[i] = encrypt(plaintext[i], key);
    }
}

void string_decrypt(const uint64_t *encrypted, const PrivateKey key, char *decrypted) {
    size_t len = strlen(decrypted);
    for (size_t i = 0; i < len; i++) {
        decrypted[i] = (char)decrypt(encrypted[i], key);
    }
    decrypted[len] = '\0';  // Null-terminate the decrypted array
}

void save_to_file(const char *filename, const uint64_t *data, size_t length) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < length; i++) {
        fprintf(file, "%" PRIu64 " ", data[i]);
    }

    fclose(file);
}

void read_from_file(const char *filename, uint64_t *data, size_t *length) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    *length = 0;
    while (fscanf(file, "%" SCNu64, &data[*length]) == 1) {
        (*length)++;
    }

    fclose(file);
}

int main() {
    PublicKey pubKey;
    PrivateKey privKey;
    int choice;
    uint64_t data;
    char text[100];
    uint64_t encryptedText[100];

    while (1) {
        printf("\nRSA Encryption/Decryption Menu:\n");
        printf("1. Generate RSA Keys\n");
        printf("2. Encrypt Data\n");
        printf("3. Decrypt Data\n");
        printf("4. Read Plaintext from File and Encrypt\n");
        printf("5. Read Ciphertext from File and Decrypt\n");
        printf("6. Save Encrypted Text to File\n");
        printf("7. Save Decrypted Text to File\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Clear the input buffer
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                generate_keys(&pubKey, &privKey);
                printf("Public Key (n, e): (%" PRIu64 ", %" PRIu64 ")\n", pubKey.n, pubKey.e);
                printf("Private Key (n, d): (%" PRIu64 ", %" PRIu64 ")\n", privKey.n, privKey.d);
                break;

            case 2:
                printf("Enter plaintext to encrypt: ");
                fgets(text, sizeof(text), stdin);
                string_encrypt(text, pubKey, encryptedText);
                printf("Encrypted data: ");
                for (size_t i = 0; i < strlen(text); i++) {
                    printf("%" PRIu64 " ", encryptedText[i]);
                }
                printf("\n");
                break;

            case 3:
                printf("Enter private key components (n, d) for decryption:\n");
                printf("Enter n: ");
                scanf("%" SCNu64, &privKey.n);
                printf("Enter d: ");
                scanf("%" SCNu64, &privKey.d);

                // Clear the input buffer
                while (getchar() != '\n');

                printf("Enter ciphertext to decrypt: ");
                fgets(text, sizeof(text), stdin);

                // Tokenizing the input line to extract encrypted values
                size_t i = 0;
                int numRead;
                char *textPointer = text;  // Separate pointer variable

                while (sscanf(textPointer, "%" SCNu64 "%n", &encryptedText[i], &numRead) == 1) {
                    textPointer += numRead;  // Move the pointer by the number of characters read
                    i++;

                    // Skip any non-digit characters
                    while (*textPointer && !isdigit(*textPointer)) {
                        textPointer++;
                    }
                }

                string_decrypt(encryptedText, privKey, text);
                printf("Decrypted data: %s\n", text);
                break;

            case 4:
                printf("Enter the filename for plaintext input: ");
                scanf("%s", text);
                read_from_file(text, encryptedText, &i);
                string_encrypt(text, pubKey, encryptedText);
                printf("Encrypted data: ");
                for (size_t i = 0; i < strlen(text); i++) {
                    printf("%" PRIu64 " ", encryptedText[i]);
                }
                printf("\n");
                break;

            case 5:
                printf("Enter the filename for ciphertext input: ");
                scanf("%s", text);
                read_from_file(text, encryptedText, &i);
                string_decrypt(encryptedText, privKey, text);
                printf("Decrypted data: %s\n", text);
                break;

            case 6:
                printf("Enter the filename for encrypted text output: ");
                scanf("%s", text);
                save_to_file(text, encryptedText, i);
                printf("Encrypted text saved to %s\n", text);
                break;

            case 7:
                printf("Enter the filename for decrypted text output: ");
                scanf("%s", text);
                save_to_file(text, encryptedText, i);
                printf("Decrypted text saved to %s\n", text);
                break;

            case 0:
                printf("Exiting.\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

