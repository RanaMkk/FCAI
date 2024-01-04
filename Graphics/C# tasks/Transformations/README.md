# Project Specifications

## Overview
The project involves creating a Windows Forms application for 2D transformations. The application allows users to input coordinates of a shape and apply various transformations such as translation, scaling, rotation, reflection, and shearing.

## Technologies Used
- C#
- Windows Forms

## Classes and Components
### Form1
- Inherits from the Form class.
- Represents the main form of the application.

### Fields
- `Dx` and `Dy`: Represent translation distances.
- `Points`: 2D array to store the coordinates of the shape.
- `Translation`: 3x3 matrix for translation transformations.
- `Scalling`: Array for scaling transformations.
- `Reflection`: Array for reflection transformations.
- `Shearing`: Array for shearing transformations.
- `angle`: Variable to store the rotation angle.


  
To run this C# code, you'll need a development environment with the necessary tools installed. Here are the steps to run the provided code using Visual Studio, a popular integrated development environment (IDE) for C#:

1. **Install Visual Studio:**
   - Download and install Visual Studio from the official Microsoft website: [Visual Studio Downloads](https://visualstudio.microsoft.com/downloads/)

2. **Open Visual Studio:**
   - Launch Visual Studio after installation.

3. **Create a New Project:**
   - Choose "Create a new project."
   - Select "Windows Forms App (.NET Framework)" as the project template.

4. **Configure the Project:**
   - Choose a project name and location.
   - Click "Create."

5. **Replace Code:**
   - In the Solution Explorer, find the automatically generated Form file (e.g., `Form1.cs`).
   - Replace the existing code in `Form1.cs` with the provided C# code.

6. **Run the Application:**
   - Press `F5` or click the "Start Debugging" button (green triangle) to build and run the application.

7. **Interact with the Application:**
   - The application window will appear.
   - Input coordinates for the shape in the textboxes.
   - Check transformation checkboxes and provide transformation parameters.
   - Click the "Apply Transformations" button to see the results in the chart.

8. **Close the Application:**
   - Close the application by stopping debugging (`Shift + F5`) or closing the window.

Note: Make sure your system has the necessary .NET Framework version installed to support the Windows Forms application.

If you encounter any issues or errors during the process, refer to the Visual Studio documentation or seek assistance on relevant forums.
