#include "img_lib.h"

#include "jpeg_image.h"
#include "ppm_image.h"
#include "bmp_image.h"

namespace img_lib {
using namespace std;

Image::Image(int w, int h, Color fill)
    : width_(w)
    , height_(h)
    , step_(w)
    , pixels_(step_ * height_, fill) {
}

Color* Image::GetLine(int y) {
    assert(y >= 0 && y < height_);
    return pixels_.data() + step_ * y;
}

const Color* Image::GetLine(int y) const {
    return const_cast<Image*>(this)->GetLine(y);
}

int Image::GetWidth() const {
    return width_;
}

int Image::GetHeight() const {
    return height_;
}

// шаг задаёт смещение соседних строк изображения
// он обычно совпадает с width, но может быть больше
int Image::GetStep() const {
    return step_;
}
    
namespace format {
    Format GetFormatByExtension(const img_lib::Path& input_file) {
        const std::string ext = input_file.extension().string();
        if (ext == ".jpg"sv || ext == ".jpeg"sv) {
            return Format::JPEG;
        }

        if (ext == ".ppm"sv) {
            return Format::PPM;
        }
        
        if (ext == ".bmp"sv) {
            return Format::BMP;
        }

        return Format::UNKNOWN;
    }

    bool ImageFormatInterface::SaveImage(const img_lib::Path& file, const img_lib::Image& image) const {        
        Format f = GetFormatByExtension(file);
        switch(f) {
            case Format::JPEG:
                SaveJPEG(file, image);
                break;
            case Format::PPM:
                SavePPM(file, image);
                break;
            case Format::BMP:
                SaveBMP(file, image);
                break;
            default:
                false;
        }
        return true;
    }
    
    img_lib::Image PPM::LoadImage(const img_lib::Path& file) const {
        return LoadPPM(file);
    };
    
    img_lib::Image JPEG::LoadImage(const img_lib::Path& file) const {
        return LoadJPEG(file);
    };
    
    img_lib::Image BMP::LoadImage(const img_lib::Path& file) const {
        return LoadBMP(file);
    };
    
    const ImageFormatInterface* GetFormatInterface(const img_lib::Path& path) {
        Format f = GetFormatByExtension(path);
        switch(f) {
            case Format::JPEG:
                return &jpegInterface;
            case Format::PPM:
                return &ppmInterface;
            case Format::BMP:
                return &bmpInterface;
            case Format::UNKNOWN:
                return nullptr;
        }
        return nullptr;
    }
}

}  // namespace img_lib