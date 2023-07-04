#include <iostream>
#include <fstream>
#include <vector>

struct Pixel {
    unsigned char r, g, b; // 各チャンネルの値
};

bool loadImage(const std::string& imagePath, std::vector<Pixel>& imagePixels, int& width, int& height) {
    std::ifstream file(imagePath, std::ios::binary);
    if (!file) {
        std::cout << "Failed to open the image file." << std::endl;
        return false;
    }

    // PGM画像の読み込み処理
    std::string format;
    file >> format;
    if (format != "P5") {
        std::cout << "Invalid image format. Only PGM format (P5) is supported." << std::endl;
        file.close();
        return false;
    }

    file.ignore(256, '\n'); // 不要なヘッダー行をスキップ

    // スキップするコメント行
    std::string comment;
    std::getline(file, comment);

    file >> width >> height;
    if (width <= 0 || height <= 0) {
        std::cout << "Invalid image dimensions." << std::endl;
        file.close();
        return false;
    }

    file.ignore(256, '\n'); // 最大輝度値の行をスキップ

    imagePixels.resize(width * height);
    file.read(reinterpret_cast<char*>(imagePixels.data()), width * height);

    file.close();
    return true;
}

bool saveImage(const std::string& imagePath, const std::vector<Pixel>& imagePixels, int width, int height) {
    std::ofstream file(imagePath, std::ios::binary);
    if (!file) {
        std::cout << "Failed to create the output image file." << std::endl;
        return false;
    }

    // PGM画像の保存処理
    file << "P5\n";
    file << width << " " << height << "\n";
    file << "255\n";
    file.write(reinterpret_cast<const char*>(imagePixels.data()), width * height);

    file.close();
    return true;
}

std::vector<Pixel> resizeImage(const std::vector<Pixel>& inputImage, int inputWidth, int inputHeight, int outputWidth, int outputHeight) {
    std::vector<Pixel> outputImage(outputWidth * outputHeight);

    double scaleX = static_cast<double>(inputWidth) / outputWidth;
    double scaleY = static_cast<double>(inputHeight) / outputHeight;

    for (int y = 0; y < outputHeight; ++y) {
        for (int x = 0; x < outputWidth; ++x) {
            int inputX = static_cast<int>(x * scaleX);
            int inputY = static_cast<int>(y * scaleY);

            int inputIndex = inputY * inputWidth + inputX;
            int outputIndex = y * outputWidth + x;

            outputImage[outputIndex] = inputImage[inputIndex];
        }
    }

    return outputImage;
}

void printImage(const std::vector<Pixel>& imagePixels, int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * width + x;
            std::cout << static_cast<int>(imagePixels[index].r) << " ";
            std::cout << static_cast<int>(imagePixels[index].g) << " ";
            std::cout << static_cast<int>(imagePixels[index].b) << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::string inputImagePath = "map_tsudanuma_move_orign.pgm";
    std::string outputImagePath = "map_tsudanuma_move_1472x1000.pgm";
    int targetWidth = 1472; // 目標の幅
    int targetHeight = 1000; // 目標の高さ

    std::vector<Pixel> inputImage;
    int inputWidth, inputHeight;

    // 画像の読み込み
    if (!loadImage(inputImagePath, inputImage, inputWidth, inputHeight)) {
        return -1;
    }

    // 読み込んだ画像の幅と高さを出力
    std::cout << "Input Image Dimensions: " << inputWidth << " x " << inputHeight << std::endl;

    // 画像のリサイズ
    std::vector<Pixel> resizedImage = resizeImage(inputImage, inputWidth, inputHeight, targetWidth, targetHeight);

    // 画像の保存
    if (!saveImage(outputImagePath, resizedImage, targetWidth, targetHeight)) {
        return -1;
    }

    std::cout << "Image resolution changed successfully." << std::endl;

    // リサイズ後の画像を出力
    std::cout << "Resized Image:" << std::endl;
    //printImage(resizedImage, targetWidth, targetHeight);

    return 0;
}
