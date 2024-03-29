// Module for interfacing with the OneWire interface

#include "module.h"
#include "lauxlib.h"
#include "platform.h"
#include "driver/onewire.h"

// Lua: ow.setup( id )
static int ow_setup( lua_State *L )
{
  unsigned id = luaL_checkinteger( L, 1 );

  if(id==0)
    return luaL_error( L, "no 1-wire for D0" );

  MOD_CHECK_ID( ow, id );

  onewire_init( id );
  return 0;
}

// Lua: r = ow.reset( id )
static int ow_reset( lua_State *L )
{
  unsigned id = luaL_checkinteger( L, 1 );
  MOD_CHECK_ID( ow, id );
  lua_pushinteger( L, onewire_reset(id) );
  return 1;
}

// Lua: ow.skip( id )
static int ow_skip( lua_State *L )
{
  unsigned id = luaL_checkinteger( L, 1 );
  MOD_CHECK_ID( ow, id );
  onewire_skip(id);
  return 0;
}

// Lua: ow.select( id, buf[8])
static int ow_select( lua_State *L )
{
  uint8_t rom[8];
  size_t datalen;
  int numdata, i;
  unsigned id = luaL_checkinteger( L, 1 );
  const char *pdata;
  MOD_CHECK_ID( ow, id );

  if( lua_istable( L, 2 ) )
  {
    datalen = lua_objlen( L, 2 );
    if (datalen!=8)
      return luaL_error( L, "wrong arg range" );
    for( i = 0; i < datalen; i ++ )
    {
      lua_rawgeti( L, 2, i + 1 );
      numdata = ( int )luaL_checkinteger( L, -1 );
      lua_pop( L, 1 );
      if( numdata > 255 )
        return luaL_error( L, "wrong arg range" );
      rom[i] = (uint8_t)numdata;
    }
  }
  else
  {
    pdata = luaL_checklstring( L, 2, &datalen );
    if (datalen!=8)
      return luaL_error( L, "wrong arg range" );
    for( i = 0; i < datalen; i ++ ){
      rom[i] = pdata[i];
    }
  }

  onewire_select(id, rom);
  return 0;
}

// Lua: ow.write( id, v, power)
static int ow_write( lua_State *L )
{
  int power = 0;
  unsigned id = luaL_checkinteger( L, 1 );
  MOD_CHECK_ID( ow, id );

  int v = (int)luaL_checkinteger( L, 2 );
  if( v > 255 )
    return luaL_error( L, "wrong arg range" );
  if(lua_isnumber(L, 3))
    power = lua_tointeger(L, 3);
  if(power!=0)
    power = 1;

  onewire_write((uint8_t)id, (uint8_t)v, (uint8_t)power);

  return 0;
}

// Lua: ow.write_bytes( id, buf, power)
static int ow_write_bytes( lua_State *L )
{
  int power = 0;
  size_t datalen;
  unsigned id = luaL_checkinteger( L, 1 );
  MOD_CHECK_ID( ow, id );

  const uint8_t *pdata = luaL_checklstring( L, 2, &datalen );

  if(lua_isnumber(L, 3))
    power = lua_tointeger(L, 3);
  if(power!=0)
    power = 1;

  onewire_write_bytes((uint8_t)id, pdata, (uint16_t)datalen, (uint8_t)power);

  return 0;
}

// Lua: r = ow.read( id )
static int ow_read( lua_State *L )
{
  unsigned id = luaL_checkinteger( L, 1 );
  MOD_CHECK_ID( ow, id );
  lua_pushinteger( L, onewire_read(id) );
  return 1;
}

