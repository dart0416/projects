import os
import shutil
import random
import string

def encrypt_file(input_filename, output_filename, key):
    with open(input_filename, 'rb') as input_file:
        with open(output_filename, 'wb') as output_file:
            while True:
                byte = input_file.read(1)
                if not byte:
                    break
                encrypted_byte = ord(byte) ^ ord(key)
                output_file.write(bytes([encrypted_byte]))

def decrypt_file(input_filename, output_filename, key):
    encrypt_file(input_filename, output_filename, key)  # Encryption and decryption are the same for XOR

def generate_random_password(length):
    charset = string.ascii_letters + string.digits + ',.-#?!'
    return ''.join(random.choice(charset) for _ in range(length))

def save_password_to_file(filename, password):
    with open(filename, 'w') as file:
        file.write(password)

def process_files_in_folder(input_folder, output_folder, key, action):
    for item in os.listdir(input_folder):
        input_path = os.path.join(input_folder, item)
        output_path = os.path.join(output_folder, item)
        if os.path.isdir(input_path):
            if action == 'encrypt':
                os.makedirs(output_path, exist_ok=True)
                process_files_in_folder(input_path, output_path, key, action)
            else:
                # Skipping directory for decryption
                continue
        elif os.path.isfile(input_path):
            if action == 'encrypt':
                encrypt_file(input_path, output_path, key)
            elif action == 'decrypt':
                decrypt_file(input_path, output_path, key)
            print(f"File {item} processed successfully!")

def main():
    input_folder = input("Enter the input folder: ")
    output_folder = input("Enter the output folder: ")
    action = input("Choose action (encrypt/e, decrypt/d): ").lower()

    if action == 'encrypt' or action == 'e':
        password_length = int(input("Enter the password length: "))
        password = generate_random_password(password_length)
        print(f"Generated Password: {password}")
        password_file = "password.txt"
        save_password_to_file(password_file, password)
        print(f"Password saved to {password_file}")
        key = password[0]
        process_files_in_folder(input_folder, output_folder, key, 'encrypt')
    elif action == 'decrypt' or action == 'd':
        key = input("Enter the encryption key: ")
        process_files_in_folder(input_folder, output_folder, key, 'decrypt')
    else:
        print("Invalid action. Please choose 'encrypt'/'e' for encryption or 'decrypt'/'d' for decryption.")

if __name__ == "__main__":
    main()

