#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

//encryption rsa using XOR 

// Function to encrypt a single file using XOR encryption
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
    } //ecrypting using XOR

    fclose(inputFile);
    fclose(outputFile);
}

// Function to decrypt a single file
void decryptFile(const char *inputFilename, const char *outputFilename, char key) {
    encryptFile(inputFilename, outputFilename, key);
}

// function to process files in a folder based on the provided processFile function (encrypt or decrypt)
void processFilesInFolder(const char *inputFolder, const char *outputFolder, char key, void (*processFile)(const char *, const char *, char)) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(inputFolder);
    if (dir == NULL) {
        perror("Error opening input folder");
        return;
    }

    mkdir(outputFolder, 0777); //creating output folder with 777 perms 

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Check if it's a regular file
            char *inputPath;
            char *outputPath;

            // allocate memory for input and output paths
            inputPath = (char *)malloc(strlen(inputFolder) + strlen(entry->d_name) + 2); // +2 for '/' and null terminator
            outputPath = (char *)malloc(strlen(outputFolder) + strlen(entry->d_name) + 12); // +12 for '_encrypted' and null terminator

            if (inputPath == NULL || outputPath == NULL) {
                perror("Memory allocation error");
                return;
            }

            // Construct the input and output paths
            sprintf(inputPath, "%s/%s", inputFolder, entry->d_name);
            sprintf(outputPath, "%s/%s_encrypted", outputFolder, entry->d_name);

            // Process the file (either encrypt or decrypt)
            processFile(inputPath, outputPath, key);
            printf("File %s processed successfully!\n", entry->d_name);

            // Free dynamically allocated memory
            free(inputPath);
            free(outputPath);
        }
    }

    closedir(dir);
}

int main() {
    char inputFolder[256]; // name length
    char outputFolder[256];
    char key;
    char action;

    // Get user input for action (encrypt or decrypt)
    printf("Choose action (e for encrypt, d for decrypt): ");
    scanf(" %c", &action);

    // Get user input for input folder
    printf("Enter the input folder: ");
    scanf("%s", inputFolder);

    // Get user input for output folder
    printf("Enter the output folder: ");
    scanf("%s", outputFolder);

    // Get user input for encryption/decryption key
    printf("Enter the encryption/decryption key: ");
    scanf(" %c", &key);

    // Determine whether to encrypt or decrypt based on user input
    if (action == 'e' || action == 'E') {
        processFilesInFolder(inputFolder, outputFolder, key, encryptFile);
    } else if (action == 'd' || action == 'D') {
        processFilesInFolder(inputFolder, outputFolder, key, decryptFile);
    } else {
        printf("Invalid action. Please choose 'e' for encrypt or 'd' for decrypt.\n");
    }

    return 0;
}
