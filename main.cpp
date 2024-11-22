//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//  modified by Badiuzzaman on 3/11/24.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "basic_camera.h"


#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawTableChair(unsigned int VAO, Shader ourShader, glm::mat4 sm);
void drawFan(unsigned int VAO, Shader ourShader, glm::mat4 translateMatrix, glm::mat4 sm);

// draw object functions
void drawCube(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, float posX = 0.0, float posY = 0.0, float posz = 0.0, float rotX = 0.0, float rotY = 0.0, float rotZ = 0.0,float scX = 1.0, float scY = 1.0, float scZ=1.0);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 45.0;
float rotateAngle_Y = 45.0;
float rotateAngle_Z = 45.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 1.35, eyeY = 4.8, eyeZ = 10.0;
float lookAtX = 4.0, lookAtY = 4.0, lookAtZ = 6.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

bool on = false;

//birds eye
bool birdEye = false;
glm::vec3 cameraPos(-2.0f, 5.0f, 13.0f); 
glm::vec3 target(-2.0f, 0.0f, 5.5f);   
float birdEyeSpeed = 1.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    Shader constantShader("vertexShader.vs", "fragmentShaderV2.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //red
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,//green
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,//blue
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,//yellow
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f
    };*/
    /*unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };*/

    float cube_vertices[] = {
        
         0.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, // 0 - bottom-left-back
         0.5f,  0.0f,  0.0f, 1.0f, 0.2f, 1.0f, // 1 - bottom-right-back
         0.5f,  0.0f,  0.5f, 1.0f, 1.0f, 1.0f, // 2 - bottom-right-front
         0.0f,  0.0f,  0.5f, 0.3f, 1.0f, 1.0f, // 3 - bottom-left-front
         0.0f,  0.5f,  0.0f, 1.0f, 1.0f, 0.4f, // 4 - top-left-back
         0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, // 5 - top-right-back
         0.5f,  0.5f,  0.5f, 0.5f, 1.0f, 1.0f, // 6 - top-right-front
         0.0f,  0.5f,  0.5f, 1.0f, 0.5f, 1.0f  // 7 - top-left-front
    };

    unsigned int cube_indices[] = {
        // back face
        1,5,0,
        5,4,0,

        // front face
        2,6,3,
        6,7,3,

        // left face
        7,3,0,
        4,7,0,

        // right face
        6,2,1,
        5,6,1,

        // bottom face
        1,0,3,
        2,0,3,

        // Top face
        5,4,7,
        6,5,7
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ourShader.use();
    //constantShader.use();

    float r = 0.0f;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(basic_camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        //constantShader.setMat4("projection", projection);

        // camera/view transformation

        glm::mat4 view;

        if (birdEye) {
            glm::vec3 up(0.0f, 1.0f, 0.0f);
            view = glm::lookAt(cameraPos, target, up);
        }
        else {
            view = basic_camera.createViewMatrix();
        }

        //glm::mat4 view = basic_camera.createViewMatrix();
        ourShader.setMat4("view", view);
        //constantShader.setMat4("view", view);
        glm::mat4 identityMatrix = glm::mat4(1.0f);
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, modelCentered,
            translateMatrixprev;
        translateMatrix = identityMatrix;
        // drawTableChair(VAO, ourShader, identityMatrix);

        int i,j;
        float z = 0.0f;

        for (i = 1;i <= 4;i++)
        {
            for (j = 1;j <= 4;j++)
            {
                drawTableChair(VAO, ourShader, translateMatrix);
                translateMatrix *= glm::translate(identityMatrix, glm::vec3(-1.5f, 0.0f, 0.0f));
            }
            z += 2.5f;
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, z));
            
            //drawTableChair(VAO, ourShader, translateMatrix);
            
        }

        //desk
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -1));
        rotateXMatrix = glm::rotate(translateMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateZMatrix = glm::rotate(rotateXMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        drawTableChair(VAO, ourShader, rotateZMatrix);

        
        //floor
        translateMatrix *= glm::translate(identityMatrix, glm::vec3(-5.0f, -1.05f, -4.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(13.0f, 0.1, 30.0));
        model = translateMatrix* scaleMatrix;
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.65, 0.70, 0.73, 1.0));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //left wall
        translateMatrix *= glm::translate(identityMatrix, glm::vec3(0.0, 0.05, 0.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 12.0f, 30.0));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.75, 0.73, 0.71, 1.0));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        translateMatrixprev = translateMatrix;
        //front wall
        //translateMatrix *= glm::translate(identityMatrix, glm::vec3(0.05, 0.0, 0.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(13.0f, 12.0f, -0.1));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.9, 0.87, 0.86, 1.0));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //right wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.47, -0.99, -5.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 12.0f, 30.0));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.75, 0.73, 0.71, 1.0));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //board
        translateMatrix= translateMatrixprev* glm::translate(identityMatrix, glm::vec3(1.25, 1.5, 0.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(8.0f, 5.0f, 0.1));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //roof
        translateMatrix = translateMatrixprev * glm::translate(identityMatrix, glm::vec3(0.0, 6.0, 0.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(13.0f, 0.1f, 30.0));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.9, 0.9, 0.9, 1.0));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //fan stick
        translateMatrixprev = translateMatrix;
        translateMatrix= translateMatrixprev * glm::translate(identityMatrix, glm::vec3(3.0, 0.0, 3.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -3.0f, 0.2));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //drawFan(VAO, ourShader, translateMatrix, identityMatrix);

        //fan rotation 
        translateMatrixprev = translateMatrix;
        glm::mat4 translateMatrix2, translateMatrixBack,test;

        translateMatrix2 = glm::translate(identityMatrix, glm::vec3(-1.95, 3.5, -1.95));
        translateMatrixBack = glm::translate(identityMatrix, glm::vec3(1.95, -3.5, 1.95));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0, 1.0, 0.0));
        model = translateMatrixBack * rotateYMatrix * translateMatrix2;
        drawFan(VAO, ourShader, translateMatrix, rotateYMatrix);

        

        if (on)
        {
            r += 1;
        }
        else
        {
            r = 0.0f;
        }
        
       /* translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.05, -1.5, 0.05));
       
        translateMatrix2= glm::translate(identityMatrix, glm::vec3(0.25, 0.0, 0.25));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0, 1.0, 0.0));
        translateMatrix2Rev= glm::translate(identityMatrix, glm::vec3(-0.25, 0.0, -0.25));

        model = rotateYMatrix * translateMatrix;
        drawFan(VAO, ourShader, translateMatrixprev, model);
        r += 1;*/
        // drawCube(ourShader, VAO, identityMatrix, translate_X, translate_Y, translate_Z, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, scale_X, scale_Y, scale_Z);

        // render boxes
        //for (unsigned int i = 0; i < 10; i++)
        //{
        //    // calculate the model matrix for each object and pass it to shader before drawing
        //    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //    model = glm::translate(model, cubePositions[i]);
        //    float angle = 20.0f * i;
        //    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //    drawCube(ourShader, VAO, model);
        //}

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

