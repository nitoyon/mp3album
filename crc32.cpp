/*
  Copyright (c) 1990-1999 Info-ZIP.  All rights reserved.

  See the accompanying file LICENSE, version 1999-Oct-05 or later
  (the contents of which are also included in zip.h) for terms of use.
  If, for some reason, both of these files are missing, the Info-ZIP license
  also may be found at:  ftp://ftp.cdrom.com/pub/infozip/license.html
*/
/* crc32.c -- compute the CRC-32 of a data stream
 * Copyright (C) 1995 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/* $Id: crc32.cpp,v 1.2 2002/12/09 04:39:42 nitoyon Exp $ */

#define __CRC32_C       /* identifies this source module */

#include "zip.h"

#ifndef USE_ZLIB
#ifndef ASM_CRC

#ifndef ZCONST
#  define ZCONST const
#endif

#ifdef CRC32
#  undef CRC32
#endif
#define CRC32(c, b) (crc_table[((int)(c) ^ (b)) & 0xff] ^ ((c) >> 8))
#define DO1(buf)  crc = CRC32(crc, *buf++)
#define DO2(buf)  DO1(buf); DO1(buf)
#define DO4(buf)  DO2(buf); DO2(buf)
#define DO8(buf)  DO4(buf); DO4(buf)

/* ========================================================================= */
ulg crc32(register ulg crc, register ZCONST uch *buf, extent len)
{
  register ZCONST ulg near *crc_table;

  if (buf == NULL) return 0L;

  crc_table = get_crc_table();

  crc = crc ^ 0xffffffffL;
#ifndef NO_UNROLLED_LOOPS
  while (len >= 8) {
    DO8(buf);
    len -= 8;
  }
#endif
  if (len) do {
    DO1(buf);
  } while (--len);
  return crc ^ 0xffffffffL;     /* (instead of ~c for 64-bit machines) */
}
#endif /* !ASM_CRC */
#endif /* !USE_ZLIB */
