namespace data
{
  struct map1a
  {
    enum extcmd
      : u08
    {
      got_clear_request = 1 << 0,
      got_map_info      = 1 << 1,
      got_darkness      = 1 << 2,
      got_health        = 1 << 3,
      got_width         = 1 << 4,
      got_player        = 1 << 5,
      got_flags         = 1 << 6
    };

    u08 x;
    u08 y;

    u08 stat_width;
    u08 stat_hp;

    u32 player;

    u16 face[3];
    u16 darkness;

    u08 flags;

    extcmd got;

    map1a ()
      : darkness (256)
    {
    }
  };
}
