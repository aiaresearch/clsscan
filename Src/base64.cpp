#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Function to encode a file into base64
std::string base64_encode(const std::string &in) {
    std::string out;

    // The base64 characters
    const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    for (const auto &c : in) {
        char_array_3[i++] = c;
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; i < 4; i++) {
                out += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i != 0) {
        for (j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++) {
            out += base64_chars[char_array_4[j]];
        }

        while ((i++ < 3)) {
            out += '=';
        }
    }

    return out;
}

// Function to read file content into a string
std::string readFile(const std::string &filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        throw std::runtime_error("Failed to open file for reading: " + filename);
    }
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}

int main() {
    try {
        // Read image file into a string
        std::string filename = "img_finish.jpg"; // Replace with your image file
        std::string imageData = readFile(filename);

        // Encode the image data to base64
        std::string base64ImageData = base64_encode(imageData);

        // Output the base64 encoded image data
        std::cout << "Base64 Encoded Image Data:\n" << base64ImageData << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}



 
 