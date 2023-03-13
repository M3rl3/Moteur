import glob
import subprocess
import sys

num_colors = 100
path = '../textures/'
png_files = glob.glob('../textures/*.png')
pvr_files = glob.glob('../textures/*.pvr')

# Check if files were already processed
if len(png_files) == len(pvr_files):
    print(f"files already converted to pvr format: {pvr_files}")
    sys.exit()

# Lossy compression of all your PNG files using pngquant
for png_file in png_files:
    filename = png_file.split("\\")[-1]
    input_file = path + filename
    subprocess.run(['pngquant', '--force', '--speed=1', f'--quality=0-{num_colors}', '--output', input_file, input_file])
    print("pngquant: Successfully processed file " + filename)


# Lossless compression of all your PNG files using optipng
for png_file in png_files:
    filename = png_file.split("\\")[-1]
    input_file = path + filename
    subprocess.run(["optipng", "-o2", "-quiet", "-out", input_file, input_file])
    print("optipng: Successfully processed file " + filename)


# LConversion of all your PNG files to an optimized compressed texture format PVR
for png_file in png_files:
    filename = png_file.split("\\")[-1]
    input_file = path + filename
    output_file = path + filename.split(".")[0] + ".pvr"
    subprocess.run(['PVRTexToolCLI', '-i', input_file, '-f', 'ETC2_RGB', '-q', 'pvrtcbest', '-o', output_file])
    print("PVRTexToolCLI: Successfully processed file " + filename)