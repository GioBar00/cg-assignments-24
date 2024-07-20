

/**************
 Creae the meshes, as described below
 
 WARNING!
 Since it is a C program, you can use for loops and functions if you think they can be helpful in your solution.
 However, please include all your code in this file, since it will be put in an automatic correction process
 for the final evaluation. Please also be cautious when using standard libraries and symbols, since they
 might not be available in all the development environments (especially, they might not be available
 in the final evaluation environment, preventing your code from compiling).
 This WARNING will be valid far ALL THE ASSIGNMENTs, but it will not be repeated in the following texts,
 so please remember these advices carefully!
 
***************/

void MakeSquare(float size, std::vector<glm::vec3> &vertices, std::vector<uint32_t> &indices) {
// Creates a square, on the xz-plane, aligned with the axis, and centered in the origin.
// The length of the four sides is in parameter >size<.
// The procedure should fill the array contained in the >vertices< parameter, with the positions of
// the vertices of the primitive, expressed with their local coordinates in a glm::vec3 element. 
// Indices should be returned in the >indices< array, starting from 0, and up to vertices.size()-1.
// The primitive is encoded as an indexed triangle list, so the size of the >indices< array, should
// be a multiple of 3: each group of three indices, defines a different triangle.
//
// HINT: this procedure has already been implemented. You can keep it as is
    vertices = {
            {-size / 2.0f, 0.0f, -size / 2.0f},
            {-size / 2.0f, 0.0f, size / 2.0f},
            {size / 2.0f,  0.0f, -size / 2.0f},
            {size / 2.0f,  0.0f, size / 2.0f}};
    indices = {0, 1, 2, 1, 3, 2};
}

void MakeCube(float size, std::vector<glm::vec3> &vertices, std::vector<uint32_t> &indices) {
// Creates a cube, with the faces perpendicular to the axis, and centered in the origin.
// The length of one edge of the cube is >size<.
// The procedure should fill the array contained in the >vertices< parameter, with the positions of
// the vertices of the primitive, expressed with their local coordinates in a glm::vec3 element. 
// Indices should be returned in the >indices< array, starting from 0, and up to vertices.size()-1.
// The primitive is encoded as an indexed triangle list, so the size of the >indices< array, should
// be a multiple of 3: each group of three indices, defines a different triangle.
//
// HINT: the procedure below creates a square. You can use it as a side of the cube (please remember
// to change the value of the y component, otherwise the result will be wrong
    vertices = {
            {-size / 2.0f, -size / 2.0f, -size / 2.0f}, // 0
            {-size / 2.0f, -size / 2.0f, size / 2.0f}, // 1
            {size / 2.0f,  -size / 2.0f, -size / 2.0f}, // 2
            {size / 2.0f,  -size / 2.0f, size / 2.0f}, // 3

            {-size / 2.0f, size / 2.0f,  -size / 2.0f}, // 4
            {-size / 2.0f, size / 2.0f,  size / 2.0f}, // 5
            {size / 2.0f,  size / 2.0f,  -size / 2.0f}, // 6
            {size / 2.0f,  size / 2.0f,  size / 2.0f}, // 7
    };
    indices = {
            // bottom
            0, 2, 1,
            1, 2, 3,
            // top
            4, 5, 6,
            5, 7, 6,
            // front
            1, 3, 5,
            5, 3, 7,
            // back
            0, 4, 2,
            4, 6, 2,
            // left
            0, 1, 4,
            1, 5, 4,
            // right
            2, 6, 3,
            3, 6, 7
    };

}

