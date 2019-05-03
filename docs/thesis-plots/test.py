import matplotlib.pyplot as p
from mpl_toolkits.mplot3d import Axes3D

save_dir = "./"


def create3d(x_label: str = "x", y_label: str = "y", z_label: str = "z"):
    fig = p.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    ax.set_zlabel(z_label)
    return fig


def create2d(x_label: str = "x", y_label: str = "y"):
    fig = p.figure()
    fig.xlabel = x_label
    fig.ylabel = y_label
    return fig


def save(figure: p.Figure, filename: str):
    figure.savefig(save_dir + filename, transparent=True, frameon=False)


def display(figure: p.Figure):
    figure.show()


f2 = create2d()
save(f2, "test")

