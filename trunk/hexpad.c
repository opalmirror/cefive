#include "hexpad.h"

static ColorConfig* digit_color(HexPad* prPad, const int x, const int y);
static int render_cursor(HexPad* prPad);
static int render_digit(HexPad* prPad, const int x, const int y);
static int render_value(HexPad* prPad);

static ColorConfig* digit_color(HexPad* prPad, const int x, const int y) {
    CursorPos* prCursor = NULL;
    if (prPad != NULL) {
        prCursor = hexpad_get_cursorpos(prPad);
        if ((prCursor->x == x) && (prCursor->y == y)) {
            return hexpad_get_cursorcolor(prPad);
        } else {
            return hexpad_get_panelcolor(prPad);
        }
    }
    return NULL;
}

int hexpad_button_circle(HexPad* prPad) {
    return HEXPAD_SUCCESS;
}

int hexpad_button_cross(HexPad* prPad) {
    CursorPos* prCursor = NULL;
    u8 value = 0;
    
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    prCursor = hexpad_get_cursorpos(prPad);
    value = (u8)((prCursor->y * 4) + prCursor->x);
    prPad->byteval[prPad->digit] = value;
    if (hexpad_next_digit(prPad) < 0) {
        return HEXPAD_FAILURE;
    }
    return HEXPAD_SUCCESS;
}

int hexpad_button_square(HexPad* prPad) {
    return HEXPAD_SUCCESS;
}

int hexpad_button_triangle(HexPad* prPad) {
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    prPad->cancelled = 0;
    prPad->visible = 0;
}

int hexpad_prev_digit(HexPad* prPad) {
    int digit = 0;
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    digit = prPad->digit;
    digit--;
    if (digit < 0) {
        digit = 7;
    }
    prPad->digit = digit;
    if (hexpad_set_cursordigit(prPad, prPad->byteval[prPad->digit]) < 0) {
        return HEXPAD_FAILURE;
    }
    return HEXPAD_SUCCESS;
}

int hexpad_next_digit(HexPad* prPad) {
    int digit = 0;
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    digit = prPad->digit;
    digit++;
    if (digit > 7) {
        digit = 0;
        prPad->visible = 0;
        prPad->cancelled = 0;
        return HEXPAD_SUCCESS;
    }
    prPad->digit = digit;
    if (hexpad_set_cursordigit(prPad, prPad->byteval[prPad->digit]) < 0) {
        return HEXPAD_FAILURE;
    }
    return HEXPAD_SUCCESS;
}

int hexpad_cursor_down(HexPad* prPad) {
    CursorPos* prCursor = NULL;
    int y = 0;
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    prCursor = hexpad_get_cursorpos(prPad);
    y = prCursor->y;
    y++;
    if (y > 3) {
        y = 0;
    }
    prCursor->y = y;
    return HEXPAD_SUCCESS;
}

int hexpad_cursor_left(HexPad* prPad) {
    CursorPos* prCursor = NULL;
    int x = 0;
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    prCursor = hexpad_get_cursorpos(prPad);
    x = prCursor->x;
    x--;
    if (x < 0) {
        x = 3;
    }
    prCursor->x = x;
    return HEXPAD_SUCCESS;
}

int hexpad_cursor_right(HexPad* prPad) {
    CursorPos* prCursor = NULL;
    int x = 0;
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    prCursor = hexpad_get_cursorpos(prPad);
    x = prCursor->x;
    x++;
    if (x > 3) {
        x = 0;
    }
    prCursor->x = x;
    return HEXPAD_SUCCESS;
}

int hexpad_cursor_up(HexPad* prPad) {
    CursorPos* prCursor = NULL;
    int y = 0;
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    prCursor = hexpad_get_cursorpos(prPad);
    y = prCursor->y;
    y--;
    if (y < 0) {
        y = 3;
    }
    prCursor->y = y;
    return HEXPAD_SUCCESS;
}

PanelConfig* hexpad_get_config(HexPad* prPad) {
    if (prPad != NULL) {
        return &prPad->panelConfig;
    }
    return NULL;
}

