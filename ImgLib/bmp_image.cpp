#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>

using namespace std;

namespace img_lib {

PACKED_STRUCT_BEGIN BitmapFileHeader {
    // поля заголовка Bitmap File Header
    char BM[2];
    unsigned int size;
    unsigned int space;
    unsigned int padding;
}
PACKED_STRUCT_END

istream& operator >>(istream& input, BitmapFileHeader& header) {
    input >> header.BM[0] >> header.BM[1];
    input.read(reinterpret_cast<char*>(&header.size), 4);
    input.read(reinterpret_cast<char*>(&header.space), 4);
    input.read(reinterpret_cast<char*>(&header.padding), 4);
    return input;
}

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    // поля заголовка Bitmap Info Header
    unsigned int size;
    int width;
    int height;
    int16_t planes_count;
    int16_t bit_per_pixel;
    unsigned int compression_type;
    unsigned int byte_count;
    int horizontal_resolution;
    int vertical_resolution;
    int colors_count;
    int significant_colors_count;
}
PACKED_STRUCT_END

istream& operator >>(istream& input, BitmapInfoHeader& header) {
    input.read(reinterpret_cast<char*>(&header.size), 4);
    input.read(reinterpret_cast<char*>(&header.width), 4);
    input.read(reinterpret_cast<char*>(&header.height), 4); 
    input.read(reinterpret_cast<char*>(&header.planes_count), 2);
    input.read(reinterpret_cast<char*>(&header.bit_per_pixel), 2);
    input.read(reinterpret_cast<char*>(&header.compression_type), 4);
    input.read(reinterpret_cast<char*>(&header.byte_count), 4);
    input.read(reinterpret_cast<char*>(&header.horizontal_resolution), 4);
    input.read(reinterpret_cast<char*>(&header.vertical_resolution), 4);
    input.read(reinterpret_cast<char*>(&header.colors_count), 4);
    input.read(reinterpret_cast<char*>(&header.significant_colors_count), 4);
    return input;
}

// функция вычисления отступа по ширине
static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

// напишите эту функцию
bool SaveBMP(const Path& file, const Image& image) {
    ofstream ofs(file, ios::binary);
    
    BitmapFileHeader header = {
        {'B', 'M'},
        54u + (unsigned) GetBMPStride(image.GetWidth()) * image.GetHeight(),
        0,
        54
    };
    BitmapInfoHeader info = {
        40,
        image.GetWidth(),
        image.GetHeight(),
        1,
        24,
        0,
        (unsigned) GetBMPStride(image.GetWidth()) * image.GetHeight(),
        11811,
        11811,
        0,
        0x1000000
    };
    ofs.write(reinterpret_cast<char*>(&header), sizeof(header));
    ofs.write(reinterpret_cast<char*>(&info), sizeof(info));

    int stride_size = GetBMPStride(image.GetWidth());
    char stride[stride_size];

    const int w = image.GetWidth();
    const int h = image.GetHeight();
    std::vector<char> buff(stride_size);

    for (int y = 0; y < h; ++y) {
        const Color* line = image.GetLine(h - y - 1);
        for (int x = 0; x < w; ++x) {
            buff[x * 3 + 0] = static_cast<char>(line[x].b);
            buff[x * 3 + 1] = static_cast<char>(line[x].g);
            buff[x * 3 + 2] = static_cast<char>(line[x].r);
        }
        ofs.write(buff.data(), stride_size);
    }
    
    return true;
}

// напишите эту функцию
Image LoadBMP(const Path& file) {
    // открываем поток с флагом ios::binary
    // поскольку будем читать данные в двоичном формате
    ifstream ifs(file, ios::binary);
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;
    
    ifs >> fileHeader >> infoHeader;

    int stride_size = GetBMPStride(infoHeader.width);
    std::vector<char> stride(stride_size);

    Image result(infoHeader.width, infoHeader.height, Color::Black());
    std::vector<char> buff(infoHeader.width * 3);
    unsigned h = infoHeader.height;
    unsigned w = infoHeader.width;

    for (int y = 0; y < h; ++y) {
        Color* line = result.GetLine(h - y - 1);
        ifs.read(buff.data(), stride_size);

        for (int x = 0; x < w; ++x) {
            line[x].b = static_cast<byte>(buff[x * 3 + 0]);
            line[x].g = static_cast<byte>(buff[x * 3 + 1]);
            line[x].r = static_cast<byte>(buff[x * 3 + 2]);
        }
    }

    return result;
}

}  // namespace img_lib