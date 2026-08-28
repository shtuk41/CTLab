#include <algorithm>
#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include <io/ioData.h>

VolumeData::VolumeData(const std::string filePath, bool read)
{
    std::ifstream file;

    if (read)
    {
        file.open(filePath.c_str(), std::ios::binary);

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
    else
    {
        file.open(filePath.c_str());

        if (file.is_open())
        {
            if (!readTextHeader(file))
            {
                std::string message = std::string("incorrect format: ") + filePath;
                throw std::runtime_error(message.c_str());
            }
        }
        else
        {
            std::string message = std::string("unable to open file") + filePath;
            throw std::runtime_error(message.c_str());
        }
    }
}

std::string trim(const std::string& str) 
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

template <typename T>
bool readValue(std::istringstream& istream, std::string key, T &value)
{
    std::string line;

    std::getline(istream, line);
    size_t delimiter_pos = line.find(':');

    std::string foundKey = trim(line.substr(0, delimiter_pos));
    std::string foundValue = trim(line.substr(delimiter_pos + 1));

    if (key.compare(foundKey) == 0)
    {
        std::istringstream ss(foundValue);

        if (std::is_same<T, bool>::value)
        {
            value = foundValue.compare("true") == 0 ? true : false;
            return true;
        }
        
        if ((ss >> value) && ss.eof())
        {
            return true;
        }

        if (ss.fail())
        {
            std::cerr << "Error: Conversion failed to type T.\n";
        }
        else if (!ss.eof())
        {
            std::cerr << "Error: Trailing characters.";
        }
    }

    return false;
}

bool VolumeData::readTextHeader(std::ifstream& file)
{
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string file_contents = buffer.str();
    std::istringstream input_stream(file_contents);

    if (!readValue<unsigned int>(input_stream, "headSize", header.headSize)) return false;//: uint32(bytes 0 - 3) : size of the header

    if (header.headSize != 1024)
        return false;

    if (!readValue<unsigned int>(input_stream, "mirrorZ", header.mirrorZ)) return false;// : uint32(bytes 4 - 7) : UNKNOWN what this is or represents
    if (!readValue<unsigned int>(input_stream, "numBitsVox", header.numBitsVox)) return false;// : uint32(bytes 8 - 11) : number of bits per voxel
    if (!readValue<unsigned int>(input_stream, "recoX", header.recoX)) return false;// : uint32(bytes 12 - 15) : size of volume in x - dimension
    if (!readValue<unsigned int>(input_stream, "recoY", header.recoY)) return false;// : uint32(bytes 16 - 19) : size of volume in y - dimension
    if (!readValue<unsigned int>(input_stream, "recoZ", header.recoZ)) return false;// : uint32(bytes 20 - 23) : size of volume in z - dimension
    if (!readValue<double>(input_stream, "voxSizeX", header.voxSizeX)) return false;//: float64(bytes 24 - 31) : size of voxel in x
    if (!readValue<double>(input_stream, "voxSizeY", header.voxSizeY)) return false;// : float64(bytes 32 - 39) : size of voxel in y
    if (!readValue<double>(input_stream, "voxSizeZ", header.voxSizeZ)) return false;// : float64(bytes 40 - 47) : size of voxel in z
    if (!readValue<double>(input_stream, "minValue", header.minValue)) return false;// : float64(bytes 48 - 55) : minimum gray - scale value as determined by CERA
    if (!readValue<double>(input_stream, "maxValue", header.maxValue)) return false;// : float64(bytes 56 - 63) : maximum gray - scale value as determined by CERA
    if (!readValue<double>(input_stream, "tubeX", header.tubeX)) return false;// : float64(bytes 64 - 71) : beam center in x - dimension
    if (!readValue<double>(input_stream, "tubeY", header.tubeY)) return false;// : float64(bytes 72 - 79) : beam center in y - dimension
    if (!readValue<double>(input_stream, "tubeZ", header.tubeZ)) return false;// : float64(bytes 80 - 87) : beam center in z - dimension
    if (!readValue<int>(input_stream, "tubeCurrent", header.tubeCurrent)) return false;// : int32(bytes 88 - 91) : x - ray tube current
    if (!readValue<int>(input_stream, "tubeVoltage", header.tubeVoltage)) return false;//: int32(bytes 92 - 95) : x - ray tube voltage
    if (!readValue<double>(input_stream, "rtDataX", header.rtDataX)) return false;// : float64(bytes 96 - 103) : rotary table X - position
    if (!readValue<double>(input_stream, "rtDataY", header.rtDataY)) return false;// : float64(bytes 104 - 111) : rotary table Y - position
    if (!readValue<double>(input_stream, "rtDataZ", header.rtDataZ)) return false;// : float64(bytes 112 - 119) : rotary table Z - position
    if (!readValue<double>(input_stream, "detIntTime", header.detIntTime)) return false;// : float64(bytes 120 - 127) : detector integration time
    if (!readValue<double>(input_stream, "detGain", header.detGain)) return false;// : float64(bytes 128 - 135) : detector gain
    if (!readValue<double>(input_stream, "detXPos", header.detXPos)) return false;// : float64(bytes 136 - 143) : detector X - position
    if (!readValue<double>(input_stream, "detYPos", header.detYPos)) return false;// : float64(bytes 144 - 151) : detector Y - position
    if (!readValue<double>(input_stream, "detZPos", header.detZPos)) return false;// : float64(bytes 152 - 159) : detector Z - position
    if (!readValue<float>(input_stream, "detPixWidth", header.detPixWidth)) return false;// : float32(bytes 160 - 163) : the size of the detector pixel(width)
    if (!readValue<float>(input_stream, "detPixHeight", header.detPixHeight)) return false;// : float32(bytes 164 - 167) : the size of the detector pixel(height)
    if (!readValue<int>(input_stream, "detImgBitDepth", header.detImgBitDepth)) return false;// : int32(bytes 168 - 171) : bit depth of each pixel in the image
    if (!readValue<int>(input_stream, "detSizeWidth", header.detSizeWidth)) return false;// : int32(bytes 172 - 175) : number of pixels the detector contains(width)
    if (!readValue<int>(input_stream, "detSizeHeight", header.detSizeHeight)) return false;// : int32(bytes 176 - 179) : number of pixels the detector contains(height)
    if (!readValue<int>(input_stream, "detImgSizeWidth", header.detImgSizeWidth)) return false;// : int32(bytes 180 - 183) : image width
    if (!readValue<int>(input_stream, "detImgSizeHeight", header.detImgSizeHeight)) return false;// : int32(bytes 184 - 187) : image height
    if (!readValue<unsigned int>(input_stream, "numProj", header.numProj)) return false;// : uint32(bytes 188 - 191) : number of projections
    if (!readValue<int>(input_stream, "roiXulc", header.roiXulc)) return false;// : int32(bytes 192 - 195) : region of interest x, upper left corner
    if (!readValue<int>(input_stream, "roiYulc", header.roiYulc)) return false;// : int32(bytes 196 - 199) : region of interest y, upper left corner
    if (!readValue<int>(input_stream, "roiWidth", header.roiWidth)) return false;// : int32(bytes 200 - 203) : region of interest(width)
    if (!readValue<int>(input_stream, "roiHeight", header.roiHeight)) return false;// : int32(bytes 204 - 207) : region of interest(height)
    if (!readValue<double>(input_stream, "filtNoiseReduct", header.filtNoiseReduct)) return false;// : float64(bytes 208 - 215) : filter for noise reduction
    if (!readValue<double>(input_stream, "voxReductFact", header.voxReductFact)) return false;// : float64(bytes 216 - 223) : voxel reduction factor
    if (!readValue<float>(input_stream, "imgAvg", header.imgAvg)) return false;// : float32(bytes 224 - 227) : gain, according to GUI(1x, 2x, 8x, 16x, ...)
    if (!readValue<float>(input_stream, "binMode", header.binMode)) return false;// : float32(bytes 228 - 231) : detector pixel binning mode
    memset(header.preFilter, 0, 128);// : UTF - 8   (bytes 232 - 359) : pre - filter used, 32 unicode characters
    if (!readValue<double>(input_stream, "volStartX", header.volStartX)) return false;// : float64(bytes 360 - 367) : volume start position in x - dimension
    if (!readValue<double>(input_stream, "volStartY", header.volStartY)) return false;// : float64(bytes 368 - 375) : volume start position in y - dimension
    if (!readValue<double>(input_stream, "volStartZ", header.volStartZ)) return false;// : float64(bytes 376 - 383) : volume start position in z - dimension
    if (!readValue<float>(input_stream, "minValueData", header.minValueData)) return false;// : float32(bytes 384 - 387) : minimum pixel value recorded in the data
    if (!readValue<float>(input_stream, "maxValueData", header.maxValueData)) return false;// : float32(bytes 388 - 391) : maximum pixel value recorded in the data
    if (!readValue<float>(input_stream, "volDefAngle", header.volDefAngle)) return false;// : float32(bytes 392 - 395) : Angle of first projection of dataset
    if (!readValue<bool>(input_stream, "volumeTogether", header.volumeTogether)) return false;// : 8bit(bytes 396) : 8 - bit boolean

    data.resize(header.recoX * header.recoY * header.recoZ);

    return true;
}

const uint16_csv_volume* VolumeData::getHeader() const
{
    return &header;
}

void VolumeData::saveHeaderToFile(std::string fileName)
{
    std::ofstream toFile(fileName, std::ios::trunc);
    toFile << getHeaderString();
}

std::string VolumeData::getHeaderString() const
{
    std::ostringstream oss;

    oss << std::format("headSize: {}\n", header.headSize);
    //unsigned int mirrorZ;// : uint32(bytes 4 - 7) : UNKNOWN what this is or represents
    oss << std::format("mirrorZ: {}\n", header.mirrorZ);
    // : uint32(bytes 8 - 11) : number of bits per voxel
    oss << std::format("numBitsVox: {}\n", header.numBitsVox);
    // : uint32(bytes 12 - 15) : size of volume in x - dimension
    oss << std::format("recoX: {}\n", header.recoX);
    // : uint32(bytes 16 - 19) : size of volume in y - dimension
    oss << std::format("recoY: {}\n", header.recoY);
    // : uint32(bytes 20 - 23) : size of volume in z - dimension
    oss << std::format("recoZ: {}\n", header.recoZ);
    //: float64(bytes 24 - 31) : size of voxel in x
    oss << std::format("voxSizeX: {}\n", header.voxSizeX);
    // : float64(bytes 32 - 39) : size of voxel in y
    oss << std::format("voxSizeY: {}\n", header.voxSizeY);
    // : float64(bytes 40 - 47) : size of voxel in z
    oss << std::format("voxSizeZ: {}\n", header.voxSizeZ);
    // : float64(bytes 48 - 55) : minimum gray - scale value as determined by CERA
    oss << std::format("minValue: {}\n", header.minValue);
    // : float64(bytes 56 - 63) : maximum gray - scale value as determined by CERA
    oss << std::format("maxValue: {}\n", header.maxValue);
    // : float64(bytes 64 - 71) : beam center in x - dimension
    oss << std::format("tubeX: {}\n", header.tubeX);
    // : float64(bytes 72 - 79) : beam center in y - dimension
    oss << std::format("tubeY: {}\n", header.tubeY);
    // : float64(bytes 80 - 87) : beam center in z - dimension
    oss << std::format("tubeZ: {}\n", header.tubeZ);
    // : int32(bytes 88 - 91) : x - ray tube current
    oss << std::format("tubeCurrent: {}\n", header.tubeCurrent);
    //: int32(bytes 92 - 95) : x - ray tube voltage
    oss << std::format("tubeVoltage: {}\n", header.tubeVoltage);
    // : float64(bytes 96 - 103) : rotary table X - position
    oss << std::format("rtDataX: {}\n", header.rtDataX);
    // : float64(bytes 104 - 111) : rotary table Y - position
    oss << std::format("rtDataY: {}\n", header.rtDataY);
    // : float64(bytes 112 - 119) : rotary table Z - position
    oss << std::format("rtDataZ: {}\n", header.rtDataZ);
    // : float64(bytes 120 - 127) : detector integration time
    oss << std::format("detIntTime: {}\n", header.detIntTime);
    // : float64(bytes 128 - 135) : detector gain
    oss << std::format("detGain: {}\n", header.detGain);
    // : float64(bytes 136 - 143) : detector X - position
    oss << std::format("detXPos: {}\n", header.detXPos);
    // : float64(bytes 144 - 151) : detector Y - position
    oss << std::format("detYPos: {}\n", header.detYPos);
    // : float64(bytes 152 - 159) : detector Z - position
    oss << std::format("detZPos: {}\n", header.detZPos);
    // : float32(bytes 160 - 163) : the size of the detector pixel(width)
    oss << std::format("detPixWidth: {}\n", header.detPixWidth);
    // : float32(bytes 164 - 167) : the size of the detector pixel(height)
    oss << std::format("detPixHeight: {}\n", header.detPixHeight);
    // : int32(bytes 168 - 171) : bit depth of each pixel in the image
    oss << std::format("detImgBitDepth: {}\n", header.detImgBitDepth);
    // : int32(bytes 172 - 175) : number of pixels the detector contains(width)
    oss << std::format("detSizeWidth: {}\n", header.detSizeWidth);
    // : int32(bytes 176 - 179) : number of pixels the detector contains(height)
    oss << std::format("detSizeHeight: {}\n", header.detSizeHeight);
    // : int32(bytes 180 - 183) : image width
    oss << std::format("detImgSizeWidth: {}\n", header.detImgSizeWidth);
    // : int32(bytes 184 - 187) : image height
    oss << std::format("detImgSizeHeight: {}\n", header.detImgSizeHeight);
    // : uint32(bytes 188 - 191) : number of projections
    oss << std::format("numProj: {}\n", header.numProj);
    // : int32(bytes 192 - 195) : region of interest x, upper left corner
    oss << std::format("roiXulc: {}\n", header.roiXulc);
    // : int32(bytes 196 - 199) : region of interest y, upper left corner
    oss << std::format("roiYulc: {}\n", header.roiYulc);
    // : int32(bytes 200 - 203) : region of interest(width)
    oss << std::format("roiWidth: {}\n", header.roiWidth);
    // : int32(bytes 204 - 207) : region of interest(height)
    oss << std::format("roiHeight: {}\n", header.roiHeight);
    // : float64(bytes 208 - 215) : filter for noise reduction
    oss << std::format("filtNoiseReduct: {}\n", header.filtNoiseReduct);
    // : float64(bytes 216 - 223) : voxel reduction factor
    oss << std::format("voxReductFact: {}\n", header.voxReductFact);
    // : float32(bytes 224 - 227) : gain, according to GUI(1x, 2x, 8x, 16x, ...)
    oss << std::format("imgAvg: {}\n", header.imgAvg);
    // : float32(bytes 228 - 231) : detector pixel binning mode
    oss << std::format("binMode: {}\n", header.binMode);
    //char preFilter[128];// : UTF - 8   (bytes 232 - 359) : pre - filter used, 32 unicode characters
    //oss << std::format(": {}\n", header.);
    // : float64(bytes 360 - 367) : volume start position in x - dimension
    oss << std::format("volStartX: {}\n", header.volStartX);
    // : float64(bytes 368 - 375) : volume start position in y - dimension
    oss << std::format("volStartY: {}\n", header.volStartY);
    // : float64(bytes 376 - 383) : volume start position in z - dimension
    oss << std::format("volStartZ: {}\n", header.volStartZ);
    // : float32(bytes 384 - 387) : minimum pixel value recorded in the data
    oss << std::format("minValueData: {}\n", header.minValueData);
    // : float32(bytes 388 - 391) : maximum pixel value recorded in the data
    oss << std::format("maxValueData: {}\n", header.maxValueData);
    // : float32(bytes 392 - 395) : Angle of first projection of dataset
    oss << std::format("volDefAngle: {}\n", header.volDefAngle);
    // : 8bit(bytes 396) : 8 - bit boolean
    oss << std::format("volumeTogether: {}\n", header.volumeTogether);
    //char padding[627];// : UNKNOWN(627 bytes) : garbage bytes padding out to headSize, which is the size of the header

    std::string output = oss.str();

    return output;
}

void VolumeData::fillBuffer() 
{
    const int width = header.recoX;
    const int height = header.recoY;
    const int depth = header.recoZ;

    volumeDataTex.resize(width * height * depth, 0);

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
                size_t dstIdx = x + width * (y + height * z);

                if (x < width && y < height && z < depth)
                {
                    size_t srcIdx = x + width * (y + height * z); // Fortran-order
                    //size_t srcIdx = z + volumeDepth * (y + volumeHeight * x);
                    float norm = (static_cast<float>(data[srcIdx]) - minVal) / range;
                    volumeDataTex[dstIdx] = static_cast<GLubyte>(std::clamp(norm, 0.0f, 1.0f) * 255.0f);
                }
                else {
                    volumeDataTex[dstIdx] = 0;
                }
            }
        }
    }
}

