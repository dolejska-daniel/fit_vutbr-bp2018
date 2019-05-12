from modules.utils import *
import numpy as np
import scipy.stats as stats
import math

print("Plotting simple functions...")

# -----------------------------------------------------dd--
#   Texture intensity function - Grass
# -----------------------------------------------------dd--
mu = 0
variance = 0.005
sigma = math.sqrt(variance)

x = np.arange(-1.0, 1.0, 0.01)
y = stats.norm.pdf(x, mu, sigma)
grass_intensity = y

ff1, ff1_ax = p.subplots()
ff1_ax.plot(x, y)

# ff1_ax.set(xlabel='point height', ylabel='grass texture intensity')
ff1_ax.set_yticklabels([])
ff1_ax.set_xticklabels([])

ff1_ax.grid()

figure_to_pdf(ff1, "texture_intensity_grass")


# -----------------------------------------------------dd--
#   Texture intensity function - Dirt
# -----------------------------------------------------dd--
x = np.arange(-1.0, 1.0, 0.01)
x_offset = np.subtract(x, .3)
y = np.maximum(0.0, np.subtract(np.multiply(x_offset, np.subtract(np.sign(x_offset), 1.0)), grass_intensity / 4))

ff2, ff2_ax = p.subplots()
ff2_ax.plot(x, y)

# ff2_ax.set(xlabel='point height', ylabel='dirt texture intensity')
ff2_ax.set_yticklabels([])
ff2_ax.set_xticklabels([])

ff2_ax.grid()

figure_to_pdf(ff2, "texture_intensity_dirt")


# -----------------------------------------------------dd--
#   Texture intensity function - Rock
# -----------------------------------------------------dd--
x = np.arange(-1.0, 1.0, 0.01)
x_offset = np.subtract(x, .1)
y = np.maximum(0.0, np.subtract(np.multiply(x_offset, np.add(np.sign(x_offset), 1.0)), grass_intensity / 8))

ff3, ff3_ax = p.subplots()
ff3_ax.plot(x, y)

# ff3_ax.set(xlabel='point height', ylabel='rock texture intensity')
ff3_ax.set_yticklabels([])
ff3_ax.set_xticklabels([])

ff3_ax.grid()

figure_to_pdf(ff3, "texture_intensity_rock")
