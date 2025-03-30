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
  std::array<unsigned char, 3 * 1024 * 1024> buf;
  int refCount;

  Data(std::ifstream& videoAccess) {
    // Fill buffer with dummy data
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
  std::weak_ptr<Data> buf;
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

  const std::string enhypen1camRefVPath = "C:\\Users\\user\\CLionProjects\\CppTestPad\\V1H.asv";
  std::ifstream videoFileStream(enhypen1camRefVPath, std::ios::binary | std::ios::ate);
  videoFileStream.seekg(0, std::ios::beg);

  auto videoSamplePtr = std::make_shared<Data>(videoFileStream);
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
      rtp->buf = videoSamplePtr;  // Assign shared buffer to weak_ptr

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

        // Try to access the buffer safely
        if (auto sharedBuf = packetInfoPtr->buf.lock()) {
          std::cout << "Buffer[0]: " << static_cast<char>(sharedBuf->buf[packetInfoPtr->offset]) << "\n";
        } else {
          std::cout << "Buffer expired!\n";
        }
        ++popCount;
        rtpPacketPool.destroy(packetInfoPtr);
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
