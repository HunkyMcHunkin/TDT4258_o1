#include "graphics.h"

#define FB_DRAW 0x4680

int framebuffer_fd;
uint16_t *framebuffer_pointer;

int screensize_pixels;
int screensize_bytes;

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
struct fb_copyarea rect;

int init_framebuffer()
{
  printf("[FRAMEBUFFER] Initializing framebuffer...\n");

  framebuffer_fd = open("/dev/fb0", O_RDWR);
  
  if (framebuffer_fd == -1)
  {
    printf("[FRAMEBUFFER] ERROR: Couldn't initialize framebuffer (framebuffer_fd)...\n");
    return EXIT_FAILURE;
  }
  
  if (ioctl(framebuffer_fd, FBIOGET_VSCREENINFO, &vinfo) == -1)
  {
    printf("[FRAMEBUFFER] ERROR: Couldn't get screen info (IOCTL, VSCREENINFO)...\n");
    return EXIT_FAILURE;
  }
  
  if (ioctl(framebuffer_fd, FBIOGET_FSCREENINFO, &finfo) == -1)
  {
    printf("[FRAMEBUFFER] ERROR: Couldn't get screen info (IOCTL, FSCREENINFO)...\n");
    return EXIT_FAILURE;
  }

  rect.dx = 0;
  rect.dy = 0;
  rect.width = vinfo.xres;
  rect.height = vinfo.yres;
  ioctl(framebuffer_fd, FB_DRAW, &rect);

  screensize_pixels = vinfo.xres * vinfo.yres;
  screensize_bytes = screensize_pixels * vinfo.bits_per_pixel / 8;

  framebuffer_pointer = (uint16_t*)mmap(NULL, screensize_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, framebuffer_fd, 0);

  if (framebuffer_pointer == MAP_FAILED)
  {
    printf("[FRAMEBUFFER] ERROR: Failed to memorymap framebuffer...\n");
    return EXIT_FAILURE;
  }

  for (int i = 0; i < screensize_pixels; i++)
  {
    framebuffer_pointer[i] = LightGrey;  
  }

  return EXIT_SUCCESS;
}

void deinit_framebuffer()
{
  munmap(framebuffer_pointer, screensize_bytes);
  close(framebuffer_fd);
}

void refresh_fb()
{
  ioctl(framebuffer_fd, FB_DRAW, &rect);
}

void draw_grid()
{
  int i;

  for (i = 0; i < screensize_pixels; i++)
  {
    int row = get_row(i);
    int col = get_col(i, row);
    
    if ((col <= 75 && col < 80) || (col <= (75*2) && col > (80*2)) || (col <= (75*3) && col > (80*3)))
    {
      framebuffer_pointer[i] = Black;
    }
    if ((row <= 75 && row < 80) || (row <= (75*2) && row > (75*2)))
    {
      framebuffer_pointer[i] = Black;
    }
  }

  for (i = 0; i < screensize_pixels; i++)
  {
    int row = get_row(i);
    int col = get_col(i, row);

    if (col > 240)
    {
      framebuffer_pointer[i] = LightGrey;
    }
  }
}

void fill_block(int maxR, int minR, int maxC, int minC, bool player)
{
  int i;
  for (i = 0; i < screensize_pixels; i++)
  {
    int row = get_row(i);
    int col = get_col(i, row);
    if (((col < maxC) && (col >= minC)) && ((row < maxR) && (row >= minR)))
    {
      if (player)
      {
        framebuffer_pointer[i] = Red;
      }
      else
      {
        framebuffer_pointer[i] = Blue;
      }
    }
  }
}

int get_row (int pixel)
{
  return (int) pixel / 320;
}

int get_col (int pixel, int row)
{
  return (int) ((int)pixel - (row * 320));
}
/*
int set_pixel(uint32_t x, uint32_t y, uint16_t color)
{
  long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel/16) + (y + vinfo.yoffset) * finfo.line_length;

  framebuffer_pointer[location] = hex;

  return 0;
}

int set_pixel_scale(uint32_t x, uint32_t y, uint16_t color, uint scale)
{
 return get_pixel(x*scale, y*scale/2);
}

uint16_t get_pixel(uint32_t x, uint32_t y)
{
  long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel/16) + (y + vinfo.yoffset) * finfo.line_length;

  return framebuffer_pointer[location] = hex;
}*/
