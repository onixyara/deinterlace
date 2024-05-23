#include <iostream>
#include <fstream>
#include <jpeglib.h>
#include <stdexcept>
#include <vector>

class JpegImage {
public:
    JpegImage(const char* input_path, const char* output_path);
    ~JpegImage();

    void deinterlace();

private:
    struct jpeg_decompress_struct cinfo;
    struct jpeg_compress_struct cinfo_out;
    struct jpeg_error_mgr jerr;
    FILE* input_file;
    FILE* output_file;
    std::vector<JSAMPLE> buffer;
    std::vector<JSAMPLE> previous_buffer;
    JDIMENSION width;
    JDIMENSION height;
    int num_components;

    void readImage();
    void writeImage();
    void applyDeinterlace(JDIMENSION row);
};

JpegImage::JpegImage(const char* input_path, const char* output_path) {
    input_file = fopen(input_path, "rb");
    if (!input_file) {
        throw std::runtime_error("Error opening input file");
    }

    output_file = fopen(output_path, "wb");
    if (!output_file) {
        fclose(input_file);
        throw std::runtime_error("Error opening output file");
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, input_file);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    width = cinfo.output_width;
    height = cinfo.output_height;
    num_components = cinfo.output_components;

    buffer.resize(width * num_components);
    previous_buffer.resize(width * num_components);

    cinfo_out.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo_out);
    jpeg_stdio_dest(&cinfo_out, output_file);

    cinfo_out.image_width = width;
    cinfo_out.image_height = height;
    cinfo_out.input_components = num_components;
    cinfo_out.in_color_space = cinfo.out_color_space;

    jpeg_set_defaults(&cinfo_out);
    jpeg_start_compress(&cinfo_out, TRUE);
}

JpegImage::~JpegImage() {
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(input_file);

    jpeg_finish_compress(&cinfo_out);
    jpeg_destroy_compress(&cinfo_out);
    fclose(output_file);
}

void JpegImage::readImage() {
    for (JDIMENSION y = 0; y < height; y++) {
        JSAMPROW row_pointer = buffer.data();
        jpeg_read_scanlines(&cinfo, &row_pointer, 1);

        applyDeinterlace(y);

        jpeg_write_scanlines(&cinfo_out, &row_pointer, 1);
    }
}

void JpegImage::applyDeinterlace(JDIMENSION row) {
    if (row > 0) {
        for (JDIMENSION i = 0; i < width * num_components; ++i) {
            buffer[i] = (buffer[i] + previous_buffer[i]) / 2;
        }
    }
    std::copy(buffer.begin(), buffer.end(), previous_buffer.begin());
}

void JpegImage::deinterlace() {
    readImage();
}

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
