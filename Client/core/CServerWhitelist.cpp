/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CServerWhitelist.cpp
 *  PURPOSE:     Compiled-in allowed-server whitelist matching.
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"
#include "CServerWhitelist.h"
#include <ServerBrowser/CServerList.h>            // CServerListItem::Parse (host -> in_addr)

namespace CServerWhitelist
{
    bool IsAllowed(const SString& strHost, const in_addr& resolvedAddr, unsigned short usPort)
    {
        for (const SServerEntry& entry : Servers)
        {
            // The port is always part of the match. Dev and MapEditor may live on the same IP
            // on different ports, so a host/IP match alone must never be enough - otherwise any
            // port on that IP would be accepted.
            if (usPort != entry.usPort)
                continue;

            // (a) Direct case-insensitive host-string match. This reliably covers domain entries
            // (including dyndns) where the caller connected using the same host string.
            if (strHost.CompareI(entry.szHost))
                return true;

            // (b) Resolved-IP match. Covers the case where the caller passed an IP for a domain
            // entry (or vice versa): resolve the entry host and compare the numeric address.
            in_addr entryAddr;
            if (CServerListItem::Parse(entry.szHost, entryAddr) && entryAddr.s_addr == resolvedAddr.s_addr)
                return true;
        }

        return false;
    }
}            // namespace CServerWhitelist
