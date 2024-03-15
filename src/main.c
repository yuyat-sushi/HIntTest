
// *****************************************************************************
//  Scaling Example
//
//  Written in 2021 by Andreas Dietrich
// *****************************************************************************

// -----------------------------------------------------------------------------
//  Includes
// -----------------------------------------------------------------------------

// SGDK
#include <genesis.h>

// -------------------------------------------------------------------------
//  Interrupt handlers
// -------------------------------------------------------------------------

u16 beforeJoypad = 0;

bool toggle_subwindow = true;
bool toggle_mainwindow = true;

static vu8 lineDisplay   = 0;             // line position on display screen
static vu8 lineWindow    = 3;
static vu8 lineWindow2   = 3;
HINTERRUPT_CALLBACK HIntHandler_DW()
{
    register u8 window;
    register u8 blue;

    // Count raster lines
    lineDisplay++;

    // Pallet Change
    if(lineDisplay > 159){
        window = (lineDisplay & 15);
        if(!window){
            lineWindow++;
        }
        blue = lineWindow;
        if(lineWindow > 4){
            //前の色のディザリング
            if((window == 1)||(window == 4)){
                blue--;
            }
        }
        if(lineWindow < 7){
            //次の色のディザリング
            if((window == 11)||(window == 14)){
                blue++;
            }
        }
        PAL_setColor(0, blue << 9);
        return;
    }else if(lineDisplay > 127){
        window = (lineDisplay & 7);
        if(!window){
            lineWindow2++;
        }
        blue = lineWindow2;
        if(lineWindow2 > 4){
            //前の色のディザリング
            if(window == 1){
                blue--;
            }
        }
        if(lineWindow2 < 7){
            //次の色のディザリング
            if(window == 6){
                blue++;
            }
        }
        PAL_setColor(0, blue << 9);
        return;
    }
}

HINTERRUPT_CALLBACK HIntHandler_SW()
{
    register u8 window;
    register u8 blue;

    // Count raster lines
    lineDisplay++;

    // Pallet Change
    if(lineDisplay > 159){
        window = (lineDisplay & 15);
        if(!window){
            lineWindow++;
        }
        blue = lineWindow;
        if(lineWindow > 4){
            //前の色のディザリング
            if((window == 1)||(window == 4)){
                blue--;
            }
        }
        if(lineWindow < 7){
            //次の色のディザリング
            if((window == 11)||(window == 14)){
                blue++;
            }
        }
        PAL_setColor(0, blue << 9);
        return;
    }
}

void VBlankHandler()
{
    // Reset to line 0
    lineDisplay = 0;
    lineWindow  = 3;
    lineWindow2 = 3;
    PAL_setColor(0, RGB3_3_3_TO_VDPCOLOR(0, 0, 0));
}

/**
 * \brief 
 *      押した瞬間を受け取るジョイパッド入力
 * \param joy
 *      使用するコントローラ入力 JOY_1
 * \return u16 joypad 
 */
u16 Once_readJoypad(u16 joy){
        JOY_update();
	u16 joypad = JOY_readJoypad(joy);
        if(joypad == beforeJoypad){
                joypad = 0;
        }
        beforeJoypad = JOY_readJoypad(joy);
        return joypad;
}

// *****************************************************************************
//
//  Main
//
// *****************************************************************************

int main()
{
    //
    // Initalization
    //

    // Setup VDP
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    PAL_setColors(0, palette_black, 64, CPU);

    // Setup interrupt handlers
    SYS_disableInts();
    {
        SYS_setVBlankCallback(VBlankHandler);
        SYS_setHIntCallback(HIntHandler_DW);
        VDP_setHIntCounter(0);
        VDP_setHInterrupt(1);
    }
    SYS_enableInts();

    //
    // Display loop
    //

    while (TRUE)
    {
        SYS_doVBlankProcess();
        //toggle
        if(Once_readJoypad(JOY_1)){
            toggle_subwindow = !toggle_subwindow;
            SYS_disableInts();
            if(toggle_subwindow){
                SYS_setHIntCallback(HIntHandler_DW);
            }else{
                SYS_setHIntCallback(HIntHandler_SW);
            }
            SYS_enableInts();
        }
    }
}
