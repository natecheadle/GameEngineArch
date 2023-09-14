# ---
# jupyter:
#   jupytext:
#     custom_cell_magics: kql
#     formats: ipynb,py:percent
#     text_representation:
#       extension: .py
#       format_name: percent
#       format_version: '1.3'
#       jupytext_version: 1.11.2
#   kernelspec:
#     display_name: .venv
#     language: python
#     name: python3
# ---

# %% [markdown]
# Seperating Axis Theorem is an algorithm for doing collision detection. This notebook provides a summary and examples of an implementation for that theorem.
#
# The following sources were used as a reference:
#
# - [2D Collision Detection](https://developer.mozilla.org/en-US/docs/Games/Techniques/2D_collision_detection)
# - [Hyperplane Separation Theorem](https://en.wikipedia.org/wiki/Hyperplane_separation_theorem)
# - [Separating Axis Theorem (SAT) Explanation](https://www.sevenson.com.au/programming/sat/)
#
# The first step is to define a perpendicular vector function.
#
# Two vectors are perpendicular if their dot product is zero.
#
# The dot product of a vector is defind as the following
#
# $$
# \vec{a_n} \cdot \vec{b_n} = \sum_{i=0}^{n} a_i * b_i
# $$
#
# therefore we can solve for an arbitrary perpendicular vector by setting all the terms except one to 1.0 and solving.
#

# %%
import ipywidgets as widgets

x_val = widgets.FloatSlider(min=-1.0, max=1.0, value=1.0, step=0.1, description="VEC_X", orientation='horizontal', readout=True, readout_format='.1f',)
y_val = widgets.FloatSlider(min=-1.0, max=1.0, value=0.0, step=0.1, description="VEC_Y", orientation='horizontal', readout=True, readout_format='.1f',)

display(x_val)
display(y_val)


# %%
import numpy as np
import matplotlib.pyplot as plt


def mag(vec : np.array) -> float:
    return np.sqrt(np.sum(vec*vec))

def make_unit_vector(vec : np.array) -> np.array:
    dist = mag(vec)
    vec = vec / dist
    return vec

def get_perp(vec : np.array) -> np.array:
    if(vec.size != 2):
        raise ValueError("vec must be of size 2")
    
    x = vec[0]
    y = vec[1]
    
    y_perp = 1.0
    x_perp = 1.0
    if x != 0:   
        x_perp = -y / x
    else:
        y_perp = -x / y
        
    rslt = np.array([x_perp, y_perp])
    
    return make_unit_vector(rslt)

vec1 = make_unit_vector(np.array([x_val.value, y_val.value]))
vec_perp = get_perp(vec1)

origin = np.array([[0, 0],[0,0]]) # origin point

plt.quiver(*origin, [vec1[0],vec_perp[0]], [vec1[1], vec_perp[1]], color=['r','b'], scale=5)
plt.show()



# %% [markdown]
# Before jumping to seperating axis theorem we can use some simpler algorithms to detect collisions for rectangles if the axes are aligned. This is called the axis aligned bounding box method. It is essentially a simplied special case of seperating axis theorem.

# %%

def get_rotation_matrix_2d(angle : float) -> np.array:
    return np.array([[np.cos(angle),np.sin(angle)],[-np.sin(angle), np.cos(angle)]])

class Shape:
    
    def _set_corners(self, value: np.array):
        self._corners = value
        
    def _get_corners(self) -> np.array:
        return self._corners
    
    def _get_test_axes(self) -> np.array:
        axes = []
        for i in range(1, len(self._corners)):
            axes.append(get_perp(self._corners[i] - self._corners[i-1]))
        
        axes.append(get_perp(self._corners[len(self._corners) - 1] - self._corners[0]))
        return axes
    
    def __init__(self, corners : np.array):
        self._corners = corners
        
    
    Corners : np.array = property(fget=_get_corners)
    TestAxes : np.array = property(fget=_get_test_axes)

