static inline u32
read_ber32 (std::istream &is)
{
  u32 value = 0;
  u08 digit;

  while ((digit = read_u08 (is)) & 0x80)
    {
      value |= digit & ~0x80;
      value <<= 7;
    }
  value |= digit;

  return value;
}
