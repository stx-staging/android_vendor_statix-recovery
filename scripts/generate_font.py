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

    max_width = 0
    max_height = 0
    # Dummy canvas using "L" (Grayscale)
    dummy_draw = ImageDraw.Draw(Image.new("L", (1, 1)))

    for c in chars:
        bbox_r = dummy_draw.textbbox((0, 0), c, font=font_reg)
        w_r = bbox_r[2] - bbox_r[0]
        h_r = bbox_r[3] - bbox_r[1]

        bbox_b = dummy_draw.textbbox((0, 0), c, font=font_bold)
        w_b = bbox_b[2] - bbox_b[0]
        h_b = bbox_b[3] - bbox_b[1]

        max_width = max(max_width, w_r + 4, w_b + 4) 
        max_height = max(max_height, h_r + 8, h_b + 8)

    total_width = max_width * 96
    total_height = max_height * 2

    print(f"Calculated Grid: {max_width}x{max_height} per character.")
    print(f"Total Image Size: {total_width}x{total_height} pixels.")

    # Create a Grayscale ("L") canvas filled with pure Black (0)
    img = Image.new("L", (total_width, total_height), 0)
    draw = ImageDraw.Draw(img)

    for i, c in enumerate(chars):
        x_position = i * max_width

        # Draw the text in pure White (255)
        draw.text((x_position + 2, 0), c, font=font_reg, fill=255)
        draw.text((x_position + 2, max_height), c, font=font_bold, fill=255)

    # --- THE FIX: Ensure output directory exists before saving ---
    output_dir = os.path.dirname(output_path)
    if output_dir:
        os.makedirs(output_dir, exist_ok=True)

    img.save(output_path)
    print(f"Success! Grayscale font generated at: {output_path}")

if __name__ == "__main__":
    # Set up command-line arguments
    parser = argparse.ArgumentParser(description="Generate minui font.png for Android Recovery")
    parser.add_argument("-r", "--regular", required=True, help="Path to the Regular TTF font")
    parser.add_argument("-b", "--bold", required=True, help="Path to the Bold TTF font")
    parser.add_argument("-s", "--size", type=int, default=36, help="Font size (default: 36)")
    parser.add_argument("-o", "--output", default="../res/images/font.png", help="Output path (default: ../res/images/font.png)")

    args = parser.parse_args()

    # Pass the arguments to the generator
    generate_minui_font(args.regular, args.bold, args.size, args.output)