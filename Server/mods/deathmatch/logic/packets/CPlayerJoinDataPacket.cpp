/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        mods/deathmatch/logic/packets/CPlayerJoinDataPacket.cpp
 *  PURPOSE:     Player join data packet class
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"
#include "CPlayerJoinDataPacket.h"

bool CPlayerJoinDataPacket::Read(NetBitStreamInterface& BitStream)
{
    // Read out the stuff
    if (!BitStream.Read(m_usNetVersion) || !BitStream.Read(m_usMTAVersion))
        return false;

    if (!BitStream.Read(m_usBitStreamVersion))
        return false;

    if (!BitStream.ReadString(m_strPlayerVersion))
        return false;

    if (!BitStream.ReadBit(m_bOptionalUpdateInfoRequired))
        return false;

    if (BitStream.Read(m_ucGameVersion) && BitStream.ReadStringCharacters(m_strNick, MAX_PLAYER_NICK_LENGTH) &&
        BitStream.Read(reinterpret_cast<char*>(&m_Password), 16))
    {
        // Shrink string sizes to fit
        m_strNick = *m_strNick;

        // Canonical trailing-field order (MUST stay identical to the client write side in
        // CClientGame::StartGame):
        //   1. reserved MAX_SERIAL_LENGTH field (legacy community slot / SGS serial on the
        //      sgs-serial branch) - read and discarded here.
        //   2. SGS ecosystem version, length-prefixed (this feature).
        // Both reads are non-fatal: an older/vanilla client may send an empty reserved field and
        // no SGS version, which leaves m_strSgsVersion empty. The server-side gate in
        // CGame::Packet_PlayerJoinData then rejects the join with a clear message.
        char szReserved[MAX_SERIAL_LENGTH] = {0};
        BitStream.Read(szReserved, MAX_SERIAL_LENGTH);
        BitStream.ReadString(m_strSgsVersion);
        return true;
    }

    return false;
}
