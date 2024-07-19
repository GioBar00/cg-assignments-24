#include <glm/gtx/transform2.hpp>

void SetupProjectionMatrices(Assignment04 *A, float Ar) {
/**************
 Method Matrix(int N, glm::mat4 M) of object A,
 receives in N the number of the matrix that needs to be set, and the matrix in M
***************/

	glm::mat4 S;
    float zoom = 1.5f;
    float half_w = 10 / zoom;
    float near = -50;
    float far = 50;
    glm::mat4 Port = glm::scale(glm::mat4(1.0), glm::vec3(1,-1,1)) *
                        glm::ortho(-half_w, half_w, -half_w / Ar, half_w / Ar, near, far);
    
    
	// Isometric projection
	S = Port * glm::rotate(glm::mat4(1.0f), glm::radians(35.264f), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// Dimetric
    S = Port * glm::rotate(glm::mat4(1.0f), glm::radians(55.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// Trimetric
    S = Port * glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// Cabinet
    float angle = 60.0f;
    float red = 0.5f;
    S = Port * glm::shearZ3D(glm::mat4(1.0f), -red * glm::cos(glm::radians(angle)), -red * glm::sin(glm::radians(angle)));
	A->Matrix(4, S); // sets the matrix corresponding to piece 4

}
