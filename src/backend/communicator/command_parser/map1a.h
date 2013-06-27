/**
 * S->C: map1a <coord1>[darkness1][face1a][face1b][face1c]<coord2>[darkness2][face2a]...
 *
 * This is an update of the map command to support large map sizes.
 * The old map command supported a maximum map size of 15x15 -
 * anything larger than that required a new command.
 *
 * Given that this larger map now needs 2 bytes for the coordinates -
 * the same as for the images, trying to optimise one vs the other
 * does not makes much sense.
 *
 * The map1a is an enhancement to the map1 command.
 *
 * All data is in standard binary form.
 */
void
command_parser::on_map1a (std::istream &args)
{
  while (args.peek () != EOF)
    {
      // the coord values are flags + x + y values.  The value itself, but
      // the data represented looks like this:
      u16 const coord = read_u16 (args);

      // first 6 bits: the x coordinate
      s16 const coord_x = (coord >> 10) & 63;
      // next 6 bits: the y coordinate
      s16 const coord_y = (coord >>  4) & 63;
      // last 4 bits: MSB - true if we send darkness
      //              MSB-1 - will send floor face
      //              MSB-2 - will send intermediate face
      //              MSB-3 (aka LSB) - will send top face
      u08 const flags = coord & 15;

      s16 const x = map.x + coord_x;
      s16 const y = map.y + coord_y;

      auto &cell = map (x, y);

      if (flags)
        {
          if (!cell.darkness)
            cell.darkness = 256;

          if (flags & 8)
            {
              if (setup.extmap)
                {
                  u08 ext;

                  do
                    {
                      ext = read_u08 (args);
                      u08 const cmd = ext & 0x7f;

                      if (cmd < 4)
                        cell.darkness = 255 - ext * 64 + 1;
                      if (cmd == 5) // health
                        {
                          cell.stat_width = 1;
                          cell.stat_hp = read_u08 (args);
                        }
                      else if (cmd == 6) // monster width
                        {
                          cell.stat_width = read_u08 (args) + 1;
                        }
                      else if (cmd == 0x47)
                        {
                          u08 bytes = read_u08 (args);
                          switch (bytes)
                            {
                            case 1:
                              cell.player = read_u08 (args);
                              break;
                            case 2:
                              cell.player = read_u16 (args);
                              break;
                            case 3:
                              cell.player = read_u24 (args);
                              break;
                            case 4:
                              cell.player = read_u32 (args);
                              break;
                            default:
                              args.ignore (bytes);
                              break;
                            }
                        }
                      else if (cmd == 8) // cell flags
                        cell.flags = read_u08 (args);
                      else if (ext & 0x40) // unknown, multibyte => skip
                        args.ignore (read_u08 (args));
                    }
                  while (ext & 0x80);
                }
              else // no extmap
                cell.darkness = read_u08 (args);
            }

          for (int z = 0; z < 3; ++z)
            if (flags & (4 >> z))
              cell.face[z] = read_u16 (args);
        }
      else
        {
          // If all the flag values are 0, this then means the space is considered
          // blocked from view, and should be drawn as black.  This conserves
          // bandwidth for sending blocked spaces, which occur pretty frequently.
          // Once a space is marked as block, if it re-appears within view, the 3
          // layers should be marked is blank.
          cell.clear ();
        }
      
      assert (args);
    }
  PARSE_END;

  emit map (map);
}
