#pragma once

#include <GWCA/GameContainers/Array.h>
#include <GWCA/Utilities/Export.h>

namespace GW {
    struct PreGameContext;
    GWCA_API PreGameContext* GetPreGameContext();

    struct CharacterInformation;
    GWCA_API Array<CharacterInformation>* GetAvailableChars();

    struct LoginCharacter {
        uint32_t unk0;
        uint32_t pvp_or_campaign;
        uint32_t UnkPvPData01;
		uint32_t UnkPvPData02;
		uint32_t UnkPvPData03;
		uint32_t UnkPvPData04;
        uint32_t Unk01[4];
        uint32_t Level;
		uint32_t current_map_id;
		uint32_t Unk02[7];
        wchar_t character_name[20];
    };

    // Character information available at the login screen. Returned by
    // GetAvailableChars(). Distinct from PreGameContext::chars; py4gw's
    // Python AvailableCharacterStruct mirrors this layout.
    struct CharacterInformation {
        /* +h0000 */ uint32_t h0000[2];
        /* +h0008 */ uint32_t uuid[4];
        /* +h0018 */ wchar_t  name[20];
        /* +h0040 */ uint32_t props[17];

        uint32_t GetMapId()              const { return (props[0] >> 16) & 0xFFFF; }
        uint32_t GetPrimaryProfession()  const { return (props[2] >> 20) & 0xF; }
        uint32_t GetSecondaryProfession() const { return (props[7] >> 10) & 0xF; }
        uint32_t GetCampaign()           const { return  props[7]        & 0xF; }
        uint32_t GetLevel()              const { return (props[7] >> 4)  & 0x3F; }
        bool     IsPvP()                 const { return ((props[7] >> 9) & 0x1) == 0x1; }
    };

    struct PreGameContext {
        uint32_t frame_id;
		uint32_t Unk01[20];
        float h0054;
        float h0058;
        uint32_t Unk02[2];
        float h0060;
        uint32_t Unk03[2];
        float h0068;
        uint32_t Unk04;
        float h0070;
        uint32_t Unk05;
        float h0078;
        uint32_t Unk06[8];
        float h00a0;
        float h00a4;
		float h00a8;
        uint32_t Unk07[9];
        uint32_t chosen_character_index;
        uint32_t Unk08;
        GW::Array<LoginCharacter> chars;
    };
}
