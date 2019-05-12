from modules.utils import *
from matplotlib.collections import LineCollection

# -----------------------------------------------------dd--
#   Lines
# -----------------------------------------------------dd--
fl1, fl1_ax = p.subplots()

lines = [
    [(0, 0), (0, 5)],
    [(0, 5), (5, 5)],
    [(5, 5), (5, 0)],
    [(5, 0), (0, 0)],
]
fl1_lc = LineCollection(lines, linewidths=2, colors='r')
fl1_ax.add_collection(fl1_lc)
fl1_ax.autoscale()

points = [
    (1, 1)
]
fl1_scat = fl1_ax.scatter(*zip(*points))

# https://stackoverflow.com/questions/42268211/adding-line-markers-when-using-linecollection

# figure_to_pdf(fl1, "lines")
