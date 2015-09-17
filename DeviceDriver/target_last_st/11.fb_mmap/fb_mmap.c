#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <linux/fb.h>
#include <sys/mman.h>
#include <assert.h>


unsigned char* framebuf = 0;

int gwidth, gheight;


static void print_bitfield(char* color_name, struct fb_bitfield * bf)
{
    printf("%s ", color_name);
    printf(" MSB:%s ", bf->msb_right != 0 ? "RIGHT":"LEFT ");
    printf(" offset(%02d) length(%02d)\n", bf->offset, bf->length);
}

void print_var_scrinfo(int fd_fb)
{
    int ret;
    struct fb_var_screeninfo var_scrinfo;

    /* get screen info */
    ret = ioctl(fd_fb, FBIOGET_VSCREENINFO, &var_scrinfo);
    if(ret < 0) {
        perror("framebuffer ioctl");
        exit(1);
    }

    /*
     * display var_scrinfo
     */
    printf("\nfb_var_screeninfo\n");
    printf("\t xres           %d\n", var_scrinfo.xres);
    printf("\t yres           %d\n", var_scrinfo.yres);
    printf("\t xres_virtual   %d\n", var_scrinfo.xres_virtual);
    printf("\t yres_virtual   %d\n", var_scrinfo.yres_virtual);
    printf("\t xoffset        %d\n", var_scrinfo.xoffset);
    printf("\t yoffset        %d\n", var_scrinfo.yoffset);
    printf("\t bits_per_pixel %d\n", var_scrinfo.bits_per_pixel);
    printf("\t grayscale      %d\n", var_scrinfo.grayscale);
    print_bitfield("\t red          ", &var_scrinfo.red);
    print_bitfield("\t green        ", &var_scrinfo.green);
    print_bitfield("\t blue         ", &var_scrinfo.blue);
    print_bitfield("\t transp       ", &var_scrinfo.transp);

    printf("\t nonstd         %s\n", var_scrinfo.nonstd != 0 ? "Non standard": "Standard");
    printf("\t activate       %s\n", var_scrinfo.activate == 0 ? "No hardware accelerator" : "hardware accelerator");
    printf("\t height         %d\n", var_scrinfo.height);
    printf("\t width          %d\n", var_scrinfo.width);
    printf("\t accel_flags    %d\n", var_scrinfo.accel_flags);
}

void print_fix_scrinfo(int fd_fb)
{
    char id[20];
    int  ret;
    struct fb_fix_screeninfo fix_scrinfo;

    ret = ioctl(fd_fb, FBIOGET_FSCREENINFO, &fix_scrinfo);
    if(ret < 0) {
        perror("framebuffer ioctl");
        exit(1);
    }

    /*
     * display fix_scrinfo
     */
    memcpy(id, fix_scrinfo.id, 16);
    printf("fb_fix_screeninfo\n");
    printf("\t id             %s\n", id);
    printf("\t smem_start     0x%08X\n", fix_scrinfo.smem_start);
    printf("\t smem_len       %d\n", fix_scrinfo.smem_len);

    /* print type */
    {
        char* fbtype;

        switch(fix_scrinfo.type) {
            case FB_TYPE_PACKED_PIXELS:
                fbtype = "FB_TYPE_PACKED_PIXELS";
                break;
            case FB_TYPE_PLANES:
                fbtype = "FB_TYPE_PLANES";
                break;
            case FB_TYPE_INTERLEAVED_PLANES:
                fbtype = "FB_TYPE_INTERLEAVED_PLANES";
                break;
            case FB_TYPE_TEXT:
                fbtype = "FB_TYPE_TEXT";
                break;
            case FB_TYPE_VGA_PLANES:
                fbtype = "FB_TYPE_VGA_PLANES";
                break;
            default:
                fbtype = "UNKNOWN";
        }
        printf("\t type           %s\n", fbtype);
    }

    printf("\t type_aux       %d\n", fix_scrinfo.type_aux);

    /* print visual */
    {
        char* visual;
        switch(fix_scrinfo.visual) {
            case FB_VISUAL_MONO01:
                visual = "FB_VISUAL_MONO01";
                break;
            case FB_VISUAL_MONO10:
                visual = "FB_VISUAL_MONO10";
                break;
            case FB_VISUAL_TRUECOLOR:
                visual = "FB_VISUAL_TRUECOLOR";
                break;
            case FB_VISUAL_PSEUDOCOLOR:
                visual = "FB_VISUAL_PSEUDOCOLOR";
                break;
            case FB_VISUAL_DIRECTCOLOR:
                visual = "FB_VISUAL_DIRECTCOLOR";
                break;
            case FB_VISUAL_STATIC_PSEUDOCOLOR:
                visual = "FB_VISUAL_STATIC_PSEUDOCOLOR";
                break;
            default:
                visual = "UNKNOWN";
        }
        printf("\t visual         %s\n", visual);
    }

    printf("\t xpanstep       %d\n", fix_scrinfo.xpanstep);
    printf("\t ypanstep       %d\n", fix_scrinfo.ypanstep);
    printf("\t ywrapstep      %d\n", fix_scrinfo.ywrapstep);
    printf("\t line_length    %d (byte)\n", fix_scrinfo.line_length);
    printf("\t mmio_start     0x%08X\n", fix_scrinfo.mmio_start);
    printf("\t mmio_len       %d\n", fix_scrinfo.mmio_len);
    printf("\t accel          %d\n", fix_scrinfo.accel);
}

