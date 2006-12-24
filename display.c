#include "display.h"
#include "nxt_lcd.h"
#include "systick.h"

#include <string.h>


#define DISPLAY_WIDTH (NXT_LCD_WIDTH)
#define DISPLAY_DEPTH (NXT_LCD_DEPTH)

static U8 display_buffer[DISPLAY_DEPTH][DISPLAY_WIDTH];

/* Font table for a 5x8 font. 1 pixel spacing between chars */
#define N_CHARS 128
#define FONT_WIDTH 5
#define CELL_WIDTH (FONT_WIDTH + 1)
#define DISPLAY_CHAR_WIDTH (DISPLAY_WIDTH/(CELL_WIDTH))
#define DISPLAY_CHAR_DEPTH (DISPLAY_DEPTH)

static const U8 font [N_CHARS][FONT_WIDTH] = {
/* 0x00 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x01 */ {0x3E,0x55,0x61,0x55,0x3E},
/* 0x02 */ {0x3E,0x6B,0x5F,0x6B,0x3E},
/* 0x03 */ {0x0C,0x1E,0x3C,0x1E,0x0C},
/* 0x04 */ {0x08,0x1C,0x3E,0x1C,0x08},
/* 0x05 */ {0x18,0x5E,0x7E,0x5E,0x18},
/* 0x06 */ {0x18,0x5C,0x7E,0x5C,0x18},
/* 0x07 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x08 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x09 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x0A */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x0B */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x0C */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x0D */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x0E */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x0F */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x10 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x11 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x12 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x13 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x14 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x15 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x16 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x17 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x18 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x19 */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x1A */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x1B */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x1C */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x1D */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x1E */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x1F */ {0x3E,0x36,0x2A,0x36,0x3E},
/* 0x20 */ {0x00,0x00,0x00,0x00,0x00},
/* 0x21 */ {0x00,0x00,0x5F,0x00,0x00},
/* 0x22 */ {0x00,0x07,0x00,0x07,0x00},
/* 0x23 */ {0x14,0x3E,0x14,0x3E,0x14},
/* 0x24 */ {0x04,0x2A,0x7F,0x2A,0x10},
/* 0x25 */ {0x26,0x16,0x08,0x34,0x32},
/* 0x26 */ {0x36,0x49,0x59,0x26,0x50},
/* 0x27 */ {0x00,0x00,0x07,0x00,0x00},
/* 0x28 */ {0x00,0x1C,0x22,0x41,0x00},
/* 0x29 */ {0x00,0x41,0x22,0x1C,0x00},
/* 0x2A */ {0x2A,0x1C,0x7F,0x1C,0x2A},
/* 0x2B */ {0x08,0x08,0x3E,0x08,0x08},
/* 0x2C */ {0x00,0x50,0x30,0x00,0x00},
/* 0x2D */ {0x08,0x08,0x08,0x08,0x08},
/* 0x2E */ {0x00,0x60,0x60,0x00,0x00},
/* 0x2F */ {0x20,0x10,0x08,0x04,0x02},
/* 0x30 */ {0x3E,0x51,0x49,0x45,0x3E},
/* 0x31 */ {0x00,0x42,0x7F,0x40,0x00},
/* 0x32 */ {0x42,0x61,0x51,0x49,0x46},
/* 0x33 */ {0x21,0x41,0x45,0x4B,0x31},
/* 0x34 */ {0x18,0x14,0x12,0x7F,0x10},
/* 0x35 */ {0x27,0x45,0x45,0x45,0x39},
/* 0x36 */ {0x3C,0x4A,0x49,0x49,0x30},
/* 0x37 */ {0x01,0x01,0x79,0x05,0x03},
/* 0x38 */ {0x36,0x49,0x49,0x49,0x36},
/* 0x39 */ {0x06,0x49,0x49,0x29,0x1E},
/* 0x3A */ {0x00,0x36,0x36,0x00,0x00},
/* 0x3B */ {0x00,0x56,0x36,0x00,0x00},
/* 0x3C */ {0x08,0x14,0x22,0x41,0x00},
/* 0x3D */ {0x14,0x14,0x14,0x14,0x14},
/* 0x3E */ {0x41,0x22,0x14,0x08,0x00},
/* 0x3F */ {0x02,0x01,0x59,0x05,0x02},
/* 0x40 */ {0x1C,0x2A,0x36,0x3E,0x0C},
/* 0x41 */ {0x7E,0x09,0x09,0x09,0x7E},
/* 0x42 */ {0x7F,0x49,0x49,0x49,0x3E},
/* 0x43 */ {0x3E,0x41,0x41,0x41,0x22},
/* 0x44 */ {0x7F,0x41,0x41,0x22,0x1C},
/* 0x45 */ {0x7F,0x49,0x49,0x49,0x41},
/* 0x46 */ {0x7F,0x09,0x09,0x09,0x01},
/* 0x47 */ {0x3E,0x41,0x41,0x49,0x3A},
/* 0x48 */ {0x7F,0x08,0x08,0x08,0x7F},
/* 0x49 */ {0x00,0x41,0x7F,0x41,0x00},
/* 0x4A */ {0x20,0x40,0x41,0x3F,0x01},
/* 0x4B */ {0x7F,0x08,0x14,0x22,0x41},
/* 0x4C */ {0x7F,0x40,0x40,0x40,0x40},
/* 0x4D */ {0x7F,0x02,0x04,0x02,0x7F},
/* 0x4E */ {0x7F,0x04,0x08,0x10,0x7F},
/* 0x4F */ {0x3E,0x41,0x41,0x41,0x3E},
/* 0x50 */ {0x7F,0x09,0x09,0x09,0x06},
/* 0x51 */ {0x3E,0x41,0x51,0x21,0x5E},
/* 0x52 */ {0x7F,0x09,0x19,0x29,0x46},
/* 0x53 */ {0x26,0x49,0x49,0x49,0x32},
/* 0x54 */ {0x01,0x01,0x7F,0x01,0x01},
/* 0x55 */ {0x3F,0x40,0x40,0x40,0x3F},
/* 0x56 */ {0x1F,0x20,0x40,0x20,0x1F},
/* 0x57 */ {0x7F,0x20,0x18,0x20,0x7F},
/* 0x58 */ {0x63,0x14,0x08,0x14,0x63},
/* 0x59 */ {0x03,0x04,0x78,0x04,0x03},
/* 0x5A */ {0x61,0x51,0x49,0x45,0x43},
/* 0x5B */ {0x00,0x7F,0x41,0x41,0x00},
/* 0x5C */ {0x02,0x04,0x08,0x10,0x20},
/* 0x5D */ {0x00,0x41,0x41,0x7F,0x00},
/* 0x5E */ {0x04,0x02,0x01,0x02,0x04},
/* 0x5F */ {0x40,0x40,0x40,0x40,0x40},
/* 0x60 */ {0x00,0x00,0x07,0x00,0x00},
/* 0x61 */ {0x20,0x54,0x54,0x54,0x78},
/* 0x62 */ {0x7f,0x48,0x44,0x44,0x38},
/* 0x63 */ {0x30,0x48,0x48,0x48,0x20},
/* 0x64 */ {0x38,0x44,0x44,0x48,0x7f},
/* 0x65 */ {0x38,0x54,0x54,0x54,0x18},
/* 0x66 */ {0x08,0x7e,0x09,0x09,0x02},
/* 0x67 */ {0x0c,0x52,0x52,0x52,0x3e},
/* 0x68 */ {0x7f,0x08,0x04,0x04,0x78},
/* 0x69 */ {0x00,0x44,0x7d,0x40,0x00},
/* 0x6A */ {0x20,0x40,0x40,0x3d,0x00},
/* 0x6B */ {0x7f,0x10,0x28,0x44,0x00},
/* 0x6C */ {0x00,0x41,0x7f,0x40,0x00},
/* 0x6D */ {0x7c,0x04,0x18,0x04,0x78},
/* 0x6E */ {0x7c,0x08,0x04,0x04,0x78},
/* 0x6F */ {0x38,0x44,0x44,0x44,0x38},
/* 0x70 */ {0xfc,0x14,0x14,0x14,0x08},
/* 0x71 */ {0x08,0x14,0x14,0x18,0x7c},
/* 0x72 */ {0x7c,0x08,0x04,0x04,0x08},
/* 0x73 */ {0x48,0x54,0x54,0x54,0x20},
/* 0x74 */ {0x04,0x3f,0x44,0x40,0x20},
/* 0x75 */ {0x3c,0x40,0x40,0x20,0x7c},
/* 0x76 */ {0x1c,0x20,0x40,0x20,0x1c},
/* 0x77 */ {0x3c,0x40,0x38,0x40,0x3c},
/* 0x78 */ {0x44,0x28,0x10,0x28,0x44},
/* 0x79 */ {0x0c,0x50,0x50,0x50,0x3c},
/* 0x7A */ {0x44,0x64,0x54,0x4c,0x44},
/* 0x7B */ {0x00,0x08,0x36,0x41,0x00},
/* 0x7C */ {0x00,0x00,0x7F,0x00,0x00},
/* 0x7D */ {0x00,0x41,0x36,0x08,0x00},
/* 0x7E */ {0x00,0x07,0x00,0x07,0x00},
/* 0x7F */ {0x3E,0x36,0x2A,0x36,0x3E},
};