ColorConfig* hexpad_get_cursorcolor(HexPad* prPad) {
    PanelConfig* prConfig = NULL;
    if (prPad != NULL) {
        prConfig = hexpad_get_config(prPad);
        return panelconfig_get_cursorcolor(prConfig);
    }
    return NULL;
}

CursorPos* hexpad_get_cursorpos(HexPad* prPad) {
    if (prPad != NULL) {
        return &prPad->cursorPos;
    }
    return NULL;
}

ColorConfig* hexpad_get_panelcolor(HexPad* prPad) {
    PanelConfig* prConfig = NULL;
    if (prPad != NULL) {
        prConfig = hexpad_get_config(prPad);
        return panelconfig_get_panelcolor(prConfig);
    }
    return NULL;
}

CursorPos* hexpad_get_position(HexPad* prPad) {
    PanelConfig* prConfig = NULL;
    if (prPad != NULL) {
        prConfig = hexpad_get_config(prPad);
        return panelconfig_get_position(prConfig);
    }
    return NULL;
}

Dimension* hexpad_get_size(HexPad* prPad) {
    PanelConfig* prConfig = NULL;
    if (prPad != NULL) {
        prConfig = hexpad_get_config(prPad);
        return panelconfig_get_size(prConfig);
    }
    return NULL;
}

SceUInt32 hexpad_get_value(HexPad* prPad) {
    SceUInt32 value = 0;
    int i = 0;
    
    if (prPad != NULL) {
        for (i = 0; i < 8; i++) {
            value |= prPad->byteval[i];
            if (i < 7) {
                value <<= 4;
            }
        }
    }
    return value;
}

int hexpad_init(HexPad* prPad) {
    PanelConfig* prConfig = NULL;
    CursorPos* prCursor = NULL;
    Dimension* prSize = NULL;
    
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    prConfig = hexpad_get_config(prPad);
    if (panelconfig_init(prConfig) < 0) {
        return HEXPAD_FAILURE;
    }
    prCursor = hexpad_get_cursorpos(prPad);
    if (cursorpos_init(prCursor) < 0) {
        return HEXPAD_FAILURE;
    }
    prSize = hexpad_get_size(prPad);
    if (dimension_set(prSize, 12, 14) < 0) {
        return HEXPAD_FAILURE;
    }
    
    return HEXPAD_SUCCESS;
}

int hexpad_redraw(HexPad* prPad) {
    int x = 0;
    int y = 0;
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    if (prPad->visible != 1) {
        return HEXPAD_SUCCESS;
    }
    for (y = 0; y < 4; y++) {
        for (x = 0; x < 4; x++) {
            if (render_digit(prPad, x, y) < 0) {
                return HEXPAD_FAILURE;
            }
        }
    }
    if (render_value(prPad) < 0) {
        return HEXPAD_FAILURE;
    }
    if (render_cursor(prPad) < 0) {
        return HEXPAD_FAILURE;
    }
    
    return HEXPAD_SUCCESS;
}

int hexpad_set_cursorcolor(HexPad* prPad, const u32 background, const u32 text)
{
    ColorConfig* prColor = NULL;
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    prColor = hexpad_get_cursorcolor(prPad);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
        return HEXPAD_FAILURE;
    }
    return HEXPAD_SUCCESS;
}

int hexpad_set_cursordigit(HexPad* prPad, const int digit) {
    int x = 0;
    int y = 0;
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    if ((digit < 0) || (digit > 0xF)) {
        return HEXPAD_INVIDX;
    }
    y = (int)(digit / 4);
    x = digit % 4;
    if (hexpad_set_cursorpos(prPad, x, y) < 0) {
        return HEXPAD_FAILURE;
    }
    return HEXPAD_SUCCESS;
}

int hexpad_set_cursorpos(HexPad* prPad, const int x, const int y) {
    CursorPos* prCursor = NULL;
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    if ((x < 0) || (x > 3)) {
        return HEXPAD_INVIDX;
    }
    if ((y < 0) || (y > 3)) {
        return HEXPAD_INVIDX;
    }
    prCursor = hexpad_get_cursorpos(prPad);
    prCursor->x = x;
    prCursor->y = y;
    return HEXPAD_SUCCESS;
}

