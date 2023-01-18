#include <ssd1306_i2c.h>
#include <linux/input.h>
#include <fcntl.h>
#include <stdlib.h>

//init input keyboard
int fd, bytes;
struct input_event data;
int fd2, bytes2;
struct input_event data2;

const char *pEncoder = "/dev/input/event1";
const char *pButton = "/dev/input/event0";

signed int selectorPosY = 4;
signed int selectorPosX = 0;
uint8_t page = 0;

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
    for (uint8_t i = oled->height - 1; i < oled->height; ++i)
    {
        for (uint8_t j = 0; j < oled->width; ++j)
        {
            ssd1306_framebuffer_put_pixel(fbp, j, i, true);
        }
    }
    // ssd1306_framebuffer_hexdump(fbp);
    ssd1306_i2c_display_update(oled, fbp);
    // ssd1306_framebuffer_bitdump(fbp);
    // sleep(1);
    ssd1306_framebuffer_box_t bbox;
    ssd1306_graphics_options_t opts[1];
    opts[0].type = SSD1306_OPT_FONT_FILE;
    //    opts[0].value.font_file = "/home/pi/.fonts/petme/PetMe.ttf";
    opts[0].value.font_file = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf";

    // #ifdef LIBSSD1306_HAVE_UNISTR_H
    //     ssd1306_framebuffer_draw_text(fbp, "sasha", 0, 5, 40, SSD1306_FONT_DEFAULT, 10, &bbox);
    // #else
    //     ssd1306_framebuffer_draw_text(fbp, "ABCDeF", 0, 32, 16, SSD1306_FONT_DEFAULT, 4, &bbox);
    // #endif
    //     // ssd1306_framebuffer_bitdump(fbp);
    //     ssd1306_i2c_display_update(oled, fbp);
    //     sleep(1);
    //     ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_DISP_INVERTED, 0, 0);
    //     sleep(1);
    //     ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_DISP_NORMAL, 0, 0);
    //     sleep(1);
    //     ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_DISP_INVERTED, 0, 0);
    //     sleep(1);
    //     ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_DISP_NORMAL, 0, 0);
    // //    ssd1306_framebuffer_clear(fbp);
    for (int i = 0; i < 100; i++)
    {
        ssd1306_framebuffer_put_pixel(fbp, rand() % 128, rand() % 64, true);
        //        ssd1306_framebuffer_bitdump(fbp);
        ssd1306_i2c_display_update(oled, fbp);
    }
    sleep(1);
    ssd1306_framebuffer_clear(fbp);
    //     ssd1306_framebuffer_draw_text(fbp, "SASHA", 0, 5, 40, SSD1306_FONT_DEFAULT, 10, &bbox);
    //     // ssd1306_framebuffer_bitdump(fbp);
    //     ssd1306_i2c_display_update(oled, fbp);
    ssd1306_framebuffer_clear(fbp);
    ssd1306_framebuffer_draw_text_extra(fbp, "Pd Patch", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
    ssd1306_framebuffer_draw_text_extra(fbp, "Pd kill", 0, 0, (oled->height / 4 * 2) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
    ssd1306_framebuffer_draw_text_extra(fbp, "Restart", 0, 0, (oled->height / 4 * 3) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
    ssd1306_framebuffer_draw_text_extra(fbp, "Power Off", 0, 0, (oled->height / 4 * 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
    //    ssd1306_framebuffer_clear(fbp);
    //ssd1306_i2c_display_clear(oled);

    // for (int i = 0; i < 20; i++)
    // {
    //     ssd1306_framebuffer_draw_line(fbp, rand() % 128, rand() % 64, rand() % 128, rand() % 64, true);
    //     ssd1306_i2c_display_update(oled, fbp);
    // }
    // for (int i = 0; i < 3; i++)
    // {
    //     ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_DISP_INVERTED, 0, 0);
    //     sleep(1);
    //     ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_DISP_NORMAL, 0, 0);
    //     sleep(1);
    // }

    // ssd1306_i2c_display_clear(oled);
    //    ssd1306_framebuffer_bitdump(fbp);
    ssd1306_i2c_display_update(oled, fbp);
    // ssd1306_i2c_run_cmd(oled, SSD1306_I2C_CMD_POWER_OFF, 0, 0);
    initInput();
    while (1)
    {
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
                if(selectorPosY < 0)selectorPosY = 3;
                if(selectorPosY > 3)selectorPosY = 0;
                // selectorPosY += data.value % oled->height;
                // ssd1306_framebuffer_draw_line(fbp, 0, selectorPosY, 127, selectorPosY, true);
                ssd1306_framebuffer_draw_text_extra(fbp, "Pd Patch", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                ssd1306_framebuffer_draw_text_extra(fbp, "Pd kill", 0, 0, (oled->height / 4 * 2) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                ssd1306_framebuffer_draw_text_extra(fbp, "Restart", 0, 0, (oled->height / 4 * 3) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                ssd1306_framebuffer_draw_text_extra(fbp, "Power Off", 0, 0, (oled->height / 4 * 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                ssd1306_framebuffer_draw_text_extra(fbp, "_______________<", 0, 0, (oled->height / 4 * (selectorPosY+1)) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                //                for (uint8_t i = oled->height - 1; i < oled->height; ++i)
                //                {
                //                    for (uint8_t j = 0; j < oled->width; ++j)
                //                    {
                //                        ssd1306_framebuffer_invert_pixel(fbp, j, i);
                //                    }
                //                }
                ssd1306_i2c_display_update(oled, fbp);
            }
            if (data.value == 1)
            {
                // printf("Encoder incr.\n");
                ssd1306_framebuffer_clear(fbp);
                selectorPosY++;
                if(selectorPosY < 0)selectorPosY = 3;
                if(selectorPosY > 3)selectorPosY = 0;
                // selectorPosY += data.value % oled->height;
                // ssd1306_framebuffer_draw_line(fbp, 0, selectorPosY, 127, selectorPosY, true);
                ssd1306_framebuffer_draw_text_extra(fbp, "Pd Patch", 0, 0, (oled->height / 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                ssd1306_framebuffer_draw_text_extra(fbp, "Pd kill", 0, 0, (oled->height / 4 * 2) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                ssd1306_framebuffer_draw_text_extra(fbp, "Restart", 0, 0, (oled->height / 4 * 3) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                ssd1306_framebuffer_draw_text_extra(fbp, "Power Off", 0, 0, (oled->height / 4 * 4) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                ssd1306_framebuffer_draw_text_extra(fbp, "_______________<", 0, 0, (oled->height / 4 * (selectorPosY+1)) - 4, SSD1306_FONT_CUSTOM, 3, opts, 1, &bbox);
                //                for (uint8_t i = oled->height / 4 - 1; i < oled->height / 4; ++i)
                //                {
                //                    for (uint8_t j = 0; j < oled->width; ++j)
                //                    {
                //                        ssd1306_framebuffer_invert_pixel(fbp, j, i);
                //                    }
                //                }
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
                if(selectorPosY == 0) system("pd -nogui -open ~/Documents/Pd/weivtest_theremin.pd &");
                if(selectorPosY == 1) system("pkill pd");
                if(selectorPosY == 2){
                    ssd1306_framebuffer_clear(fbp);
                    ssd1306_framebuffer_destroy(fbp);
                    fbp = NULL;
                    ssd1306_i2c_close(oled);
                    oled = NULL;
                    system("sudo systemctl reboot");
                }
                if(selectorPosY == 3){
                    ssd1306_framebuffer_clear(fbp);
                    ssd1306_framebuffer_destroy(fbp);
                    fbp = NULL;
                    ssd1306_i2c_close(oled);
                    oled = NULL;
                    system("sudo systemctl poweroff");
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
