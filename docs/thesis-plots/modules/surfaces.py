from modules.utils import *
from matplotlib.ticker import LinearLocator, FormatStrFormatter
from matplotlib import cm

print("Plotting surfaces...")

# -----------------------------------------------------dd--
#   Surface random
# -----------------------------------------------------dd--
fs1 = p.figure()
fs1_ax = fs1.add_subplot(111, projection='3d')
fs1_ax.set_xlabel('x')
fs1_ax.set_ylabel('y')
fs1_ax.set_zlabel('z')

x, y, z = surface_random()
fs1_surf = fs1_ax.plot_surface(x, y, z, rstride=1, cstride=1, shade=True, cmap=p.get_cmap("jet"),
                               linewidth=0, antialiased=False)

fs1_ax.set_xlim(0, 10)
fs1_ax.set_ylim(0, 10)
fs1_ax.set_zlim(0, 10)
fs1_ax.zaxis.set_major_locator(LinearLocator(10))
fs1_ax.zaxis.set_major_formatter(FormatStrFormatter('%2.00f'))
fs1.colorbar(fs1_surf, shrink=0.6, aspect=15)

figure_to_pdf(fs1, "surface_random")

# -----------------------------------------------------dd--
#   Surface Perlin
# -----------------------------------------------------dd--
fs2 = p.figure()
fs2_ax = fs2.add_subplot(111, projection='3d')
fs2_ax.set_xlabel('x')
fs2_ax.set_ylabel('y')
fs2_ax.set_zlabel('z')

x, y, z = surface_perlin()
fs2_surf = fs2_ax.plot_surface(x, y, z, rstride=1, cstride=1, shade=True, cmap=p.get_cmap("jet"),
                               linewidth=0, antialiased=False)

fs2_ax.set_xlim(0, 10)
fs2_ax.set_ylim(0, 10)
fs2_ax.zaxis.set_major_locator(LinearLocator(10))
fs2_ax.zaxis.set_major_formatter(FormatStrFormatter('%2.00f'))
fs2.colorbar(fs2_surf, shrink=0.6, aspect=15)

figure_to_pdf(fs2, "surface_perlin")

# -----------------------------------------------------dd--
#   Surface Perlin - wireframe
# -----------------------------------------------------dd--
fs3 = p.figure()
fs3_ax = fs3.add_subplot(111, projection='3d')
fs3_ax.set_xlabel('x')
fs3_ax.set_ylabel('y')
fs3_ax.set_zlabel('z')

x, y, z = surface_perlin(.7)

norm = p.Normalize(z.min(), z.max())
colors = cm.jet(norm(z))
rcount, ccount, _ = colors.shape

fs3_surf = fs3_ax.plot_surface(x, y, z, rcount=rcount, ccount=ccount,
                               facecolors=colors, shade=False)
fs3_surf.set_facecolor((0, 0, 0, 0))

fs3_ax.set_xlim(0, 10)
fs3_ax.set_ylim(0, 10)
fs3_ax.zaxis.set_major_locator(LinearLocator(10))
fs3_ax.zaxis.set_major_formatter(FormatStrFormatter('%2.00f'))
# fs3.colorbar(fs3_surf, shrink=0.6, aspect=15)

figure_to_pdf(fs3, "surface_perlin_wireframe")