class Rectangle(Shape):
    
    @staticmethod
    def _get_rect_corners(length : float, width : float, rotation : float, center : np.array) -> np.array:
        corners = np.array([[width / 2.0, length / 2.0],[-width / 2.0, length / 2.0],[-width / 2.0, -length / 2.0],[width / 2.0, -length / 2.0]])
        
        if rotation != 0.0:
            rot_mat = get_rotation_matrix_2d(rotation)
            for corner in corners:
                temp = [[corner[0]],[corner[1]]]
                temp = np.matmul(rot_mat, temp)
                corner[0] = temp[0][0]
                corner[1] = temp[1][0]
                
                
        if center != [0.0, 0.0]:
            for corner in corners:
                corner += center
        
        return corners
    
    def __init__(self, length : float = 1.0, width : float  = 1.0, rotation : float = 0.0, center : np.array = [0.0,0.0]):
        Shape.__init__(self, corners = Rectangle._get_rect_corners(length, width, rotation, center))
        self._length = length
        self._width = width
        self._rotation = rotation
        self._center = center
            
    def _get_length(self) -> float:
        return self._length
    
    def _get_width(self) -> float:
        return self._width
    
    def _get_rotation(self) -> float:
        return self._rotation
    
    def _get_center(self) -> np.array:
        return self._center
    
    def _set_length(self, value : float):
        self._length = value
        self._set_corners(self._get_rect_corners(value, self._width, self._rotation, self._center))
        
    def _set_width(self, value : float):
        self._width = value
        self._set_corners(self._get_rect_corners(self._length, value, self._rotation, self._center))
        
    def _set_rotation(self, value : float):
        self._rotation = value
        self._set_corners(self._get_rect_corners(self._length, self._width, value, self._center))
        
    def _set_center(self, value : np.array):
        self._center = value
        self._set_corners(self._get_rect_corners(self._length, self._width, self._rotation, value))
        
    Length : float = property(fget=_get_length, fset=_set_length)
    Width : float = property(fget=_get_width, fset=_set_width)
    Rotation : float = property(fget=_get_rotation, fset=_set_rotation)
    Center : np.array = property(fget=_get_center, fset=_set_center)

def test_collision_axis_aligned_box(rect1 : Rectangle, rect2 : Rectangle) -> bool:
    if rect1.Rotation != 0.0 or rect2.Rotation != 0.0:
        raise ValueError("rotation angle must be zero to use this algorithm.")
    
    colX = rect1.Center[0] + rect1.Length / 2.0 >= rect2.Center[0] - rect2.Length / 2.0 and rect2.Center[0] + rect2.Length / 2.0 >= rect1.Center[0] - rect1.Length / 2.0
    colY = rect1.Center[1] + rect1.Width / 2.0 >= rect2.Center[1] - rect2.Width / 2.0 and rect2.Center[1] + rect2.Width / 2.0 >= rect1.Center[1] - rect1.Width / 2.0
    
    return colX and colY
    

rect_size = widgets.FloatSlider(min=0.0, max=5.0, value=1.0, step=1.0, description="RECT_SIZE", orientation='horizontal', readout=True, readout_format='.1f',)
rect1_x = widgets.FloatSlider(min=-5.0, max=5.0, value=0.0, step=0.1, description="RECT1_X", orientation='horizontal', readout=True, readout_format='.1f',)
rect1_y = widgets.FloatSlider(min=-5.0, max=5.0, value=0.0, step=0.1, description="RECT1_Y", orientation='horizontal', readout=True, readout_format='.1f',)
rect2_x = widgets.FloatSlider(min=-5.0, max=5.0, value=0.0, step=0.1, description="RECT2_X", orientation='horizontal', readout=True, readout_format='.1f',)
rect2_y = widgets.FloatSlider(min=-5.0, max=5.0, value=0.0, step=0.1, description="RECT2_Y", orientation='horizontal', readout=True, readout_format='.1f',)

display(rect_size)
display(rect1_x)
display(rect1_y)
display(rect2_x)
display(rect2_y)


# %%
from matplotlib.patches import Polygon
from matplotlib.collections import PatchCollection

def plot_aabb_method():
    
    rect1 = Rectangle(length=rect_size.value, width=rect_size.value, rotation=0.0, center=[rect1_x.value, rect1_y.value])
    rect2 = Rectangle(length=rect_size.value, width=rect_size.value, rotation=0.0, center=[rect2_x.value, rect2_y.value])

    rect1_points = rect1.Corners
    rect2_points = rect2.Corners
    
    p1 = Polygon(rect1_points, facecolor='red', closed=True)
    p2 = Polygon(rect2_points, facecolor='red', closed=True)
    
    if not test_collision_axis_aligned_box(rect1, rect2):
        p1 = Polygon(rect1_points, facecolor='green', closed=True)
        p2 = Polygon(rect2_points, facecolor='green', closed=True)
        
        
    fig,ax = plt.subplots()

    ax.add_patch(p1)
    ax.add_patch(p2)
        
    ax.set_xlim([-10,10])
    ax.set_ylim([-10,10])
    plt.show()
    
plot_aabb_method()

