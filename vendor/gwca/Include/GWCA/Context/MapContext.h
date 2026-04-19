#pragma once

#include <GWCA/GameContainers/List.h>
#include <GWCA/GameContainers/Array.h>
#include <GWCA/GameContainers/GamePos.h>
#include <GWCA/Utilities/Export.h>

namespace GW {
    struct PathingMap;
    struct MapProp;
    struct PropByType;
    struct PropModelInfo;

    typedef Array<PathingMap> PathingMapArray;

    struct PropsContext {
        /* +h0000 */ uint32_t pad1[0x1b];
        /* +h006C */ Array<TList<PropByType>> propsByType;
        /* +h007C */ uint32_t h007C[0xa];
        /* +h00A4 */ Array<PropModelInfo> propModels;
        /* +h00B4 */ uint32_t h00B4[0x38];
        /* +h0194 */ Array<MapProp*> propArray;
    };
    static_assert(sizeof(PropsContext) == 0x1A4, "struct PropsContext has incorrect size");

    // Static per-map pathing data. Names and offsets per mamba GWCA.
    struct MapStaticData {
        /* +h0000 */ uint32_t h0000[6];
        /* +h0018 */ PathingMapArray map;
        /* +h0028 */ uint32_t h0028[4];
        /* +h0038 */ void*    blocking_props_buffer;   // BlockingProp* — collision-only obstacles sent from server (e.g. gates)
        /* +h003C */ uint32_t blocking_props_capacity;
        /* +h0040 */ uint32_t blocking_props_size;
        /* +h0044 */ uint32_t h0044[16];
        /* +h0084 */ uint32_t nextTrapezoidId;         // Starts at 0, incremented per trapezoid. Equals total trapezoid count.
        /* +h0088 */ uint32_t h0088;
        /* +h008C */ uint32_t map_id;                  // GW::Constants::MapID
        /* +h0090 */ uint32_t h0090[4];
    };
    static_assert(sizeof(MapStaticData) == 0xA0, "struct MapStaticData has incorrect size");

    // Per-map runtime pathing state. Top-level fields named; opaque substructs
    // (NodeCache, PrioQ, ObjectPool, PathNode, PathWaypoint) are left as raw
    // byte padding of the correct size — define those types upstream if needed.
    struct PathContext {
        /* +h0000 */ MapStaticData* staticData;
        /* +h0004 */ uint32_t       blockedPlanes[3];           // BaseArray<uint32_t>: runtime-gated planes (e.g. foundry gates). {buffer, capacity, size}.
        /* +h0010 */ uint32_t       pathNodes[3];               // BaseArray<PathNode*>: indexed by trapezoid id.
        /* +h001C */ uint8_t        nodeCache[0x14];            // NodeCache (cachedCount*, m_mask, BaseArray<u32>).
        /* +h0030 */ uint8_t        openList[0x14];             // PrioQ<PathNode>.
        /* +h0044 */ uint8_t        freeIPathNode[0x0C];        // ObjectPool.
        /* +h0050 */ uint32_t       allocatedPathNodes[3];      // BaseArray<PathNode*>: all allocated nodes for cleanup.
        /* +h005C */ uint32_t       h005C;
        /* +h0060 */ uint32_t       h0060;
        /* +h0064 */ Array<void*>   waypoints;                  // Array<PathWaypoint>.
        /* +h0074 */ Array<void*>   nodeStack;                  // Array<Node*>.
        /* +h0084 */ uint32_t       h0084[4];
    };
    static_assert(sizeof(PathContext) == 0x94, "struct PathContext has incorrect size");

    struct MapContext {
        /* +h0000 */ uint32_t         map_type;    // < 4
        /* +h0004 */ Vec2f            start_pos;
        /* +h000C */ Vec2f            end_pos;
        /* +h0014 */ uint32_t         h0014[6];
        /* +h002C */ Array<void *>    spawns1;     // Arena spawns. Entry is X, Y, unk u32, unk u32.
        /* +h003C */ Array<void *>    spawns2;     // Same as above
        /* +h004C */ Array<void *>    spawns3;     // Same as above
        /* +h005C */ float            h005C[6];    // Some trapezoid i think.
        /* +h0074 */ PathContext*     path;
        /* +h0078 */ void*            path_engine; // PathEngineContext* — optional DLL-based pathfinder.
        /* +h007C */ PropsContext*    props;
        /* +h0080 */ uint32_t         h0080;
        /* +h0084 */ void*            terrain;
        /* +h0088 */ uint32_t         h0088;
        /* +h008C */ uint32_t         map_id;      // GW::Constants::MapID
        /* +h0090 */ uint32_t         h0090[40];
        /* +h0130 */ void*            zones;
        /* +h0134 */ uint32_t         h0134;
    };
    static_assert(sizeof(MapContext) == 0x138, "struct MapContext has incorrect size");

    GWCA_API MapContext* GetMapContext();
}
