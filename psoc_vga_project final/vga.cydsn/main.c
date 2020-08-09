#include <device.h>
#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

////////////////////////////////////////////////////////
///// enums                                        /////
////////////////////////////////////////////////////////
enum controller_dir {up, down, left, right, center};

////////////////////////////////////////////////////////
///// type defs                                    /////
////////////////////////////////////////////////////////
typedef struct coordTuples {
    int row;
    int col;
} coordTuple;

////////////////////////////////////////////////////////
///// function declarations                        /////
////////////////////////////////////////////////////////
void update();
void rect(int x_1, int y_1, int x_2, int y_2, uint color);
void generateWalls();
void generateDots();
void generatePacman(int center_row, int center_col, enum controller_dir dir);
void removePacman(int center_row, int center_col);
bool isValidCoord(int center_row, int center_col);
enum controller_dir getControllerDir();
void movePacman(int center_row, int center_col, enum controller_dir dir);
bool uneatenDot(int row, int col);

////////////////////////////////////////////////////////
///// constants                                    /////
////////////////////////////////////////////////////////
const uint8 Colors_BLACK = 0; // 00 00 00b
const uint8 Colors_RED = 3; // 00 00 11b
const uint8 Colors_GREEN = 12; // 00 11 00b
const uint8 Colors_BLUE = 48; // 11 00 00b
const uint8 Colors_WHITE = 63; // 11 11 11b
const uint8 Colors_YELLOW = 47; // from testing

////////////////////////////////////////////////////////
///// defines                                      /////
////////////////////////////////////////////////////////
#define numDots 244 //number of dots in level 1

////////////////////////////////////////////////////////
///// global vars                                  /////
////////////////////////////////////////////////////////
int pacman_center_x = 63; //initial pacman starting x
int pacman_center_y = 64; //initial pacman starting y
coordTuple dotsCoords[numDots]; //keeps track of coords of all dots
bool isEaten[numDots]; //keeps track whether they are eaten (T) or not (F)
uint8 score = 0; //score (number of dots consumed by pacman)

// Video buffer gets its own SRAM to prevent bus contention.
// The user-facing buffer is in regular system RAM.
// This is set in the custom linker script (custom.ld).
uint8 buf[96][128], vbuf[96][128] __attribute__ ((section(".vram")));

// Now we set up the DMA to copy pixels from vbuf to the screen.
// For timing, we rely on the fact that it takes the DMA exactly
// 8 clocks to move each byte. After each line the DMA is updated
// to point to the next line in vbuf.
uint8 dma_chan, dma_td;
volatile int flag = 1;
CY_ISR(newline) {
    uint16 line = 805 - VERT_ReadCounter();
    if (line % 8 == 0) {
        if (line < 768) {
            CY_SET_REG16(CY_DMA_TDMEM_STRUCT_PTR[dma_td].TD1,
                         LO16((uint32) vbuf[line / 8]));
        } else if (line == 768 && flag) {  // refresh the buffer during vsync
            CyDmaChDisable(dma_chan);
            if (flag) {
                memcpy(*vbuf, *buf, 96 * 128);
                flag = 0;
            }
            CyDmaChEnable(dma_chan, 1);
        }
    }
}


////////////////////////////////////////////////////////
///// functios                                     /////
////////////////////////////////////////////////////////
inline void update() {
    flag = 1;
    while (flag);
}

void rect(int x1, int y1, int x2, int y2, uint color) {
    //Given two diagonal coords of a rectangle (s.t. coord (x1,y1) is
    //more upper left than (x2,y2)), makes a rectangle that has color.
    //Coords are inclusive.
    int i = 0;
    int j = 0;
    for (i = x1; i <= x2; ++i) {
        for (j = y1; j <= y2; ++j) {
            buf[i][j] = color;
        }
    }
}

