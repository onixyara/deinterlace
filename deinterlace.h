#ifndef DEINTERLACE_H
#define DEINTERLACE_H

#include <cstddef>
#include <cstdio>
#include <jpeglib.h>
#include <vector>
#include <string>

class JpegImage {
public:
    JpegImage(const std::string& input_path, const std::string& output_path);
    ~JpegImage();

    void deinterlace();

private:
    struct jpeg_decompress_struct cinfo;
    struct jpeg_compress_struct cinfo_out;
    struct jpeg_error_mgr jerr;
    FILE* input_file = nullptr;
    FILE* output_file = nullptr;
    std::vector<JSAMPLE> buffer;
    std::vector<JSAMPLE> previous_buffer;
    JDIMENSION width = 0;
    JDIMENSION height = 0;
    int num_components = 0;

    void readImage();
    void applyDeinterlace(JDIMENSION row);
};

#endif // DEINTERLACE_H