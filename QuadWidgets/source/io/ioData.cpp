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

        for (uint16_t& val : data)
        {
            val = (val >> 8) | (val << 8);
        }
    }
}

const uint16_csv_volume* VolumeData::getHeader() const
{
    return &header;
}

void VolumeData::fillBuffer(std::vector<GLubyte>& volData, int width, int height, int depth)
{
    int w = header.recoX;
    int h = header.recoY;
    int d = header.recoZ;

    if (volData.size() != size_t(width * height * depth))
    {
        throw std::exception("VolumeData fillBuffer: destination size doesn't match volume dimensions.");
    }

    auto [minIt, maxIt] = std::minmax_element(data.begin(), data.end());
    unsigned short min = *minIt;
    unsigned short max = *maxIt;

    // Fill volume with normalized data + zero padding
    for (int z = 0; z < depth; ++z)
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                int dstIdx = x + y * width + z * width * height;

                if (x < w && y < h && z < d)
                {
                    int srcIdx = x + y * w + z * w * h;
                    float norm = float(data[srcIdx] - min) / float(max - min);
                    norm = std::clamp(norm, 0.0f, 1.0f);
                    volData[dstIdx] = static_cast<GLubyte>(norm * 255.0f);
                }
                else
                {
                    volData[dstIdx] = 0; // padding
                }
            }
        }
    }
}