void generateWalls() {
    //Generates the static walls of level 1 of Pac-Man
    //OUTER WALLS
    //top wall
    rect(15, 35, 17, 93, Colors_BLUE);
    rect(18, 63, 25, 65, Colors_BLUE);
    //left wall
    rect(18, 35, 34, 37, Colors_BLUE);
    rect(35, 35, 37, 47, Colors_BLUE);
    rect(38, 45, 40, 47, Colors_BLUE);
    rect(41, 38, 43, 47, Colors_BLUE);
    rect(47, 38, 49, 47, Colors_BLUE);
    rect(50, 45, 52, 47, Colors_BLUE);
    rect(53, 35, 55, 47, Colors_BLUE);
    rect(56, 35, 76, 37, Colors_BLUE);
    rect(65, 38, 67, 41, Colors_BLUE);
    //right wall
    rect(18, 91, 34, 93, Colors_BLUE);
    rect(35, 81, 37, 93, Colors_BLUE);
    rect(38, 81, 40, 83, Colors_BLUE);
    rect(41, 81, 43, 90, Colors_BLUE);
    rect(47, 81, 49, 90, Colors_BLUE);
    rect(50, 81, 52, 83, Colors_BLUE);
    rect(53, 81, 55, 93, Colors_BLUE);
    rect(56, 91, 76, 93, Colors_BLUE);
    rect(65, 87, 67, 90, Colors_BLUE);
    //bottom wall
    rect(77, 35, 79, 93, Colors_BLUE);
    //INNER OBSTACLES
    rect(21, 41, 25, 47, Colors_BLUE);
    rect(21, 51, 25, 59, Colors_BLUE);
    rect(21, 69, 25, 77, Colors_BLUE);
    rect(21, 81, 25, 87, Colors_BLUE);
    rect(29, 57, 31, 71, Colors_BLUE);
    rect(32, 63, 37, 65, Colors_BLUE);
    rect(29, 41, 31, 47, Colors_BLUE);
    rect(29, 81, 31, 87, Colors_BLUE);
    rect(29, 51, 43, 53, Colors_BLUE);
    rect(35, 54, 37, 59, Colors_BLUE);
    rect(29, 75, 43, 77, Colors_BLUE);
    rect(35, 69, 37, 74, Colors_BLUE);
    rect(47, 51, 55, 53, Colors_BLUE);
    rect(47, 75, 55, 77, Colors_BLUE);
    rect(53, 57, 55, 71, Colors_BLUE);
    rect(56, 63, 61, 65, Colors_BLUE);
    rect(59, 51, 61, 59, Colors_BLUE);
    rect(59, 69, 61, 77, Colors_BLUE);
    rect(59, 41, 61, 47, Colors_BLUE);
    rect(59, 81, 61, 87, Colors_BLUE);
    rect(62, 45, 67, 47, Colors_BLUE);
    rect(62, 81, 67, 83, Colors_BLUE);
    rect(65, 57, 67, 71, Colors_BLUE);
    rect(68, 63, 73, 65, Colors_BLUE);
    rect(65, 51, 70, 53, Colors_BLUE);
    rect(71, 41, 73, 59, Colors_BLUE);
    rect(65, 75, 70, 77, Colors_BLUE);
    rect(71, 69, 73, 87, Colors_BLUE);
    //GHOST PEN
    rect(41, 57, 42, 61, Colors_BLUE);
    rect(43, 57, 47, 58, Colors_BLUE);
    rect(48, 57, 49, 71, Colors_BLUE);
    rect(41, 67, 42, 71, Colors_BLUE);
    rect(43, 70, 47, 71, Colors_BLUE);
    rect(41, 62, 42, 66, Colors_WHITE);
}

void generatePacman(int center_row, int center_col, enum controller_dir dir) {
    //Generates a pacman given the center x and y
    //coords.
    //make pacman cross
    buf[center_row][center_col] = Colors_YELLOW;
    buf[center_row-1][center_col-1] = Colors_YELLOW;
    buf[center_row-1][center_col+1] = Colors_YELLOW;
    buf[center_row+1][center_col-1] = Colors_YELLOW;
    buf[center_row+1][center_col+1] = Colors_YELLOW;
    if (dir == left) {
        buf[center_row][center_col+1] = Colors_YELLOW;
        buf[center_row-1][center_col] = Colors_YELLOW;
        buf[center_row+1][center_col] = Colors_YELLOW;
    }
    else if (dir == right) {
        buf[center_row][center_col-1] = Colors_YELLOW;
        buf[center_row-1][center_col] = Colors_YELLOW;
        buf[center_row+1][center_col] = Colors_YELLOW;
    }
    else if (dir == up) {
        buf[center_row][center_col-1] = Colors_YELLOW;
        buf[center_row][center_col+1] = Colors_YELLOW;
        buf[center_row-1][center_col] = Colors_YELLOW;
    }
    else if (dir == down) {
        buf[center_row][center_col-1] = Colors_YELLOW;
        buf[center_row][center_col+1] = Colors_YELLOW;
        buf[center_row+1][center_col] = Colors_YELLOW;
    }
    else {
        buf[center_row][center_col] = Colors_YELLOW;
        buf[center_row][center_col-1] = Colors_YELLOW;
        buf[center_row][center_col+1] = Colors_YELLOW;
        buf[center_row-1][center_col] = Colors_YELLOW;
        buf[center_row+1][center_col] = Colors_YELLOW;
    }
}

