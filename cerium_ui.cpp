#include "cerium_ui.h"
#include "minui/minui.h"
#include <android-base/logging.h>
#include <string>
#include <vector>

CeriumRecoveryUI::CeriumRecoveryUI() : 
    screen_width(0), screen_height(0), 
    block_padding(24), block_margin(12),
    char_width(0), char_height(0) {}

bool CeriumRecoveryUI::Init(const std::string& locale) {
    bool result = ScreenRecoveryUI::Init(locale);
    
    screen_width = gr_fb_width();
    screen_height = gr_fb_height();
    
    gr_font_size(gr_sys_font(), &char_width, &char_height);
    
    if (screen_width > 1080) {
        block_padding = 48;
        block_margin = 20;
    }

    return result;
}

void CeriumRecoveryUI::draw_background_locked() {
    gr_color(15, 23, 42, 255); 
    gr_fill(0, 0, screen_width, screen_height);
}

void CeriumRecoveryUI::DrawCeriumHeader() {
    int y_cursor = 60; 

    gr_color(0, 0, 0, 255);
    gr_fill(0, 0, screen_width, y_cursor + char_height + 40);

    gr_color(14, 165, 233, 255); 
    gr_fill(0, y_cursor + char_height + 38, screen_width, y_cursor + char_height + 42);

    gr_color(248, 250, 252, 255); 
    std::string title = "CERIUM RECOVERY";
    int x_pos = (screen_width - (title.length() * char_width)) / 2;
    gr_text(gr_sys_font(), x_pos, y_cursor + char_height, title.c_str(), 1);
}

void CeriumRecoveryUI::DrawCeriumMenu() {
    if (!menu_ || menu_->empty()) return;

    int menu_start_y = 200; 
    size_t current_selection = menu_->selection();

    int block_w = screen_width - (block_margin * 2);
    int block_h = char_height + (block_padding * 2);

    size_t start = menu_->MenuStart();
    size_t end = menu_->MenuEnd();

    for (size_t i = start; i < end; ++i) {
        size_t display_index = i - start;
        
        int item_top = menu_start_y + (display_index * (block_h + block_margin));
        int item_bottom = item_top + block_h;

        if (item_bottom > screen_height - 100) break;

        std::string label = menu_->TextItem(i);

        if (i == current_selection) {
            gr_color(14, 165, 233, 255);
            gr_fill(block_margin, item_top, block_margin + block_w, item_bottom);

            gr_color(2, 132, 199, 255);
            gr_fill(block_margin + 4, item_top + 4, block_margin + block_w - 4, item_top + 6);
            gr_fill(block_margin + 4, item_bottom - 6, block_margin + block_w - 4, item_bottom - 4);

            gr_color(15, 23, 42, 255);
        } else {
            gr_color(0, 0, 0, 255);
            gr_fill(block_margin, item_top, block_margin + block_w, item_bottom);

            gr_color(51, 65, 85, 255);
            gr_fill(block_margin, item_top, block_margin + block_w, item_top + 2);
            gr_fill(block_margin, item_bottom - 2, block_margin + block_w, item_bottom);

            gr_color(226, 232, 240, 255);
        }

        int text_x = block_margin + block_padding;
        int text_y = item_top + block_padding + char_height;

        gr_text(gr_sys_font(), text_x, text_y, label.c_str(), (i == current_selection) ? 1 : 0);
    }
}

void CeriumRecoveryUI::draw_foreground_locked() {
    DrawCeriumHeader();
    DrawCeriumMenu();
}

Device* make_device() {
    return new CeriumDevice(new CeriumRecoveryUI);
}