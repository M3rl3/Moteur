#include "Callbacks.h"

void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        theEditMode = MOVING_CAMERA;
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        theEditMode = MOVING_SELECTED_OBJECT;
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        //theEditMode = TAKE_CONTROL;
        //cameraTarget = player_mesh->position;
        //cameraEye = player_mesh->position - glm::vec3(20.f, -4.f, 0.f);
    }
    // Wireframe
    if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        for (int i = 0; i < meshArray.size(); i++) {
            meshArray[i]->isWireframe = true;
        }
    }
    if (key == GLFW_KEY_X && action == GLFW_RELEASE) {
        for (int i = 0; i < meshArray.size(); i++) {
            meshArray[i]->isWireframe = false;
        }
    }
    if (key == GLFW_KEY_LEFT_ALT) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    /*
    *    updates translation of all objects in the scene based on changes made to scene
    *    description files, resets everything if no changes were made
    */
    if (key == GLFW_KEY_U && action == GLFW_PRESS) {
        ReadSceneDescription();
        player_mesh->particle->position = player_mesh->position;
    }
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        enableMouse = !enableMouse;
    }

    switch (theEditMode)
    {
    case MOVING_CAMERA:
    {
        const float CAMERA_MOVE_SPEED = 1.f;
        if (key == GLFW_KEY_A)     // Left
        {
            ::cameraEye.x -= CAMERA_MOVE_SPEED;
        }
        if (key == GLFW_KEY_D)     // Right
        {
            ::cameraEye.x += CAMERA_MOVE_SPEED;
        }
        if (key == GLFW_KEY_W)     // Forward
        {
            ::cameraEye.z += CAMERA_MOVE_SPEED;
        }
        if (key == GLFW_KEY_S)     // Backwards
        {
            ::cameraEye.z -= CAMERA_MOVE_SPEED;
        }
        if (key == GLFW_KEY_Q)     // Down
        {
            ::cameraEye.y -= CAMERA_MOVE_SPEED;
        }
        if (key == GLFW_KEY_E)     // Up
        {
            ::cameraEye.y += CAMERA_MOVE_SPEED;
        }

        if (key == GLFW_KEY_1)
        {
            ::cameraEye = glm::vec3(0.f, 0.f, -5.f);
        }
    }
    break;
    case MOVING_SELECTED_OBJECT:
    {
        const float OBJECT_MOVE_SPEED = 1.f;
        if (key == GLFW_KEY_A)     // Left
        {
            meshArray[object_index]->position.x -= OBJECT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_D)     // Right
        {
            meshArray[object_index]->position.x += OBJECT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_W)     // Forward
        {
            meshArray[object_index]->position.z += OBJECT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_S)     // Backwards
        {
            meshArray[object_index]->position.z -= OBJECT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_Q)     // Down
        {
            meshArray[object_index]->position.y -= OBJECT_MOVE_SPEED;
        }
        if (key == GLFW_KEY_E)     // Up
        {
            meshArray[object_index]->position.y += OBJECT_MOVE_SPEED;
        }

        // Cycle through objects in the scene
        if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        {
            if (!enableMouse) cameraTarget = glm::vec3(0.f, 0.f, 0.f);
        }
        if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        {
            object_index++;
            if (object_index > meshArray.size() - 1) {
                object_index = 0;
            }
            if (!enableMouse) cameraTarget = meshArray[object_index]->position;
        }
        if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        {
            object_index--;
            if (object_index < 0) {
                object_index = meshArray.size() - 1;
            }
            if (!enableMouse) cameraTarget = meshArray[object_index]->position;
        }
    }
    break;
    case TAKE_CONTROL: {
        if (key == GLFW_KEY_W) {
            player_mesh->velocity.x += 0.5f;
            player_mesh->rotation = glm::quat(glm::vec3(0.f, 67.55f, 0.f));
        }
        if (key == GLFW_KEY_S) {
            player_mesh->velocity.x -= 0.5f;
            player_mesh->rotation = glm::quat(glm::vec3(0.f, -67.55f, 0.f));
        }
        if (key == GLFW_KEY_A) {
            player_mesh->velocity.z -= 0.5f;
            player_mesh->rotation = glm::quat(glm::vec3(0.f, 0.f, 0.f));
        }
        if (key == GLFW_KEY_D) {
            player_mesh->velocity.z += 0.5f;
            player_mesh->rotation = glm::quat(glm::vec3(0.f, 135.10f, 0.f));
        }
        if (key == GLFW_KEY_Q) {
            player_mesh->velocity.y += 0.5f;
        }
        if (key == GLFW_KEY_E) {
            player_mesh->velocity.y -= 0.5f;
        }
    }
    break;
    }
    return;
}

void MouseCallBack(GLFWwindow* window, double xposition, double yposition) {

    if (firstMouse) {
        lastX = xposition;
        lastY = yposition;
        firstMouse = false;
    }

    float xoffset = xposition - lastX;
    float yoffset = lastY - yposition;  // reversed since y coordinates go from bottom to up
    lastX = xposition;
    lastY = yposition;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // prevent perspective from getting flipped by capping it
    if (pitch > 89.f) {
        pitch = 89.f;
    }
    if (pitch < -89.f) {
        pitch = -89.f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    if (enableMouse) {
        cameraTarget = glm::normalize(front);
    }
}

void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
    if (fov >= 1.f && fov <= 45.f) {
        fov -= yoffset;
    }
    if (fov <= 1.f) {
        fov = 1.f;
    }
    if (fov >= 45.f) {
        fov = 45.f;
    }
}

//read scene description files
void ReadSceneDescription() {
    std::ifstream File("sceneDescription.txt");
    if (!File.is_open()) {
        std::cerr << "Could not load file." << std::endl;
        return;
    }

    int number = 0;
    std::string input0;
    std::string input1;
    std::string input2;
    std::string input3;

    std::string temp;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    File >> number;

    for (int i = 0; i < number; i++) {
        File >> input0
            >> input1 >> position.x >> position.y >> position.z
            >> input2 >> rotation.x >> rotation.y >> rotation.z
            >> input3 >> scale.x >> scale.y >> scale.z;

        /*  long_highway
            position 0.0 -1.0 0.0
            rotation 0.0 0.0 0.0
            scale 1.0 1.0 1.0
        */

        temp = input0;

        if (input1 == "position") {
            meshArray[i]->position.x = position.x;
            meshArray[i]->position.y = position.y;
            meshArray[i]->position.z = position.z;
        }
        if (input2 == "rotation") {
            /*meshArray[i]->rotation.x = rotation.x;
            meshArray[i]->rotation.y = rotation.y;
            meshArray[i]->rotation.z = rotation.z;*/
            meshArray[i]->AdjustRoationAngleFromEuler(rotation);
        }
        if (input3 == "scale") {
            meshArray[i]->scale.x = scale.x;
            meshArray[i]->scale.y = scale.y;
            meshArray[i]->scale.z = scale.z;
        }
    }
    File.close();

    std::string yes;
    float x, y, z;
    std::ifstream File1("cameraEye.txt");
    if (!File1.is_open()) {
        std::cerr << "Could not load file." << std::endl;
        return;
    }
    while (File1 >> yes >> x >> y >> z) {
        ::cameraEye.x = x;
        ::cameraEye.y = y;
        ::cameraEye.z = z;
    }
    File1.close();
}