#include "deliantra/data/statistics.h"

#include "adt/index_map.h"

using data::statistics;

static index_map<s16 (statistics::*)> const u16_stats {
  { CS_STAT_HP,               &statistics::s_hp               },
  { CS_STAT_MAXHP,            &statistics::s_maxhp            },
  { CS_STAT_SP,               &statistics::s_sp               },
  { CS_STAT_MAXSP,            &statistics::s_maxsp            },
  { CS_STAT_STR,              &statistics::s_str              },
  { CS_STAT_INT,              &statistics::s_int              },
  { CS_STAT_WIS,              &statistics::s_wis              },
  { CS_STAT_DEX,              &statistics::s_dex              },
  { CS_STAT_CON,              &statistics::s_con              },
  { CS_STAT_CHA,              &statistics::s_cha              },
  { CS_STAT_LEVEL,            &statistics::s_level            },
  { CS_STAT_WC,               &statistics::s_wc               },
  { CS_STAT_AC,               &statistics::s_ac               },
  { CS_STAT_DAM,              &statistics::s_dam              },
  { CS_STAT_ARMOUR,           &statistics::s_armour           },
  { CS_STAT_FOOD,             &statistics::s_food             },
  { CS_STAT_POW,              &statistics::s_pow              },
  { CS_STAT_GRACE,            &statistics::s_grace            },
  { CS_STAT_MAXGRACE,         &statistics::s_maxgrace         },
  { CS_STAT_FLAGS,            &statistics::s_flags            },
  { CS_STAT_RES_PHYS,         &statistics::s_res_phys         },
  { CS_STAT_RES_MAG,          &statistics::s_res_mag          },
  { CS_STAT_RES_FIRE,         &statistics::s_res_fire         },
  { CS_STAT_RES_ELEC,         &statistics::s_res_elec         },
  { CS_STAT_RES_COLD,         &statistics::s_res_cold         },
  { CS_STAT_RES_CONF,         &statistics::s_res_conf         },
  { CS_STAT_RES_ACID,         &statistics::s_res_acid         },
  { CS_STAT_RES_DRAIN,        &statistics::s_res_drain        },
  { CS_STAT_RES_GHOSTHIT,     &statistics::s_res_ghosthit     },
  { CS_STAT_RES_POISON,       &statistics::s_res_poison       },
  { CS_STAT_RES_SLOW,         &statistics::s_res_slow         },
  { CS_STAT_RES_PARA,         &statistics::s_res_para         },
  { CS_STAT_TURN_UNDEAD,      &statistics::s_turn_undead      },
  { CS_STAT_RES_FEAR,         &statistics::s_res_fear         },
  { CS_STAT_RES_DEPLETE,      &statistics::s_res_deplete      },
  { CS_STAT_RES_DEATH,        &statistics::s_res_death        },
  { CS_STAT_RES_HOLYWORD,     &statistics::s_res_holyword     },
  { CS_STAT_RES_BLIND,        &statistics::s_res_blind        },
};

static index_map<s32 (statistics::*)> const u32_stats {
  { CS_STAT_EXP,              &statistics::s_exp              },
  { CS_STAT_WEIGHT_LIM,       &statistics::s_weight_lim       },
  { CS_STAT_SPELL_ATTUNE,     &statistics::s_spell_attune     },
  { CS_STAT_SPELL_REPEL,      &statistics::s_spell_repel      },
  { CS_STAT_SPELL_DENY,       &statistics::s_spell_deny       },
};

static index_map<s64 (statistics::*)> const u64_stats {
  { CS_STAT_EXP64,            &statistics::s_exp64            },
};

static index_map<std::string (statistics::*)> const string_stats {
  { CS_STAT_RANGE,            &statistics::s_range            },
  { CS_STAT_TITLE,            &statistics::s_title            },
};

static index_map<float (statistics::*)> const float_stats {
  { CS_STAT_SPEED,            &statistics::s_speed            },
  { CS_STAT_WEAP_SP,          &statistics::s_weap_sp          },
};

void
command_parser::on_stats (std::istream &args)
{
  while (args.peek () != EOF)
    {
      u08 stat = read_u08 (args);

      auto const &logs = LOG;
      logs ("server") << "stat " << u32 (stat) << ": ";

      if (u16_stats[stat])
        logs << (stats.*u16_stats[stat] = read_u16 (args));
      else if (u32_stats[stat])
        logs << (stats.*u32_stats[stat] = read_u32 (args));
      else if (u64_stats[stat])
        logs << (stats.*u64_stats[stat] = read_u64 (args));
      else if (string_stats[stat])
        logs << (stats.*string_stats[stat] = read_string08 (args));
      else if (float_stats[stat])
        logs << (stats.*float_stats[stat] = (1 / FLOAT_MULTF) * read_u32 (args));
      else if (stat >= CS_STAT_SKILLINFO && stat < CS_STAT_SKILLINFO + CS_NUM_SKILLS)
        {
          auto level = read_u08 (args);
          auto value = read_u64 (args);
          logs << "skillinfo: " << u32 (stat) << ": " << u32 (level) << ", " << value;
          stats.s_skillinfo[stat - CS_STAT_SKILLINFO] = value;
        }
      else
        {
          logs << "\nserver: unknown stat " << u32 (stat) << '\n';
          throw std::runtime_error ("unknown stat");
        }

      logs << '\n';

      assert (args);
    }
  PARSE_END;

  emit stats (stats);
}
