from modules.utils import *
import numpy as np

# -----------------------------------------------------dd--
#   Vectors
# -----------------------------------------------------dd--
origin = [0, 0, 0]
vectors = np.array([
    [0.799319, -3.477045e-01, 0.490093],
    [0.852512, 9.113778e-16, -0.522708],
    [0.296422, 9.376042e-01, 0.181748],
])

fv1 = p.figure()
fv1_ax = fv1.add_subplot(111, projection='3d')
colors = ['r', 'g', 'b']
color_id = 0

for vector in vectors:
    vlength = np.linalg.norm(vector)
    fv1_q = fv1_ax.quiver(*origin, *vector, arrow_length_ratio=0.3 / vlength,
                          colors=[colors[color_id]],
                          pivot='tail', length=vlength)
    color_id += 1

fv1_ax.set_xlim([-4, 4])
fv1_ax.set_ylim([-4, 4])
fv1_ax.set_zlim([0, 4])
fv1_ax.set_xlabel('x')
fv1_ax.set_ylabel('y')
fv1_ax.set_zlabel('z')

# figure_to_pdf(fv1, "vectors")
