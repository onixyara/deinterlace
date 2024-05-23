#include "deinterlace.h"
#include <stdexcept>
#include <algorithm>

JpegImage::JpegImage(const std::string& input_path, const std::string& output_path) {
    input_file = fopen(input_path.c_str(), "rb");
    if (!input_file) {
        throw std::runtime_error("Error opening input file: " + input_path);
    }

    output_file = fopen(output_path.c_str(), "wb");
    if (!output_file) {
        fclose(input_file);
        throw std::runtime_error("Error opening output file: " + output_path);
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
    if (input_file) {
        fclose(input_file);
    }

    jpeg_finish_compress(&cinfo_out);
    jpeg_destroy_compress(&cinfo_out);
    if (output_file) {
        fclose(output_file);
    }
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