/* RGB888 */
#define RED   0xFF0000
#define GREEN 0x00FF00
#define BLUE  0x0000FF

extern void print_var_scrinfo(int fd_fb);
extern void print_fix_scrinfo(int fd_fb);

#define CONV_5BIT_COLOR(_8bit_colr)  \
                            ((((unsigned int)(_8bit_colr))&0x000000FF) >> 3 )
                            
#define CONV_6BIT_COLOR(_8bit_colr)  \
                            ((((unsigned int)(_8bit_colr))&0x000000FF) >> 2 )

#define GET_RED(rgb888)     (((rgb888)&0x00FF0000) >> 16)
#define GET_GREEN(rgb888)   (((rgb888)&0x0000FF00) >> 8)
#define GET_BLUE(rgb888)     ((rgb888)&0x000000FF)

void set_pixel(unsigned char* fb, struct fb_var_screeninfo * scinfo, int x, int y, unsigned int rgb888)
{
    unsigned short rgb565;
    unsigned char* pixel;
    int BytePerPixel = scinfo->bits_per_pixel/8;

    rgb565 = (CONV_5BIT_COLOR(GET_RED(rgb888)) << 11) |
             (CONV_6BIT_COLOR(GET_GREEN(rgb888)) << 5)  |
              CONV_5BIT_COLOR(GET_BLUE(rgb888));

    pixel = fb + (x + y*scinfo->xres) * BytePerPixel;

    (*(unsigned short*)pixel) = rgb565;
}


/* create 16 bit 5/6/5 format pixel from RGB triplet */
#define RGB2PIXEL565(r,g,b)   \
   ((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3))

/* Draw horizontal line from x1,y to x2,y including final point*/
static void
linear16_drawhorzline(int x1, int x2, int y, unsigned short c)
{
   unsigned short   *addr = (unsigned short *)framebuf;

   assert (addr != 0);

   addr += x1 + y * gwidth;
   while(x1++ <= x2)
	   *addr++ = c;
}


/*
 * fixghc: 2001.02.17
 * device.h에 설정된 인자는 x, y, width, height, color 인데
 * 실제로 구현이 되어 있는 놈은 이상하게도
 * x1, y1, x2, y2로 구현이 되어 있다.
 */  
void gen_fillrect(  int x1, int y1, int x2, int y2, unsigned short c)
{
   while(y1 <= y2)
      linear16_drawhorzline( x1, x2, y1++, c);
}



// ghcstop: 2001.02.17
void gfx_draw_sized_rect(  int x1, int y1, int width, int height, unsigned short c)
{
  int x2 = x1+width-1;
  int y2 = y1+height-1;

  gen_fillrect(x1, y1, x2, y2, c);
}


int main(void)
{
    int ret, fd_fb;
    struct fb_fix_screeninfo fix_scrinfo;
    struct fb_var_screeninfo var_scrinfo;

    /* open video device */
    fd_fb = open("/dev/fb0", O_RDWR);
    if(fd_fb < 0) {
        perror("framebuffer open");
        exit(1);
    }

    /* get screen info */
    ret = ioctl(fd_fb, FBIOGET_VSCREENINFO, &var_scrinfo);
    if(ret < 0) {
        perror("framebuffer ioctl");
        exit(1);
    }
    
    gwidth  = var_scrinfo.xres;
    gheight = var_scrinfo.yres;
    
    ret = ioctl(fd_fb, FBIOGET_FSCREENINFO, &fix_scrinfo);
    if(ret < 0) {
        perror("framebuffer ioctl");
        exit(1);
    }
    
    print_var_scrinfo(fd_fb);
    print_fix_scrinfo(fd_fb);

    /* get frame buffer address */
    framebuf = (unsigned char*)mmap(0, 
    	fix_scrinfo.smem_len, PROT_READ|PROT_WRITE, MAP_SHARED, 
    	fd_fb, 0); 
    if(framebuf == 0) {
        perror("mmap");
        exit(1);
    }
    printf("frame buffer(memory mapped) : 0x%08X\n", framebuf);

    /* clear screen */
    memset(framebuf, 0x0, fix_scrinfo.smem_len);


	//green
	linear16_drawhorzline(100, 200, 200, RGB2PIXEL565(128, 128, 128) );
	sleep(5);

#if 0
	//red -> violet
	linear16_drawhorzline(100, 200, 200, RGB2PIXEL565(0, 255, 255) );
	sleep(2);

	//white
	linear16_drawhorzline(100, 200, 200, RGB2PIXEL565(0xFF, 0xFF, 0xFF) );
	sleep(2);

	//black
	linear16_drawhorzline(100, 200, 200, RGB2PIXEL565(0x0, 0x0, 0x0) );
	sleep(2);
#endif

	gfx_draw_sized_rect(0, 0, 800, 480, RGB2PIXEL565(128, 128, 128) );

    
    munmap(framebuf, fix_scrinfo.smem_len);
    
    close(fd_fb);

    return 0;
}
