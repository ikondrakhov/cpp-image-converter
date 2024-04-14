#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>

#include <filesystem>
#include <string_view>
#include <iostream>

using namespace std;

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <in_file> <out_file>"sv << endl;
        return 1;
    }

    img_lib::Path in_path = argv[1];
    img_lib::Path out_path = argv[2];
    
    const img_lib::format::ImageFormatInterface* formatInterface = img_lib::format::GetFormatInterface(in_path);
    
    if(formatInterface == nullptr) {
        cerr << "Unknown format of the input file"sv << endl;
        return 2;
    }
    
    if(img_lib::format::GetFormatByExtension(out_path) == img_lib::format::Format::UNKNOWN) {
        cerr << "Unknown format of the output file"sv << endl;
        return 3;
    }

    img_lib::Image image = formatInterface->LoadImage(in_path);
    if (!image) {
        cerr << "Loading failed"sv << endl;
        return 4;
    }

    if (!formatInterface->SaveImage(out_path, image)) {
        cerr << "Saving failed"sv << endl;
        return 5;
    }

    cout << "Successfully converted"sv << endl;
}