void
MakeCylinder(float radius, float height, int slices, std::vector<glm::vec3> &vertices, std::vector<uint32_t> &indices) {
// Creates a cylinder, approximated by a prism with a base composed by a regular polygon with >slices< sides.
// The radius of the cylinder is >radius<, and it height is >height<. The cylinder has its centere
// in the origin, and spans half above and half below the plane that passes thorugh the origin.
// The top and bottom caps are are aligned with xz-plane and perpendicular to the y-axis.
// The procedure should fill the array contained in the >vertices< parameter, with the positions of
// the vertices of the primitive, expressed with their local coordinates in a glm::vec3 element. 
// Indices should be returned in the >indices< array, starting from 0, and up to vertices.size()-1.
// The primitive is encoded as an indexed triangle list, so the size of the >indices< array, should
// be a multiple of 3: each group of three indices, defines a different triangle.
//
// HINT: the procedure below creates a rectangle. You have to change it, or you will obtain a wrong result
// You should use a for loop, and you should start from the procedure to create a circle seen during the lesson

    vertices.resize((slices + 1) * 2);
    indices.resize(4 * 3 * slices);
    vertices[slices] = {0.0f, -height / 2.0f, 0.0f};
    vertices[slices + slices + 1] = {0.0f, height / 2.0f, 0.0f};
    for (int i = 0; i < slices; i++) {
        float ang = 2 * M_PI * (float) i / (float) slices;
        vertices[i] = glm::vec3(radius * cos(ang), -height / 2.0f, radius * sin(ang));
        vertices[i + slices + 1] = glm::vec3(radius * cos(ang), height / 2.0f, radius * sin(ang));

        indices[3 * i] = slices;
        indices[3 * i + 1] = i;
        indices[3 * i + 2] = (i + 1) % slices;

        indices[3 * i + 3 * slices] = slices + slices + 1;
        indices[3 * i + 2 + 3 * slices] = i + slices + 1;
        indices[3 * i + 1 + 3 * slices] = (i + 1) % slices + slices + 1;

        indices[3 * i + 3 * slices * 2] = i;
        indices[3 * i + 1 + 3 * slices * 2] = i + slices + 1;
        indices[3 * i + 2 + 3 * slices * 2] = (i + 1) % slices;

        indices[3 * i + 3 * slices * 3] = i + slices + 1;
        indices[3 * i + 1 + 3 * slices * 3] = (i + 1) % slices + slices + 1;
        indices[3 * i + 2 + 3 * slices * 3] = (i + 1) % slices;
    }

//    for (int i = 0; i < slices; i++) {
//        indices[3 * i + 3 * slices * 2] = i;
//        indices[3 * i + 1 + 3 * slices * 2] = i + slices + 1;
//        indices[3 * i + 2 + 3 * slices * 2] = (i + 1) % slices;
//
//        indices[3 * i + 3 + 3 * slices * 2] = i + slices + 1;
//        indices[3 * i + 4 + 3 * slices * 2] = (i + 1) % slices + slices + 1;
//        indices[3 * i + 5 + 3 * slices * 2] = (i + 1) % slices;
//    }
}

void MakeCone(float radius, float height, int slices,
              std::vector<glm::vec3> &vertices, std::vector<uint32_t> &indices) {
// Creates a cone, approximated by a pyramid with a base composed by a regular polygon with >slices< sides.
// The radius of the cone is >radius<, and it height is >height<. The cone has its centere
// in the origin, and spans half above and half below the plane that passes thorugh the origin.
// The bottom cap is aligned with xz-plane and perpendicular to the y-axis.
// The procedure should fill the array contained in the >vertices< parameter, with the positions of
// the vertices of the primitive, expressed with their local coordinates in a glm::vec3 element. 
// Indices should be returned in the >indices< array, starting from 0, and up to vertices.size()-1.
// The primitive is encoded as an indexed triangle list, so the size of the >indices< array, should
// be a multiple of 3: each group of three indices, defines a different triangle.
//
// HINT: the procedure below creates a triangle. You have to change it, or you will obtain a wrong result
// You should use a for loop, and you should start from the procedure to create a circle seen during the lesson
    vertices.resize(slices + 2);
    indices.resize(3 * slices * 2);
    vertices[slices] = {0.0f, -height / 2.0f, 0.0f};
    vertices[slices + 1] = {0.0f, height / 2.0f, 0.0f};
    for (int i = 0; i < slices; i++) {
        float ang = 2 * M_PI * (float) i / (float) slices;
        vertices[i] = glm::vec3(radius * cos(ang), -height / 2.0f, radius * sin(ang));

        indices[3 * i] = slices;
        indices[3 * i + 1] = i;
        indices[3 * i + 2] = (i + 1) % slices;

        indices[3 * i + 3 * slices] = slices + 1;
        indices[3 * i + 2 + 3 * slices] = i;
        indices[3 * i + 1 + 3 * slices] = (i + 1) % slices;
    }
}

