#include <iostream>
#include <fstream>
#include <vector>

// Function to read image file
std::vector<char> readImageFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file: " << filename << std::endl;
        return std::vector<char>();
    }

    // Read the entire image file into a vector
    std::vector<char> imageData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return imageData;
}

// Function to write image file
void writeImageFile(const std::string& filename, const std::vector<char>& imageData) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to create file: " << filename << std::endl;
        return;
    }

    // Write the image data to the file
    file.write(imageData.data(), imageData.size());
}

// Function to perform encryption using boolean functions
void encryptImage(std::vector<char>& imageData) {
    // Apply boolean operations (e.g., AND, OR, NOT) to the pixel values of the image
    // Example: Performing a bitwise NOT operation on each pixel value
    for (char& pixel : imageData) {
        pixel = ~pixel; // Replace with other boolean operations as needed (e.g., pixel = pixel & 0x0F)
    }
}

// Function to perform decryption using boolean functions
void decryptImage(std::vector<char>& imageData) {
    // Decrypt the image (inverse of encryption operation)
    // Example: Performing the inverse operation to restore original pixel values
    for (char& pixel : imageData) {
        pixel = ~pixel; // For example, the inverse of NOT operation is NOT itself
    }
}

int main() {
    std::cout << "Select an option:" << std::endl;
    std::cout << "1. Encrypt" << std::endl;
    std::cout << "2. Decrypt" << std::endl;

    int choice;
    std::cout << "Enter your choice (1 or 2): ";
    std::cin >> choice;

    std::string imageFilename;
    std::cout << "Enter the image file name: ";
    std::cin >> imageFilename;

    std::vector<char> imageData = readImageFile(imageFilename);
    if (imageData.empty()) {
        return 1;
    }

    switch(choice) {
        case 1: // Encrypt
            encryptImage(imageData);
            writeImageFile("encrypted_image.jpg", imageData);
            std::cout << "Image encrypted and saved as 'encrypted_image.jpg'" << std::endl;
            break;
        case 2: // Decrypt
            decryptImage(imageData);
            writeImageFile("decrypted_image.jpg", imageData);
            std::cout << "Image decrypted and saved as 'decrypted_image.jpg'" << std::endl;
            break;
        default:
            std::cout << "Invalid choice! Please select either 1 or 2." << std::endl;
            break;
    }

    return 0;
}

