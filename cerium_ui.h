// Copyright (C) 2026 Project Cerium

#ifndef __CERIUM_RECOVERY_UI_H__
#define __CERIUM_RECOVERY_UI_H__

#include "recovery_ui/device.h"
#include "recovery_ui/screen_ui.h"
#include <vector>
#include <string>

#define CERIUM_COLOR(color) gr_color((color)[2], (color)[1], (color)[0], (color)[3])

struct CeriumTheme {
    int background[4] = {15, 23, 42, 255};
    int header_accent[4] = {14, 165, 233, 255};
    int header_text[4] = {248, 250, 252, 255};
    
    int item_active_bg[4] = {14, 165, 233, 255};
    int item_active_text[4] = {255, 255, 255, 255};
    
    int item_inactive_bg[4] = {30, 41, 59, 255};
    int item_inactive_text[4] = {148, 163, 184, 255};
};

class CeriumRecoveryUI : public ScreenRecoveryUI {
public:
    CeriumRecoveryUI();
    bool Init(const std::string& locale) override;

protected:
    void update_screen_locked() override;
    void draw_background_locked() override;
    void draw_foreground_locked() override;

    int screen_width;
    int screen_height;
    int block_padding;
    int block_margin;
    int char_width;
    int char_height;
    CeriumTheme theme;

    void DrawCeriumHeader();
    void DrawCeriumMenu();
};

class CeriumDevice : public Device {
public:
    explicit CeriumDevice(ScreenRecoveryUI* ui) : Device(ui) {}
    const std::vector<std::string>& GetMenuItems() override;
private:
    std::vector<std::string> filtered_menu_;
};

#endif // __CERIUM_RECOVERY_UI_H__