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
dot_out_format = "png"

noise_img_res = (512, 512)

""" =======================================================================dd==
        PLOT SHOWS
    =======================================================================dd== """

if not getenv("PLOTS_SHOW"):
    p.show()


""" =======================================================================dd==
        DOT GRAPHS
    =======================================================================dd== """

for in_filename in glob(dot_src_path + "*.dot"):
    out_filename = path.basename(in_filename).rpartition('.')[0]

    graphs = dot.graph_from_dot_file(in_filename)
    for graph in graphs:
        graph.write(save_dir + out_filename + "_{}.{}".format(graph.get_name(), dot_out_format), format=dot_out_format)


""" =======================================================================dd==
        NOISES
    =======================================================================dd== """

# -----------------------------------------------------dd--
#   Random noise
# -----------------------------------------------------dd--
imageData = np.random.rand(*noise_img_res)
imageData[:] *= 255

imageio.imwrite(save_dir + 'noise_random.png', imageData.astype('uint8')[:, :])

# -----------------------------------------------------dd--
#   Perlin noise
# -----------------------------------------------------dd--
imageData = np.zeros([*noise_img_res])
for x in range(noise_img_res[0]):
    for y in range(noise_img_res[1]):
        imageData[x][y] = glm.perlin(glm.vec2(x, y) / 80) + 1
        imageData[x][y] /= 2

imageData[:] *= 255

imageio.imwrite(save_dir + 'noise_perlin.png', imageData.astype('uint8')[:, :])

# -----------------------------------------------------dd--
#   Simplex noise
# -----------------------------------------------------dd--
imageData = np.zeros([*noise_img_res])
for x in range(noise_img_res[0]):
    for y in range(noise_img_res[1]):
        imageData[x][y] = glm.simplex(glm.vec2(x, y) / 120) + 1
        imageData[x][y] /= 2

imageData[:] *= 255

imageio.imwrite(save_dir + 'noise_simplex.png', imageData.astype('uint8')[:, :])
