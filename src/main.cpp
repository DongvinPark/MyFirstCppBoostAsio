#include <boost/lockfree/spsc_queue.hpp>
#include <memory>
#include <iostream>
#include <fstream>
#include <array>

class Data {
public:
    std::vector<unsigned char> buf;
    // {offset, length} pair vector
    std::vector<std::pair<long, long>> meta;

    explicit Data(std::ifstream &videoAccess, const int sampleLen) {
        buf.resize(sampleLen);
        if (videoAccess.read(reinterpret_cast<char *>(buf.data()), sampleLen)) {
            for (int i = 0; i < 10; ++i){
                meta.emplace_back(i,i*100);
            }
            std::cout << "!!! Data (buf) allocated !!!\n";
        } else {
            std::cout << "!!! Data (buf) not allocated !!!\n";
        }
    }

    ~Data() {
        std::cout << "!!! Data (buf) destructed !!!\n";
    }
};

// SPSC queue supports std::shared_ptr<Data>
boost::lockfree::spsc_queue<std::shared_ptr<Data>> dataQueue(100);

int main() {
    std::ifstream videoFile(
        "/Users/dongvin99/Documents/for Mac Studio Dev Contents BackUP/streaming_contents_3.0/enhypen-test-1cam-H/cam0/V1H.asv",
        std::ios::binary | std::ios::ate
        );

    if (!videoFile) {
        std::cerr << "Failed to open video file.\n";
        return 1;
    }

    videoFile.seekg(0, std::ios::beg);

    std::shared_ptr<Data> dataPtr = std::make_shared<Data>(videoFile, 10000);
    if (!dataQueue.push(dataPtr)) {
        std::cerr << "Queue push failed!\n";
    }

    std::shared_ptr<Data> retrievedData;
    if (dataQueue.pop(retrievedData)) {
        std::cout << "Retrieved buffer metaSize: " << retrievedData->meta.size() << "\n";
        std::cout << "Retrieved buf size: " << retrievedData->buf.size() << "\n";
    }

    return 0;
}