// Lua: r = ow.read_bytes( id, size )
static int ow_read_bytes( lua_State *L )
{
  unsigned id = luaL_checkinteger( L, 1 );
  MOD_CHECK_ID( ow, id );
  u32 size = ( u32 )luaL_checkinteger( L, 2 );
  if( size == 0 )
    return 0;

  luaL_argcheck(L, size <= LUAL_BUFFERSIZE, 2, "Attempt to read too many characters");

  luaL_Buffer b;
  luaL_buffinit( L, &b );
  char *p = luaL_prepbuffer(&b);

  onewire_read_bytes(id, (uint8_t *)p, size);

  luaL_addsize(&b, size);
  luaL_pushresult( &b );
  return 1;
}

// Lua: ow.depower( id )
static int ow_depower( lua_State *L )
{
  unsigned id = luaL_checkinteger( L, 1 );
  MOD_CHECK_ID( ow, id );
  onewire_depower(id);
  return 0;
}

#if ONEWIRE_SEARCH
// Clear the search state so that if will start from the beginning again.
// Lua: ow.reset_search( id )
static int ow_reset_search( lua_State *L )
{
  unsigned id = luaL_checkinteger( L, 1 );
  MOD_CHECK_ID( ow, id );
  onewire_reset_search(id);
  return 0;
}


// Setup the search to find the device type 'family_code' on the next call
// to search(*newAddr) if it is present.
// Lua: ow.target_search( id, family_code)
static int ow_target_search( lua_State *L )
{
  unsigned id = luaL_checkinteger( L, 1 );
  MOD_CHECK_ID( ow, id );

  int code = (int)luaL_checkinteger( L, 2 );
  if( code > 255 )
    return luaL_error( L, "wrong arg range" );

  onewire_target_search((uint8_t)id, (uint8_t)code);

  return 0;
}

// Look for the next device. Returns 1 if a new address has been
// returned. A zero might mean that the bus is shorted, there are
// no devices, or you have already retrieved all of them.  It
// might be a good idea to check the CRC to make sure you didn't
// get garbage.  The order is deterministic. You will always get
// the same devices in the same order.

// Lua: r = ow.search( id )
static int ow_search( lua_State *L )
{
  unsigned id = luaL_checkinteger( L, 1 );
  MOD_CHECK_ID( ow, id );

  luaL_Buffer b;
  luaL_buffinit( L, &b );
  char *p = luaL_prepbuffer(&b);
  uint8_t alarm_search = 0;

  if(lua_isnumber(L, 2))
    alarm_search = lua_tointeger(L, 2);
  if(alarm_search != 0)
    alarm_search = 1;


  if(onewire_search(id, (uint8_t *)p, alarm_search)){
    luaL_addsize(&b, 8);
    luaL_pushresult( &b );
  } else {
    luaL_pushresult(&b);  /* close buffer */
    lua_pop(L,1);
    lua_pushnil(L);
  }
  return 1;
}
#endif

#if ONEWIRE_CRC
// uint8_t onewire_crc8(const uint8_t *addr, uint8_t len);
// Lua: r = ow.crc8( buf )
static int ow_crc8( lua_State *L )
{
  size_t datalen;
  const uint8_t *pdata = luaL_checklstring( L, 1, &datalen );
  if(datalen > 255)
    return luaL_error( L, "wrong arg range" );
  lua_pushinteger( L, onewire_crc8(pdata, (uint8_t)datalen) );
  return 1;
}

#if ONEWIRE_CRC16
// bool onewire_check_crc16(const uint8_t* input, uint16_t len, const uint8_t* inverted_crc, uint16_t crc);
// Lua: b = ow.check_crc16( buf, inverted_crc0, inverted_crc1, crc )
static int ow_check_crc16( lua_State *L )
{
  size_t datalen;
  uint8_t inverted_crc[2];
  const uint8_t *pdata = luaL_checklstring( L, 1, &datalen );
  if(datalen > 65535)
    return luaL_error( L, "wrong arg range" );

  int crc = 0;
  crc = luaL_checkinteger( L, 2 );
  if(datalen > 255)
    return luaL_error( L, "wrong arg range" );
  inverted_crc[0] = (uint8_t)crc;

  crc = luaL_checkinteger( L, 3 );
  if(datalen > 255)
    return luaL_error( L, "wrong arg range" );
  inverted_crc[1] = (uint8_t)crc;

  crc = 0;
  if(lua_isnumber(L, 4))
    crc = lua_tointeger(L, 4);
  if(crc > 65535)
    return luaL_error( L, "wrong arg range" );

  lua_pushboolean( L, onewire_check_crc16(pdata, (uint16_t)datalen, inverted_crc, (uint16_t)crc) );

  return 1;
}

