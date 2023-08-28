#include <Dolphin/CARD.h>
#include <Dolphin/OS.h>
#include <Dolphin/math.h>
#include <Dolphin/string.h>
#include <Dolphin/types.h>

#include <JSystem/J2D/J2DOrthoGraph.hxx>
#include <JSystem/J2D/J2DTextBox.hxx>

#include <SMS/System/Application.hxx>

#include <BetterSMS/game.hxx>
#include <BetterSMS/loading.hxx>
#include <BetterSMS/module.hxx>
#include <BetterSMS/settings.hxx>
#include <BetterSMS/stage.hxx>

#include "mirror.hxx"
#include "settings.hxx"

/*
/ Example module that logs to the console and draws to the screen during gameplay
*/

static int getTextWidth(J2DTextBox *textbox) {
    const size_t textLength = strlen(textbox->mStrPtr);

    size_t textWidth = 0;
    for (int i = 0; i < textLength; ++i) {
        JUTFont::TWidth width;
        textbox->mFont->getWidthEntry(textbox->mStrPtr[i], &width);
        textWidth += width.mWidth;
    }

    return textWidth + (Max(textLength - 1, 0) * textbox->mCharSpacing);
}

/*
/ Settings
*/

static MirrorSetting sMirrorActiveSetting;

static const u8 sSaveBnr[] = {
    0x09, 0x00, 0x00, 0x60, 0x00, 0x20, 0x00, 0x00, 0x01, 0x02, 0x00, 0x7f, 0x00, 0x00, 0x0c, 0x20,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x04, 0x04,
    0x00, 0x00, 0x00, 0x11, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x05, 0x17, 0x17, 0x17, 0x14,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x09, 0x0a,
    0x14, 0x05, 0x10, 0x00, 0x0f, 0x10, 0x03, 0x00, 0x14, 0x14, 0x05, 0x05, 0x14, 0x14, 0x05, 0x13,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x0c, 0x0d, 0x0c, 0x0e,
    0x00, 0x00, 0x11, 0x10, 0x12, 0x00, 0x00, 0x11, 0x00, 0x00, 0x05, 0x05, 0x10, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x04, 0x04, 0x04, 0x05, 0x00, 0x00,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x10, 0x00, 0x17, 0x17, 0x17, 0x15, 0x15, 0x15, 0x05, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x10, 0x03, 0x00,
    0x00, 0x00, 0x00, 0x13, 0x15, 0x15, 0x05, 0x13, 0x00, 0x00, 0x00, 0x05, 0x15, 0x15, 0x05, 0x05,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x10, 0x12, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x05, 0x05, 0x10, 0x03, 0x05, 0x0f, 0x00, 0x10, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x05, 0x05, 0x05, 0x0f, 0x00, 0x16, 0x15, 0x15, 0x15,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x05, 0x11, 0x00, 0x00, 0x00, 0x00,
    0x16, 0x15, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x15, 0x15, 0x15, 0x05, 0x05, 0x00, 0x10, 0x15,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x05, 0x11, 0x00, 0x00,
    0x0f, 0x05, 0x16, 0x15, 0x05, 0x05, 0x00, 0x00, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x05, 0x05, 0x05, 0x05, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x10, 0x16, 0x15, 0x00, 0x00, 0x16, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x05, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x10, 0x15, 0x15, 0x15, 0x17, 0x17, 0x00, 0x13, 0x15, 0x15, 0x15, 0x15, 0x04, 0x05,
    0x00, 0x10, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x00, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x17, 0x17, 0x05, 0x05, 0x14, 0x14, 0x14, 0x05, 0x15, 0x15, 0x05, 0x05, 0x17, 0x17, 0x17, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15,
    0x12, 0x05, 0x14, 0x04, 0x05, 0x00, 0x10, 0x14, 0x05, 0x14, 0x14, 0x14, 0x05, 0x13, 0x14, 0x14,
    0x15, 0x15, 0x17, 0x17, 0x05, 0x10, 0x17, 0x17, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x05, 0x15,
    0x14, 0x14, 0x14, 0x14, 0x14, 0x17, 0x05, 0x05, 0x14, 0x14, 0x04, 0x05, 0x14, 0x14, 0x05, 0x05,
    0x14, 0x14, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x05, 0x05,
    0x00, 0x00, 0x00, 0x05, 0x15, 0x15, 0x15, 0x05, 0x00, 0x00, 0x00, 0x05, 0x17, 0x17, 0x15, 0x15,
    0x00, 0x00, 0x00, 0x05, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x05, 0x14, 0x14, 0x14, 0x14,
    0x10, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x05, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15,
    0x14, 0x14, 0x17, 0x17, 0x05, 0x05, 0x15, 0x15, 0x14, 0x14, 0x14, 0x14, 0x05, 0x05, 0x14, 0x14,
    0x15, 0x15, 0x15, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x15, 0x15, 0x15, 0x18,
    0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x05, 0x14, 0x14, 0x04, 0x05, 0x15, 0x15, 0x15, 0x05,
    0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x15, 0x15,
    0x05, 0x15, 0x15, 0x15, 0x15, 0x05, 0x15, 0x15, 0x05, 0x15, 0x15, 0x15, 0x04, 0x05, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x18, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05,
    0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x05, 0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x04, 0x05,
    0x05, 0x11, 0x10, 0x15, 0x15, 0x15, 0x05, 0x05, 0x05, 0x0f, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05,
    0x05, 0x15, 0x15, 0x15, 0x05, 0x15, 0x15, 0x15, 0x05, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15,
    0x00, 0x00, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x10, 0x05, 0x15, 0x15, 0x15, 0x05, 0x15, 0x15,
    0x05, 0x05, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x05, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15,
    0x15, 0x15, 0x05, 0x11, 0x00, 0x00, 0x00, 0x00, 0x15, 0x15, 0x15, 0x05, 0x19, 0x00, 0x00, 0x00,
    0x15, 0x15, 0x15, 0x10, 0x1a, 0x00, 0x00, 0x00, 0x15, 0x15, 0x04, 0x10, 0x1b, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x05, 0x05, 0x05, 0x15, 0x15, 0x15, 0x00, 0x11, 0x05, 0x05, 0x05, 0x05, 0x05, 0x15,
    0x00, 0x11, 0x05, 0x15, 0x15, 0x05, 0x05, 0x15, 0x00, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x05, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x05, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x05, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x05, 0x05,
    0x15, 0x1c, 0x1d, 0x1e, 0x05, 0x05, 0x05, 0x15, 0x05, 0x1f, 0x20, 0x21, 0x05, 0x05, 0x15, 0x15,
    0x05, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x15, 0x15, 0x15, 0x05, 0x05,
    0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05,
    0x00, 0x00, 0x00, 0x05, 0x17, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x05, 0x15, 0x15, 0x17, 0x14,
    0x00, 0x00, 0x00, 0x05, 0x15, 0x15, 0x05, 0x15, 0x00, 0x00, 0x00, 0x05, 0x15, 0x15, 0x05, 0x15,
    0x14, 0x14, 0x14, 0x14, 0x05, 0x05, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x05, 0x05, 0x14, 0x17,
    0x15, 0x05, 0x15, 0x15, 0x05, 0x05, 0x17, 0x17, 0x05, 0x05, 0x15, 0x15, 0x05, 0x05, 0x17, 0x15,
    0x14, 0x14, 0x05, 0x05, 0x14, 0x15, 0x15, 0x05, 0x17, 0x14, 0x05, 0x05, 0x14, 0x14, 0x14, 0x14,
    0x17, 0x14, 0x05, 0x05, 0x14, 0x14, 0x14, 0x14, 0x15, 0x05, 0x05, 0x05, 0x14, 0x14, 0x14, 0x14,
    0x05, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x14, 0x14, 0x14, 0x05, 0x05, 0x05, 0x14, 0x17,
    0x14, 0x14, 0x05, 0x05, 0x05, 0x05, 0x14, 0x14, 0x14, 0x14, 0x14, 0x05, 0x05, 0x05, 0x14, 0x14,
    0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x05, 0x05, 0x17, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x05,
    0x14, 0x14, 0x14, 0x14, 0x05, 0x05, 0x05, 0x05, 0x14, 0x14, 0x14, 0x14, 0x14, 0x05, 0x05, 0x05,
    0x15, 0x15, 0x15, 0x05, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x04, 0x15, 0x15, 0x15, 0x05, 0x17, 0x17, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x05, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x16, 0x05, 0x17, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x05, 0x10, 0x00, 0x00, 0x00, 0x00, 0x15, 0x05, 0x05, 0x0f, 0x00, 0x00, 0x00, 0x00,
    0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x05, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x05, 0x05, 0x17, 0x17, 0x17, 0x17, 0x15, 0x00, 0x05, 0x05, 0x05, 0x17, 0x17, 0x17, 0x17,
    0x00, 0x12, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x15, 0x05, 0x05, 0x05, 0x15, 0x15, 0x05, 0x05, 0x17, 0x05, 0x1f, 0x1e, 0x17, 0x17, 0x05, 0x05,
    0x05, 0x28, 0x22, 0x15, 0x30, 0x05, 0x05, 0x05, 0x05, 0x39, 0x31, 0x15, 0x2b, 0x05, 0x05, 0x05,
    0x05, 0x1f, 0x22, 0x23, 0x05, 0x05, 0x05, 0x15, 0x05, 0x27, 0x20, 0x28, 0x29, 0x05, 0x05, 0x05,
    0x05, 0x1c, 0x20, 0x21, 0x05, 0x00, 0x05, 0x05, 0x05, 0x2b, 0x22, 0x20, 0x1f, 0x05, 0x05, 0x05,
    0x15, 0x15, 0x05, 0x05, 0x15, 0x05, 0x05, 0x05, 0x15, 0x2a, 0x2b, 0x2a, 0x15, 0x05, 0x05, 0x00,
    0x05, 0x31, 0x15, 0x31, 0x05, 0x05, 0x05, 0x00, 0x1f, 0x15, 0x15, 0x23, 0x05, 0x05, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x05, 0x15, 0x15, 0x05, 0x05, 0x00, 0x00, 0x00, 0x05, 0x15, 0x15, 0x05, 0x05,
    0x00, 0x00, 0x13, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x13, 0x05, 0x05, 0x05, 0x05, 0x03,
    0x05, 0x05, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x15, 0x04, 0x05, 0x05, 0x15, 0x17, 0x14, 0x14, 0x15, 0x05, 0x05, 0x05, 0x15, 0x15, 0x15, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x10, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x14, 0x14, 0x14, 0x14, 0x05, 0x05, 0x14, 0x14, 0x15, 0x15, 0x17, 0x14, 0x05, 0x05, 0x14, 0x14,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x05, 0x05, 0x14, 0x05, 0x14, 0x14, 0x2c, 0x2c, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x32, 0x2a, 0x27, 0x33, 0x05, 0x05, 0x05, 0x05, 0x3a, 0x3b, 0x36, 0x1d,
    0x05, 0x24, 0x24, 0x05, 0x25, 0x05, 0x26, 0x24, 0x27, 0x05, 0x27, 0x2d, 0x2e, 0x2f, 0x2a, 0x05,
    0x2e, 0x2d, 0x34, 0x35, 0x36, 0x1d, 0x37, 0x32, 0x3c, 0x3a, 0x3b, 0x3d, 0x3e, 0x3f, 0x40, 0x41,
    0x05, 0x05, 0x14, 0x14, 0x14, 0x14, 0x14, 0x15, 0x27, 0x05, 0x2c, 0x2c, 0x2c, 0x05, 0x14, 0x14,
    0x36, 0x38, 0x27, 0x27, 0x27, 0x05, 0x05, 0x05, 0x3c, 0x42, 0x1d, 0x3b, 0x43, 0x05, 0x05, 0x05,
    0x15, 0x15, 0x05, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00,
    0x04, 0x04, 0x05, 0x05, 0x13, 0x00, 0x00, 0x00, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0f, 0x05, 0x1c, 0x1f, 0x44, 0x22, 0x2b, 0x28, 0x00, 0x13, 0x10, 0x05, 0x1e, 0x20, 0x45, 0x45,
    0x00, 0x00, 0x11, 0x05, 0x2b, 0x2b, 0x45, 0x45, 0x00, 0x00, 0x12, 0x10, 0x1c, 0x44, 0x45, 0x1f,
    0x2b, 0x45, 0x21, 0x45, 0x21, 0x39, 0x1e, 0x22, 0x20, 0x45, 0x31, 0x23, 0x31, 0x21, 0x22, 0x15,
    0x2b, 0x23, 0x21, 0x22, 0x21, 0x23, 0x31, 0x45, 0x31, 0x22, 0x20, 0x15, 0x15, 0x22, 0x23, 0x31,
    0x15, 0x44, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x15, 0x28, 0x05, 0x00, 0x00, 0x0f, 0x10, 0x03,
    0x21, 0x05, 0x00, 0x00, 0x13, 0x15, 0x15, 0x05, 0x23, 0x05, 0x00, 0x00, 0x0f, 0x15, 0x15, 0x15,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x10, 0x12, 0x00, 0x00,
    0x13, 0x00, 0x00, 0x05, 0x05, 0x10, 0x00, 0x00, 0x05, 0x00, 0x10, 0x15, 0x15, 0x05, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x17, 0x17, 0x05, 0x00, 0x00, 0x11, 0x10, 0x17, 0x17, 0x17, 0x17, 0x05, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x55, 0x15, 0x15, 0x56, 0x05, 0x05, 0x57, 0x00, 0x55, 0x14, 0x14, 0x17, 0x17, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x13, 0x00, 0x03, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x58, 0x00, 0x11, 0x00, 0x00, 0x00, 0x13, 0x10, 0x05, 0x12, 0x05, 0x05, 0x05, 0x15, 0x15, 0x15,
    0x10, 0x05, 0x10, 0x05, 0x36, 0x35, 0x2e, 0x3c, 0x00, 0x00, 0x12, 0x03, 0x50, 0x2e, 0x3c, 0x4d,
    0x04, 0x15, 0x15, 0x05, 0x05, 0x3a, 0x34, 0x59, 0x15, 0x15, 0x15, 0x05, 0x05, 0x53, 0x5b, 0x2b,
    0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x48, 0x4c, 0x15, 0x15, 0x15, 0x51, 0x52, 0x51, 0x15, 0x15,
    0x17, 0x17, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x14, 0x14, 0x17, 0x15, 0x15, 0x15, 0x15,
    0x4d, 0x4e, 0x36, 0x4f, 0x3c, 0x05, 0x05, 0x05, 0x17, 0x14, 0x53, 0x54, 0x27, 0x13, 0x00, 0x00,
    0x15, 0x17, 0x3b, 0x5a, 0x05, 0x00, 0x00, 0x00, 0x15, 0x5c, 0x21, 0x2d, 0x05, 0x00, 0x00, 0x00,
    0x05, 0x05, 0x05, 0x05, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x13, 0x10, 0x05, 0x1f, 0x15, 0x2b, 0x23, 0x03, 0x05, 0x2a, 0x2b, 0x44, 0x20, 0x2b, 0x31,
    0x03, 0x39, 0x20, 0x20, 0x20, 0x1e, 0x2b, 0x31, 0x03, 0x05, 0x28, 0x1f, 0x45, 0x1e, 0x1f, 0x21,
    0x1e, 0x1e, 0x15, 0x5d, 0x5d, 0x20, 0x22, 0x2b, 0x60, 0x61, 0x22, 0x27, 0x16, 0x15, 0x22, 0x23,
    0x04, 0x60, 0x21, 0x1c, 0x18, 0x15, 0x22, 0x23, 0x1c, 0x1c, 0x22, 0x1c, 0x60, 0x20, 0x21, 0x23,
    0x1d, 0x27, 0x05, 0x05, 0x0f, 0x15, 0x15, 0x15, 0x23, 0x20, 0x39, 0x00, 0x0f, 0x15, 0x15, 0x15,
    0x23, 0x20, 0x15, 0x31, 0x0f, 0x15, 0x15, 0x15, 0x23, 0x1d, 0x30, 0x05, 0x0f, 0x15, 0x15, 0x15,
    0x05, 0x10, 0x15, 0x15, 0x15, 0x05, 0x00, 0x00, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x00, 0x05,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x04, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15,
    0x0f, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x10, 0x15, 0x15, 0x15, 0x05, 0x15, 0x15, 0x15, 0x05,
    0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x05, 0x15, 0x15, 0x15,
    0x00, 0x56, 0x17, 0x17, 0x14, 0x14, 0x14, 0x14, 0x03, 0x62, 0x15, 0x15, 0x15, 0x15, 0x17, 0x17,
    0x05, 0x64, 0x15, 0x15, 0x15, 0x65, 0x15, 0x15, 0x05, 0x68, 0x15, 0x15, 0x15, 0x05, 0x69, 0x15,
    0x05, 0x05, 0x05, 0x17, 0x17, 0x17, 0x15, 0x15, 0x17, 0x05, 0x05, 0x14, 0x17, 0x17, 0x17, 0x17,
    0x15, 0x0f, 0x05, 0x17, 0x17, 0x17, 0x17, 0x05, 0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x05,
    0x15, 0x15, 0x15, 0x05, 0x12, 0x05, 0x5b, 0x33, 0x17, 0x17, 0x17, 0x05, 0x00, 0x05, 0x3a, 0x33,
    0x04, 0x04, 0x05, 0x05, 0x00, 0x05, 0x36, 0x66, 0x15, 0x05, 0x05, 0x05, 0x00, 0x05, 0x36, 0x66,
    0x15, 0x15, 0x5e, 0x2c, 0x17, 0x5f, 0x5a, 0x15, 0x15, 0x15, 0x38, 0x32, 0x17, 0x63, 0x38, 0x17,
    0x17, 0x17, 0x50, 0x32, 0x15, 0x2d, 0x2d, 0x17, 0x15, 0x15, 0x6a, 0x17, 0x15, 0x15, 0x39, 0x15,
    0x15, 0x37, 0x21, 0x05, 0x12, 0x00, 0x00, 0x00, 0x15, 0x40, 0x54, 0x05, 0x00, 0x00, 0x00, 0x00,
    0x14, 0x42, 0x67, 0x05, 0x00, 0x00, 0x00, 0x00, 0x17, 0x42, 0x5a, 0x05, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x12, 0x03, 0x10, 0x05, 0x1e, 0x20, 0x28, 0x23, 0x00, 0x00, 0x11, 0x10, 0x05, 0x20, 0x39, 0x6e,
    0x00, 0x00, 0x00, 0x0f, 0x2a, 0x2b, 0x15, 0x61, 0x00, 0x00, 0x12, 0x05, 0x39, 0x15, 0x31, 0x45,
    0x21, 0x31, 0x31, 0x31, 0x21, 0x22, 0x1d, 0x23, 0x23, 0x31, 0x31, 0x21, 0x21, 0x23, 0x1f, 0x23,
    0x05, 0x30, 0x1f, 0x1f, 0x30, 0x1f, 0x23, 0x21, 0x44, 0x1e, 0x39, 0x2b, 0x31, 0x1d, 0x23, 0x21,
    0x23, 0x05, 0x05, 0x05, 0x0f, 0x15, 0x15, 0x15, 0x1f, 0x05, 0x00, 0x00, 0x0f, 0x15, 0x15, 0x15,
    0x1e, 0x05, 0x00, 0x00, 0x10, 0x17, 0x17, 0x05, 0x15, 0x05, 0x00, 0x00, 0x05, 0x17, 0x17, 0x05,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15,
    0x15, 0x15, 0x05, 0x15, 0x15, 0x05, 0x05, 0x05, 0x17, 0x05, 0x05, 0x15, 0x15, 0x05, 0x05, 0x05,
    0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x04, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05,
    0x05, 0x6b, 0x15, 0x15, 0x6c, 0x05, 0x15, 0x15, 0x05, 0x69, 0x15, 0x15, 0x60, 0x05, 0x15, 0x15,
    0x05, 0x60, 0x15, 0x15, 0x70, 0x15, 0x15, 0x15, 0x05, 0x71, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x68, 0x05, 0x05, 0x15, 0x15, 0x15, 0x05, 0x05, 0x05, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x05, 0x05, 0x05, 0x00, 0x05, 0x6d, 0x33, 0x05, 0x05, 0x05, 0x05, 0x00, 0x05, 0x6f, 0x67,
    0x05, 0x05, 0x05, 0x05, 0x00, 0x05, 0x5b, 0x3c, 0x15, 0x15, 0x15, 0x05, 0x00, 0x27, 0x72, 0x3c,
    0x15, 0x15, 0x15, 0x15, 0x17, 0x17, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x17, 0x15,
    0x14, 0x17, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x14, 0x14, 0x14, 0x17, 0x15, 0x15, 0x15, 0x15,
    0x15, 0x40, 0x1e, 0x05, 0x00, 0x00, 0x00, 0x00, 0x15, 0x40, 0x54, 0x05, 0x00, 0x00, 0x00, 0x00,
    0x17, 0x54, 0x48, 0x05, 0x00, 0x00, 0x00, 0x00, 0x15, 0x73, 0x22, 0x25, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x12, 0x05, 0x31, 0x21, 0x6e, 0x05, 0x00, 0x00, 0x12, 0x05, 0x2a, 0x05, 0x10, 0x10,
    0x00, 0x00, 0x00, 0x12, 0x12, 0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1e, 0x20, 0x44, 0x31, 0x44, 0x28, 0x2a, 0x30, 0x05, 0x30, 0x31, 0x45, 0x05, 0x05, 0x10, 0x05,
    0x10, 0x05, 0x45, 0x2b, 0x05, 0x11, 0x00, 0x11, 0x13, 0x05, 0x1c, 0x05, 0x03, 0x00, 0x00, 0x00,
    0x22, 0x39, 0x00, 0x00, 0x05, 0x14, 0x14, 0x05, 0x05, 0x05, 0x00, 0x00, 0x05, 0x14, 0x14, 0x05,
    0x11, 0x00, 0x00, 0x13, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x13, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x17, 0x17, 0x05, 0x05, 0x10, 0x05, 0x05, 0x05, 0x14, 0x14, 0x04, 0x00, 0x00,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x03, 0x00, 0x05, 0x05, 0x05, 0x05, 0x10, 0x00,
    0x05, 0x17, 0x17, 0x17, 0x17, 0x15, 0x05, 0x05, 0x05, 0x05, 0x17, 0x17, 0x17, 0x05, 0x05, 0x05,
    0x11, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x11, 0x00, 0x03, 0x05, 0x05, 0x05, 0x05, 0x03, 0x00,
    0x05, 0x74, 0x15, 0x15, 0x15, 0x15, 0x15, 0x05, 0x00, 0x10, 0x15, 0x15, 0x15, 0x15, 0x79, 0x05,
    0x00, 0x05, 0x25, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00,
    0x05, 0x05, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15, 0x12, 0x00, 0x05, 0x15, 0x15, 0x15, 0x15, 0x15,
    0x00, 0x00, 0x0f, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x12, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x15, 0x15, 0x15, 0x05, 0x0f, 0x33, 0x5b, 0x34, 0x15, 0x15, 0x04, 0x05, 0x05, 0x43, 0x3a, 0x40,
    0x05, 0x05, 0x05, 0x05, 0x12, 0x2a, 0x3c, 0x2a, 0x05, 0x05, 0x05, 0x0f, 0x00, 0x00, 0x7b, 0x00,
    0x75, 0x46, 0x76, 0x46, 0x77, 0x78, 0x78, 0x44, 0x37, 0x3b, 0x21, 0x40, 0x2b, 0x3b, 0x21, 0x3b,
    0x2d, 0x1f, 0x2b, 0x33, 0x7a, 0x7a, 0x67, 0x1f, 0x7c, 0x7d, 0x05, 0x05, 0x05, 0x05, 0x05, 0x7d,
    0x52, 0x2e, 0x4f, 0x2d, 0x0f, 0x00, 0x00, 0x00, 0x4e, 0x3b, 0x1d, 0x50, 0x05, 0x00, 0x00, 0x00,
    0x2d, 0x27, 0x33, 0x05, 0x12, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0a, 0x85, 0x0f, 0xeb, 0x40, 0x00, 0x84, 0x45, 0x70, 0x00, 0x0e, 0xb5, 0x0f, 0xe6,
    0x0f, 0xf9, 0x0f, 0xff, 0x04, 0x10, 0x10, 0x02, 0x09, 0xbf, 0x09, 0x9f, 0x09, 0xcf, 0x50, 0x00,
    0x60, 0x00, 0x20, 0x00, 0x10, 0x00, 0x30, 0x00, 0x7a, 0xff, 0x7f, 0xff, 0x8c, 0x68, 0x7d, 0xff,
    0x90, 0xab, 0x01, 0x27, 0x02, 0x27, 0x01, 0x11, 0xa4, 0xe3, 0xff, 0x25, 0xd2, 0x4b, 0xcd, 0x82,
    0xff, 0xd8, 0xff, 0x8d, 0xff, 0xf1, 0xf2, 0x85, 0xcf, 0xbd, 0x84, 0x41, 0xd7, 0xbd, 0x94, 0x81,
    0xa4, 0x80, 0x84, 0x24, 0x90, 0x40, 0xde, 0x05, 0xcb, 0x5a, 0xa9, 0x21, 0xfb, 0x8a, 0xb9, 0xa4,
    0xb8, 0xe0, 0xf6, 0xea, 0x9c, 0xc2, 0xc9, 0xe2, 0xf7, 0x49, 0xff, 0xea, 0xff, 0x65, 0xe6, 0xc9,
    0xbd, 0x82, 0xb5, 0x66, 0xff, 0x85, 0xea, 0xe6, 0xd6, 0x01, 0xdf, 0x48, 0xcb, 0x08, 0xce, 0xc6,
    0xe6, 0xa4, 0xef, 0x03, 0xf2, 0xe5, 0xf2, 0xc3, 0xf3, 0x2f, 0xff, 0xb3, 0xe7, 0x0d, 0xeb, 0x10,
    0xf3, 0x4e, 0xae, 0x47, 0xa6, 0xcd, 0xa6, 0x05, 0xdb, 0x0f, 0xdf, 0x31, 0xe7, 0x0a, 0xf7, 0x2a,
    0xb5, 0x41, 0xea, 0xf2, 0xf7, 0x75, 0xda, 0x42, 0xe2, 0xa9, 0xc6, 0xb4, 0xba, 0x52, 0x22, 0x21,
    0x19, 0x85, 0xe3, 0xbb, 0xc9, 0xe5, 0xff, 0xee, 0xf7, 0x72, 0xb6, 0x13, 0xbd, 0xe6, 0xd7, 0x9b,
    0x71, 0x22, 0x94, 0xe9, 0xb2, 0x0f, 0xa5, 0x44, 0xa5, 0xad, 0xa1, 0xcd, 0xde, 0x86, 0xce, 0x05,
    0x99, 0x6a, 0x72, 0x33, 0xca, 0x6f, 0x99, 0x29, 0xc2, 0xf5, 0xff, 0x83, 0xa0, 0x00, 0xff, 0xc5,
    0x89, 0x07, 0x60, 0x21, 0xff, 0xf5, 0xe6, 0xac, 0x50, 0x10, 0xdf, 0x74, 0xda, 0xee, 0xeb, 0x74,
    0xea, 0xed, 0x80, 0x83, 0xda, 0x44, 0x08, 0x60, 0x02, 0x10, 0x05, 0x40, 0x06, 0x40};

