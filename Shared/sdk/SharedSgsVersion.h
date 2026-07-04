/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Shared/sdk/SharedSgsVersion.h
 *  PURPOSE:     Single source of truth for the SGS ecosystem version.
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include <cstring>
#include <cstdlib>

// SGS is our own client<->server ecosystem lock. The client sends this version at join time and
// the server rejects any client whose SGS version is missing or incompatible; the client likewise
// refuses to connect to a server that does not advertise a compatible SGS version. Both sides read
// these constants, so this header is the single source of truth for both the client and the server.
//
// Wire format is the human-readable string SGS_VERSION_STRING ("SGS vMAJOR.MINOR.MAINTENANCE").
// Compatibility rule: MAJOR must match (v1.x.x are interoperable; a MAJOR bump is a breaking change).
// MINOR / MAINTENANCE are informational only.

#define SGS_VERSION_MAJOR       1
#define SGS_VERSION_MINOR       0
#define SGS_VERSION_MAINTENANCE 0
#define SGS_VERSION_STRING      "SGS v1.0.0"

// Prefix that a valid SGS version string must start with.
#define SGS_VERSION_PREFIX "SGS v"

// Parses the MAJOR number out of an "SGS vMAJOR.MINOR.MAINTENANCE" string.
// Returns -1 if the string is null, empty, or not a valid SGS version string (e.g. a vanilla
// client/server that sends nothing or some other value). Callers treat -1 as "not an SGS peer".
inline int GetSgsVersionMajor(const char* szVersion)
{
    if (!szVersion || !szVersion[0])
        return -1;

    const size_t prefixLen = sizeof(SGS_VERSION_PREFIX) - 1;
    if (strncmp(szVersion, SGS_VERSION_PREFIX, prefixLen) != 0)
        return -1;

    // The character right after the prefix must be a digit for this to be a well-formed version.
    const char* pMajor = szVersion + prefixLen;
    if (*pMajor < '0' || *pMajor > '9')
        return -1;

    return atoi(pMajor);
}

// Returns true if a peer's SGS version string is compatible with this build (MAJOR match).
inline bool IsSgsVersionCompatible(const char* szPeerVersion)
{
    return GetSgsVersionMajor(szPeerVersion) == SGS_VERSION_MAJOR;
}
