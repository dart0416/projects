#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

void encryptFile(const char *inputFilename, const char *outputFilename, char key) {
    FILE *inputFile, *outputFile;
    inputFile = fopen(inputFilename, "rb");
    outputFile = fopen(outputFilename, "wb");

    if (inputFile == NULL || outputFile == NULL) {
        perror("Error opening files");
        return;
    }

    int ch;
    while ((ch = fgetc(inputFile)) != EOF) {
        ch = ch ^ key;
        fputc(ch, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
}

void decryptFile(const char *inputFilename, const char *outputFilename, char key) {
    encryptFile(inputFilename, outputFilename, key); // Encryption and decryption are the same for XOR
}

void generateRandomPassword(char *password, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#?!";
    srand((unsigned int)time(NULL)); // Initialize the random number generator

    for (int i = 0; i < length; i++) {
        int key = rand() % (sizeof(charset) - 1);
        password[i] = charset[key];
    }

    password[length] = '\0'; // Null-terminate the string
}

void savePasswordToFile(const char *filename, const char *password) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file to save password");
        return;
    }

    fprintf(file, "%s", password);
    fclose(file);
}

void processFilesInFolder(const char *inputFolder, const char *outputFolder, char key, void (*processFile)(const char *, const char *, char)) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(inputFolder)) == NULL) {
        perror("Error opening input folder");
        return;
    }

    mkdir(outputFolder, 0777);

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char *inputPath = malloc(strlen(inputFolder) + strlen(entry->d_name) + 2);
        char *outputPath = malloc(strlen(outputFolder) + strlen(entry->d_name) + 2);

        sprintf(inputPath, "%s/%s", inputFolder, entry->d_name);
        sprintf(outputPath, "%s/%s", outputFolder, entry->d_name);

        if (entry->d_type == DT_DIR) {
            // Skip directories or implement recursive processing
        } else if (entry->d_type == DT_REG) {
            processFile(inputPath, outputPath, key);
            printf("File %s processed successfully!\n", entry->d_name);
        }

        free(inputPath);
        free(outputPath);
    }

    closedir(dir);
}

int main() {
    char inputFolder[256];
    char outputFolder[256];
    char passwordFile[256] = "password.txt"; // Default password file name
    char action;

    printf("Choose action (e for encrypt, d for decrypt, g for generate new password and encrypt): ");
    scanf(" %c", &action);

    if (action == 'g') {
        int passwordLength;
        printf("Enter the password length: ");
        scanf("%d", &passwordLength);
        char password[passwordLength + 1]; // +1 for null terminator
        generateRandomPassword(password, passwordLength);
        printf("Password generated succesfull \n");
        savePasswordToFile(passwordFile, password);
        printf("Password saved to %s\n", passwordFile);
        // Use the first character of the password as the encryption key for simplicity
        char key = password[0]; 
        printf("Enter the input folder: ");
        scanf("%s", inputFolder);
        printf("Enter the output folder: ");
        scanf("%s", outputFolder);
        processFilesInFolder(inputFolder, outputFolder, key, encryptFile);
    } else {
        printf("Enter the input folder: ");
        scanf("%s", inputFolder);
        printf("Enter the output folder: ");
        scanf("%s", outputFolder);
        if (action == 'e' || action == 'd') {
            char key;
            printf("Enter the encryption/decryption key: ");
            scanf(" %c", &key);
            if (action == 'e') {
               processFilesInFolder(inputFolder, outputFolder, key, encryptFile);
            } else {
                processFilesInFolder(inputFolder, outputFolder, key, decryptFile);
            }
        } else {
            printf("Invalid action. Please choose 'e' for encrypt, 'd' for decrypt, or 'g' for generate new password and encrypt.\n");
        }
    }

    return 0;
}
 