void removePacman(int center_row, int center_col) {
    //Removes a pacman given the center x and y
    //coords.
    rect(center_row-1, center_col-1, center_row+1, center_col+1, Colors_BLACK);
}

bool isValidCoord(int center_row, int center_col) {
    //Given center_x and center_y coords,
    //determines if they are valid
    //trivial cases (outside of top, bot, left, right walls)
    if (center_row < 19) //top
        return false;
    if (center_row > 75) //bot
        return false;
    if (center_col < 39) //left
        return false;
    if (center_col > 89) //right
        return false;
    //horizontal bars
    if (center_row == 19) {
        if (center_col < 62 || center_col > 66)
            return true;
        return false;
    }
    else if (center_row == 27) {
        return true;
    }
    else if (center_row == 33) {
        if (center_col < 50 || center_col > 78)
            return true;
        else if (center_col > 54 && center_col < 62)
            return true;
        else if (center_col > 66 && center_col < 74)
            return true;
        return false;
    }
    else if (center_row == 39) {
        if (center_col == 49 || center_col == 79)
            return true;
        else if (center_col > 54 && center_col < 74)
            return true;
        return false;
    }
    else if (center_row == 45) {
        if (center_col < 56 || center_col > 72)
            return true;
        return false;
    }
    else if (center_row == 51) {
        if (center_col == 49 || center_col == 79)
            return true;
        else if (center_col > 54 && center_col < 74)
            return true;
        return false;
    }
    else if (center_row == 57) {
        if (center_col < 62 || center_col > 66)
            return true;
        return false;
    }
    else if (center_row == 63) {
        if (center_col < 44 || center_col > 84)
            return true;
        else if (center_col > 48 && center_col < 80)
            return true;
        return false;
    }
    else if (center_row == 69) {
        if (center_col < 50 || center_col > 78)
            return true;
        else if (center_col > 54 && center_col < 62)
            return true;
        else if (center_col > 66 && center_col < 74)
            return true;
        return false;
    }
    else if (center_row == 75) {
        return true;
    }
    //vertical bars
    if (center_col == 39 || center_col==89) {
        if (center_row < 34 || center_row > 68)
            return true;
        else if (center_row > 56 && center_row < 64)
            return true;
        return false;
    }
    else if (center_col==43 || center_col==85) {
        if (center_row > 62 && center_row < 70)
            return true;
        return false;
    }
    else if (center_col==49 || center_col==79) {
        if (center_row < 70)
            return true;
        return false;
    }
    else if (center_col==55 || center_col==73) {
        if (center_row > 26 && center_row < 34)
            return true;
        else if (center_row > 38 && center_row < 58)
            return true;
        else if (center_row > 62 && center_row < 70)
            return true;
        return false;
    }
    else if (center_col==61 || center_col==67) {
        if (center_row < 28 || center_row > 68)
            return true;
        else if (center_row > 32 && center_row < 40)
            return true;
        else if (center_row > 56 && center_row < 64)
            return true;
        return false;
    }
    //invalid coord
    return false;
}

enum controller_dir getControllerDir() {
    //Reads DACs to get the direction the controller is in.
    enum controller_dir dir;
    int16 adcx;
    int16 adcy;
    //Joystick x
    if( ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT) )
	{
		adcx = ADC_SAR_1_GetResult16();		// read the adc and assign the value adcResult 
	}
    if (adcx <= 60) {
        dir = left;
        return dir;
    }
    else if (adcx >= 200) {
        dir = right;
        return dir;
    }
    //Joystick y
    if( ADC_SAR_2_IsEndConversion(ADC_SAR_2_WAIT_FOR_RESULT) )
	{
		adcy = ADC_SAR_2_GetResult16();		// read the adc and assign the value adcResult 
	}
    if (adcy <= 60) {
        dir = down;
        return dir;
    }
    else if (adcy >= 200) {
        dir = up;
        return dir;
    }
    //centered
    dir = center;
    return dir;
}