# %% [markdown]
# To do seperating axis theorem we will first need to create a helper function that gets the corners of a shape projected onto an axis.
#
# To do this we will solve a simple system of linear equations to find the intersection point of an arbitrary axis and a line perpendicular to that axis and crossing through a corner.
#
# $$
#
# let\;c = corner\\
# ax(x) = m_{ax} * x + 0.0\\
# ax_{perp}(x) = -(m_{ax}) * (x - c_x) + c_y\\
# $$
#
# intersection occurs when f(x) == ax(x)
#
# $$
# \\
# -(m_{ax}) * (x - c_x) + c_y = m_{ax} * x + 0.0\\
# x = (m_{ax} * c_x + c_y) / (2.0 * m_{ax})
#
# $$
#

# %%
rect_len = widgets.FloatSlider(min=0.0, max=5.0, value=1.0, step=1.0, description="RECT_LEN", orientation='horizontal', readout=True, readout_format='.1f',)
rect_width = widgets.FloatSlider(min=0.0, max=5.0, value=1.0, step=1.0, description="RECT_WIDTH", orientation='horizontal', readout=True, readout_format='.1f',)
rect_rot = widgets.FloatSlider(min=-180, max=180.0, value=0.0, step=10.0, description="RECT_ROT", orientation='horizontal', readout=True, readout_format='.1f',)
rect_x = widgets.FloatSlider(min=-5.0, max=5.0, value=0.0, step=0.1, description="RECT_X", orientation='horizontal', readout=True, readout_format='.1f',)
rect_y = widgets.FloatSlider(min=-5.0, max=5.0, value=0.0, step=0.1, description="RECT_Y", orientation='horizontal', readout=True, readout_format='.1f',)
testaxis_x = widgets.FloatSlider(min=0.0, max=5.0, value=1.0, step=1.0, description="TEST_AXIS_X", orientation='horizontal', readout=True, readout_format='.1f',)
testaxis_y = widgets.FloatSlider(min=0.0, max=5.0, value=1.0, step=1.0, description="TEST_AXIS_Y", orientation='horizontal', readout=True, readout_format='.1f',)

display(rect_len)
display(rect_width)
display(rect_rot)
display(rect_x)
display(rect_y)
display(testaxis_x)
display(testaxis_y)


# %%
def get_projected_points(shape : Shape, axis : np.array) -> np.array:
    corners = shape.Corners
    points = np.zeros([len(corners), 2])
    
    axis_perp = get_perp(axis)
    
    for i in range(0, len(corners)):
        x = 0.0
        y = 0.0
        if axis[0] != 0.0 and axis[1] != 0.0:
            m = axis[1] / axis[0]
            m2 = axis_perp[1] / axis_perp[0]
            x = (-m2 * corners[i][0] + corners[i][1]) / (m - m2)
            y = m * x
            
        elif axis[0] == 0.0:
            y = corners[i][1]
        else:
            x = corners[i][0]
            
        points[i][0] = x
        points[i][1] = y

    return points

def plot_shape_projected_on_line():
    rect = Rectangle(length=rect_len.value, width=rect_width.value, rotation=rect_rot.value * np.pi / 180.0, center=[rect_x.value,rect_y.value])
    
    rect_points = rect.Corners
    testaxis = np.array([testaxis_x.value,testaxis_y.value])
    
    intersection_points = get_projected_points(rect, testaxis)
    
    poly = Polygon(rect.Corners,  facecolor='green', closed=True)
    
    fig,ax = plt.subplots()
    ax.add_patch(poly)
    plt.axline([0,0], testaxis, color='b')
    for i in range(len(intersection_points)):
        point = [intersection_points[i][0], intersection_points[i][1]]
        plt.plot(point[0], point[1], 'ro')
        
    ax.set_xlim([-10,10])
    ax.set_ylim([-10,10])
    plt.show()
    
plot_shape_projected_on_line()


# %% [markdown]
# To do seperating axis theorem our axes we will project one will be one for each side of the polygon. In the case of parallelograms we can skip parallel sides. 
#
# Onces all the points are projected we sort them according to their order on the axis. If there is overlap of points from two shapes that indicates their might be a collision and we should keep testing.
#
# If there is overlap on all test axes that means there is a collision. If there is no overlap on a single axis that means there is no collision.

# %%
from functools import cmp_to_key

def sort_point_x(point1, point2):
    if point1[1][0] < point2[1][0]:
        return -1
    if point1[1][0] > point2[1][0]:
        return 1
    return 0

