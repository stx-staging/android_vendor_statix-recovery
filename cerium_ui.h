#ifndef __CERIUM_RECOVERY_UI_H__
#define __CERIUM_RECOVERY_UI_H__

#include "recovery_ui/device.h"
#include "recovery_ui/screen_ui.h"

class CeriumRecoveryUI : public ScreenRecoveryUI {
public:
    CeriumRecoveryUI();
    
    bool Init(const std::string& locale) override;

protected:
    void draw_background_locked() override;
    void draw_foreground_locked() override;

    int screen_width;
    int screen_height;
    int block_padding;
    int block_margin;
    int char_width;
    int char_height;

    void DrawCeriumHeader();
    void DrawCeriumMenu();
};

class CeriumDevice : public Device {
public:
    explicit CeriumDevice(ScreenRecoveryUI* ui) : Device(ui) {}
};

#endif // __CERIUM_RECOVERY_UI_H__