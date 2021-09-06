
#define GLEW_STATIC
#include "GL/glew.h"


#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include <windows.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <time.h>

#define SCREEN_WIDTH 1920   
#define SCREEN_HEIGHT 1080

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void DrawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength);

void drawTorus(double, double, int, int, int);

void drawTorus(double r = 0.07, double c = 0.15,
    int rSeg = 16, int cSeg = 8,
    int texture = 0)
{
    glFrontFace(GL_CW);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    const double PI = 3.1415926535897932384626433832795;
    const double TAU = 2 * PI;

    for (int i = 0; i < rSeg; i++) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= cSeg; j++) {
            for (int k = 0; k <= 1; k++) {
                double s = (i + k) % rSeg + 0.5;
                double t = j % (cSeg + 1);

                double x = (c + r * cos(s * TAU / rSeg)) * cos(t * TAU / cSeg);
                double y = (c + r * cos(s * TAU / rSeg)) * sin(t * TAU / cSeg);
                double z = r * sin(s * TAU / rSeg);

                double u = (i + k) / (float)rSeg;
                double v = t / (float)cSeg;

                glTexCoord2d(u, v);
                glNormal3f(2 * x, 2 * y, 2 * z);
                glVertex3d(2 * x, 2 * y, 2 * z);
            }
        }
        glEnd();
    }

    glFrontFace(GL_CCW);
}
GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
int f = 2;

int main(void)
{
    GLFWwindow* window;
    srand(time(0));
    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    const GLfloat rotationSpeed = 10;

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glViewport(0.0f, 0.0f, screenWidth, screenHeight); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT,0, 2000); // essentially set coordinate system
    glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity(); // same as above comment
    int step = 10;
    bool size = true;
    GLfloat halfScreenWidth = SCREEN_WIDTH / 2;
    GLfloat halfScreenHeight = SCREEN_HEIGHT / 2;
 

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Render OpenGL here
        int R = 150;
        int r = 50;
        glPushMatrix();
        glTranslatef(halfScreenWidth, halfScreenHeight, R*-1);
        glRotatef(rotationX, 1, 0, 0);
        glRotatef(rotationY, 0, 1, 0);
        glTranslatef(-halfScreenWidth, -halfScreenHeight,R);
       
                for(int b=0;b<360;b+=30)
                    for(int c=-180;c<180;c+=1)
            {
                        DrawCube(halfScreenWidth + (R + r * cos(c * 3.14 / 180)) * cos(b * 3.14 / 180), halfScreenHeight + (R + r * cos(c * 3.14 / 180)) * sin(b * 3.14 / 180), r* sin(c), f);
                
     
            }
       
                drawTorus();
        if (size) f += 1;
        else f -= 1;
        if (f > 1 || f < 1) size = !size;
        glPopMatrix();


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
        Sleep(1000.0f/60);
    }

    glfwTerminate();

    return 0;
}



void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //std::cout << key << std::endl;

    const GLfloat rotationSpeed = 10;

    // actions are GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            rotationX -= rotationSpeed;
            break;
        case GLFW_KEY_DOWN:
            rotationX += rotationSpeed;
            break;
        case GLFW_KEY_RIGHT:
            rotationY += rotationSpeed;
            break;
        case GLFW_KEY_LEFT:
            rotationY -= rotationSpeed;
            break;
        }



    }
}


void DrawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength)
{
    GLfloat halfSideLength = edgeLength * 0.5f;

    GLfloat vertices[] =
    {
        // front face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left

        // back face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top left
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom left

        // left face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left

        // right face
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left

        // top face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // bottom left

        // top face
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength  // bottom left
    };
    float colour[3] = {1.0f,1.0f,1.0f };
    //colour[0] = (float)(rand() % 255) / 255.0f;
    //colour[1] = (float)(rand() % 255) / 255.0f;
    //colour[2] = (float)(rand() % 255) / 255.0f;
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f( colour[0],colour[1],colour[2] );
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glDrawArrays(GL_TRIANGLES, 0, 24);

    glDisableClientState(GL_VERTEX_ARRAY);
}