# Project Specification

## 2D Graphics Package Implementation

### Overview

The goal of this project is to implement a 2D graphics package with a variety of features for creating and manipulating graphical elements. The package will include a graphical window where users can interact using the mouse, along with a set of drawing and filling algorithms for different shapes. Additionally, the package will support file operations for saving and loading data.

### Requirements

1. **Window Appearance:**
   - Implement a drop-down list for the following window appearance requirements:
     a. Change the background of the window to white.

2. **Window Interaction:**
   - Allow users to interact with the window using the mouse only.
   - Try to change the shape of the window mouse.

3. **Console Integration:**
   - Allow a combination of interaction between the graphical window and console.

4. **Drawing Options:**
   - Provide options in the menu to choose the shape color before drawing.

5. **Screen Manipulation:**
   - Implement an item in the menu to clear the screen from drawn shapes.
   - Implement a save function to save all data on the screen.
   - Implement a load function to load data from files.

6. **Line Algorithms:**
   - Implement line algorithms: DDA, Midpoint, and Parametric.

7. **Circle Algorithms:**
   - Implement circle algorithms: Direct, Polar, Iterative Polar, Midpoint, and Modified Midpoint.
   - Allow filling the circle with lines after taking filling quarter input from the user.
   - Allow filling the circle with other circles after taking filling quarter input from the user.

8. **Square and Rectangle Filling:**
   - Fill a square with a Hermit Curve (Vertical).
   - Fill a rectangle with a Bezier Curve (Horizontal).

9. **Convex and Non-Convex Filling:**
   - Implement convex and non-convex filling algorithms.

10. **Flood Fill:**
    - Implement recursive and non-recursive flood fill algorithms.

11. **Spline Curve:**
    - Implement Cardinal Spline Curve.

12. **Ellipse Algorithms:**
    - Implement ellipse algorithms: Direct, Polar, and Midpoint.

13. **Clipping Algorithms:**
    - Implement clipping algorithms using a rectangle as the clipping window for point, line, and polygon.
    - Implement clipping algorithms using a square as the clipping window for point and line.
