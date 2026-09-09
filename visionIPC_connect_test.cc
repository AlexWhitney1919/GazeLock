#include "msgq/visionipc/visionipc_client.h"

#include <iostream>
#include <cstddef>
#include <chrono>
#include <thread>

int main() {
  VisionIpcClient client(
    "camerad",
    VISION_STREAM_DRIVER,
    true
  );

  while (!client.connect(false)) {
    std::cout << "Waiting for VisionIPC client connection..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::cout << "Connected!" << std::endl;
  size_t size = 0;
  int frames_received = 0;

  while (true) {
    VisionIpcBufExtra extra{};


    VisionBuf* frame = client.recv(&extra);

    if (frame == nullptr)  {
      continue;
    }

    std::cout << "Frame " << extra.frame_id << " - Size: " << frame->len << " - Valid: " << extra.valid << std::endl;

    if (size == 0 ) {
      size = frame->len;
    } else if (size != frame->len) {
      std::cout << "SIZE CHANGED!" << std::endl;
    }

    ++frames_received;

    if (frames_received == 1000) {
      return 0;
    }
  }
}