/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_MAPINFO_H_
#define _COPS_V7_EMULATOR_MSG_MAPINFO_H_

#include "common.h"
#include "msg.h"

class GameMap;

/**
 * Msg sent to the client by the MsgServer to fill all the map variables.
 */
class MsgMapInfo : public Msg
{
public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** Unique Id of the map */
        uint32_t UniqId;
        /** Real Id of the map. */
        uint32_t DocId;
        /** Type (flags) of the map. */
        uint32_t Type;
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgMapInfo packet for the specified map.
     *
     * @param[in]   aMap     a reference to the map object
     */
    MsgMapInfo(const GameMap& aMap);

    /* destructor */
    virtual ~MsgMapInfo();

private:
    /* internal filling of the packet */
    void create(const GameMap& aMap);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_MAPINFO_H_
