static inline std::string
read_string08 (std::istream &is)
{
  u08 n = read_u08 (is);
  char data[n];
  is.read (data, n);
  return std::string (data, n);
}

static inline std::string
read_string16 (std::istream &is)
{
  u16 n = read_u16 (is);
  char data[n];
  is.read (data, n);
  return std::string (data, n);
}

static inline std::string
read_string32 (std::istream &is)
{
  u32 n = read_u32 (is);
  char data[n];
  is.read (data, n);
  return std::string (data, n);
}
