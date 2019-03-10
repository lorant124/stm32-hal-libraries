// Copyright (c) Konstantin Belyalov. All rights reserved.
// Licensed under the MIT license.
#include "debug.h"
#include <stdio.h>

UART_HandleTypeDef *debug_uart = NULL;

static uint8_t *uint_to_str(uint64_t value, uint8_t base, uint8_t *buf, uint8_t len)
{
  uint8_t *current = buf + len - 1;

  if (len < 2 || buf == NULL) {
    return buf;
  }

  // Null terminated string
  *current = 0;

  // Special case for zero value
  if (value == 0) {
    current--;
    *current = '0';
    return current;
  }

  while (value > 0 && current != buf) {
    uint8_t mod = value % base;
    current--;
    *current = mod > 9 ? 'W' + mod : '0' + mod;
    value /= base;
  }

  return current;
}

uint8_t *debug_uint64_to_hexstring(uint64_t value, uint8_t *buf, uint8_t len)
{
  return uint_to_str(value, 16, buf, len);
}

uint8_t *debug_uint64_to_string(uint64_t value, uint8_t *buf, uint8_t len)
{
  return uint_to_str(value, 10, buf, len);
}

uint8_t *debug_int64_to_string(int64_t value, uint8_t *buf, uint8_t len)
{
  if (value >= 0) {
    return debug_uint64_to_string(value, buf, len);
  }

  if (len < 3) {
    return buf;
  }

  // Reserve space for sign
  buf++;
  uint8_t *res = uint_to_str(0 - value, 10, buf, len - 1);
  res--;
  *res = '-';

  return res;
}
