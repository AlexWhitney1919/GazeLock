#pragma once  // only inlude once per compilation

#include <cstddef>    // needed for size_t
#include <cstdint>    // needed for uint8 and other fixed size integer representations
#include <fstream>    // file streaming
#include <string>     // need for output directory naming

class SegmentWriter {
public:
  SegmentWriter(      // constructor
    const std::string& output_directory,      // take string name for output directory (pass by reference, not by copy, promise not to modify with const)
    std::size_t frames_per_segment            // take number of frames in a segment
  );

  ~SegmentWriter();   // destructor

  bool write_frame(   // writes frames, returns boolean value representing success/failure of write
    const std::uint8_t* data,                 // take frame data by referece, const prevents modification
    std::size_t size                          // size of the data in memory in bytes
  );

  bool close();       // allow caller to close segment manually (no return needed)

private:
  bool open_next_segment();                   // internal helper function

  std::ofstream output_;                      // output file stream
  std::string output_directory_;              // string representing the output directory
  std::size_t frames_per_segment_;             // how many frames should be written in one bytestream
  std::size_t frames_in_segment_;             // how many frames are currently written to the segment
  std::size_t segment_number_;                // which segment file we're currently writing
  std::uint64_t bytes_written_;               // how many bytes have been written (large number, 64 bits reserved to represent it)
};