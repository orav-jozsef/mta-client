/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CServerWhitelist.h
 *  PURPOSE:     Compiled-in list of servers this client is allowed to join.
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

// This header is intentionally data-only: the table below is the single compiled-in
// source of truth for both the main-menu server buttons and the connect whitelist.
// It is baked into the client binary on purpose (no runtime config file), so a player
// cannot edit it to bypass the whitelist. To change the list, edit this table and
// rebuild the client. The matching logic lives in CServerWhitelist.cpp.

namespace CServerWhitelist
{
    // Kind of server. Only Production entries are offered as buttons in the menu;
    // Dev and MapEditor are reachable through the console / mtasa:// only.
    enum class EServerKind
    {
        Production,
        Dev,
        MapEditor,
    };

    struct SServerEntry
    {
        const char*    szName;        // Display name (used only for menu entries)
        const char*    szHost;        // IP address or domain name
        unsigned short usPort;        // Game port (e.g. 22003)
        EServerKind    kind;          // Production | Dev | MapEditor
        bool           bShownInMenu;  // true -> shown as a Connect button; false -> console/URI only
    };

    // The allowed-server table.
    //
    // Murppy: replace the host/port of every entry below with the real servers before
    // shipping. The addresses here are RFC 5737 documentation placeholders (192.0.2.x)
    // and will not connect to anything. Add or remove rows as needed and rebuild.
    inline constexpr SServerEntry Servers[] = {
        // name          host              port    kind                     shownInMenu
        {"Main RP",      "192.0.2.10",     22003, EServerKind::Production, true},
        {"Event",        "192.0.2.11",     22003, EServerKind::Production, true},
        {"Dev",          "192.0.2.20",     22005, EServerKind::Dev,        false},
        {"Map Editor",   "192.0.2.20",     22007, EServerKind::MapEditor,  false},
    };

    // Returns true if (strHost / resolvedAddr) + usPort matches an allowed entry.
    // Checked against the WHOLE table (Production + Dev + MapEditor). Implemented in the
    // .cpp so the host->IP resolution is not inlined into every translation unit.
    bool IsAllowed(const SString& strHost, const in_addr& resolvedAddr, unsigned short usPort);
}            // namespace CServerWhitelist
