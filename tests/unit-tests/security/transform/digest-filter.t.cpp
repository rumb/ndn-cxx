/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2013-2015 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#include "security/transform/buffer-source.hpp"
#include "security/transform/step-source.hpp"
#include "security/transform/digest-filter.hpp"
#include "security/transform/stream-sink.hpp"
#include "encoding/buffer-stream.hpp"

#include "boost-test.hpp"

namespace ndn {
namespace security {
namespace transform {
namespace tests {

BOOST_AUTO_TEST_SUITE(TransformDigestFilter)

BOOST_AUTO_TEST_CASE(Basic)
{
  uint8_t in[128] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
  };

  uint8_t out[] = {
    0x3e, 0x14, 0xfd, 0x66, 0x9a, 0x79, 0x80, 0x65,
    0xc4, 0x0d, 0x61, 0xf8, 0x6a, 0xc7, 0x98, 0x29,
    0xc0, 0x6b, 0x90, 0x8f, 0xbb, 0x19, 0xa0, 0x85,
    0xf7, 0xfa, 0x7b, 0x2d, 0xd6, 0x8c, 0xd5, 0xa3
  };

  OBufferStream os;
  bufferSource(in, sizeof(in)) >> digestFilter(DIGEST_ALGORITHM_SHA256) >> streamSink(os);

  ConstBufferPtr buf = os.buf();
  BOOST_CHECK_EQUAL_COLLECTIONS(out, out + sizeof(out), buf->begin(), buf->end());
}

BOOST_AUTO_TEST_CASE(StepByStep)
{
  uint8_t in[128] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
  };

  uint8_t out[] = {
    0x3e, 0x14, 0xfd, 0x66, 0x9a, 0x79, 0x80, 0x65,
    0xc4, 0x0d, 0x61, 0xf8, 0x6a, 0xc7, 0x98, 0x29,
    0xc0, 0x6b, 0x90, 0x8f, 0xbb, 0x19, 0xa0, 0x85,
    0xf7, 0xfa, 0x7b, 0x2d, 0xd6, 0x8c, 0xd5, 0xa3
  };

  StepSource source;
  OBufferStream os;
  source >> digestFilter(DIGEST_ALGORITHM_SHA256) >> streamSink(os);
  source.write(in, 32);
  source.write(in + 32, 1);
  source.write(in + 33, 2);
  source.write(in + 35, 3);
  source.write(in + 38, 26);
  source.write(in + 64, 64);
  source.end();

  ConstBufferPtr buf = os.buf();
  BOOST_CHECK_EQUAL_COLLECTIONS(out, out + sizeof(out), buf->begin(), buf->end());
}

BOOST_AUTO_TEST_CASE(EmptyInput)
{
  uint8_t out[] = {
    0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14,
    0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
    0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
    0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55
  };

  OBufferStream os;
  StepSource source;
  source >> digestFilter(DIGEST_ALGORITHM_SHA256) >> streamSink(os);
  source.end();

  ConstBufferPtr buf = os.buf();
  BOOST_CHECK_EQUAL_COLLECTIONS(out, out + sizeof(out), buf->begin(), buf->end());
}

BOOST_AUTO_TEST_CASE(Error)
{
  OBufferStream os;
  BOOST_REQUIRE_THROW(stepSource() >> digestFilter(DIGEST_ALGORITHM_NONE) >> streamSink(os),
                      transform::Error);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace tests
} // namespace transform
} // namespace security
} // namespace ndn