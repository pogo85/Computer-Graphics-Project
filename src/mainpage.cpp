/*! \mainpage Drawing A Pleasant Scene In OpenGL
 *
 * \section intro_sec Introduction
 *
 * This is the documentation for Computer Graphics Assignment 2.<br>
 * A bedroom scene is drawn OpenGL, GLFW and GLAD libraries and coded in C++ .<br>
 * <br>
 * Group members are :<br>
 **                 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2018A7PS0201H -- <b>AKSHAY TIWARI</b><br>
 **                 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2018B5A70477H -- <b>MUSHKAN SUREKHA</b><br>
 **                 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2018A4PS0982H -- <b>MAHESH SWAMINATHAN</b><br>
 *<br>
 * \subsection install_sec How To Run This : 
 * 1. Make sure you have latest GLAD and GLFW files (Search on google for setup guides on these).<br>
 * 2. Similarly set up GLM.<br>
 * 3. Put the glad.c file with the main files.<br>
 * 4. To compile - open command line.<br>
 * 5. Run this command - g++ -o A_Safe_Spot <list of all .cpp and .c files> -Idir<Relative path of include directories of GLAD GLFW AND GLM> -L<Relative path to all library directories> <br>
 * 6. Run A_Safe_Spot.exe<br>
 * \subsection guide_sec How To Move Camera : 
 * Program allows two types of movement - <b>Free Camera Movement</b> and <b>'Restricted' Camera  Movement</b><br>
 * Movement mode can be toggled using I key.<br>
 * \subsection freemov 1. Free Camera Movement : 
 * This is the initial movement mode.<br>
 * Use your mouse to look around the scene.<br>
 * Use W and S keys to move camera forward and backward.<br>
 * Use A and D keys to move camera left and right.<br>
 * \subsection freemov 2. 'Restricted' Camera Movement : 
 * W and S to zoom.<br>
 * A and D to pan.<br>
 * Q and E to roll the camera (rotate around Z-axis). <br>
 * UP and DOWN Arrow keys to pitch the camera (rotate around X-axis). <br>
 * LEFT and RIGHT Arrow keys to yaw the camers (rotate around Y-axis). 
 * 
 * \section images Sample Images : 
 * \image html View_One.png "Bed" width=1000cm
 * <br>
 * \image html View_Two.png "Table" width=1000cm
 * <br>
 * \image html View_Three.png "Shelf" width=1000cm
 * <br>
 * \image html View_Four.png "Bed_Front" width=1000cm
 * <br>
 * \section time Time Analysis : 
 * 1. Loading Objects using loadObjs() : 1048 milliseconds<br>
 * 2. Initializing VBOs using initVBO() : 701 milliseconds<br>
 * 3. Initializing VAO using initVAO() : 426 milliseconds<br>
 * 4. Initializing Lights using initLights() : 54 milliseconds<br>
 * 5. Computing Matrices using computeMatricesFromInputs() : 18 milliseconds<br>
 * 6. Drawing Objects using drawObjects() : 1466 milliseconds<br>
 * \section issues General Issues In Coding : 
 * 1. Switching from Old OpenGL to Modern OpenGL took some time.<br>
 * 2. GLFW and GLAD provided significant help but were difficult to set up.<br>
 * 3. Importing objects using self-written parser and online tutorials also took some time. Objects were being imported but there were holes initially.<br>
 * 4. We have included lighting in our scene. Setting up fragment and vertex shaders was also a bit confusing.
 * 5. Concept of VAO and VBO was quite confusiong at the start.
 * 6. Implementing camera controls was probably the toughest but most satisfying task.
 * \section remarks Remarks
 * This assignment was much more interesting and required more effort than assignment 1.<br>
 * Working with modern OpenGL was really interesting as we got to know more about the 3D Pipeline by actually implementing it from scratch.<br>
 * We thank our instructor Prof. Tathagata Ray for giving us this opportunity to learn and helping us learn the 3D Pipeline.
 */