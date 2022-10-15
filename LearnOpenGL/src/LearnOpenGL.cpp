// LearnOpenGL.cpp : Defines the entry point for the application.
//

#define GLEW_STATIC
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

   

using namespace std;

// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;

const char* TITLE = "OpenGL";


const char* pathVertexShader = "E:\\Projects\\LearnOpenGL\\LearnOpenGL\\shaders\\vertex.glsl";
const char* pathFragmentShader = "E:\\Projects\\LearnOpenGL\\LearnOpenGL\\shaders\\fragment.glsl";
const char* pathModelObj = "E:\\Projects\\LearnOpenGL\\LearnOpenGL\\models\\nanosuit\\nanosuit.obj";

GLfloat mCurrentFactor = 0.0f;

// 模型矩阵, 变换模型到世界坐标
glm::mat4 model(1.0f);

// 光源模型举证
glm::mat4 modelForLight(1.0f);

// 光源位置
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// 观察矩阵又或者叫摄像机矩阵
glm::mat4 view(1.f);

// 投影举证
glm::mat4 projection(1.f);

//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// 平衡处理速度
float deltaTime = 0.0f;  // 当前帧与上一帧德时间差
float lastFrameTime = 0.0f; // 上一帧德时间

// 鼠标的上一帧移动位置
float lastX = WIDTH / 2, lastY = HEIGHT / 2;

// 欧拉角
//float pitch = 0.0f;// 俯仰角
//float yaw = -90.0f; // 偏航角 初始化为-90度时因为 初始化时我们希望朝向世界坐中z轴的负方向, 因为屏幕面向我们的方向是z轴正方向                                                                                                  
//float roll = 0.0f; // 滚转角

// 视野
//float fov = 45.0f;

// 处理第一次焦点时的 鼠标事件
bool firstMouseEvent = true;

int mWidth = WIDTH, mHeight = HEIGHT;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    mWidth = width;
    mHeight = height;
}

static void setUpModelMatrix()
{
    // 将标准矩阵 围绕x轴 旋转-55度角度
    //model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));// translate it down so it at center of scene
    model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
}

static void setUpProjectionMatrix()
{
    float width = mWidth;
    float height = mHeight;
    // 这个投射矩阵 已经包含了 右手坐标得变换, 投射矩阵构建在左手坐标中, 而openGL NDC系统是右手坐标系统,因此投射矩阵不仅要处理投射变换,还有处理左右坐标系变换
    // 默认情况下 glm 库已经处理了这个变换,无需自己编写
    // 这里构建投射矩阵得传递参数也都是 按照左手坐标系来设定得,比如0.1f 表示z周正方向上得距离观察点得坐标位置(观察点是原点), 100.f也是如此
    // 但是最后生成得projection 已经包含 左右坐标系统变换得内容
    projection = glm::perspective(glm::radians(camera.Zoom), width / height, 0.1f, 100.f);
}

static void setUpCameraMatrix()
{
    // 世界坐标系中得点
   // glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 3.f);
    // 世界坐标系得远点
  //  glm::vec3 cameraTarget = glm::vec3(0.0f, 0.f, 0.f);
    // 虽然叫摄像机方向,但其实是 摄像机方向得反方向,因为我们希望得到一个执行 z轴正方的向量
  //  glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

    // 世界坐标系的 上向量
  //  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // 利用 向量叉积 的向量垂直与 两个向量这个特性
    // 注意叉积的先后顺序, 
  //  glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

    // 继续利用 向量叉积
  //  glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);


    view = camera.GetViewMatrix();
}

static void processInput(GLFWwindow* window)
{
    // if user press esc key

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {

        camera.ProcessKeyboard(BACKWARD, deltaTime);
        
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {   
        camera.ProcessKeyboard(LEFT, deltaTime);   
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
       camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}


static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (firstMouseEvent)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouseEvent = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 注意这里是相反的因为鼠标y轴是从底部到顶部依次增大
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(mWidth, mHeight, TITLE, nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    
    // setUp Model View Projection matrix
    setUpModelMatrix();
    setUpCameraMatrix();
    setUpProjectionMatrix();

    // setup Model
    Shader shader(pathVertexShader, pathFragmentShader);

    // load Models
    Model ourModel(pathModelObj);
    
    shader.use();
    // 设置定向光, 为由灯指向世界坐标原点
    shader.setVec3("dirLight.direction", lightPos.x, lightPos.y, lightPos.z);
    shader.setVec3("dirLight.ambient", 1.0f, 1.0f, 1.0f);
    shader.setVec3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader.setVec3("dirLight.specular", 0.3f, 0.3f, 0.3f); 
    shader.setMat4("model", model);
    shader.setMat4f("projection", glm::value_ptr(projection));


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        // Render
        // Clear the colorbuffer
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;
        processInput(window);

        setUpCameraMatrix();
       
        setUpProjectionMatrix();


        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        shader.use();
        shader.setMat4f("view", glm::value_ptr(view));
        shader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
        ourModel.Draw(shader);
 
        // Swap the screen buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDisable(GL_DEPTH_TEST);

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

	return 0;
}
