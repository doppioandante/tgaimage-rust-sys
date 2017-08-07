#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <fstream>

#pragma pack(push,1)
struct TGA_Header {
    char idlength;
    char colormaptype;
    char datatypecode;
    short colormaporigin;
    short colormaplength;
    char colormapdepth;
    short x_origin;
    short y_origin;
    short width;
    short height;
    char  bitsperpixel;
    char  imagedescriptor;
};
#pragma pack(pop)

struct TGAColor {
    unsigned char bgra[4];
    unsigned char bytespp;

    TGAColor();
	TGAColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A=255);
    TGAColor(unsigned char v);
    TGAColor(const unsigned char *p, unsigned char bpp);
    unsigned char& operator[](const int i);
    TGAColor operator *(float intensity) const;
};

class TGAImage {
private:
    unsigned char* data;
    int width;
    int height;
    int bytespp;

    bool   load_rle_data(std::ifstream &in);
    bool unload_rle_data(std::ofstream &out) const;
public:
    enum Format {
        GRAYSCALE=1, RGB=3, RGBA=4
    };

    TGAImage();
    TGAImage(int w, int h, int bpp);
    TGAImage(const TGAImage &img);
    bool read_tga_file(const char *filename);
    bool write_tga_file(const char *filename, bool rle=true) const;
    bool flip_horizontally();
    bool flip_vertically();
    bool scale(int w, int h);
    TGAColor get(int x, int y) const;
    bool set(int x, int y, TGAColor &c);
    bool set(int x, int y, const TGAColor &c);
    ~TGAImage();
    TGAImage & operator =(const TGAImage &img);
    int get_width() const;
    int get_height() const;
    int get_bytespp() const;
    unsigned char *buffer() const;
    void clear();
};

#endif //__IMAGE_H__

