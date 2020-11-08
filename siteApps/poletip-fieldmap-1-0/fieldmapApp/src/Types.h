// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2020.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Oliver Kohlbacher $
// $Authors: Marc Sturm, Clemens Groepl $
// --------------------------------------------------------------------------

#include <ctime>
#include <cstddef> // for size_t & ptrdiff_t
#include <limits>

// If possible use the ISO C99-compliant header stdint.h
// to define the portable integer types.
#ifdef OPENMS_HAS_STDINT_H
#include <cstdint>
#endif

  /**
    @brief Signed integer type (32bit)

    @ingroup Concept
  */
//typedef OPENMS_INT32_TYPE Int32;
	typedef int Int32;

  /**
    @brief Unsigned integer type (32bit)

    @ingroup Concept
  */
  //typedef OPENMS_UINT32_TYPE UInt32;
  typedef unsigned int UInt32;

  /**
    @brief Signed integer type (64bit)

    @ingroup Concept
  */
  typedef long long Int64;

  /**
    @brief Unsigned integer type (64bit)

    @ingroup Concept
  */
  typedef unsigned long long UInt64;

  /**
    @brief Time type

    Use this type to represent a point in time (as a synonym for time_t).

    @ingroup Concept
  */
  typedef time_t  Time;

  /**
    @brief Unsigned integer type

    @ingroup Concept
  */
  //typedef size_t UInt;
  typedef unsigned int UInt;

  /**
    @brief Signed integer type

    @ingroup Concept
  */
  //typedef OPENMS_SIZE_T_SIGNED Int;
  typedef int Int;

  /**
    @brief Byte type

    Use this type to represent byte data (8 bit length). A Byte is always unsigned.

    @ingroup Concept
  */
  typedef unsigned short Byte;

  /**
    @brief A unique object ID (as unsigned 64bit type).

    @see PersistentObject

    @ingroup Concept
  */
  typedef unsigned long long UID;

  /**
    @brief Size type e.g. used as variable which can hold result of size()

    @ingroup Concept
  */
  typedef size_t Size;

  /**
    @brief Signed Size type e.g. used as pointer difference

    @ingroup Concept
  */
  typedef ptrdiff_t SignedSize;

  enum ASCII
  {
    ASCII__BACKSPACE        = '\b',
    ASCII__BELL             = '\a',
    ASCII__CARRIAGE_RETURN  = '\r',
    ASCII__HORIZONTAL_TAB   = '\t',
    ASCII__NEWLINE          = '\n',
    ASCII__RETURN           = ASCII__NEWLINE,
    ASCII__SPACE            = ' ',
    ASCII__TAB              = ASCII__HORIZONTAL_TAB,
    ASCII__VERTICAL_TAB     = '\v',

    ASCII__COLON            = ':',
    ASCII__COMMA            = ',',
    ASCII__EXCLAMATION_MARK = '!',
    ASCII__POINT            = '.',
    ASCII__QUESTION_MARK    = '?',
    ASCII__SEMICOLON        = ';'
  };

