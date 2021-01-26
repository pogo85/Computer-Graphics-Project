/**
    * File:    Source.cpp
    *
    * Author1:  Akshay Tiwari (2018A7PS0201H)
    * Author2:  Mushkan Surekha  (2018B5A70477H)
    * Author3:  Mahesh Swaminathan  (2018A4PS0982H)
    * Date:     09-04-2020
    * Course:   Computer Graphics (IS F311)
    *
    * Summary of File:
    *
    *   Program to display a pleasant bedroom scene using GLFW and GLAD.
    *	Also has movement with either mouse or keyboard inputs as desired by the user.
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
using namespace std;
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderFile.h"
#include "objimporter.hpp"
#include "textureLoader.hpp"
#include "vboindexer.hpp"
#include "control.hpp"
const unsigned int SCR_WIDTH = 1920;///<Screen Width
const unsigned int SCR_HEIGHT = 1080;///<Screen Height
static int componentcount = 21;///<Number of imported components/obj files
GLFWwindow* window;///<Variable for context window
int movType;///<Movement Type - Press I to toggle
GLuint MatrixID;///<Matrix ID for shader program
GLuint ViewMatrixID;///<ViewMatrix ID for shader program
GLuint ModelMatrixID;///<ModelMatrix ID for shader program
std::vector<glm::vec4> elecount;///<Vector to store data about each component
std::vector<unsigned short> indices;///<Vector to store indicies of triangles to be plotted
std::vector<glm::vec3> indexed_vertices;///<Vector to stored indexed vertices
std::vector<glm::vec2> indexed_uvs;///<Vector to stored indexed uvs
std::vector<glm::vec3> indexed_normals;///<Vector to stored indexed normals
GLuint vertexbuffer;///<Vertex Buffer Object
GLuint uvbuffer;///<UV Buffer Object
GLuint normalbuffer;///<Normal Buffer Object
GLuint elementbuffer;///<Element (Indices) Buffer Object
GLuint ObjectVertexArrayID;///<Vertex Array Object
struct shape
{
    std::vector<glm::vec3> vertices;///<Vector to stored vertices
    std::vector<glm::vec2> uvs;///<Vector to stored uvs
    std::vector<glm::vec3> normals;///<Vector to stored normals
    GLuint Texture;///<Vector to stored texture
};
struct shape component[100];///<Shape structure to store components
glm::vec3 pointLightPositions[] = {
        glm::vec3(0.0f, 16.0f, -15.f),
        glm::vec3(-9.0f,  4.0f,  0.0f),
        glm::vec3(6.0f, 4.8f, -1.8f),
        glm::vec3(-6.0f, 4.8f, -1.8f)
};///<Light Positions in worldspace
/**
    *   void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    *
    *   Summary of framebuffer_size_callback(GLFWwindow* window, int width, int height) function:
    *
    *       Changes viewport width and height variables on resizing window
    *
    *   Parameters  :
    *
    *		GLFWwindow* window - current context window
    *       int height - height of window
    *       int width - width of window
    *
    *   Return Value :
    *
    *		None (Void function)
    *
    *   Description :
    *
    *       Changed viewport width and height variables on resizing window.
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
/**
    *   void processInput(GLFWwindow* window);
    *
    *   Summary of processInput function:
    *
    *       Terminates the program on ESC Button press
    *
    *   Parameters  :
    *
    *       GLFWwindow* window - Current context window
    *
    *   Return Value :
    *
    *       None (Void function)
    *
    *   Description :
    *
    *       Terminates the program on ESC Button press.
*/
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
/**
    *   void loadObjs();
    *
    *   Summary of loadObjs(); function:
    *
    *       Loads .obj files into corresponding vectors.
    *
    *   Parameters  :
    *   
    *       None
    *
    *   Return Value :
    *
    *       None (Void function)
    *
    *   Description :
    *
    *       loadObjs uses our simple parser to scan the .obj files into required
    *       vectors.
*/
void loadObjs() {
    bool loaded = loadOBJ("./Models/Cube.obj", component[0].vertices, component[0].uvs, component[0].normals);
    component[0].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/BedOneTwo.obj", component[1].vertices, component[1].uvs, component[1].normals);
    component[1].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/MatressTwo.obj", component[2].vertices, component[2].uvs, component[2].normals);
    component[2].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/Sheet.obj", component[3].vertices, component[3].uvs, component[3].normals);
    component[3].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/PillowOne.obj", component[4].vertices, component[4].uvs, component[4].normals);
    component[4].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/CubeArt.obj", component[5].vertices, component[5].uvs, component[5].normals);
    component[5].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/Floor.obj", component[6].vertices, component[6].uvs, component[6].normals);
    component[6].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/WallLampStand.obj", component[7].vertices, component[7].uvs, component[7].normals);
    component[7].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/WallLamp.obj", component[8].vertices, component[8].uvs, component[8].normals);
    component[8].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/CushionTwoOne.obj", component[9].vertices, component[9].uvs, component[9].normals);
    component[9].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/Pot.obj", component[10].vertices, component[10].uvs, component[10].normals);
    component[10].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/Leaf.obj", component[11].vertices, component[11].uvs, component[11].normals);
    component[11].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/Soil.obj", component[12].vertices, component[12].uvs, component[12].normals);
    component[12].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/TableTop.obj", component[13].vertices, component[13].uvs, component[13].normals);
    component[13].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/TableLegs.obj", component[14].vertices, component[14].uvs, component[14].normals);
    component[14].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/openbook.obj", component[15].vertices, component[15].uvs, component[15].normals);
    component[15].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/Chair.obj", component[16].vertices, component[16].uvs, component[16].normals);
    component[16].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/Shelf.obj", component[17].vertices, component[17].uvs, component[17].normals);
    component[17].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/BookCov.obj", component[18].vertices, component[18].uvs, component[18].normals);
    component[18].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/BookShelf.obj", component[19].vertices, component[19].uvs, component[19].normals);
    component[19].Texture = loadDDS("./Models/Sample.DDS");

    loaded = loadOBJ("./Models/TopLight.obj", component[20].vertices, component[20].uvs, component[20].normals);
    component[20].Texture = loadDDS("./Models/Sample.DDS");
}
/**
    *   int initialize()
    *
    *   Summary of initialize() function:
    *
    *       Initializes GLFW, GLAD and sets other GL variables
    *
    *   Parameters  :
    *
    *       None (Void function)
    *
    *   Return Value :
    *
    *       int - 1 if properly initialized; 0 otherwise
    *
    *   Description :
    *
    *       Initializes GLFW, output window, GLAD and sets other GL variables like z buffer test and input mode etc.
*/
int initialize() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "A Safe Spot", NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    movType = 1;
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);//For Key Input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//For Curs0r Movement
    glfwPollEvents();//Continously Checks For Input
    glfwSetCursorPos(window, 1920 / 2, 1080 / 2);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    glEnable(GL_CULL_FACE);
    return 1;

}
/**
    *   void initVBO() 
    *
    *   Summary of initVBO() function:
    *
    *       Initializes VBO and stored indexed vertices, uvs and normal in respective VBOs
    *
    *   Parameters  :
    *
    *       None
    *
    *   Return Value :
    *
    *       None (Void Funtion)
    *
    *   Description :
    *
    *       Initializes VBO and stored indexed vertices, uvs and normal in respective VBOs.
    *       First it indexed the vertices,uvs and normals and stores in the index array and then initializes the buffers.
*/
void initVBO() {

    for (int i = 0;i < componentcount;i++)
    {
        indexVBO(component[i].vertices, component[i].uvs, component[i].normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
        elecount.push_back(glm::vec4(indexed_vertices.size(), indexed_uvs.size(), indexed_normals.size(), indices.size()));
    }

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

}
/**
    *   void initVAO()
    *
    *   Summary of initVAO() function:
    *
    *       Initializes VAO and sets the required attribute pointers
    *
    *   Parameters  :
    *
    *       None
    *
    *   Return Value :
    *
    *       None (Void Funtion)
    *
    *   Description :
    *
    *       Initializes VAO and sets the required attribute pointers i.e. pointer to vertex buffer, uv buffer and normal buffer.
    *       Also initializes the element buffer object and stores the data in it.
*/
void initVAO() {
    glGenVertexArrays(1, &ObjectVertexArrayID);
    glBindVertexArray(ObjectVertexArrayID);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elecount[componentcount - 1][3] * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}
/**
    *   void initLights(Shader shaderProg)
    *
    *   Summary of initLights(Shader shaderProg) function:
    *
    *       Initializes the light variables required.
    *
    *   Parameters  :
    *
    *       Shader shaderProg - The shaderProg variable
    *
    *   Return Value :
    *
    *       None (Void function)
    *
    *   Description :
    *
    *       Initializes the light variables required i.e. position,power and color of the lights.
*/
void initLights(Shader shaderProg) {
    shaderProg.setVec3("LightPosition_worldspace", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    shaderProg.setVec3("LightPosition1_worldspace", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
    shaderProg.setVec3("LightPosition2_worldspace", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
    shaderProg.setVec3("LightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    shaderProg.setFloat("LightPower", 10.0f);
    shaderProg.setFloat("LightPower1", 2.0f);
    shaderProg.setFloat("LightPower2", 2.0f);
    shaderProg.setVec3("MaterialAmbientColorConst", glm::vec3(0.2, 0.2, 0.2));
}
/**
    *   void drawObjects(Shader shaderProg,glm::mat4 ProjectionMatrix,glm::mat4 ViewMatrix)
    *
    *   Summary of drawObjects(Shader shaderProg,glm::mat4 ProjectionMatrix,glm::mat4 ViewMatrix) function:
    *
    *       Draws the objects using the VBOs
    *
    *   Parameters  :
    *
    *       Shader shaderProg - The shaderProg variable
    *       glm::mat4 ProjectionMatrix - The projection matrix
    *       glm::mat4 ViewMatrix - The view matrix
    *
    *   Return Value :
    *
    *       None (Void function)
    *
    *   Description :
    *
    *       Draws the objects using the VBOs after applying required transfomations to get the desired result.
    *       Objects can be drawn any number of times by using correct parameters.
*/
void drawObjects(Shader shaderProg,glm::mat4 ProjectionMatrix,glm::mat4 ViewMatrix) {
    //Wall
        //FRONT
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1, 0.6, 0));
        shaderProg.setFloat("alpha", 1.0f);
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 10.0f, -30.0f)) * glm::scale(ModelMatrix, glm::vec3(15.0f, 10.0f, 0.02f));
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1, 1, 1));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.7f, -29.96f)) * glm::scale(ModelMatrix, glm::vec3(15.0f, 0.7f, 0.04f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.5f, -29.92f)) * glm::scale(ModelMatrix, glm::vec3(15.0f, 0.5f, 0.04f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);
        //FRONT-END

        //RIGHT
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1, .1, .7));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(15.0f, 10.0f, -15.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(15.0f, 10.0f, 0.02f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1, 1, 1));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(14.96f, 0.7f, -15.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(15.0f, 0.7f, 0.04f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(14.92f, 0.5f, -15.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(15.0f, 0.5f, 0.04f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);
        //RIGHT-END

        //LEFT
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1, .5, .7));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-15.0f, 10.0f, -15.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(15.0f, 10.0f, 0.02f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);
        
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1, 1, 1));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.96f, 0.7f, -15.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(15.0f, 0.7f, 0.04f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.92f, 0.5f, -15.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(15.0f, 0.5f, 0.04f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);
        //LEFT-END

        //BACK
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1, 0.6, 0));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 10.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(15.0f, 10.0f, 0.02f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1, 1, 1));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.7f, -0.04f)) * glm::scale(ModelMatrix, glm::vec3(15.0f, 0.7f, 0.04f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.5f, -0.08f)) * glm::scale(ModelMatrix, glm::vec3(15.0f, 0.5f, 0.04f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);
        //BACK-END
    //WALL-END

    //CEILING
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, .8392, .7491));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 20.0f, -15.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(15.0f, 15.0f, 0.02f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);
    //CEILING-END

    //FLOOR
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(.808, .73333, 0.6196));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.01f, -15.0f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[6][3] - elecount[5][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[5][3]));
    //FLOOR-END

    //BED
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(.804, 0.6667, .4902));
        shaderProg.setFloat("alpha", 1.0f);
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.15f, -6.5f)) * glm::scale(ModelMatrix, glm::vec3(1.2f, 1.2f, 1.2f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[1][3] - elecount[0][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[0][3]));
    //BED-END

    //MATRESS
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(.973, .973, 1.0));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.15f, -6.5f)) * glm::scale(ModelMatrix, glm::vec3(1.2f, 1.2f, 1.2f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[2][3] - elecount[1][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[1][3]));
    //MATRESS-END

    //SHEET
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, .8588, .8588));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.15f, -6.5f)) * glm::scale(ModelMatrix, glm::vec3(1.2f, 1.2f, 1.2f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[3][3] - elecount[2][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[2][3]));
    //SHEET-END

    //PILLOW
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(.83, .83, .83));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-0.3f, 1.91f, -1.6f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[4][3] - elecount[3][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[3][3]));       
    //PILLOW-END

    //WALLART
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(.83, .83, .83));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 4.81f, -1.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[5][3] - elecount[4][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[4][3]));

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1.0f, 5.31f, -1.2f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(1.2f, 1.2f, 1.2f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[5][3] - elecount[4][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[4][3]));

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1.1f, 9.81f, -1.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[5][3] - elecount[4][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[4][3]));

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(1.4f, 6.31f, -1.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[5][3] - elecount[4][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[4][3]));

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(2.4f, 8.31f, -1.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[5][3] - elecount[4][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[4][3]));

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.3f, 6.62f, -1.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.5f, 1.5f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[5][3] - elecount[4][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[4][3]));

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-2.3f, 9.81f, -1.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 0.4f, 0.4f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[5][3] - elecount[4][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[4][3]));

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-0.75f, 10.61f, -1.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 0.4f, 0.4f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[5][3] - elecount[4][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[4][3]));

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-0.80f, 12.61f, -1.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 0.4f, 0.4f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[5][3] - elecount[4][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[4][3]));

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(2.75f, 10.61f, -1.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 0.4f, 0.4f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[5][3] - elecount[4][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[4][3]));

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(1.9f, 8.51f, -1.0f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.2f, 1.2f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[5][3] - elecount[4][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[4][3]));
    //WALLART-END

    //LAMPS
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.533, 0.435, 0.247));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(6.0f, 2.0f, -1.2f)) * glm::scale(ModelMatrix, glm::vec3(0.55f, 0.8f, 0.8f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[7][3] - elecount[6][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[6][3]));

        shaderProg.setVec3("MaterialAmbientColorConst", glm::vec3(0.1, 0.1, 0.1));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(.9647, 0.804, .5451));
        shaderProg.setFloat("alpha", 1.0f);
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(6.0f, 2.5f, -1.2f)) * glm::scale(ModelMatrix, glm::vec3(0.6f, 0.6f, 0.6f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[8][3] - elecount[7][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[7][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.533, 0.435, 0.247));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-6.5f, 2.0f, -1.2f)) * glm::scale(ModelMatrix, glm::vec3(0.55f, 0.8f, 0.8f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[7][3] - elecount[6][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[6][3]));

        shaderProg.setVec3("MaterialAmbientColorConst", glm::vec3(0.1, 0.1, 0.1));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(.9647, 0.804, .5451));
        shaderProg.setFloat("alpha", 1.0f);
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-6.5f, 2.5f, -1.2f)) * glm::scale(ModelMatrix, glm::vec3(0.6f, 0.6f, 0.6f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[8][3] - elecount[7][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[7][3]));
    //LAMPS-END

    //CUSHIONS
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(2.50f, 2.29f, -1.6f)) * glm::rotate(ModelMatrix, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(ModelMatrix, glm::radians(-40.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(1.2f, 1.2f, 1.2f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[9][3] - elecount[8][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[8][3]));

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-3.05f, 2.29f, -1.6f)) * glm::rotate(ModelMatrix, glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(ModelMatrix, glm::radians(-40.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(1.2f, 1.2f, 1.2f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[9][3] - elecount[8][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[8][3]));
    //CUSHIONS-END

    //POTS
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.627, 0.4, 0.172));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-13.05f, 0.0f, -1.6f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[10][3] - elecount[9][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[9][3]));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(13.05f, 0.0f, -1.6f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[10][3] - elecount[9][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[9][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.376, 0.2, 0.082));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(13.05f, 0.0f, -1.6f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[12][3] - elecount[11][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[11][3]));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-13.05f, 0.0f, -1.6f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[12][3] - elecount[11][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[11][3]));
    //POTS-END

    //LEAF
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.207, 0.509, 0.227));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(13.05f, 1.0f, -1.6f)) * glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[11][3] - elecount[10][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[10][3]));
    
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-13.05f, 1.0f, -1.6f)) * glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[11][3] - elecount[10][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[10][3]));
    //LEAF-END

    //TABLE
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.753, 0.258, 0.152));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(10.05f, 0.0f, -26.8f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[13][3] - elecount[12][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[12][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.753, 0.753, 0.753));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(10.05f, 0.00f, -26.8f)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[14][3] - elecount[13][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[13][3]));
    //TABLE-END

    //CHAIR
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.376, 0.2, 0.082));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(10.05f, 0.40f, -23.8f)) * glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0)) * glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[16][3] - elecount[15][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[15][3]));
    //CHAIR-END
        
    //BOOK
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.176, 0.278, 0.823));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(10.05f, 4.27f, -25.8f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(0.4f, 0.01f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(10.05f, 4.37f, -25.8f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(0.4f, 0.01f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);

        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(9.35f, 4.32f, -25.8f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(0.4f, 0.05f, 0.026f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);
        
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(10.05f, 4.32f, -25.8f)) * glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(0.35f, 0.03f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)elecount[0][3], GL_UNSIGNED_SHORT, (void*)0);
    //BOOK-END

    //SHELF
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.376, 0.2, 0.082));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, 0.2f, -24.5f))* glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[17][3] - elecount[16][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[16][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.815, 0.443, 0.470));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, 0.0f, -24.5f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.454, 0.815, 0.443));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, 0.0f, -24.8f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.815, 0.443, 0.470));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, 0.0f, -25.1f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.105, 0.258, 0.101));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, 0.0f, -25.4f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.101, 0.337, 0.458));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, 0.0f, -25.7f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.894, 0.066, 0.517));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, 0.0f, -26.0f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.066, 0.270, 0.270));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, 0.0f, -26.3f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.137, 0.203, 0.164));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, 0.0f, -26.6f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.360, 0.541, 0.435));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, 0.0f, -26.9f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.596, 0.733, 0.207));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, 0.0f, -27.2f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.815, 0.443, 0.470));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, -3.0f, -29.8f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.054, 0.184, 0.180));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, -3.0f, -30.1f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.815, 0.443, 0.470));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f,-3.0f, -30.4f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.105, 0.258, 0.101));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, -3.0f, -30.7f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.101, 0.337, 0.458));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, -3.0f, -31.0f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.470, 0.827, 0.819));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, -3.0f, -31.3f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.180, 0.101, 0.796));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, -3.0f, -31.6f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.611, 0.250, 0.392));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, -3.0f, -31.9f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.862, 0.549, 0.313));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, -3.0f, -32.2f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.243, 0.290, 0.109));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, -3.0f, -32.5f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.054, 0.184, 0.180));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, -2.9f, -17.4f)) * glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.815, 0.443, 0.470));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, -3.4f, -17.4f)) * glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));

        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.105, 0.258, 0.101));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-14.55f, -3.15f, -17.4f)) * glm::rotate(ModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[18][3] - elecount[17][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[17][3]));
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(1.0, 1.0, 1.0));
        glDrawElements(GL_TRIANGLES, (int)(elecount[19][3] - elecount[18][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[18][3]));
    //SHELF-END

    //LIGHTTOP
        shaderProg.setVec3("MaterialDiffuseColor", glm::vec3(0.784, 0.835, 0.835));
        ModelMatrix = glm::mat4(1.0);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 19.2f, -15.f)) * glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, (int)(elecount[20][3] - elecount[19][3]), GL_UNSIGNED_SHORT, (void*)(2 * (int)elecount[19][3]));
    //LIGHTTOP-END
    glClearColor(0.0549f, 0.30196f, 0.57255f, 0.0f);
}
/**
    *   int main()
    *
    *   Summary of main() function:
    *
    *       main function of our program
    *
    *   Parameters  :
    *
    *       None
    *
    *   Return Value :
    *
    *       int - exit code
    *
    *   Description :
    *
    *       main function of our program. Always the first to run.
    *       Calls all other functions and has the main display loop.
*/
int main()
{
    if (initialize() < 0)
        return -1;

    Shader shaderProg("vShader.vertexshader", "fShader.fragmentshader");
    MatrixID = glGetUniformLocation(shaderProg.ID, "MVP");
    ViewMatrixID = glGetUniformLocation(shaderProg.ID, "V");
    ModelMatrixID = glGetUniformLocation(shaderProg.ID, "M");
    shaderProg.use();
    
    loadObjs();
    initVBO();
    initVAO();
    initLights(shaderProg);

    while (!glfwWindowShouldClose(window))
    {

        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

        computeMatricesFromInputs(window,&movType);
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        drawObjects(shaderProg, ProjectionMatrix, ViewMatrix);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}