void movePacman(int center_row, int center_col, enum controller_dir dir) {
    //Given Pac-Man old coords and a controller direction, move Pac-Man
    //by 1 pixel in controller direction if it's valid.
    //In the same process, check if Pacman's new location is a valid dot
    //able to be consumed
    if (dir == left) {
        if (center_row == 45 && center_col == 39) { //teleportation l->r
            pacman_center_y = 89;
            removePacman(center_row, center_col);
            generatePacman(pacman_center_x, pacman_center_y, left);
        }
        else if (isValidCoord(center_row, center_col-1)) {
            //move pacman
            pacman_center_y -= 1;
            removePacman(center_row, center_col);
            generatePacman(center_row, center_col-1, left);
        }
    }
    else if (dir == right) {
        if (center_row == 45 && center_col == 89) { //teleportation r->l
            pacman_center_y = 39;
            removePacman(center_row, center_col);
            generatePacman(pacman_center_x, pacman_center_y, right);
        }
        else if (isValidCoord(center_row, center_col+1)) {
            //move pacman
            pacman_center_y += 1;
            removePacman(center_row, center_col);
            generatePacman(center_row, center_col+1, right);
        }
    }
    else if (dir == up) {
        if (isValidCoord(center_row+1, center_col)) {
            //move pacman
            pacman_center_x += 1;
            removePacman(center_row, center_col);
            generatePacman(center_row+1, center_col, up);
        }
    }
    else if (dir == down) {
        if (isValidCoord(center_row-1, center_col)) {
            //move pacman
            pacman_center_x -= 1;
            removePacman(center_row, center_col);
            generatePacman(center_row-1, center_col, down);
        }
    }
    //check for dots
    if (uneatenDot(pacman_center_x, pacman_center_y)) {
        score+=1;
    }
}

void generateDots() {
    //Generates inital dots
    int row;
    int col;
    int inx = 0;
    coordTuple tmp;
    //top half
    for (row=19; row<34; row+=2) {
        for (col=39; col<90; col+=2) {
            if (isValidCoord(row, col)) {
                buf[row][col] = Colors_WHITE;
                tmp.row = row;
                tmp.col = col;
                dotsCoords[inx] = tmp;
                inx+=1;
            }
        }
    }
    //bottom half
    for (row=57; row<76; row+=2) {
        for (col=39; col<90; col+=2) {
            if (isValidCoord(row, col)) {
                if ((row!=63) || (col<62||col>66)) { //no dots on starting pacman pos
                    buf[row][col] = Colors_WHITE;
                    tmp.row = row;
                    tmp.col = col;
                    dotsCoords[inx] = tmp;
                    inx+=1;
                }
            }
        }
    }
    //left col
    for (row=35; row<56; row+=2) {
        buf[row][49] = Colors_WHITE;
        tmp.row = row;
        tmp.col = col;
        dotsCoords[inx] = tmp;
        inx+=1;
    }
    //right col
    for (row=35; row<56; row+=2) {
        buf[row][79] = Colors_WHITE;
        tmp.row = row;
        tmp.col = col;
        dotsCoords[inx] = tmp;
        inx+=1;
    }
}

bool uneatenDot(int row, int col) {
    //Returns True if dot is uneaten and changes it to eaten
    //and False if dot has been eaten
    int i;
    for (i=0; i<244; i++) {
        if (dotsCoords[i].row==row && dotsCoords[i].col==col && !isEaten[i]) {
            isEaten[i] = true;
            return true;
        }
    }
    return false; //not found
}

////////////////////////////////////////////////////////
///// Main                                         /////
////////////////////////////////////////////////////////
void main() {
    CyGlobalIntEnable;
    
    //UART
    UART_Start();
    
    //ADC
    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert();
    ADC_SAR_2_Start();
    ADC_SAR_2_StartConvert();
    
    // Initialize the DMA.
    dma_td = CyDmaTdAllocate();
    dma_chan = DMA_DmaInitialize(1, 0, HI16(CYDEV_SRAM_BASE), HI16(CYDEV_PERIPH_BASE));
    CyDmaTdSetConfiguration(dma_td, 128, dma_td, DMA__TD_TERMOUT_EN | TD_INC_SRC_ADR);
    CyDmaTdSetAddress(dma_td, 0, LO16((uint32) PIXEL_Control_PTR));
    CyDmaChSetInitialTd(dma_chan, dma_td);
    CyDmaChEnable(dma_chan, 1);
    
    // Start all of the timing counters.
    HORIZ_Start();
    VERT_Start();
    HSYNC_Start();
    VSYNC_Start();
    NEWLINE_StartEx(newline);
    
    //WALL GENERATE
    generateWalls();
    generateDots();
    generatePacman(pacman_center_x, pacman_center_y, center);
    update();
    
    for(;;) {
        //move pacman
        enum controller_dir dir = getControllerDir();
        movePacman(pacman_center_x, pacman_center_y, dir);
        
        //transmit score (UART)
        UART_PutChar(score); //low byte

        //misc.
        update();
        CyDelay(55);  // can't check UART too quickly or bus saturation causes graphics blips
    }
}

