# Lines Drawing Application

## Overview
This Windows Forms application allows users to draw lines using two different algorithms: Bresenham's Line Drawing Algorithm and Digital Differential Analyzer (DDA). Users can input the coordinates of two points and choose the algorithm they want to use to draw the line.
![image](https://github.com/RanaMkk/FCAI/assets/99143413/ea27f7f1-9ecd-457c-bb2d-440bc4330221)

## Technologies Used
- C#
- Windows Forms

## Classes and Components
### Form1
- Inherits from the Form class.
- Represents the main form of the application.

### Methods
- `GetOctant(int x1, int y1, int x2, int y2)`: Determines the octant in which the line lies based on the coordinates of two points.
- `Swap(int x, int y)`: Swaps the values of x and y in an array.
- `button1_Click_1(object sender, EventArgs e)`: Draws a line using Bresenham's algorithm or DDA, based on user selection.
- `button2_Click_1(object sender, EventArgs e)`: Clears the chart and input fields.

### UI Components
- Labels: Provide information and labels for input fields.
- Textboxes: Allow users to input coordinates of two points.
- Checkboxes: Select the line drawing algorithm (Bresenham's or DDA).
- Buttons: Trigger actions like drawing lines and clearing the chart.
- Chart: Visualizes the drawn lines.

## Line Drawing Logic
1. **Bresenham's Algorithm**
   - Determines the octant in which the line lies.
   - Calculates the necessary parameters (e.g., dx, dy, p0) based on the octant.
   - Plots the points along the line in the specified octant.

2. **Digital Differential Analyzer (DDA)**
   - Calculates the slope of the line.
   - Determines the number of steps based on the maximum difference in x or y.
   - Increments x and y to draw points along the line.

## User Interface
- Textboxes for entering coordinates of two points.
- Checkboxes for selecting the line drawing algorithm (Bresenham's or DDA).
- Buttons for drawing lines and clearing the chart.
- Chart for visualizing the drawn lines.

## Usage
1. Enter coordinates of two points in the respective textboxes.
2. Choose the line drawing algorithm using checkboxes.
3. Click the "Draw Line" button to visualize the drawn line.
4. Click the "Clear Chart" button to reset the chart and input fields.

## Reset
- The "Clear Chart" button resets the chart and input fields.
- Users can choose between Bresenham's and DDA algorithms for line drawing.

## Notes
- The application supports drawing lines in different octants.
- Users can choose between two different line drawing algorithms.

## Future Enhancements
- Additional line drawing algorithms.
- Improved user feedback and visualization.
- Error handling for invalid inputs.

## How to Run the Application
1. Ensure that you have the .NET Framework installed on your machine.
2. Clone or download the project files.
3. Open the project in Visual Studio or your preferred C# development environment.
4. Build and run the application.
5. Input coordinates of two points, choose the algorithm, and click the "Draw Line" button to see the result.
6. Use the "Clear Chart" button to reset the chart and input fields.