void display_update(void)
{
  nxt_lcd_data((U8 *)display_buffer);
}


void display_clear(U32 updateToo)
{
  memset(display_buffer,0,sizeof(display_buffer));
  if(updateToo)
    display_update();
}


int display_x;
int display_y;
/* text functions, top left is (0,0) */
void display_goto_xy(int x, int y)
{
  display_x = x;
  display_y = y;
}

void display_char(int c)
{
  int i;
  U8 *b;
  const U8 *f;
  if(c >= 0 && c < N_CHARS &&
     display_x >= 0 && display_x < DISPLAY_CHAR_WIDTH &&
     display_y >= 0 && display_y < DISPLAY_CHAR_DEPTH){
       b = &display_buffer[display_y][display_x * CELL_WIDTH];
       f = font[c];
       for(i = 0; i < FONT_WIDTH; i++){
         *b = *f;
         b++;
         f++;
       }
  }
}

void display_string(const char *str)
{
  while(*str){
    if(*str != '\n'){
      display_char(*str);
      display_x++;
    } else {
      display_x = 0;
      display_y++;
    }
    str++;
  }
}

static void display_unsigned_worker(U32 val, U32 places, U32 sign)
{
  char x[12]; // enough for 10 digits + sign + NULL 
                    
  char *p = &x[11];
  int p_count = 0;
  
  
  *p = 0;
  
  if(places > 11)
    places = 11;
  
  while(val) {
    p--;
    p_count++;
    *p = (val % 10) + '0';
    val/=10;
  }
  
  if(!p_count){
    p--;
    p_count++;
    *p = '0';
  }
  
  if(sign){
    p--;
    p_count++;
    *p = '-';
  }
  
  while(p_count < places){
    p--;
    p_count++;
    *p = ' ';
  }
  
  display_string(p);
}

