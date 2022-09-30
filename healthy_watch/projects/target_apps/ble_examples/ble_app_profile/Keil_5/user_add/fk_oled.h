#ifndef _FK_OLED_H_
#define _FK_OLED_H_

#define Command 0
#define Parameter 1

enum {
	step_display=0,
	bp_display,
	bo_display,
	bs_display,
	cal_display,
	hr_display
};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Initialization
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLED_Init(void);

void oled_display(unsigned char count);

void Write(unsigned char a,unsigned char b) ;

void set_position(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);

void draw_picture(unsigned char *buf,unsigned char length,unsigned char width);

void draw_picture1(unsigned char *buf,unsigned int length,unsigned char color_low,unsigned char color_high);

void clean_screen(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);

void show_number1616(unsigned char x,unsigned char y,unsigned char *buf,unsigned char number,unsigned char color_low,unsigned char color_high);
#endif
