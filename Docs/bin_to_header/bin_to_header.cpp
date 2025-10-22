// bin_to_header.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

// Function to convert a file path to a valid C++ variable name
std::string sanitize_name(const std::string& filename) {
    std::string var_name = filename;
    for (size_t i = 0; i < var_name.length(); ++i) {
        if (!isalnum(var_name[i])) {
            var_name[i] = '_';
        }
    }
    return var_name;
}

void convert_file(const std::string& input_path) {
    std::ifstream f_in(input_path, std::ios::binary | std::ios::ate);
    if (!f_in.is_open()) {
        std::cerr << "Error: Could not open input file '" << input_path << "'\n";
        return;
    }

    std::streamsize size = f_in.tellg();
    f_in.seekg(0, std::ios::beg);

    std::string filename = input_path.substr(input_path.find_last_of("/\\") + 1);
    std::string base_name = filename.substr(0, filename.find_last_of('.'));
    std::string var_name = sanitize_name(base_name);
    std::string output_path = base_name + ".embed";

    std::ofstream f_out(output_path);
    if (!f_out.is_open()) {
        std::cerr << "Error: Could not create output file '" << output_path << "'\n";
        return;
    }

    std::cout << "Converting " << filename << " -> " << output_path << "..." << std::endl;

    f_out << "// Converted from " << filename << "\n";
    f_out << "const unsigned int " << var_name << "_size = " << size << ";\n";
    f_out << "const unsigned char " << var_name << "[] = {\n    ";

    int byte_count = 0;
    char byte;
    while (f_in.get(byte)) {
        f_out << "0x" << std::hex << std::setw(2) << std::setfill('0') << (0xFF & byte) << ", ";
        byte_count++;
        if (byte_count % 16 == 0) {
            f_out << "\n    ";
        }
    }

    f_out << "\n};\n";

    f_in.close();
    f_out.close();

    std::cout << "Success!" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file1> <file2> ...\n";
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        convert_file(argv[i]);
    }

    return 0;
}