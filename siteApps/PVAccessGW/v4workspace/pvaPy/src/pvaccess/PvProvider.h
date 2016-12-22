// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

#ifndef PV_PROVIDER_H
#define PV_PROVIDER_H

#include <string>

namespace PvProvider
{

extern const char* PvaProviderName;
extern const char* CaProviderName;

enum ProviderType {
    PvaProviderType = 0,
    CaProviderType = 1,
};

void checkProviderName(const std::string& providerName);
std::string getProviderName(ProviderType providerType);

}

#endif