def sort_point_y(point1, point2):
    if point1[1][1] < point2[1][1]:
        return -1
    if point1[1][1] > point2[1][1]:
        return 1
    return 0

def sat_collision( shape1 : Shape,  shape2 :Shape) -> bool :
    test_axes =  shape1.TestAxes
    test_axes2  = shape2.TestAxes
    for axis in test_axes2:
        test_axes.append(axis)
    
    for axis in test_axes:
        points1 = get_projected_points(shape1, axis)
        points2 = get_projected_points(shape2, axis)
        
        all_points = []
        for point in points1:
            all_points.append((shape1, point))
        for point in points2:
            all_points.append((shape2, point))
        
        if abs(axis[0]) > abs(axis[1]):
            all_points.sort(key=cmp_to_key(sort_point_x))
        else:
            all_points.sort(key=cmp_to_key(sort_point_y))
        
        overlap = False
        firstShape = all_points[0][0]
        for i in range(1, len(firstShape.Corners)):
            if all_points[i][0] is not firstShape:
                overlap = True
                break
        #no collision on this test axis return false
        if not overlap:
            return False
    
    #made it to the end with all axes having a collision
    return True

sat_rect_size = widgets.FloatSlider(min=0.0, max=5.0, value=1.0, step=1.0, description="RECT_SIZE", orientation='horizontal', readout=True, readout_format='.1f',)

rect3_rot = widgets.FloatSlider(min=-180, max=180.0, value=0.0, step=10.0, description="RECT3_ROT", orientation='horizontal', readout=True, readout_format='.1f',)
rect3_x = widgets.FloatSlider(min=-5.0, max=5.0, value=0.0, step=0.1, description="RECT3_X", orientation='horizontal', readout=True, readout_format='.1f',)
rect3_y = widgets.FloatSlider(min=-5.0, max=5.0, value=0.0, step=0.1, description="RECT3_Y", orientation='horizontal', readout=True, readout_format='.1f',)

rect4_rot = widgets.FloatSlider(min=-180, max=180.0, value=0.0, step=10.0, description="RECT4_ROT", orientation='horizontal', readout=True, readout_format='.1f',)
rect4_x = widgets.FloatSlider(min=-5.0, max=5.0, value=0.0, step=0.1, description="RECT4_X", orientation='horizontal', readout=True, readout_format='.1f',)
rect4_y = widgets.FloatSlider(min=-5.0, max=5.0, value=0.0, step=0.1, description="RECT4_Y", orientation='horizontal', readout=True, readout_format='.1f',)

test_axis = widgets.IntSlider(min=0, max=7, value=0, step=1, description="TEST_AXIS", orientation='horizontal', readout=True,)

display(sat_rect_size)
display(rect3_rot)
display(rect3_x)
display(rect3_y)
display(rect4_rot)
display(rect4_x)
display(rect4_y)
display(test_axis)
        

# %%
def test_plot_sat_collision():
    rect3 = Rectangle(length = sat_rect_size.value, width=sat_rect_size.value, rotation=rect3_rot.value * np.pi / 180.0, center=[rect3_x.value, rect3_y.value])
    rect4 = Rectangle(length = sat_rect_size.value, width=sat_rect_size.value, rotation=rect4_rot.value * np.pi / 180.0, center=[rect4_x.value, rect4_y.value])
    
    rect3_points = rect3.Corners
    rect4_points = rect4.Corners
    
    p1 = Polygon(rect3_points, facecolor='red', closed=True)
    p2 = Polygon(rect4_points, facecolor='red', closed=True)
    
    if not sat_collision(rect3, rect4):
        p1 = Polygon(rect3_points, facecolor='green', closed=True)
        p2 = Polygon(rect4_points, facecolor='green', closed=True)

    test_axes =  rect3.TestAxes
    test_axes2  = rect4.TestAxes
    for axis in test_axes2:
        test_axes.append(axis)
    
    points1 = get_projected_points(rect3, test_axes[test_axis.value])
    points2 = get_projected_points(rect4, test_axes[test_axis.value])
    
    fig,ax = plt.subplots()
    
    plt.axline([0,0], test_axes[test_axis.value], color='b')
    
    all_points =[]
    for point in points1:
        all_points.append(point)
    for point in points2:
        all_points.append(point)
    
    for point in all_points:
        plt.plot(point[0], point[1], 'ro')
        

    ax.add_patch(p1)
    ax.add_patch(p2)
        
    ax.set_xlim([-10,10])
    ax.set_ylim([-10,10])
    plt.show()
    
test_plot_sat_collision()