void MakeSphere(float radius, int rings, int slices, std::vector<glm::vec3> &vertices, std::vector<uint32_t> &indices) {
// Creates a sphere, approximated by a poliedron composed by >slices<, and >rings< rings.
// The radius of the sphere is >radius<, and it is centered in the origin.
// The procedure should fill the array contained in the >vertices< parameter, with the positions of
// the vertices of the primitive, expressed with their local coordinates in a glm::vec3 element. 
// Indices should be returned in the >indices< array, starting from 0, and up to vertices.size()-1.
// The primitive is encoded as an indexed triangle list, so the size of the >indices< array, should
// be a multiple of 3: each group of three indices, defines a different triangle.
//
// HINT: the procedure below creates a circle. You have to change it, or you will obtain a wrong result
// You should use two nested for loops, one used to span across the rings, and the other that spans along
// the rings.
    /*vertices.resize(slices * rings + 2);
    indices.resize(2 * 3 * slices + 2 * 3 * slices * (rings - 1));
    vertices[slices * rings] = {0.0f, -radius, 0.0f};
    vertices[slices * rings + 1] = {0.0f, radius, 0.0f};

    float heightStep = 2 * radius / (float)rings;

    // bottom ring
    for (int i = 0; i < slices; i++) {
        float ang = 2 * M_PI * (float) i / (float) slices;
        float y = -radius + heightStep;
        float real_radius = sqrtf(1 - powf(y, 2)) * radius;
        vertices[i] = glm::vec3(real_radius * cos(ang), y, real_radius * sin(ang));
        indices[3 * i] = slices * rings;
        indices[3 * i + 1] = i;
        indices[3 * i + 2] = (i + 1) % slices;
    }

    for (int j = 1; j < rings - 1; j++) {
        float y = -radius + heightStep * (float)(j + 1);
        float real_radius = sqrtf(1 - powf(y, 2)) * radius;
        for (int i = 0; i < slices; i++) {
            float ang = 2 * M_PI * (float) i / (float) slices;
            vertices[slices * j + i] = glm::vec3(real_radius * cos(ang), y, real_radius * sin(ang));
            indices[3 * i + 3 * slices * (j - 1)] = slices * j + i;
            indices[3 * i + 1 + 3 * slices * (j - 1)] = slices * (j - 1) + i;
            indices[3 * i + 2 + 3 * slices * (j - 1)] = slices * (j - 1) + (i + 1) % slices;

            indices[3 * i + 3 + 3 * slices * (j - 1)] = slices * j + i;
            indices[3 * i + 4 + 3 * slices * (j - 1)] = slices * (j - 1) + (i + 1) % slices;
            indices[3 * i + 5 + 3 * slices * (j - 1)] = slices * j + (i + 1) % slices;
        }

    }

    // fill top
//    for (int i = 0; i < slices; i++) {
//        indices[3 * i + 3 * slices * (rings - 1)] = slices * rings + 1;
//        indices[3 * i + 2 + 3 * slices * (rings - 1)] = slices * (rings - 1) + i;
//        indices[3 * i + 1 + 3 * slices * (rings - 1)] = slices * (rings - 1) + (i + 1) % slices;
//    }

//    for (int j = 1; j < rings - 1; j++) {
//        for (int i = 0; i < slices; i++) {
//            float ang = 2 * M_PI * (float) i / (float) slices;
//            vertices[slices * j + i] = glm::vec3(radius * cos(ang), -radius + heightStep * (float)(j + 1), radius * sin(ang));
//            indices[6 * i + 3 * slices * (j - 1)] = slices * j + i;
//            indices[6 * i + 1 + 3 * slices * (j - 1)] = slices * (j - 1) + i;
//            indices[6 * i + 2 + 3 * slices * (j - 1)] = slices * (j - 1) + (i + 1) % slices;
//
//            indices[6 * i + 3 + 3 * slices * (j - 1)] = slices * j + i;
//            indices[6 * i + 4 + 3 * slices * (j - 1)] = slices * (j - 1) + (i + 1) % slices;
//            indices[6 * i + 5 + 3 * slices * (j - 1)] = slices * j + (i + 1) % slices;
//        }
//    }
//
//    // top ring
//    for (int i = 0; i < slices; i++) {
//        float ang = 2 * M_PI * (float) i / (float) slices;
//        vertices[slices * (rings - 1) + i] = glm::vec3(radius * cos(ang), radius - heightStep, radius * sin(ang));
//        indices[3 * i + 3 * slices * (rings - 1)] = slices * rings + 1;
//        indices[3 * i + 2 + 3 * slices * (rings - 1)] = slices * (rings - 1) + i;
//        indices[3 * i + 1 + 3 * slices * (rings - 1)] = slices * (rings - 1) + (i + 1) % slices;
//    }


//    for (int i = 0; i < slices; i++) {
//        float ang = 2 * M_PI * (float) i / (float) slices;
//        vertices[i] = glm::vec3(radius * cos(ang), radius * sin(ang), 0.0f);
//        indices[3 * i] = slices;
//        indices[3 * i + 1] = i;
//        indices[3 * i + 2] = (i + 1) % slices;
//    }*/

    // add top vertex
//    auto v0 = vertices.push_back({0, radius, 0});
//
//    // generate vertices per stack / slice
//    for (int i = 0; i < n_stacks - 1; i++)
//    {
//        auto phi = M_PI * double(i + 1) / double(n_stacks);
//        for (int j = 0; j < n_slices; j++)
//        {
//            auto theta = 2.0 * M_PI * double(j) / double(n_slices);
//            auto x = std::sin(phi) * std::cos(theta);
//            auto y = std::cos(phi);
//            auto z = std::sin(phi) * std::sin(theta);
//            mesh.add_vertex(Point(x, y, z));
//        }
//    }
//
//    // add bottom vertex
//    auto v1 = mesh.add_vertex(Point(0, -1, 0));
//
//    // add top / bottom triangles
//    for (int i = 0; i < n_slices; ++i)
//    {
//        auto i0 = i + 1;
//        auto i1 = (i + 1) % n_slices + 1;
//        mesh.add_triangle(v0, Vertex(i1), Vertex(i0));
//        i0 = i + n_slices * (n_stacks - 2) + 1;
//        i1 = (i + 1) % n_slices + n_slices * (n_stacks - 2) + 1;
//        mesh.add_triangle(v1, Vertex(i0), Vertex(i1));
//    }
//
//    // add quads per stack / slice
//    for (int j = 0; j < n_stacks - 2; j++)
//    {
//        auto j0 = j * n_slices + 1;
//        auto j1 = (j + 1) * n_slices + 1;
//        for (int i = 0; i < n_slices; i++)
//        {
//            auto i0 = j0 + i;
//            auto i1 = j0 + (i + 1) % n_slices;
//            auto i2 = j1 + (i + 1) % n_slices;
//            auto i3 = j1 + i;
//            mesh.add_quad(Vertex(i0), Vertex(i1),
//                          Vertex(i2), Vertex(i3));
//        }
//    }

    vertices.emplace_back(0.0f, radius, 0.0f);

    for (int ring = 1; ring <= rings - 1; ring++) {
        float phi = glm::pi<float>() * (float) ring / (float) rings;
        float y = cos(phi);
        float sin_phi = sin(phi);

        for (int slice = 0; slice <= slices; slice++) {
            float theta = 2 * glm::pi<float>() * (float) slice / (float) slices;
            float x = sin_phi * cos(theta);
            float z = sin_phi * sin(theta);

            vertices.emplace_back(radius * x, radius * y, radius * z);
        }
    }

    vertices.emplace_back(0.0f, -radius, 0.0f);

    // Top cap indices
    for (int slice = 0; slice < slices; ++slice) {
        indices.push_back(0);
        indices.push_back(1 + (slice + 1) % slices);
        indices.push_back(1 + slice);
    }

    // Middle indices
    for (int ring = 0; ring < rings - 2; ++ring) {
        for (int slice = 0; slice < slices; ++slice) {
            int first = 1 + (ring * slices) + slice;
            int second = first + slices + 1;

            indices.push_back(first);
            indices.push_back(first + 1);
            indices.push_back(second);

            indices.push_back(second);
            indices.push_back(first + 1);
            indices.push_back(second + 1);
        }
    }
    // fill remaining space in second last ring
    for (int slice = 0; slice < slices / 2 - 2; ++slice) {
        int first = 1 + ((rings - 2) * slices) + slice;
        int second = first + slices + 1;

        indices.push_back(first);
        indices.push_back(first + 1);
        indices.push_back(second);

        indices.push_back(second);
        indices.push_back(first + 1);
        indices.push_back(second + 1);
    }

    // Bottom cap indices
    int bottomIndex = vertices.size() - 1;
    int firstBottomRingIndex = vertices.size() - 1 - slices;
    for (int slice = 0; slice < slices; ++slice) {
        indices.push_back(bottomIndex);
        indices.push_back(firstBottomRingIndex + slice);
        indices.push_back(firstBottomRingIndex + (slice + 1) % slices);
    }

}