void display_unsigned(U32 val,U32 places)
{
  display_unsigned_worker(val,places,0);
}

void display_int(int val,U32 places)
{
  display_unsigned_worker((val < 0) ? -val : val, places, (val < 0));
}


void display_bitmap_copy(const U8 *data, U32 width, U32 depth, U32 x, U32 y)
{
  U32 i;
  U32 j;
  U32 dy;
  U32 dx;

  for(i = 0; i < depth; i++)
    for(j = 0; j < width; j++){
      dx = x + j;
      dy = y + i;
      
      if( dx < DISPLAY_WIDTH && dy < DISPLAY_DEPTH)
        display_buffer[y + i][x+j] = data[width * i + j];
    }
}



void display_init(void)
{
  nxt_lcd_init();
  display_clear(1);
}

void display_test(void)
{
  int iterator = 0;
  nxt_lcd_init();
  while(1){
    display_clear(0);
    display_goto_xy(iterator,0); display_string("LEJOS NXT");
    display_goto_xy(0,1); display_string("0123456789.:/");
    display_goto_xy(0,2); display_string("abcdefghijklm");
    display_goto_xy(0,3); display_string("nopqrstuvwxyz");
    display_goto_xy(0,4); display_string("ABCDEFGHIJKLM");
    display_goto_xy(0,5); display_string("NOPQRSTUVWXYZ");
    
    display_goto_xy(0,7); display_string("TIME ");display_unsigned(systick_get_ms(),0);
    iterator = (iterator + 1) & 7;
    display_update();
    systick_wait_ms(2000);
  }
}


