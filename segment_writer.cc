#include "segment_writer.h"

#include <cstdint>
#include <cstddef>
#include <fstream>
#include <string>
#include <stdexcept>

// constructor takes output directory and a desired frames per segment value
SegmentWriter::SegmentWriter(const std::string& output_directory, std::size_t frames_per_segment)
  : output_directory_(output_directory),    // member initializer list (initalizes each data member with the value given)
    frames_per_segment_(frames_per_segment),
    frames_in_segment_(0),
    segment_number_(0),
    bytes_written_(0)
{
  // reject construction with a frames_per_segment value of zero
  if (frames_per_segment_ == 0) {
    throw std::invalid_argument("frames_per_segment must be greater than 0");
  }
}

SegmentWriter::~SegmentWriter() {           // call close on destruction to handle possible open filestreams
  close();
}

// write frame implementation
bool SegmentWriter::write_frame(const std::uint8_t* data, std::size_t size) {
  // state checking before writing:
  // reject a null data pointer or a zero byte frame
  // if the filestream is closed, or the number of frames written meets or exceeds the defined limit, then try to open next segment (or first
  // segment if not yet opened before). If opening the next segment (or first) fails, return false
  if (data == nullptr || size == 0) {
    return false;
  }

  if (!output_.is_open() || frames_in_segment_>= frames_per_segment_) {
        if(!open_next_segment()) {         // open the next (or first) segment, return if unsuccessful
          return false;
        }
      }

  // once this is reached, we have ensured that the filestream is open and number of frames are within bounds for the segment
  // note: data is already a pointer to a std::uint8_t element, so we need to cast it to const char * without modifying the actual value
  //       since std::ofstream write() requires const char * datatype, not const std::uint8_t *
  output_.write(                            // call std::ofstream write() functionality with constructer parameters data and size
    reinterpret_cast<const char *>(data),
    static_cast<std::streamsize>(size)
  );

  // check if the output filestream actually wrote by performing its own internal state check using the overloaded ! operator
  if (!output_) {
    return false;                           // return false if write fails
  }

  // update counters on successful frame write
  ++frames_in_segment_;
  bytes_written_ += size;

  return true;                              // return true if write succeeds
}

// outward facing function that allows other programs to manually close a segment
bool SegmentWriter::close() {
  if (!output_.is_open())  {
    return true;                            // successfully closed (already closed prior to call)
  }

  output_.close();                          // close the filestream, buffered data automatically written to disk on call

  if (!output_) {
    return false;                           // return false if close unsuccessful
  }

  return true;                              // return true if close successful
}

bool SegmentWriter::open_next_segment() {
  // close current output filestream if it is open
  if (!close()) {
    return false;                           // return false if close() fails
  }

  // build the filename for the new segment using given directory and segment number
  std::string filename = output_directory_ + "/segment_" + std::to_string(segment_number_) + ".bin";

  output_.open(filename, std::ios::binary); // open the output filestream under the new filename in binary write mode to prevent text mode translations

  if (!output_.is_open()) {
    return false;
  }

  // reset frames in segment count and increment total segment numbers
  frames_in_segment_ = 0;
  ++segment_number_;

  return true;
}
