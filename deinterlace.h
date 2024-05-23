#ifndef DEINTERLACE_H
#define DEINTERLACE_H

#include <stddef.h>
#include <stdio.h>
#include <jpeglib.h>
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

#endif // DEINTERLACE_H
