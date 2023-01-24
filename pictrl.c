#include <ssd1306_i2c.h>
#include <linux/input.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "pi-logo.h"

//init input keyboard
int fd, bytes;
struct input_event data;
int fd2, bytes2;
struct input_event data2;

const char *pEncoder = "/dev/input/event1";
const char *pButton = "/dev/input/event0";
int keyp;

signed int selectorPosY = 0;
signed int selectorPosX = 0;
uint8_t page = 0;
uint8_t maxPages = 2;

char output[] = "888888888888888";
char cmd7[] = "hostname -I";
char* date; //e poi?

int initInput()
{

    // Open Keyboard
    fd = open(pEncoder, O_RDONLY | O_NONBLOCK);
    if (fd == -1)
    {
        printf("ERROR Opening %s\n", pEncoder);
        return -1;
    }
    fd2 = open(pButton, O_RDONLY | O_NONBLOCK);
    if (fd2 == -1)
    {
        printf("ERROR Opening %s\n", pButton);
        return -1;
    }
    return 0;
}

int runCmd(char * command)
{

    FILE *fp;

    // Open the command for reading. 
    fp = popen("hostname -I", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }
    // Read the output a line at a time - output it. 
    fgets(output, sizeof(output), fp);
    printf("output %s\n", output);

    // close 
    pclose(fp);

    return 0;
}

