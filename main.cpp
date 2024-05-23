#include "deinterlace.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_path> <output_path>\n";
        return 1;
    }

    try {
        JpegImage image(argv[1], argv[2]);
        image.deinterlace();
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
