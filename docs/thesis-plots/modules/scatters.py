from modules.utils import *
from matplotlib.ticker import LinearLocator, FormatStrFormatter
from matplotlib import cm

print("Plotting scatter graphs...")

# -----------------------------------------------------dd--
#   High detail Perlin surface + vertices
# -----------------------------------------------------dd--
fp1 = p.figure()
fp1_ax = fp1.add_subplot(111, projection='3d')
fp1_ax.set_xlabel('x')
fp1_ax.set_ylabel('y')
fp1_ax.set_zlabel('z')

x, y, z = surface_perlin(.7)

norm = p.Normalize(z.min(), z.max())
colors = cm.jet(norm(z))
rcount, ccount, _ = colors.shape

fp1_surf = fp1_ax.plot_surface(x, y, z, rcount=rcount, ccount=ccount,
                               alpha=.2, facecolors=colors, shade=False)

fp1_scat = fp1_ax.scatter(x, y, z, c=z.ravel(), alpha=1, cmap=p.get_cmap("jet"))

fp1_ax.set_xlim(0, 10)
fp1_ax.set_ylim(0, 10)
fp1_ax.zaxis.set_major_locator(LinearLocator(10))
fp1_ax.zaxis.set_major_formatter(FormatStrFormatter('%2.00f'))
# fp1.colorbar(fp1_scat, shrink=0.6, aspect=15)

figure_to_pdf(fp1, "surface_perlin_vertices")


# -----------------------------------------------------dd--
#   Low detail Perlin surface + vertices
# -----------------------------------------------------dd--
fp2 = p.figure()
fp2_ax = fp2.add_subplot(111, projection='3d')
fp2_ax.set_xlabel('x')
fp2_ax.set_ylabel('y')
fp2_ax.set_zlabel('z')

x, y, z = surface_perlin(1.4)

norm = p.Normalize(z.min(), z.max())
colors = cm.jet(norm(z))
rcount, ccount, _ = colors.shape

fp2_surf = fp2_ax.plot_surface(x, y, z, rcount=rcount, ccount=ccount,
                               alpha=.2, facecolors=colors, shade=False)

fp2_scat = fp2_ax.scatter(x, y, z, c=z.ravel(), alpha=1, cmap=p.get_cmap("jet"))

fp2_ax.set_xlim(0, 10)
fp2_ax.set_ylim(0, 10)
fp2_ax.zaxis.set_major_locator(LinearLocator(10))
fp2_ax.zaxis.set_major_formatter(FormatStrFormatter('%2.00f'))
# fp2.colorbar(fp2_scat, shrink=0.6, aspect=15)

figure_to_pdf(fp2, "surface_perlin_vertices_low")
