import os
import argparse
from PIL import Image, ImageDraw, ImageFont

def generate_minui_font(font_regular, font_bold, font_size, output_path):
    print(f"Loading fonts at size {font_size}...")
    try:
        font_reg = ImageFont.truetype(font_regular, font_size)
        font_bold = ImageFont.truetype(font_bold, font_size)
    except IOError as e:
        print(f"Error: Could not find the font files. {e}")
        return

    chars = [chr(i) for i in range(32, 128)]

    ascent, descent = font_reg.getmetrics()
    max_height = ascent + descent + 6
    
    max_width = int(max(font_reg.getlength(c) for c in chars)) + 4

    total_width = max_width * 96
    total_height = max_height * 2

    print(f"Calculated Grid: {max_width}x{max_height} per character.")
    print(f"Total Image Size: {total_width}x{total_height} pixels.")

    img = Image.new("L", (total_width, total_height), 0)
    draw = ImageDraw.Draw(img)

    for i, c in enumerate(chars):
        x_position = i * max_width

        draw.text((x_position + 2, 2), c, font=font_reg, fill=255, anchor="la")
        draw.text((x_position + 2, max_height + 2), c, font=font_bold, fill=255, anchor="la")

    output_dir = os.path.dirname(output_path)
    if output_dir:
        os.makedirs(output_dir, exist_ok=True)

    img.save(output_path)
    print(f"Success! Grayscale font generated at: {output_path}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate minui font.png for Android Recovery")
    parser.add_argument("-r", "--regular", required=True, help="Path to the Regular TTF font")
    parser.add_argument("-b", "--bold", required=True, help="Path to the Bold TTF font")
    parser.add_argument("-s", "--size", type=int, default=36, help="Font size (default: 36)")
    parser.add_argument("-o", "--output", default="../res/images/font.png", help="Output path (default: ../res/images/font.png)")

    args = parser.parse_args()

    generate_minui_font(args.regular, args.bold, args.size, args.output)