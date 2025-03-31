#include <array>
#include <boost/lockfree/queue.hpp>
#include <boost/pool/object_pool.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <filesystem>
#include <fstream>

struct Data {
  std::vector<unsigned char> buf;
  int refCount;

  Data(std::ifstream& videoAccess, const int sampleLen) {
    // Fill buffer with dummy data
    buf.resize(sampleLen);
    if (videoAccess.read(reinterpret_cast<std::ifstream::char_type *>(buf.data()),10000)) {
      refCount = 0;
      std::cout << "!!! Data (buf) allocated !!!\n";
    } else {
      refCount = -1;
      std::cout << "!!! Data (buf) not allocated !!!\n";
    }
  }

  ~Data() {
    std::cout << "!!! Data (buf) destructed !!!\n";
  }
};

struct RtpPacketInfo {
  int flag;  // 0 for video, 1 for audio
  std::shared_ptr<Data> bufPtr;
  size_t offset;
  size_t length;
  bool isHybridMeta;

  RtpPacketInfo() {
    std::cout << "!!! RtpPacketInfo constructed !!!\n";
  }

  ~RtpPacketInfo() {
    std::cout << "!!! Destructor called !!!\n";
  }
};

// Lock-free queue with a capacity of 1000
boost::lockfree::queue<RtpPacketInfo*> queue(1000);

int main() {
  boost::object_pool<RtpPacketInfo> rtpPacketPool{1};

  // for window
  //const std::string enhypen1camRefVPath = "C:\\Users\\user\\CLionProjects\\CppTestPad\\V1H.asv";

  // for mac
  const std::string enhypen1camRefVPath
  = "/Users/dongvin99/Documents/for Mac Studio Dev Contents BackUP/streaming_contents_3.0/enhypen-test-1cam-H/cam0/V1H.asv";

  std::ifstream videoFileStream(enhypen1camRefVPath, std::ios::binary | std::ios::ate);
  videoFileStream.seekg(0, std::ios::beg);

  auto videoSamplePtr = std::make_shared<Data>(videoFileStream, 10000);
  std::cout << "Shared ptr use_count before producer: " << videoSamplePtr.use_count() << std::endl;

  // Producer thread
  std::thread t1([&]() {
    for (int i = 0; i < 100; ++i) {
      rtpPacketPool.set_next_size(1);
      auto rtp = rtpPacketPool.construct();  // Dynamically allocate memory
      rtp->flag = 1;
      rtp->offset = i * 11;
      rtp->length = 1024;
      rtp->isHybridMeta = false;
      rtp->bufPtr = videoSamplePtr;  // Assign shared buffer

      videoSamplePtr->refCount++;

      while (!queue.push(rtp)) {  // Ensure the item is pushed
        std::this_thread::yield();  // Give CPU time to consumer
      }
    }
    std::cout << "Producer done." << std::endl;
  });

  // Consumer thread
  std::thread t2([&]() {
    int popCount = 0;
    RtpPacketInfo* packetInfoPtr = nullptr;

    while (popCount < 100) {  // Ensure we consume all 100 packets
      if (queue.pop(packetInfoPtr)) {
        std::cout << "!!! Popped RTP packet with offset: " << packetInfoPtr->offset << " !!!\n";

        std::cout << "Buffer["<< packetInfoPtr->offset <<"]: "
          << static_cast<char>(packetInfoPtr->bufPtr->buf[packetInfoPtr->offset]) << "\n";
        ++popCount;
        rtpPacketPool.free(packetInfoPtr);
        videoSamplePtr->refCount--;
      } else {
        std::this_thread::yield();  // Give CPU time to producer
      }
    }
    std::cout << "Consumer done.\n";
  });

  t1.join();
  t2.join();

  if (videoSamplePtr->refCount == 0) {
    std::cout << "Shared ptr use_count after consumer: " << videoSamplePtr.use_count() << std::endl;
    videoSamplePtr.reset();  // Manually reset to trigger destructor
    std::cout << "Shared ptr reset. Exiting main." << std::endl;
  }

  return 0;
}