static const u8 sSaveIcon[] = {
    0x09, 0x00, 0x00, 0x20, 0x00, 0x40, 0x00, 0x00, 0x01, 0x02, 0x00, 0xf4, 0x00, 0x00, 0x08, 0x20,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x05, 0x0a, 0x05, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02,
    0x05, 0x01, 0x03, 0x03, 0x03, 0x02, 0x06, 0x07, 0x0b, 0x09, 0x0c, 0x0d, 0x0e, 0x09, 0x0f, 0x10,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x09, 0x06, 0x03, 0x05, 0x05, 0x04, 0x00, 0x11, 0x12, 0x13, 0x14, 0x06, 0x0b, 0x15, 0x16,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x05, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x17, 0x18, 0x19, 0x0d, 0x18, 0x00, 0x04, 0x06, 0x26, 0x27, 0x25, 0x25, 0x12,
    0x00, 0x00, 0x0d, 0x12, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x0d, 0x2c, 0x11, 0x1e, 0x1e, 0x27,
    0x0f, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x11, 0x1f, 0x28, 0x12, 0x29, 0x12, 0x27, 0x10, 0x1a, 0x2a,
    0x2d, 0x31, 0x25, 0x1b, 0x1b, 0x32, 0x33, 0x34, 0x31, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
    0x20, 0x21, 0x1a, 0x22, 0x23, 0x24, 0x1b, 0x25, 0x2b, 0x27, 0x27, 0x12, 0x2c, 0x2c, 0x29, 0x2d,
    0x35, 0x36, 0x26, 0x12, 0x10, 0x2e, 0x37, 0x2c, 0x3f, 0x40, 0x41, 0x42, 0x39, 0x43, 0x44, 0x45,
    0x18, 0x0d, 0x06, 0x03, 0x01, 0x05, 0x00, 0x00, 0x2e, 0x2c, 0x24, 0x13, 0x2f, 0x30, 0x00, 0x00,
    0x1f, 0x11, 0x1b, 0x1a, 0x28, 0x14, 0x00, 0x00, 0x46, 0x1f, 0x1b, 0x22, 0x25, 0x19, 0x00, 0x00,
    0x00, 0x00, 0x05, 0x47, 0x12, 0x11, 0x12, 0x37, 0x00, 0x00, 0x00, 0x02, 0x2f, 0x11, 0x54, 0x55,
    0x00, 0x00, 0x00, 0x02, 0x18, 0x1f, 0x5d, 0x3c, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x12, 0x11, 0x65,
    0x48, 0x49, 0x4a, 0x4b, 0x4a, 0x4b, 0x4c, 0x4d, 0x56, 0x57, 0x4a, 0x4a, 0x4a, 0x58, 0x59, 0x5a,
    0x50, 0x57, 0x57, 0x57, 0x5e, 0x5f, 0x60, 0x61, 0x66, 0x67, 0x57, 0x68, 0x69, 0x6a, 0x6b, 0x6c,
    0x4d, 0x4e, 0x3a, 0x4a, 0x4f, 0x50, 0x51, 0x51, 0x5a, 0x5b, 0x58, 0x4a, 0x4a, 0x4a, 0x57, 0x57,
    0x62, 0x60, 0x5b, 0x4d, 0x4a, 0x4a, 0x4a, 0x57, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x4a, 0x4a, 0x4b,
    0x45, 0x2c, 0x52, 0x53, 0x23, 0x02, 0x00, 0x00, 0x5c, 0x25, 0x2e, 0x1d, 0x0d, 0x01, 0x00, 0x00,
    0x63, 0x2e, 0x64, 0x2f, 0x0d, 0x05, 0x00, 0x00, 0x42, 0x72, 0x73, 0x18, 0x06, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x05, 0x02, 0x2f, 0x65, 0x65, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x7a, 0x1e,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x38, 0x5d, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x2c, 0x5d,
    0x37, 0x74, 0x56, 0x75, 0x69, 0x5f, 0x76, 0x5a, 0x37, 0x74, 0x59, 0x7b, 0x7c, 0x7d, 0x7d, 0x78,
    0x13, 0x80, 0x81, 0x7d, 0x82, 0x82, 0x83, 0x4d, 0x25, 0x85, 0x78, 0x4d, 0x86, 0x19, 0x87, 0x88,
    0x60, 0x5a, 0x77, 0x70, 0x78, 0x79, 0x4a, 0x39, 0x78, 0x7d, 0x78, 0x78, 0x78, 0x7e, 0x4a, 0x7f,
    0x83, 0x83, 0x82, 0x70, 0x70, 0x70, 0x4b, 0x84, 0x88, 0x89, 0x82, 0x83, 0x78, 0x70, 0x50, 0x22,
    0x65, 0x1e, 0x09, 0x06, 0x05, 0x00, 0x00, 0x00, 0x1b, 0x1e, 0x1c, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x12, 0x1b, 0x18, 0x02, 0x00, 0x00, 0x00, 0x00, 0x29, 0x53, 0x18, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x26, 0x5d, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x26, 0x5d,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x25, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x2c, 0x1f,
    0x29, 0x44, 0x8a, 0x8b, 0x8c, 0x4b, 0x8d, 0x88, 0x2e, 0x39, 0x91, 0x6f, 0x4a, 0x57, 0x92, 0x88,
    0x25, 0x97, 0x98, 0x99, 0x4a, 0x9a, 0x9b, 0x8c, 0x37, 0x97, 0x99, 0x4e, 0x7f, 0x67, 0x4a, 0x4a,
    0x88, 0x8d, 0x4b, 0x88, 0x8e, 0x8f, 0x90, 0x46, 0x93, 0x94, 0x57, 0x8c, 0x95, 0x96, 0x50, 0x46,
    0x3b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0x22, 0x8c, 0xa2, 0x8d, 0xa3, 0x89, 0xa4, 0xa5, 0x22,
    0x29, 0x1d, 0x0e, 0x03, 0x00, 0x00, 0x00, 0x00, 0x29, 0x1d, 0x0e, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x29, 0x73, 0x0e, 0x02, 0x00, 0x00, 0x00, 0x00, 0x29, 0x53, 0x18, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x2c, 0x5d, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x32, 0x5d,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0xae, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x43, 0xb2,
    0x37, 0x85, 0x97, 0x96, 0x0d, 0xa6, 0x4a, 0x8c, 0x25, 0xaa, 0x57, 0x60, 0x18, 0x0d, 0x8c, 0x88,
    0x25, 0xaf, 0x90, 0x67, 0xa5, 0x4e, 0x14, 0x87, 0x31, 0xb3, 0x90, 0x90, 0xb4, 0x8c, 0xa3, 0xb0,
    0x93, 0xa7, 0xa4, 0x0d, 0x4e, 0xa8, 0x4a, 0xa9, 0xab, 0xac, 0x0d, 0x19, 0xad, 0x4a, 0x57, 0x43,
    0x86, 0x8f, 0x8a, 0x58, 0xb0, 0x4a, 0x57, 0x74, 0xb0, 0xb5, 0xb5, 0xb6, 0x4a, 0x4a, 0x4a, 0x74,
    0x2e, 0x84, 0x18, 0x02, 0x00, 0x00, 0x00, 0x00, 0x12, 0x22, 0x18, 0x02, 0x00, 0x00, 0x00, 0x00,
    0xb1, 0x72, 0x23, 0x02, 0x00, 0x00, 0x00, 0x00, 0xb7, 0xb2, 0xb8, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xb9, 0x00, 0x05, 0x18, 0x39, 0xb2, 0x00, 0x00, 0x00, 0x01, 0x06, 0x24, 0xbe, 0x1e,
    0x00, 0x00, 0x00, 0x02, 0x19, 0x28, 0x11, 0x11, 0x00, 0x00, 0x00, 0x01, 0x19, 0x2c, 0x10, 0x21,
    0x64, 0x43, 0x90, 0xba, 0xba, 0xbb, 0x97, 0x60, 0x1b, 0xbe, 0x5c, 0x43, 0x7a, 0x43, 0x43, 0x43,
    0x1a, 0x1b, 0x3c, 0x12, 0x1b, 0x72, 0x12, 0x38, 0x26, 0x55, 0x38, 0x2e, 0x1b, 0x1a, 0x2e, 0x25,
    0xa8, 0x91, 0xb4, 0x4b, 0x4b, 0x4a, 0x4a, 0xa9, 0x44, 0x3c, 0x22, 0xa9, 0x22, 0x3b, 0x8c, 0x65,
    0x38, 0x2e, 0x1b, 0x65, 0xbf, 0xbf, 0x72, 0x1a, 0x2c, 0x12, 0x1a, 0x65, 0x12, 0x2e, 0x2e, 0x26,
    0x72, 0xb2, 0xbc, 0x06, 0x00, 0x00, 0xbd, 0xb9, 0x1a, 0x1b, 0x13, 0x19, 0x03, 0x00, 0x00, 0x00,
    0x1f, 0x10, 0x25, 0x18, 0x06, 0x00, 0x00, 0x00, 0x29, 0x28, 0x37, 0x19, 0x03, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x19, 0x37, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0xc0, 0xc1,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0xc9,
    0x19, 0x19, 0x2f, 0x13, 0x73, 0x73, 0x37, 0x25, 0x01, 0x01, 0xc2, 0xc3, 0x30, 0x19, 0x19, 0x23,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x01, 0x01, 0x01, 0x00, 0x00, 0xca, 0xcb, 0x00, 0x00, 0x00, 0x00,
    0x2e, 0x38, 0x73, 0x55, 0x37, 0x13, 0x1c, 0x0e, 0x1c, 0x14, 0x18, 0x19, 0xc4, 0xc5, 0xc6, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0xca, 0x00,
    0x1c, 0x25, 0x14, 0x02, 0x00, 0x00, 0x00, 0x00, 0xc7, 0xc5, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbd, 0xcc, 0xbd, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x05, 0x0a, 0x05, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02,
    0x05, 0x01, 0x03, 0x03, 0x03, 0x02, 0x06, 0x07, 0x0b, 0x09, 0x0c, 0x0d, 0x0e, 0x09, 0x0f, 0x10,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x09, 0x06, 0x03, 0x05, 0x05, 0x04, 0x00, 0x11, 0x12, 0x13, 0x14, 0x06, 0x0b, 0x15, 0x16,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x05, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x17, 0x18, 0x19, 0x0d, 0x18, 0x00, 0x04, 0x06, 0x26, 0x27, 0x25, 0x25, 0x12,
    0x00, 0x00, 0x0d, 0x12, 0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x0d, 0x2c, 0x11, 0x1e, 0x1e, 0x27,
    0x0f, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x11, 0x1f, 0x28, 0x12, 0x29, 0x12, 0x27, 0x10, 0x1a, 0x2a,
    0x2d, 0x31, 0x25, 0x1b, 0x1b, 0x32, 0x33, 0x34, 0x31, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0xcd, 0xce,
    0x20, 0x21, 0x1a, 0x22, 0x23, 0x24, 0x1b, 0x25, 0x2b, 0x27, 0x27, 0x12, 0x2c, 0x2c, 0x29, 0x2d,
    0x35, 0x36, 0x26, 0x12, 0x10, 0x2e, 0x37, 0x2c, 0xcf, 0xd0, 0x41, 0x42, 0x39, 0x43, 0x44, 0x45,
    0x18, 0x0d, 0x06, 0x03, 0x01, 0x05, 0x00, 0x00, 0x2e, 0x2c, 0x24, 0x13, 0x2f, 0x30, 0x00, 0x00,
    0x1f, 0x11, 0x1b, 0x1a, 0x28, 0x14, 0x00, 0x00, 0x46, 0x1f, 0x1b, 0x22, 0x25, 0x19, 0x00, 0x00,
    0x00, 0x00, 0x05, 0x47, 0x12, 0x11, 0x12, 0x37, 0x00, 0x00, 0x00, 0x02, 0x2f, 0x11, 0x54, 0x55,
    0x00, 0x00, 0x00, 0x02, 0x18, 0x1f, 0x5d, 0x3c, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x12, 0x11, 0x65,
    0x48, 0x49, 0x4a, 0x4b, 0xd1, 0xd2, 0x5a, 0xd3, 0x56, 0x57, 0x4a, 0x4a, 0x81, 0x59, 0x76, 0x6e,
    0x50, 0x57, 0x57, 0xd2, 0x7b, 0x81, 0xd7, 0x5a, 0x66, 0x67, 0x79, 0x69, 0x7c, 0x78, 0x78, 0x70,
    0xd3, 0x6e, 0xd4, 0x4b, 0x4f, 0x50, 0x51, 0x51, 0xd5, 0xd6, 0x58, 0x70, 0x4a, 0x4a, 0x57, 0x57,
    0x60, 0x6b, 0xd8, 0x78, 0x89, 0x4a, 0x4a, 0x57, 0x70, 0x78, 0x70, 0x70, 0x70, 0xb4, 0x4a, 0x4b,
    0x45, 0x2c, 0x52, 0x53, 0x23, 0x02, 0x00, 0x00, 0x5c, 0x25, 0x2e, 0x1d, 0x0d, 0x01, 0x00, 0x00,
    0x63, 0x2e, 0x64, 0x2f, 0x0d, 0x05, 0x00, 0x00, 0x42, 0x72, 0x73, 0x18, 0x06, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x05, 0x02, 0x2f, 0x65, 0x65, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x7a, 0x1e,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x38, 0x5d, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x2c, 0x5d,
    0x37, 0x74, 0x59, 0x7d, 0x82, 0x70, 0x89, 0xd9, 0x37, 0x80, 0x78, 0x70, 0x70, 0x0d, 0x4e, 0x98,
    0x13, 0x80, 0x70, 0x8b, 0xa6, 0xa3, 0xa6, 0x88, 0x25, 0xaa, 0x8a, 0xb0, 0x4a, 0x4a, 0xdc, 0x88,
    0xda, 0x89, 0x70, 0x70, 0x70, 0x8f, 0x4a, 0x39, 0xa6, 0x86, 0x0d, 0xd9, 0x70, 0x70, 0xd1, 0x7f,
    0x93, 0xa6, 0xa6, 0xdb, 0x89, 0x70, 0x4b, 0x84, 0x88, 0xdd, 0x4a, 0x8c, 0x95, 0x8f, 0x4a, 0x22,
    0x65, 0x1e, 0x09, 0x06, 0x05, 0x00, 0x00, 0x00, 0x1b, 0x1e, 0x1c, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x12, 0x1b, 0x18, 0x02, 0x00, 0x00, 0x00, 0x00, 0x29, 0x53, 0x18, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x26, 0x5d, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x26, 0x5d,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x25, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x2c, 0x1f,
    0x29, 0x44, 0xdb, 0xb0, 0x4a, 0xde, 0xdf, 0xe0, 0x2e, 0x3a, 0xa6, 0x4e, 0xe0, 0xe3, 0x79, 0x4a,
    0x25, 0x3a, 0xa8, 0xa4, 0x0d, 0x97, 0x4a, 0x88, 0x37, 0x85, 0x8d, 0x8c, 0x0d, 0x9c, 0x4b, 0x93,
    0x88, 0xe1, 0xe2, 0x9e, 0x9f, 0x99, 0xba, 0x46, 0xe0, 0xb0, 0xe4, 0xe0, 0xd9, 0x8e, 0xe5, 0x46,
    0xa3, 0xe6, 0xdb, 0x0d, 0x4e, 0xa4, 0x56, 0x22, 0xe7, 0xab, 0x19, 0x0d, 0xa7, 0xb6, 0x4a, 0x22,
    0x29, 0x1d, 0x0e, 0x03, 0x00, 0x00, 0x00, 0x00, 0x29, 0x1d, 0x0e, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x29, 0x73, 0x0e, 0x02, 0x00, 0x00, 0x00, 0x00, 0x29, 0x53, 0x18, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x2c, 0x5d, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0x32, 0x5d,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x0d, 0xae, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x43, 0xb2,
    0x37, 0x85, 0x4a, 0xa5, 0x97, 0x82, 0xdb, 0xe8, 0x25, 0xaa, 0x4a, 0xea, 0x4b, 0x8c, 0xeb, 0xec,
    0x25, 0xaf, 0x90, 0x90, 0xea, 0x8c, 0x99, 0xd8, 0x31, 0xb3, 0x90, 0x90, 0x90, 0xf0, 0x8d, 0x8e,
    0xe9, 0xd4, 0x0e, 0xac, 0x58, 0x4a, 0x4a, 0xa9, 0xed, 0xeb, 0x8b, 0xac, 0xee, 0x4a, 0x57, 0x43,
    0xef, 0xd4, 0xac, 0xe5, 0x4a, 0x4a, 0x57, 0x74, 0x9f, 0x6f, 0x4b, 0x4a, 0x4a, 0x4a, 0x4a, 0x74,
    0x2e, 0x84, 0x18, 0x02, 0x00, 0x00, 0x00, 0x00, 0x12, 0x22, 0x18, 0x02, 0x00, 0x00, 0x00, 0x00,
    0xb1, 0x72, 0x23, 0x02, 0x00, 0x00, 0x00, 0x00, 0xb7, 0xb2, 0xb8, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xb9, 0x00, 0x05, 0x18, 0x39, 0xb2, 0x00, 0x00, 0x00, 0x01, 0x06, 0x24, 0xbe, 0x1e,
    0x00, 0x00, 0x00, 0x02, 0x19, 0x28, 0x11, 0x11, 0x00, 0x00, 0x00, 0x01, 0x19, 0x2c, 0x10, 0x21,
    0x64, 0x43, 0x90, 0xba, 0xba, 0xf1, 0xf2, 0x6a, 0x1b, 0xbe, 0x5c, 0x43, 0x7a, 0x43, 0x43, 0xf3,
    0x1a, 0x1b, 0x3c, 0x12, 0x1b, 0x72, 0x12, 0x38, 0x26, 0x55, 0x38, 0x2e, 0x1b, 0x1a, 0x2e, 0x25,
    0xdb, 0x67, 0x4b, 0x4b, 0x4b, 0x4a, 0x4a, 0xa9, 0x49, 0x3c, 0x22, 0xa9, 0x22, 0x3b, 0x8c, 0x65,
    0x38, 0x2e, 0x1b, 0x65, 0xbf, 0xbf, 0x72, 0x1a, 0x2c, 0x12, 0x1a, 0x65, 0x12, 0x2e, 0x2e, 0x26,
    0x72, 0xb2, 0xbc, 0x06, 0x00, 0x00, 0xbd, 0xb9, 0x1a, 0x1b, 0x13, 0x19, 0x03, 0x00, 0x00, 0x00,
    0x1f, 0x10, 0x25, 0x18, 0x06, 0x00, 0x00, 0x00, 0x29, 0x28, 0x37, 0x19, 0x03, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x19, 0x37, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0xc0, 0xc1,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0xc9,
    0x19, 0x19, 0x2f, 0x13, 0x73, 0x73, 0x37, 0x25, 0x01, 0x01, 0xc2, 0xc3, 0x30, 0x19, 0x19, 0x23,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x01, 0x01, 0x01, 0x00, 0x00, 0xca, 0xc8, 0x00, 0x00, 0x00, 0x00,
    0x2e, 0x38, 0x73, 0x55, 0x37, 0x13, 0x1c, 0x0e, 0x1c, 0x14, 0x18, 0x19, 0xc4, 0xc5, 0xc6, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0xca, 0x00,
    0x1c, 0x25, 0x14, 0x02, 0x00, 0x00, 0x00, 0x00, 0xc7, 0xc5, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbd, 0xcc, 0xbd, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x30, 0x00, 0x50, 0x00, 0x40, 0x00, 0x10, 0x00, 0x20, 0x00, 0x60, 0x00, 0x77, 0x72,
    0x7d, 0xd5, 0x76, 0x62, 0x20, 0x12, 0x51, 0x10, 0xad, 0x65, 0x70, 0x00, 0x71, 0x10, 0xbd, 0xc2,
    0x7f, 0xd3, 0x7f, 0xf4, 0x7d, 0xb2, 0x78, 0x61, 0x74, 0x30, 0x64, 0x41, 0x61, 0x10, 0x53, 0x20,
    0x72, 0x20, 0x72, 0x10, 0x7f, 0xe5, 0x7e, 0xd5, 0x75, 0x40, 0x79, 0x72, 0x7f, 0xf6, 0x7f, 0xe3,
    0x7f, 0xd1, 0x7f, 0xc1, 0x7d, 0xb5, 0x73, 0x30, 0x77, 0x60, 0x7a, 0x81, 0x79, 0x81, 0x7d, 0xc2,
    0x7e, 0xc2, 0x7d, 0xa1, 0x7c, 0xd3, 0x7b, 0xb2, 0x7b, 0x91, 0x7e, 0xb1, 0x7c, 0xa2, 0x76, 0x50,
    0x63, 0x20, 0x7a, 0x70, 0x7a, 0xa2, 0x78, 0xa2, 0xb6, 0xe8, 0x75, 0xb4, 0x75, 0x92, 0x79, 0x70,
    0x7b, 0x93, 0x7c, 0xc8, 0x7c, 0xb8, 0x7e, 0xc8, 0x7d, 0xc6, 0x76, 0x83, 0x74, 0x95, 0x74, 0xa6,
    0x72, 0x72, 0x78, 0x93, 0x7f, 0xe9, 0x7b, 0xb7, 0x7b, 0xc8, 0x7b, 0xc7, 0x7d, 0xc5, 0x56, 0x50,
    0x79, 0x94, 0x7d, 0xda, 0x7f, 0xff, 0x7f, 0xed, 0x77, 0x65, 0xb4, 0x42, 0x77, 0x42, 0x7d, 0xee,
    0x7c, 0xed, 0x7b, 0xfd, 0x7e, 0xb3, 0x7b, 0x94, 0x7d, 0xb1, 0x79, 0x82, 0x7c, 0xdc, 0x7c, 0xff,
    0x7a, 0x54, 0x7c, 0x44, 0xf2, 0x94, 0xdc, 0xe6, 0x7b, 0xda, 0x7f, 0xf3, 0xcc, 0xa5, 0xe4, 0xc6,
    0xf6, 0xd5, 0xe8, 0xe8, 0xe9, 0x4a, 0x7c, 0xeb, 0x7c, 0x91, 0x7f, 0xe6, 0x7a, 0xa5, 0x7c, 0xcb,
    0xd1, 0x8c, 0xf8, 0x61, 0xe6, 0x50, 0xe9, 0xad, 0xee, 0x94, 0xf7, 0x39, 0xf2, 0x53, 0xd9, 0xcd,
    0xa4, 0x00, 0xc1, 0x29, 0x7f, 0xd6, 0x7a, 0x82, 0x7b, 0xa7, 0xf0, 0x20, 0xee, 0x31, 0xd0, 0xa5,
    0xb4, 0x00, 0x7c, 0xcc, 0x7b, 0xb5, 0xfc, 0x81, 0xd8, 0x20, 0xc4, 0x00, 0xbc, 0xe8, 0x7c, 0xa7,
    0x7b, 0x97, 0xd8, 0x42, 0x94, 0x00, 0xa4, 0x41, 0x7c, 0xa5, 0xda, 0xb2, 0xa8, 0x82, 0xb4, 0xc3,
    0xfe, 0xd1, 0xb8, 0xc5, 0xac, 0xe6, 0xd5, 0x47, 0x7f, 0xda, 0xea, 0xf5, 0xc9, 0x05, 0xa4, 0xa5,
    0x7b, 0xff, 0xcd, 0x8a, 0xa1, 0x90, 0xfe, 0x8e, 0xa5, 0xd2, 0xc5, 0x03, 0xc9, 0x69, 0xe6, 0xb1,
    0xf2, 0x6e, 0xd1, 0x8a, 0xbe, 0xfc, 0xa4, 0xe8, 0xb5, 0x49, 0xc3, 0x5e, 0xff, 0x97, 0xc4, 0xc3,
    0xd9, 0x87, 0x7a, 0x98, 0xee, 0x2b, 0xf6, 0xb0, 0xc9, 0x46, 0x7f, 0xdc, 0xea, 0x4e, 0xe5, 0x66,
    0xe2, 0x2d, 0x7c, 0xb6, 0xd6, 0xd2, 0xf9, 0xa7, 0xd1, 0x26, 0xe1, 0xa9, 0xce, 0x89, 0xca, 0xb0,
    0xda, 0x0d, 0x7e, 0xb4, 0x7f, 0xf9, 0xc6, 0xaf, 0x7b, 0xba, 0xe1, 0xca, 0xcd, 0xce, 0x7e, 0xc6,
    0xa5, 0x24, 0x01, 0x10, 0x7a, 0xed, 0xc6, 0xf6, 0xb1, 0x65, 0x02, 0x10, 0x7d, 0xd6, 0x7e, 0xc4,
    0x35, 0x30, 0x38, 0x60, 0x23, 0x20, 0x25, 0x40, 0x43, 0x30, 0x26, 0x50, 0x32, 0x10, 0x31, 0x10,
    0x06, 0x51, 0x09, 0x71, 0x04, 0x30, 0x07, 0x62, 0x08, 0x61, 0x75, 0x72, 0x76, 0x96, 0x77, 0xa7,
    0x72, 0x51, 0x7f, 0xee, 0xd5, 0x29, 0xf1, 0x8d, 0xc4, 0xe5, 0xf6, 0x74, 0xf2, 0x10, 0xe5, 0x6c,
    0xc4, 0x42, 0xb4, 0xa3, 0xb4, 0xa4, 0xd9, 0xca, 0xd3, 0x1a, 0xd3, 0x3c, 0xc7, 0x1c, 0x90, 0x85,
    0xfb, 0x35, 0x88, 0x65, 0xcb, 0x9f, 0xcb, 0x1b, 0xd3, 0x5d, 0x79, 0x88, 0xfe, 0x08, 0xfe, 0x4a,
    0xe1, 0xa7, 0xd0, 0xe3, 0x7a, 0xaa, 0xb1, 0x28, 0xc1, 0xac, 0xb9, 0x8c, 0xca, 0x53, 0xc4, 0x83,
    0xdf, 0xbd, 0xcf, 0x9a, 0xcf, 0x16, 0x7c, 0xd8};

