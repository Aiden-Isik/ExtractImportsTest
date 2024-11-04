// Copyright (c) 2024 Aiden Isik
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>

#if 'AB' == 0b100000101000010
#define LITTLE_ENDIAN_SYSTEM
#else
#define BIG_ENDIAN_SYSTEM
#endif

uint32_t get32BitFromXEX(FILE *xex)
{
  uint8_t store[4];
  fread(store, sizeof(uint8_t), 4, xex);
  
  uint32_t result = 0;

  for(int i = 0; i < 4; i++)
    {
      result |= store[i] << i * 8;
    }

  // If system and file endianness don't match we need to change it
#ifdef LITTLE_ENDIAN_SYSTEM
  result = ntohl(result);
#endif
  
  return result;
}

int main(int argc, char **argv)
{
  FILE *in = fopen(argv[1], "r");
  FILE *out = fopen("./import.bin", "w");

  while(get32BitFromXEX(in) != 0x103FF){} // Seek to import header offset
  fseek(in, get32BitFromXEX(in), SEEK_SET); // Go to import header

  // Get size to read
  uint32_t size = get32BitFromXEX(in);
  fseek(in, -0x4, SEEK_CUR);

  // Copy import header to file
  uint8_t buffer[size];
  fread(buffer, sizeof(uint8_t), size, in);
  fwrite(buffer, sizeof(uint8_t), size, out);

  fclose(in);
  fclose(out);
}
