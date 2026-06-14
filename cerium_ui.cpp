// Copyright (C) 2026 Project Cerium

#include "cerium_ui.h"
#include "minui/minui.h"
#include <android-base/logging.h>

const std::vector<std::string>& CeriumDevice::GetMenuItems() {
    if (filtered_menu_.empty()) {
        std::vector<std::string> default_items = Device::GetMenuItems();

        for (size_t i = 0; i < default_items.size(); ++i) {
            const auto& item = default_items[i];
            if (item.find("graphics test") != std::string::npos ||
                item.find("locale test") != std::string::npos ||
                item.find("rescue") != std::string::npos ||
                item.find("Apply update from SD card") != std::string::npos || 
                item.find("Mount /system") != std::string::npos) { 
                continue;
            }
            filtered_menu_.push_back(item);
        }
    }
    return filtered_menu_;
}

Device::BuiltinAction CeriumDevice::InvokeMenuItem(size_t menu_position) {
    if (menu_position >= filtered_menu_.size()) {
        return Device::NO_ACTION;
    }

    std::string item = filtered_menu_[menu_position];

    if (item.find("Reboot system now") != std::string::npos) return Device::REBOOT;
    if (item.find("Reboot to bootloader") != std::string::npos) return Device::REBOOT_BOOTLOADER;
    if (item.find("Enter fastboot") != std::string::npos) return Device::ENTER_FASTBOOT;
    if (item.find("Apply update from ADB") != std::string::npos) return Device::APPLY_ADB_SIDELOAD;
    if (item.find("Apply update from SD card") != std::string::npos) return Device::APPLY_SDCARD;
    if (item.find("Wipe data") != std::string::npos) return Device::WIPE_DATA;
    if (item.find("Wipe cache") != std::string::npos) return Device::WIPE_CACHE;
    if (item.find("Mount /system") != std::string::npos) return Device::MOUNT_SYSTEM;
    if (item.find("View recovery logs") != std::string::npos) return Device::VIEW_RECOVERY_LOGS;
    if (item.find("Power off") != std::string::npos) return Device::SHUTDOWN;

    return Device::NO_ACTION;
}

CeriumRecoveryUI::CeriumRecoveryUI() : 
    screen_width(0), screen_height(0), 
    block_padding(32), block_margin(16),
    char_width(0), char_height(0) {}

bool CeriumRecoveryUI::Init(const std::string& locale) {
    bool result = ScreenRecoveryUI::Init(locale);
    screen_width = gr_fb_width();
    screen_height = gr_fb_height();
    gr_font_size(gr_sys_font(), &char_width, &char_height);
    
    if (screen_width > 1080) {
        block_padding = 56;
        block_margin = 32;
    }
    return result;
}

void CeriumRecoveryUI::update_screen_locked() {
    CERIUM_COLOR(theme.background);
    gr_clear();

    draw_background_locked();
    draw_foreground_locked();
    gr_flip();
}

void CeriumRecoveryUI::draw_background_locked() {
}

void CeriumRecoveryUI::DrawCeriumHeader() {
    int y_cursor = screen_height * 0.10; 

    CERIUM_COLOR(theme.header_accent); 
    gr_fill(block_margin * 2, y_cursor + char_height + 16, screen_width - (block_margin * 2), y_cursor + char_height + 20);

    CERIUM_COLOR(theme.header_text); 
    std::string title = "CERIUM RECOVERY";
    int x_pos = (screen_width - (title.length() * char_width)) / 2;
    gr_text(gr_sys_font(), x_pos, y_cursor, title.c_str(), 1);
}

void CeriumRecoveryUI::DrawCeriumMenu() {
    if (!menu_) return;

    TextMenu* text_menu = static_cast<TextMenu*>(menu_.get());
    if (!text_menu || text_menu->MenuStart() == text_menu->MenuEnd()) return;

    int menu_start_y = screen_height * 0.20; 
    size_t current_selection = text_menu->selection();

    int card_margin_x = block_margin * 2; 
    int card_w = screen_width - (card_margin_x * 2);
    int card_h = char_height + (block_padding * 2);

    size_t start = text_menu->MenuStart();
    size_t end = text_menu->MenuEnd();

    for (size_t i = start; i < end; ++i) {
        size_t display_index = i - start;
        int item_top = menu_start_y + (display_index * (card_h + block_margin));
        int item_bottom = item_top + card_h;

        if (item_bottom > screen_height - (block_margin * 2)) break;

        std::string label = text_menu->TextItem(i);

        int text_y = item_top + ((card_h - char_height) / 2);

        if (i == current_selection) {
            CERIUM_COLOR(theme.item_active_bg);
            gr_fill(card_margin_x, item_top, card_margin_x + card_w, item_bottom);

            CERIUM_COLOR(theme.item_active_text);
            std::string active_text = "> " + label;

            int text_width = active_text.length() * char_width;
            int text_x = card_margin_x + ((card_w - text_width) / 2);

            gr_text(gr_sys_font(), text_x, text_y, active_text.c_str(), 1);
        } else {
            CERIUM_COLOR(theme.item_inactive_bg);
            gr_fill(card_margin_x, item_top, card_margin_x + card_w, item_bottom);

            CERIUM_COLOR(theme.item_inactive_text);

            int text_width = label.length() * char_width;
            int text_x = card_margin_x + ((card_w - text_width) / 2);

            gr_text(gr_sys_font(), text_x, text_y, label.c_str(), 0);
        }
    }
}

void CeriumRecoveryUI::draw_foreground_locked() {
    DrawCeriumHeader();
    DrawCeriumMenu();
}

extern "C" {
    Device* make_device() {
        return new CeriumDevice(new CeriumRecoveryUI);
    }
}