// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

#ifndef CHANNEL_TIMEOUT_H
#define CHANNEL_TIMEOUT_H

#include <string>
#include "PvaException.h"

/**
 * Channel timeout error.
 */
class ChannelTimeout : public PvaException
{
public:
    ChannelTimeout(const std::string& message=""); 
    ChannelTimeout(const char* message, ...);
};

#endif 
