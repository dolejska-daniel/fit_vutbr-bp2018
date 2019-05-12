import matplotlib.pyplot as p
import numpy as np
import glm

save_dir = "./output/"


def figure_save(figure: p.Figure, filename: str):
    figure.savefig(save_dir + filename,
                   transparent=True, frameon=False)


def figure_to_pdf(figure: p.Figure, filename: str):
    figure.savefig(save_dir + filename + ".pdf",
                   transparent=True, format='pdf', frameon=False)


def surface_random():
    x = y = np.arange(0, 10, .5)
    xr, yr = np.meshgrid(x, y)

    zr = np.random.rand(xr.shape[0], xr.shape[1])
    zr *= 10

    return xr, yr, zr


def surface_perlin(precision: float = .25):
    x = y = np.arange(0, 10, precision)
    xr, yr = np.meshgrid(x, y)

    zr = np.zeros(xr.shape)
    for i in range(xr.shape[0]):
        for j in range(xr.shape[1]):
            zr[i][j] = glm.perlin(glm.vec2(i, j) / (7.5 * 1 / precision)) + 1
    zr *= 6

    return xr, yr, zr