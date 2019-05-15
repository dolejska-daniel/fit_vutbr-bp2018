import math
import matplotlib.pyplot as p
from mpl_toolkits.mplot3d import axes3d
from mpl_toolkits.mplot3d import proj3d
import glm # PyGLM
import imageio
import pydot as dot
from modules import *
from glob import glob
from os import path, getenv

dot_src_path = "./dot/"
dot_out_format = "pdf"

noise_img_res = (512, 512)

""" =======================================================================dd==
        PLOT SHOWS
    =======================================================================dd== """

if not getenv("PLOTS_SHOW"):
    print("Displaying plots...")
    p.show()
else:
    print("Skipping plot display.")


""" =======================================================================dd==
        DOT GRAPHS
    =======================================================================dd== """

print("Processing dot files...")
for in_filename in glob(dot_src_path + "*.dot"):
    out_filename = path.basename(in_filename).rpartition('.')[0]

    print("Parsing {}...".format(in_filename))
    graphs = dot.graph_from_dot_file(in_filename)
    for graph in graphs:
        out_filepath = save_dir + out_filename + "_{}.{}".format(graph.get_name(), dot_out_format)
        print("Saving graph {} to {}.".format(graph.get_name(), out_filepath))
        graph.write(out_filepath, format=dot_out_format)


""" =======================================================================dd==
        NOISES
    =======================================================================dd== """

print("Generating noise images...")
# -----------------------------------------------------dd--
#   Random noise
# -----------------------------------------------------dd--
imageData = np.random.rand(*noise_img_res)
imageData[:] *= 255

imageFilepath = save_dir + 'noise_random.png'
print("Saving {}.".format(imageFilepath))
imageio.imwrite(imageFilepath, imageData.astype('uint8')[:, :])

# -----------------------------------------------------dd--
#   Perlin noise
# -----------------------------------------------------dd--
imageData = np.zeros([*noise_img_res])
for x in range(noise_img_res[0]):
    for y in range(noise_img_res[1]):
        imageData[x][y] = glm.perlin(glm.vec2(x, y) / 80) + 1
        imageData[x][y] /= 2

imageData[:] *= 255

imageFilepath = save_dir + 'noise_perlin.png'
print("Saving {}.".format(imageFilepath))
imageio.imwrite(imageFilepath, imageData.astype('uint8')[:, :])

# -----------------------------------------------------dd--
#   Perlin noise with 3 octaves
# -----------------------------------------------------dd--
octave_count = 2
frequency = 2
amplitude = 128
persistence = .5
lacunarity = 2
max_value = float('-inf')
min_value = float('inf')

imageData = np.zeros([*noise_img_res])
for x in range(noise_img_res[0]):
    for y in range(noise_img_res[1]):
        tmp_freq = frequency
        tmp_ampl = amplitude
        sample = 0
        tmp_max = 0
        for o in range(octave_count):
            p = glm.perlin(glm.vec2(x, y) * tmp_freq / 160)
            # p = (p + 1) / 2
            sample += p * tmp_ampl
            tmp_max += tmp_ampl

            tmp_ampl *= persistence
            tmp_freq *= lacunarity

        imageData[x][y] = sample # / tmp_max
        max_value = max(max_value, sample)
        min_value = min(min_value, sample)

max_value2 = float('-inf')
min_value2 = float('inf')

for x in range(noise_img_res[0]):
    for y in range(noise_img_res[1]):
        old = imageData[x][y]
        imageData[x][y] += min_value
        imageData[x][y] /= 2 * max_value # (max_value - min_value)
        imageData[x][y] = np.clip(imageData[x][y] + 1, 0, 1)
        #print(old, imageData[x][y])

        max_value2 = max(max_value2, imageData[x][y])
        min_value2 = min(min_value2, imageData[x][y])


#print(max_value)
#print(min_value)
#print(max_value2)
#print(min_value2)

imageData[:] *= 255

imageFilepath = save_dir + 'noise_perlin_octaves_2.png'
print("Saving {}.".format(imageFilepath))
imageio.imwrite(imageFilepath, imageData.astype('uint8')[:, :])

# -----------------------------------------------------dd--
#   Perlin noise with 6 octaves
# -----------------------------------------------------dd--
octave_count = 6
frequency = 2
amplitude = 128
persistence = .5
lacunarity = 2
max_value = float('-inf')
min_value = float('inf')

imageData = np.zeros([*noise_img_res])
for x in range(noise_img_res[0]):
    for y in range(noise_img_res[1]):
        tmp_freq = frequency
        tmp_ampl = amplitude
        sample = 0
        tmp_max = 0
        for o in range(octave_count):
            p = glm.perlin(glm.vec2(x, y) * tmp_freq / 160)
            # p = (p + 1) / 2
            sample += p * tmp_ampl
            tmp_max += tmp_ampl

            tmp_ampl *= persistence
            tmp_freq *= lacunarity

        imageData[x][y] = sample # / tmp_max
        max_value = max(max_value, sample)
        min_value = min(min_value, sample)

max_value2 = float('-inf')
min_value2 = float('inf')

for x in range(noise_img_res[0]):
    for y in range(noise_img_res[1]):
        old = imageData[x][y]
        imageData[x][y] += min_value
        imageData[x][y] /= 2 * max_value # (max_value - min_value)
        imageData[x][y] = np.clip(imageData[x][y] + 1, 0, 1)
        #print(old, imageData[x][y])

        max_value2 = max(max_value2, imageData[x][y])
        min_value2 = min(min_value2, imageData[x][y])


#print(max_value)
#print(min_value)
#print(max_value2)
#print(min_value2)

imageData[:] *= 255

imageFilepath = save_dir + 'noise_perlin_octaves_6.png'
print("Saving {}.".format(imageFilepath))
imageio.imwrite(imageFilepath, imageData.astype('uint8')[:, :])

# -----------------------------------------------------dd--
#   Simplex noise
# -----------------------------------------------------dd--
imageData = np.zeros([*noise_img_res])
for x in range(noise_img_res[0]):
    for y in range(noise_img_res[1]):
        imageData[x][y] = glm.simplex(glm.vec2(x, y) / 120) + 1
        imageData[x][y] /= 2

imageData[:] *= 255

imageFilepath = save_dir + 'noise_simplex.png'
print("Saving {}.".format(imageFilepath))
imageio.imwrite(imageFilepath, imageData.astype('uint8')[:, :])