bool VolumeData::fillData(const std::vector<GLubyte>& buffer)
{
    const int width = header.recoX;
    const int height = header.recoY;
    const int depth = header.recoZ;

    for (int z = 0; z < depth; ++z)
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                size_t dstIdx = x + width * (y + height * z);

                if (x < width && y < height && z < depth)
                {
                    size_t srcIdx = x + width * (y + height * z); // Fortran-order
                    float val = static_cast<float>(buffer[srcIdx]) / 255.0f;
                    data[dstIdx] = static_cast<unsigned short>(val * 65535);
                }
                else {
                    data[dstIdx] = 0;
                }
            }
        }
    }

    return true;
}
bool VolumeData::writeFile(const std::string filePath)
{
    std::ofstream file(filePath, std::ios::binary | std::ios::out);

    if (file.is_open())
    {
        file.write(reinterpret_cast<const char*>(&header), sizeof(uint16_csv_volume));

        if (file.fail()) {
            std::cerr << "Error: Failed to write header to file." << std::endl;
            file.close();
            return false;
        }

        file.write(reinterpret_cast<const char*>(data.data()),
            data.size() * sizeof(unsigned short));


        if (file.fail()) {
            std::cerr << "Error: Failed to write data to file." << std::endl;
            file.close();
            return false;
        }

        
        file.close();
    }

    return true;
    
}


const std::vector<GLubyte>& VolumeData::getVolumeDataTex() const
{
    return volumeDataTex;
}