// uint16_t onewire_crc16(const uint8_t* input, uint16_t len, uint16_t crc);
// Lua: r = ow.crc16( buf, crc )
static int ow_crc16( lua_State *L )
{
  size_t datalen;
  const uint8_t *pdata = luaL_checklstring( L, 1, &datalen );
  if(datalen > 65535)
    return luaL_error( L, "wrong arg range" );
  int crc = 0;
  if(lua_isnumber(L, 2))
    crc = lua_tointeger(L, 2);
  if(crc > 65535)
    return luaL_error( L, "wrong arg range" );

  lua_pushinteger( L, onewire_crc16(pdata, (uint16_t)datalen, (uint16_t)crc) );

  return 1;
}
#endif
#endif

// Lua: r = ow.set_timings( reset_tx, reset_wait, reset_rx, w1_low, w1_high, w0_low, w0_high, r_low, r_wait, r_delay )
static int ow_set_timings( lua_State *L )
{
  if(lua_isnumber(L, 1))
    onewire_timings.reset_tx = lua_tointeger(L, 1);
  if(lua_isnumber(L, 2))
    onewire_timings.reset_wait = lua_tointeger(L, 2);
  if(lua_isnumber(L, 3))
    onewire_timings.reset_rx = lua_tointeger(L, 3);
  if(lua_isnumber(L, 4))
    onewire_timings.w_1_low = lua_tointeger(L, 4);
  if(lua_isnumber(L, 5))
    onewire_timings.w_1_high = lua_tointeger(L, 5);
  if(lua_isnumber(L, 6))
    onewire_timings.w_0_low = lua_tointeger(L, 6);
  if(lua_isnumber(L, 7))
    onewire_timings.w_0_high = lua_tointeger(L, 7);
  if(lua_isnumber(L, 8))
    onewire_timings.r_low = lua_tointeger(L, 8);
  if(lua_isnumber(L, 9))
    onewire_timings.r_wait = lua_tointeger(L, 9);
  if(lua_isnumber(L, 10))
    onewire_timings.r_delay = lua_tointeger(L, 10);

  return 0;
}

// Module function map
LROT_BEGIN(ow, NULL, 0)
  LROT_FUNCENTRY( setup, ow_setup )
  LROT_FUNCENTRY( reset, ow_reset )
  LROT_FUNCENTRY( skip, ow_skip )
  LROT_FUNCENTRY( select, ow_select )
  LROT_FUNCENTRY( write, ow_write )
  LROT_FUNCENTRY( write_bytes, ow_write_bytes )
  LROT_FUNCENTRY( read, ow_read )
  LROT_FUNCENTRY( read_bytes, ow_read_bytes )
  LROT_FUNCENTRY( depower, ow_depower )
#if ONEWIRE_SEARCH
  LROT_FUNCENTRY( reset_search, ow_reset_search )
  LROT_FUNCENTRY( target_search, ow_target_search )
  LROT_FUNCENTRY( search, ow_search )
#endif
#if ONEWIRE_CRC
  LROT_FUNCENTRY( crc8, ow_crc8 )
#if ONEWIRE_CRC16
  LROT_FUNCENTRY( check_crc16, ow_check_crc16 )
  LROT_FUNCENTRY( crc16, ow_crc16 )
#endif
#endif
  LROT_FUNCENTRY( set_timings, ow_set_timings )
LROT_END(ow, NULL, 0)


NODEMCU_MODULE(OW, "ow", ow, NULL);
