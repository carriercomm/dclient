static inline u08
read_u08 (std::istream &is)
{
  unsigned char bytes[1];
  is.read ((char *)bytes, sizeof bytes);
  return u08 (bytes[0]) << 8 * 0
       ;
}

static inline u16
read_u16 (std::istream &is)
{
  unsigned char bytes[2];
  is.read ((char *)bytes, sizeof bytes);
  return u16 (bytes[0]) << 8 * 1
       | u16 (bytes[1]) << 8 * 0
       ;
}

static inline u32
read_u24 (std::istream &is)
{
  unsigned char bytes[3];
  is.read ((char *)bytes, sizeof bytes);
  return u32 (bytes[0]) << 8 * 2
       | u32 (bytes[1]) << 8 * 1
       | u32 (bytes[2]) << 8 * 0
       ;
}

static inline u32
read_u32 (std::istream &is)
{
  unsigned char bytes[4];
  is.read ((char *)bytes, sizeof bytes);
  return u32 (bytes[0]) << 8 * 3
       | u32 (bytes[1]) << 8 * 2
       | u32 (bytes[2]) << 8 * 1
       | u32 (bytes[3]) << 8 * 0
       ;
}

static inline u64
read_u64 (std::istream &is)
{
  unsigned char bytes[8];
  is.read ((char *)bytes, sizeof bytes);
  return u64 (bytes[0]) << 8 * 7
       | u64 (bytes[1]) << 8 * 6
       | u64 (bytes[2]) << 8 * 5
       | u64 (bytes[3]) << 8 * 4
       | u64 (bytes[4]) << 8 * 3
       | u64 (bytes[5]) << 8 * 2
       | u64 (bytes[6]) << 8 * 1
       | u64 (bytes[7]) << 8 * 0
       ;
}