void drawTableChair(unsigned int VAO, Shader ourShader, glm::mat4 sm)
{
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, modelCentered;

    //table
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 0.2, 2.0));
    model = sm*scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(1.0, 0.875, 0.737, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //left-back leg
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, -2.0, 0.2));
    model = sm * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.71, 0.58, 0.41, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //lef-middle leg
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.1));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, -1.8, 1.6));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(1.0, 0.8, 0.7, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //left-front leg
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, .9));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, -2.0, 0.2));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.71, 0.58, 0.41, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //right-front leg
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9, 0.0, 0.9));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, -2.0, 0.2));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.71, 0.58, 0.41, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //right-back leg
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9, 0.0, 0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, -2.0, 0.2));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.71, 0.58, 0.41, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //right-middle leg
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9, 0.0, 0.1));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, -1.8, 1.6));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(1.0, 0.8, 0.7, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //down part of table
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.1, -0.8, 0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.599, -0.2, 2.0));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.86, 0.73, 0.56, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //cpu
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.1, -0.8, 0.2));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, 0.8, .5));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //pc holder
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.4, 0.1, 0.2));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.6, 0.1, .3));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //pc stick
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, 0.1, 0.2));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.2, 0.5, .1));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //pc
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.325, 0.2, 0.25));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.9, 0.7, .1));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.16, 0.2, 0.25, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //keyboard
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25, 0.1, 0.6));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.8, 0.05, .3));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //mouse
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.7, 0.1, 0.65));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, 0.05, .2));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.1, 0.0, 0.0, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //chair seat
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3, -0.5, 1.5));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0, 0.1, 1.0));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.1, 0.5, 0.5, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //chair leg left-back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3, -0.5, 1.5));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, -1.0, .1));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.7, 0.5, 0.8, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //chair leg left-front
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3, -0.5, 1.95));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, -1.0, .1));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.7, 0.5, 0.8, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //chair leg right-front
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.75, -0.5, 1.95));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, -1.0, .1));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.7, 0.5, 0.8, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //chair leg right-back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.75, -0.5, 1.5));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, -1.0, .1));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.7, 0.5, 0.8, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //chair top leg left
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.425, -0.45, 1.95));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, 0.5, .1));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.7, 0.5, 0.8, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //chair top leg right
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.625, -0.45, 1.95));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, 0.5, .1));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.7, 0.5, 0.8, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //chair back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.35, -0.325, 1.9));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.8, 0.5, .1));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.6, 0.5, 0.5, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawFan(unsigned int VAO, Shader ourShader, glm::mat4 translateMatrix,  glm::mat4 sm)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, modelCentered, translateMatrixprev;
    glm::mat4 middleTranslate, leftBladeTranslate, frontBladeTranslate, rightBladeTranslate, backBladeTranslate;
    //fan middle part
    //translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(-0.2, -1.5, -0.2));
    middleTranslate= glm::translate(identityMatrix, glm::vec3(-0.2, -1.5, -0.2));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, -0.3f, 1.0));
    model = translateMatrix * sm * middleTranslate* scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.6, 0.4, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    translateMatrixprev = translateMatrix;
    //left fan
    //translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0, -0.075, 0.0));
    leftBladeTranslate = glm::translate(identityMatrix, glm::vec3(-0.2, -1.55, -0.2));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.0f, -0.15f, 1.0));
    model = translateMatrix * sm * leftBladeTranslate * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.6, 0.5, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //front fan
    //translateMatrix = translateMatrixprev * glm::translate(identityMatrix, glm::vec3(0.0, -0.075, 0.5));
    frontBladeTranslate = glm::translate(identityMatrix, glm::vec3(-0.2, -1.55, 0.3));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, -0.15f, 2.0));
    model = translateMatrix * sm * frontBladeTranslate * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.6, 0.5, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //right fan
    //translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.5, 0.0, 0.0));
    rightBladeTranslate= glm::translate(identityMatrix, glm::vec3(0.3, -1.55, 0.3));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, -0.15f, -1.0));
    model = translateMatrix * sm * rightBladeTranslate * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.6, 0.5, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //back fan
    //translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0, 0.0, -0.5));
    backBladeTranslate = glm::translate(identityMatrix, glm::vec3(0.3, -1.55, -0.2));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.0f, -0.15f, -2.0));
    model = translateMatrix * sm * backBladeTranslate * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.6, 0.5, 1.0));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.01;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.01;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.01;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.01;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.01;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.01;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.01;

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) on = true;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) on = false;

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) birdEye = true;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) birdEye = false;
    

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 1;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 1;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 1;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.eye = glm::vec3(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.eye = glm::vec3(eyeX, eyeY, eyeZ);

        //cout << "x: "<<eyeX << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.eye = glm::vec3(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.eye = glm::vec3(eyeX, eyeY, eyeZ);
        //cout << "z: " << eyeZ << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.eye = glm::vec3(eyeX, eyeY, eyeZ);
        //cout << "y: " << eyeY << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.eye = glm::vec3(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        lookAtX += 2.5 * deltaTime;
        basic_camera.lookAt = glm::vec3(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        lookAtX -= 2.5 * deltaTime;
        basic_camera.lookAt = glm::vec3(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        lookAtY += 2.5 * deltaTime;
        basic_camera.lookAt = glm::vec3(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        lookAtY -= 2.5 * deltaTime;
        basic_camera.lookAt = glm::vec3(lookAtX, lookAtY, lookAtZ);
    }

    if (birdEye) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            cameraPos.z -= birdEyeSpeed * deltaTime; 
            target.z -= birdEyeSpeed * deltaTime;
            if (cameraPos.z <= 4.0) {
                cameraPos.z = 4.0;
            }
            
            if (target.z <= -3.5) {
                target.z = -3.5;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            cameraPos.z += birdEyeSpeed * deltaTime; 
            target.z += birdEyeSpeed * deltaTime;
            /*cout << "tgt: " << target.z << endl;
            cout << "pos: " << cameraPos.z << endl;*/
            if (cameraPos.z >= 13.5) {
                cameraPos.z = 13.5;
            }
            if (target.z >= 6.0) {
                target.z = 6.0;
            }
        }
    }

    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    basic_camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    basic_camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void drawCube(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, float posX, float posY, float posZ, float rotX , float rotY, float rotZ, float scX, float scY, float scZ)
{
    shaderProgram.use();

    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, modelCentered;
    translateMatrix = glm::translate(parentTrans, glm::vec3(posX, posY, posZ));
    rotateXMatrix = glm::rotate(translateMatrix, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(rotateXMatrix, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(rotateYMatrix, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(rotateZMatrix, glm::vec3(scX, scY, scZ));
    modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", modelCentered);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}