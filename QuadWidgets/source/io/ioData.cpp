#include <algorithm>
#include <fstream>
#include <vector>

#include <QOpenGLFunctions_3_3_Core>

#include <io/ioData.h>

VolumeData::VolumeData(const std::string filePath)
{
	std::ifstream file(filePath.c_str(), std::ios::binary);

    if (file.is_open())
    {
        // Go to end to find file size
        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read into buffer
        size_t headerSize = 1024;
        file.read(reinterpret_cast<char*>(&header), headerSize);

        size -= headerSize;
        data.resize(size / 2);

        file.read(reinterpret_cast<char*>(data.data()), size);
    }
}

const uint16_csv_volume* VolumeData::getHeader() const
{
    return &header;
}

void VolumeData::fillBuffer(std::vector<GLubyte>& volData, int width, int height, int depth) 
{
    const int volumeWidth = header.recoX;
    const int volumeHeight = header.recoY;
    const int volumeDepth = header.recoZ;

    if (volData.size() != static_cast<size_t>(width * height * depth)) 
    {
        throw std::runtime_error("VolumeData::fillBuffer: destination size doesn't match specified dimensions.");
    }

    if (data.empty()) 
    {
        throw std::runtime_error("VolumeData::fillBuffer: source data is empty.");
    }

    auto [minIt, maxIt] = std::minmax_element(data.begin(), data.end());
    const uint16_t minVal = *minIt;
    const uint16_t maxVal = *maxIt;
    const float range = static_cast<float>(std::max(1, maxVal - minVal));

    for (int z = 0; z < depth; ++z) 
    {
        for (int y = 0; y < height; ++y) 
        {
            for (int x = 0; x < width; ++x) 
            {
                size_t dstIdx = x + y * width + z * width * height;

                if (x < volumeWidth && y < volumeHeight && z < volumeDepth)
                {
                    size_t srcIdx = x + volumeWidth * (y + volumeHeight * z); // Fortran-order
                    float norm = (static_cast<float>(data[srcIdx]) - minVal) / range;
                    volData[dstIdx] = static_cast<GLubyte>(std::clamp(norm, 0.0f, 1.0f) * 255.0f);
                }
                else 
                {
                    volData[dstIdx] = 0;
                }
            }
        }
    }
}