static BetterSMS::Settings::SettingsGroup sSettingsGroup(1, 0, BetterSMS::Settings::Priority::MODE);
static BetterSMS::ModuleInfo sModuleInfo{"Mirror Mode", 1, 0, &sSettingsGroup};

// Module definition

static void initModule() {
    sSettingsGroup.addSetting(&sMirrorActiveSetting);
    {
        auto &saveInfo        = sSettingsGroup.getSaveInfo();
        saveInfo.mSaveName    = "Sunshine Mirror Mode";
        saveInfo.mBlocks      = 1;
        saveInfo.mGameCode    = 'GMSM';
        saveInfo.mCompany     = 0x3031;  // '01'
        saveInfo.mBannerFmt   = CARD_BANNER_CI;
        saveInfo.mBannerImage = reinterpret_cast<const ResTIMG *>(sSaveBnr);
        saveInfo.mIconFmt     = CARD_ICON_CI;
        saveInfo.mIconSpeed   = CARD_SPEED_SLOW;
        saveInfo.mIconCount   = 2;
        saveInfo.mIconTable   = reinterpret_cast<const ResTIMG *>(sSaveIcon);
        saveInfo.mSaveGlobal  = true;
    }

    BetterSMS::registerModule(&sModuleInfo);
}

static void deinitModule() { BetterSMS::deregisterModule(&sModuleInfo); }

// Definition block
KURIBO_MODULE_BEGIN("Mirror Mode", "JoshuaMK", "v1.0") {
    // Set the load and unload callbacks to our registration functions
    KURIBO_EXECUTE_ON_LOAD {
        initModule();

        KURIBO_EXPORT_AS(SetMirrorModeActive, "SetMirrorModeActive__Fb");
        KURIBO_EXPORT_AS(GetMirrorModeActive, "GetMirrorModeActive__Fv");
    }
    KURIBO_EXECUTE_ON_UNLOAD { deinitModule(); }
}
KURIBO_MODULE_END()
