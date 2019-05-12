from modules.utils import *
from matplotlib.lines import Line2D
from matplotlib.collections import LineCollection

print("Plotting lines graphs...")

# -----------------------------------------------------dd--
#   QuadTree for points
# -----------------------------------------------------dd--
fl1 = p.figure(frameon=False)
fl1_ax = fl1.add_axes([0, 0, 1, 1])
fl1_ax.axis('off')

lines = [
    [(0, 0), (0, 5)],
    [(0, 5), (5, 5)],
    [(5, 5), (5, 0)],
    [(5, 0), (0, 0)],
]
border = LineCollection(lines, linewidths=2, colors='black')
fl1_ax.add_collection(border)
fl1_ax.autoscale()

level1_lines = [
    [(5 / 2, 0), (5 / 2, 5)],
    [(0, 5 / 2), (5, 5 / 2)],
]
level1 = LineCollection(level1_lines, linewidths=1.5, colors='black')
fl1_ax.add_collection(level1)

level1_args = {
    "family": "Consolas",
    "horizontalalignment": 'left',
    "verticalalignment": 'top',
    "color": 'black',
    "size": 24
}
p.text(.05, 5 - .05, "a", **level1_args)
p.text(5 / 2 + .05, 5 - .05, "b", **level1_args)
p.text(5 / 2 + .05, 5 / 2 - .05, "c", **level1_args)
p.text(.05, 5 / 2 - .05, "d", **level1_args)

points = [
    (1, 4)
]
fl1_ax.scatter(*zip(*points), color='b', s=180)

figure_to_pdf(fl1, "quadtree_containing_1")

points = [
    (3, 2)
]
fl1_ax.scatter(*zip(*points), color='g', s=180)

figure_to_pdf(fl1, "quadtree_containing_2")

# Level 2
level2_lines = [
    [(3 * 5 / 4, 0), (3 * 5 / 4, 2.5)],
    [(5 / 2, 5 / 4), (5, 5 / 4)],
]
level2 = LineCollection(level2_lines, linewidths=1, colors='purple')
fl1_ax.add_collection(level2)

level2_args = {
    "family": "Consolas",
    "horizontalalignment": 'right',
    "verticalalignment": 'top',
    "color": 'purple',
    "size": 24
}
p.text(3 * 5 / 4 - .05, 5 / 2 - .05, "a", **level2_args)
p.text(5 - .05, 5 / 2 - .05, "b", **level2_args)
p.text(5 - .05, 5 / 4 - .05, "c", **level2_args)
p.text(3 * 5 / 4 - .05, 5 / 4 - .05, "d", **level2_args)

# https://stackoverflow.com/questions/42268211/adding-line-markers-when-using-linecollection

# figure_to_pdf(fl1, "lines")

points = [
    (4.5, 1.7)
]
fl1_ax.scatter(*zip(*points), color='y', s=180)

figure_to_pdf(fl1, "quadtree_containing_3")

points = [
    (2, .8)
]
fl1_ax.scatter(*zip(*points), color='r', s=180)

figure_to_pdf(fl1, "quadtree_containing_4")


# -----------------------------------------------------dd--
#   QuadTree for line segments
# -----------------------------------------------------dd--
fl2 = p.figure(frameon=False)
fl2_ax = fl2.add_axes([0, 0, 1, 1])
fl2_ax.axis('off')

lines = [
    [(0, 0), (0, 5)],
    [(0, 5), (5, 5)],
    [(5, 5), (5, 0)],
    [(5, 0), (0, 0)],
]
border = LineCollection(lines, linewidths=2, colors='black')
fl2_ax.add_collection(border)
fl2_ax.autoscale()

level1_lines = [
    [(5 / 2, 0), (5 / 2, 5)],
    [(0, 5 / 2), (5, 5 / 2)],
]
level1 = LineCollection(level1_lines, linewidths=1.5, colors='black')
fl2_ax.add_collection(level1)

level1_args = {
    "family": "Consolas",
    "horizontalalignment": 'left',
    "verticalalignment": 'top',
    "color": 'black',
    "size": 24
}
p.text(.05, 5 - .05, "a", **level1_args)
p.text(5 / 2 + .05, 5 - .05, "b", **level1_args)
p.text(5 / 2 + .05, 5 / 2 - .05, "c", **level1_args)
p.text(.05, 5 / 2 - .05, "d", **level1_args)

points = [
    [1, 2],
    [4, 3]
]
fl2_ax.add_artist(Line2D(points[0], points[1], color='b', linewidth=4))
fl2_ax.scatter(points[0], points[1], color='b', s=180)

figure_to_pdf(fl2, "quadtree_lseg_containing_1")

points = [
    [1.6, 4.2],
    [0.6, 3.7]
]
fl2_ax.add_artist(Line2D(points[0], points[1], color='g', linewidth=4))
fl2_ax.scatter(points[0], points[1], color='g', s=180)

figure_to_pdf(fl2, "quadtree_lseg_containing_2")

# Level 2
level2_lines = [
    [(3 * 5 / 4, 0), (3 * 5 / 4, 2.5)],
    [(5 / 2, 5 / 4), (5, 5 / 4)],
]
level2 = LineCollection(level2_lines, linewidths=1, colors='purple')
fl2_ax.add_collection(level2)

level2_args = {
    "family": "Consolas",
    "horizontalalignment": 'right',
    "verticalalignment": 'top',
    "color": 'purple',
    "size": 24
}
p.text(3 * 5 / 4 - .05, 5 / 2 - .05, "a", **level2_args)
p.text(5 - .05, 5 / 2 - .05, "b", **level2_args)
p.text(5 - .05, 5 / 4 - .05, "c", **level2_args)
p.text(3 * 5 / 4 - .05, 5 / 4 - .05, "d", **level2_args)

# https://stackoverflow.com/questions/42268211/adding-line-markers-when-using-linecollection

# figure_to_pdf(fl2, "lines")

points = [
    [4.0, 4.7],
    [1.5, 2.3]
]
fl2_ax.add_artist(Line2D(points[0], points[1], color='y', linewidth=4))
fl2_ax.scatter(points[0], points[1], color='y', s=180)

figure_to_pdf(fl2, "quadtree_lseg_containing_3")

points = [
    [3.0, 4.5],
    [0.9, 0.4]
]
fl2_ax.add_artist(Line2D(points[0], points[1], color='r', linewidth=4))
fl2_ax.scatter(points[0], points[1], color='r', s=180)

figure_to_pdf(fl2, "quadtree_lseg_containing_4")