int hexpad_set_panelcolor(HexPad* prPad, const u32 background, const u32 text)
{
    ColorConfig* prColor = NULL;
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    prColor = hexpad_get_panelcolor(prPad);
    if (colorconfig_setcolor(prColor, background, text) < 0) {
        return HEXPAD_FAILURE;
    }
    return HEXPAD_SUCCESS;
}

int hexpad_set_position(HexPad* prPad, const int x, const int y) {
    CursorPos* prPos = NULL;
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    prPos = hexpad_get_position(prPad);
    if (cursorpos_set(prPos, x, y) < 0) {
        return HEXPAD_FAILURE;
    }
    return HEXPAD_SUCCESS;
}

int hexpad_set_value(HexPad* prPad, const SceUInt32 value) {
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    prPad->byteval[0] = (u8)((value & 0xF0000000) >> 28);
    prPad->byteval[1] = (u8)((value & 0x0F000000) >> 24);
    prPad->byteval[2] = (u8)((value & 0x00F00000) >> 20);
    prPad->byteval[3] = (u8)((value & 0x000F0000) >> 16);
    prPad->byteval[4] = (u8)((value & 0x0000F000) >> 12);
    prPad->byteval[5] = (u8)((value & 0x00000F00) >> 8);
    prPad->byteval[6] = (u8)((value & 0x000000F0) >> 4);
    prPad->byteval[7] = (u8)(value & 0x0000000F);
    return HEXPAD_SUCCESS;
}

static int render_cursor(HexPad* prPad) {
    CursorPos* prPos = NULL;
    ColorConfig* prColor = NULL;
    int bp = 0;
    int ap = 0;
    int x = 0;
    int y = 0;
    char sFmt[20];
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    bp = 3 + prPad->digit;
    ap = 12 - bp - 1;
    sprintf(sFmt, "%%%ds^%%-%ds", bp, ap);
    prColor = hexpad_get_panelcolor(prPad);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    prPos = hexpad_get_position(prPad);
    x = prPos->x;
    y = prPos->y + 13;
    pspDebugScreenSetXY(x, y);
    pspDebugScreenKprintf(sFmt, "", "");
    return HEXPAD_SUCCESS;
}

static int render_digit(HexPad* prPad, const int x, const int y) {
    CursorPos* prPos = NULL;
    ColorConfig* prColor = NULL;
    int vx = 0;
    int vy = 0;
    int digit = 0;
    
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    digit = (4 * y) + x;
    prPos = hexpad_get_position(prPad);
    vx = prPos->x + (x * 3);
    vy = prPos->y + (y * 3);
    prColor = digit_color(prPad, x, y);
    if (prColor != NULL) {
        pspDebugScreenSetBackColor(prColor->background);
        pspDebugScreenSetTextColor(prColor->text);
    }
    pspDebugScreenSetXY(vx, vy);
    pspDebugScreenPuts("   ");
    pspDebugScreenSetXY(vx, vy + 1);
    pspDebugScreenKprintf(" %X ", digit);
    pspDebugScreenSetXY(vx, vy + 2);
    pspDebugScreenPuts("   ");
    return HEXPAD_SUCCESS;
}

static int render_value(HexPad* prPad) {
    CursorPos* prPos = NULL;
    ColorConfig* prColor = NULL;
    SceUInt32 value = 0;
    
    if (prPad == NULL) {
        return HEXPAD_NULLPTR;
    }
    prPos = hexpad_get_position(prPad);
    prColor = hexpad_get_panelcolor(prPad);
    value = hexpad_get_value(prPad);
    pspDebugScreenSetXY(prPos->x, prPos->y + 12);
    pspDebugScreenSetBackColor(prColor->background);
    pspDebugScreenSetTextColor(prColor->text);
    pspDebugScreenKprintf(" 0x%08X ", value);
    
    return HEXPAD_SUCCESS;
}
