Curve Editor--Graphics
====================

This project is a curve editor homework assignment for my Graphics course. The curve editor, using OpenGL, lets the user add, delete, move, or change various types of curves. A curve can be selected, which is indicated by whether it has larger control points and is blue.

* Curves
  * Bezier
  * Lagrange
  * PolyLine
  * Catmull-Rom spline

* Usage (hold down the key)
  * 'b' - adds a Bezier Curve
  * 'l' - adds a Lagrange Curve
  * 'p' - adds a Polyline
  * 'r' - adds a Catmull-Rom spline
  * 'a' - add a control point to the selected curve
  * 'd' - delete the clicked control point from the curve

* Other features
  * space - if an object is selected, makes the next object selected instead. Cycles over all curves
  * curves are draggable (Lagrange, Polyline, and Bezier)
  * control points are draggable
  * the current number of curves is displayed in the corner using Bezier curves