int main()
{
    const char *filename = "/dev/i2c-1";
    ssd1306_i2c_t *oled = ssd1306_i2c_open(filename, 0x3c, 128, 64, NULL);

    // fprintf(stderr, "DEBUG: Using library version: %s\n", ssd1306_i2c_version());
    if (!oled)
    {
        return -1;
    }
    if (ssd1306_i2c_display_initialize(oled) < 0)
    {
        fprintf(stderr, "ERROR: Failed to initialize the display. Check if it is connected !\n");
        ssd1306_i2c_close(oled);
        return -1;
    }
    ssd1306_framebuffer_t *fbp = ssd1306_framebuffer_create(oled->width, oled->height, oled->err);
    ssd1306_i2c_display_clear(oled);
    ssd1306_i2c_display_update(oled, fbp);
    ssd1306_framebuffer_box_t bbox;
    ssd1306_graphics_options_t opts[1];
    opts[0].type = SSD1306_OPT_FONT_FILE;
    //    opts[0].value.font_file = "/home/pi/.fonts/petme/PetMe.ttf";
    opts[0].value.font_file = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf";
    // for (uint8_t i = 0; i < 50; i++)
    // {
    //     ssd1306_framebuffer_put_pixel(fbp, rand() % 128, rand() % 64, true);
    //     //        ssd1306_framebuffer_bitdump(fbp);
    //     ssd1306_i2c_display_update(oled, fbp);
    // }

    // splash image
    unsigned int pixNum = 0;
    for (uint8_t i = 0; i < oled->height; ++i)
    {
        for (uint8_t j = 0; j < oled->width; ++j)
        {
            ssd1306_framebuffer_put_pixel(fbp, j, i, pixels[pixNum]);
            pixNum++;
            // printf("pix %d %d\n", pixNum, pixels[pixNum]);
        }
    }

    //     ssd1306_framebuffer_draw_text(fbp, "SASHA", 0, 5, 40, SSD1306_FONT_DEFAULT, 10, &bbox);
    //     // ssd1306_framebuffer_bitdump(fbp);
    ssd1306_i2c_display_update(oled, fbp);
    sleep(5);
    ssd1306_framebuffer_clear(fbp);
    if (page == 0){
        ssd1306_framebuffer_draw_text_extra(fbp, "Pd Patch", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
        ssd1306_framebuffer_draw_text_extra(fbp, "Pd kill", 0, 0, (oled->height / 4 * 2) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
        ssd1306_framebuffer_draw_text_extra(fbp, "Restart", 0, 0, (oled->height / 4 * 3) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
        ssd1306_framebuffer_draw_text_extra(fbp, "Power Off", 0, 0, (oled->height / 4 * 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
    }
    for (uint8_t i = 0; i < oled->width; ++i)
    {
        for (uint8_t j = 0 + ((selectorPosY)%4 * 16); j < ((selectorPosY)%4 * 16)+16; ++j)
        {
            ssd1306_framebuffer_invert_pixel(fbp, i, j);
        }
    }
    ssd1306_i2c_display_update(oled, fbp);
    // ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_POWER_OFF, 0, 0);
    initInput();
    while (1)
    {
        // keyp = getchar();
        // printf("key press: %c\n", ch);
        usleep(10000);
        // Read Keyboard Data
        bytes = read(fd, &data, sizeof(data));
        if (bytes > 0)
        {
            // printf("Keypress  value=%d, type=%d, code=%d\n", data.value, data.type, data.code);
            if (data.value == -1)
            {
                // printf("Encoder decr.\n");
                ssd1306_framebuffer_clear(fbp);
                selectorPosY--;
                if(selectorPosY < 0)selectorPosY = 7;
                if(selectorPosY > 7)selectorPosY = 0;
                if(selectorPosY < 4)page = 0;
                if(selectorPosY > 3)page = 1;
                // selectorPosY += data.value % oled->height;
                // ssd1306_framebuffer_draw_line(fbp, 0, selectorPosY, 127, selectorPosY, true);
                if (page == 0){
                    ssd1306_framebuffer_draw_text_extra(fbp, "Pd Patch", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Pd kill", 0, 0, (oled->height / 4 * 2) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Restart", 0, 0, (oled->height / 4 * 3) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Power Off", 0, 0, (oled->height / 4 * 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    //                    ssd1306_framebuffer_draw_text_extra(fbp, "_______________<", 0, 0, (oled->height / 4 * (selectorPosY+1)) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                }
                if (page == 1){
                    ssd1306_framebuffer_draw_text_extra(fbp, "Start VNC", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Kill VNC", 0, 0, (oled->height / 4 * 2) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Show IP", 0, 0, (oled->height / 4 * 3) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Item 8", 0, 0, (oled->height / 4 * 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    //                    ssd1306_framebuffer_draw_text_extra(fbp, "_______________<", 0, 0, (oled->height / 4 * (selectorPosY+1-4)) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                }
                for (uint8_t i = 0; i < oled->width; ++i)
                {
                    for (uint8_t j = 0 + ((selectorPosY)%4 * 16); j < ((selectorPosY)%4 * 16)+16; ++j)
                    {
                        ssd1306_framebuffer_invert_pixel(fbp, i, j);
                    }
                }
                ssd1306_i2c_display_update(oled, fbp);
            }
            if (data.value == 1)
            {
                // printf("Encoder incr.\n");
                ssd1306_framebuffer_clear(fbp);
                selectorPosY++;
                if(selectorPosY < 0)selectorPosY = 7;
                if(selectorPosY > 7)selectorPosY = 0;
                if(selectorPosY < 4)page = 0;
                if(selectorPosY > 3)page = 1;
                // selectorPosY += data.value % oled->height;
                // ssd1306_framebuffer_draw_line(fbp, 0, selectorPosY, 127, selectorPosY, true);
                if (page == 0){
                    ssd1306_framebuffer_draw_text_extra(fbp, "Pd Patch", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Pd kill", 0, 0, (oled->height / 4 * 2) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Restart", 0, 0, (oled->height / 4 * 3) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Power Off", 0, 0, (oled->height / 4 * 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    //                    ssd1306_framebuffer_draw_text_extra(fbp, "_______________<", 0, 0, (oled->height / 4 * (selectorPosY+1)) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                }
                if (page == 1){
                    ssd1306_framebuffer_draw_text_extra(fbp, "Start VNC", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Kill VNC", 0, 0, (oled->height / 4 * 2) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Show IP", 0, 0, (oled->height / 4 * 3) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Item 8", 0, 0, (oled->height / 4 * 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    //                    ssd1306_framebuffer_draw_text_extra(fbp, "_______________<", 0, 0, (oled->height / 4 * (selectorPosY+1-4)) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                }
                for (uint8_t i = 0; i < oled->width; ++i)
                {
                    for (uint8_t j = 0 + ((selectorPosY)%4 * 16); j < ((selectorPosY)%4 * 16)+16; ++j)
                    {
                        ssd1306_framebuffer_invert_pixel(fbp, i, j);
                    }
                }
                ssd1306_i2c_display_update(oled, fbp);
            }
        }
        // Read Keyboard Data2
        bytes2 = read(fd2, &data2, sizeof(data2));
        if (bytes2 > 0)
        {
            // printf("Keypress2 value=%d, type=%d, code=%d\n", data2.value, data2.type, data2.code);
            if (data2.value == 1)
            {
                printf("Button press\n");
                if(selectorPosY == 0){
                    ssd1306_framebuffer_clear(fbp);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Wait...", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_i2c_display_update(oled, fbp);
                    system("pd -nogui -open ~/Documents/Pd/weivtest_theremin.pd &");
                    ssd1306_framebuffer_clear(fbp);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Program Started", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_i2c_display_update(oled, fbp);
               }
                if(selectorPosY == 1){
                    ssd1306_framebuffer_clear(fbp);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Wait...", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_i2c_display_update(oled, fbp);
                    system("pkill pd");
                    ssd1306_framebuffer_clear(fbp);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Program Killed", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_i2c_display_update(oled, fbp);
                }
               if(selectorPosY == 2){
                    ssd1306_framebuffer_clear(fbp);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Restarting...", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_i2c_display_update(oled, fbp);
                    ssd1306_framebuffer_destroy(fbp);
                    fbp = NULL;
                    ssd1306_i2c_close(oled);
                    oled = NULL;
                    system("sudo systemctl reboot");
                }
                if(selectorPosY == 3){
                    ssd1306_framebuffer_clear(fbp);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Poweroff", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_i2c_display_update(oled, fbp);
                    ssd1306_framebuffer_destroy(fbp);
                    fbp = NULL;
                    ssd1306_i2c_close(oled);
                    oled = NULL;
                    system("sudo systemctl poweroff");
                }
                if(selectorPosY == 4){
                    ssd1306_framebuffer_clear(fbp);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Wait...", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_i2c_display_update(oled, fbp);
                    system("vncserver -localhost no -geometry 1280x600 -SecurityTypes=None :0 --I-KNOW-THIS-IS-INSECURE");
                    ssd1306_framebuffer_clear(fbp);
                    ssd1306_framebuffer_draw_text_extra(fbp, "VNC Started", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_i2c_display_update(oled, fbp);
               }
                if(selectorPosY == 5){
                    ssd1306_framebuffer_clear(fbp);
                    ssd1306_framebuffer_draw_text_extra(fbp, "Wait...", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_i2c_display_update(oled, fbp);
                    system("vncserver -kill :0");
                    ssd1306_framebuffer_clear(fbp);
                    ssd1306_framebuffer_draw_text_extra(fbp, "VNC Killed", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_i2c_display_update(oled, fbp);
                }
                if(selectorPosY == 6){
                    runCmd(cmd7);
                    ssd1306_framebuffer_clear(fbp);
                    ssd1306_framebuffer_draw_text_extra(fbp, output, 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                    ssd1306_i2c_display_update(oled, fbp);
                    // sleep(3);
                }
            }
            else
            {
                // ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_DISP_NORMAL, 0, 0);
            }
        }
        else
        {
        }
    }

    ssd1306_framebuffer_destroy(fbp);
    fbp = NULL;
    ssd1306_i2c_close(oled);
    oled = NULL;
    return 0;
}
