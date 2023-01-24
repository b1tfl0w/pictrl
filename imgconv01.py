from pathlib import Path
from PIL import Image

ASCII_BITS = '0', '1'
imagepath = Path('pi-logo.jpg')

img = Image.open(imagepath).convert('1')  # Convert image to bitmap.
width, height = img.size

# Convert image data to a list of ASCII bits.
data = [ASCII_BITS[bool(val)] for val in img.getdata()]
# Convert that to 2D list (list of character lists)
data = [data[offset: offset+width] for offset in range(0, width*height, width)]

with open(f'{imagepath.stem}.h', 'w') as file:
    file.write('#include <stdint.h>\n')
    file.write(f'// Conversion of {imagepath} to Header\n')
    file.write(f'uint8_t pixels[] = ')
    for row in data:
        file.write(','.join(row))
        file.write(',')
        print('ok')
    file.write(f';\n')
