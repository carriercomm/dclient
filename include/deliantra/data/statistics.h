#pragma once

#include <string>

#include "constants.h"
#include "types.h"

namespace data
{
  struct statistics
  {
    s64 s_skillinfo[CS_NUM_SKILLS];
    s64 s_exp64;

    std::string s_range;
    std::string s_title;

    s32 s_weight_lim;
    s32 s_spell_attune;
    s32 s_spell_repel;
    s32 s_spell_deny;

    s32 s_exp;

    s16 s_hp;
    s16 s_maxhp;
    s16 s_sp;
    s16 s_maxsp;
    s16 s_str;
    s16 s_int;
    s16 s_wis;
    s16 s_dex;
    s16 s_con;
    s16 s_cha;
    s16 s_level;
    s16 s_wc;
    s16 s_ac;
    s16 s_dam;
    s16 s_armour;
    s16 s_food;
    s16 s_pow;
    s16 s_grace;

    float s_speed;
    float s_weap_sp;

    s16 s_maxgrace;
    s16 s_flags;

    s16 s_res_phys;
    s16 s_res_mag;
    s16 s_res_fire;
    s16 s_res_elec;
    s16 s_res_cold;
    s16 s_res_conf;
    s16 s_res_acid;
    s16 s_res_drain;
    s16 s_res_ghosthit;
    s16 s_res_poison;
    s16 s_res_slow;
    s16 s_res_para;
    s16 s_turn_undead;
    s16 s_res_fear;
    s16 s_res_deplete;
    s16 s_res_death;
    s16 s_res_holyword;
    s16 s_res_blind;
  };
}
