

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

void MakeSquare(float size, std::vector<std::array<float,6>> &vertices, std::vector<uint32_t> &indices) {
// Creates a square, on the xz-plane, aligned with the axis, and centered in the origin.
// The length of the four sides is in parameter >size<.
// The procedure should fill the array contained in the >vertices< parameter, with the positions of
// the vertices of the primitive, expressed with their local coordinates in a std::array<float,6> element.
// In particular, the first three elements (index 0,1,2) encode the position of the vertex (x in index 0,
// y in index 1, and z in index 2). The second set of three elements (index 3,4,5) encode the direction
// of the normal vector for the considerd vertex (N.x in index 3, N.y in index 4, and N.z in index 5).
// Indices should be returned in the >indices< array, starting from 0, and up to vertices.size()-1.
// The primitive is encoded as an indexed triangle list, so the size of the >indices< array, should
// be a multiple of 3: each group of three indices, defines a different triangle.
//
// HINT: this procedure has already been implemented. You can keep it as is
	vertices = {
				   {-size/2.0f,0.0f,-size/2.0f,0.0f,1.0f,0.0f},
				   {-size/2.0f,0.0f, size/2.0f,0.0f,1.0f,0.0f},
				   { size/2.0f,0.0f,-size/2.0f,0.0f,1.0f,0.0f},
				   { size/2.0f,0.0f, size/2.0f,0.0f,1.0f,0.0f}};
	indices = {0, 1, 2,    1, 3, 2};
}

void MakeCube(float size, std::vector<std::array<float,6>> &vertices, std::vector<uint32_t> &indices) {
// Creates a cube, with the faces perpendicular to the axis, and centered in the origin.
// The length of one edge of the cube is >size<.
// The procedure should fill the array contained in the >vertices< parameter, with the positions of
// the vertices of the primitive, expressed with their local coordinates in a std::array<float,6> element.
// In particular, the first three elements (index 0,1,2) encode the position of the vertex (x in index 0,
// y in index 1, and z in index 2). The second set of three elements (index 3,4,5) encode the direction
// of the normal vector for the considerd vertex (N.x in index 3, N.y in index 4, and N.z in index 5).
// Indices should be returned in the >indices< array, starting from 0, and up to vertices.size()-1.
// The primitive is encoded as an indexed triangle list, so the size of the >indices< array, should
// be a multiple of 3: each group of three indices, defines a different triangle.
//
// HINT: the procedure below creates a square. You can use it as a side of the cube (please remember
// to change the value of the y component, otherwise the result will be wrong
	vertices = {
            // top
            {-size/2.0f, size/2.0f,-size/2.0f,0.0f,1.0f,0.0f},
            {-size/2.0f, size/2.0f, size/2.0f,0.0f,1.0f,0.0f},
            { size/2.0f, size/2.0f,-size/2.0f,0.0f,1.0f,0.0f},
            { size/2.0f, size/2.0f, size/2.0f,0.0f,1.0f,0.0f},
            // bottom
            {-size/2.0f,-size/2.0f,-size/2.0f,0.0f,-1.0f,0.0f},
            { size/2.0f,-size/2.0f,-size/2.0f,0.0f,-1.0f,0.0f},
            {-size/2.0f,-size/2.0f, size/2.0f,0.0f,-1.0f,0.0f},
            { size/2.0f,-size/2.0f, size/2.0f,0.0f,-1.0f,0.0f},
            // front
            {-size/2.0f,-size/2.0f, size/2.0f,0.0f,0.0f,1.0f},
            {-size/2.0f, size/2.0f, size/2.0f,0.0f,0.0f,1.0f},
            { size/2.0f,-size/2.0f, size/2.0f,0.0f,0.0f,1.0f},
            { size/2.0f, size/2.0f, size/2.0f,0.0f,0.0f,1.0f},
            // back
            {-size/2.0f,-size/2.0f,-size/2.0f,0.0f,0.0f,-1.0f},
            { size/2.0f,-size/2.0f,-size/2.0f,0.0f,0.0f,-1.0f},
            {-size/2.0f, size/2.0f,-size/2.0f,0.0f,0.0f,-1.0f},
            { size/2.0f, size/2.0f,-size/2.0f,0.0f,0.0f,-1.0f},
            // left
            {-size/2.0f,-size/2.0f,-size/2.0f,-1.0f,0.0f,0.0f},
            {-size/2.0f, size/2.0f,-size/2.0f,-1.0f,0.0f,0.0f},
            {-size/2.0f,-size/2.0f, size/2.0f,-1.0f,0.0f,0.0f},
            {-size/2.0f, size/2.0f, size/2.0f,-1.0f,0.0f,0.0f},
            // right
            { size/2.0f,-size/2.0f,-size/2.0f,1.0f,0.0f,0.0f},
            { size/2.0f,-size/2.0f, size/2.0f,1.0f,0.0f,0.0f},
            { size/2.0f, size/2.0f,-size/2.0f,1.0f,0.0f,0.0f},
            { size/2.0f, size/2.0f, size/2.0f,1.0f,0.0f,0.0f}
    };
	indices = {
            // top
            0, 1, 2,
            1, 3, 2,
            // bottom
            4, 5, 6,
            5, 7, 6,
            // front
            8, 10,9,
            9,10,11,
            // back
            12,14,13,
            13,14,15,
            // left
            16,18,17,
            17,18,19,
            // right
            20,22,21,
            21,22,23
    };

}

void MakeCylinder(float radius, float height, int slices, std::vector<std::array<float,6>> &vertices, std::vector<uint32_t> &indices) {
// Creates a cylinder, approximated by a prism with a base composed by a regular polygon with >slices< sides.
// The radius of the cylinder is >radius<, and it height is >height<. The cylinder has its centere
// in the origin, and spans half above and half below the plane that passes thorugh the origin.
// The top and bottom caps are are aligned with xz-plane and perpendicular to the y-axis.
// The procedure should fill the array contained in the >vertices< parameter, with the positions of
// the vertices of the primitive, expressed with their local coordinates in a std::array<float,6> element.
// In particular, the first three elements (index 0,1,2) encode the position of the vertex (x in index 0,
// y in index 1, and z in index 2). The second set of three elements (index 3,4,5) encode the direction
// of the normal vector for the considerd vertex (N.x in index 3, N.y in index 4, and N.z in index 5).
// Indices should be returned in the >indices< array, starting from 0, and up to vertices.size()-1.
// The primitive is encoded as an indexed triangle list, so the size of the >indices< array, should
// be a multiple of 3: each group of three indices, defines a different triangle.
//
// HINT: the procedure below creates a rectangle. You have to change it, or you will obtain a wrong result
// You should use a for loop, and you should start from the procedure to create a circle seen during the lesson
    // TOP
	vertices.push_back({0.0f, height/2.0f, 0.0f, 0.0f, 1.0f, 0.0f});
    for (int i = 0; i < slices; i++) {
        float ang = 2 * M_PI * (float)i / (float)slices;
        vertices.push_back({radius * cos(ang), height / 2.0f, radius * sin(ang), 0.0f, 1.0f, 0.0f});
    }
    for (int i = 0; i < slices; i++) {
        indices.push_back(0);
        indices.push_back((i + 1) % slices + 1);
        indices.push_back(i + 1);
    }

    // BOTTOM
    std::vector<std::array<float,6>> bottomVertices{};
    std::vector<uint32_t> bottomIndices{};
    bottomVertices.push_back({0.0f, -height/2.0f, 0.0f, 0.0f, -1.0f, 0.0f});
    for (int i = 0; i < slices; i++) {
        float ang = 2 * M_PI * (float)i / (float)slices;
        bottomVertices.push_back({radius * cos(ang), -height / 2.0f, radius * sin(ang), 0.0f, -1.0f, 0.0f});
    }
    for (int i = 0; i < slices; i++) {
        bottomIndices.push_back(0);
        bottomIndices.push_back(i + 1);
        bottomIndices.push_back((i + 1) % slices + 1);
    }


    // SIDE
    std::vector<std::array<float,6>> sideVertices{};
    std::vector<uint32_t> sideIndices{};
    for (int i = 0; i < slices; i++) {
        float ang = 2 * M_PI * (float)i / (float)slices;
        sideVertices.push_back({radius * cos(ang), height / 2.0f, radius * sin(ang), cos(ang), 0.0f, sin(ang)});
        sideVertices.push_back({radius * cos(ang), -height / 2.0f, radius * sin(ang), cos(ang), 0.0f, sin(ang)});
    }
    for (int i = 0; i < slices; i++) {
        sideIndices.push_back(2 * i);
        sideIndices.push_back(2 * ((i + 1) % slices) + 1);
        sideIndices.push_back(2 * i + 1);

        sideIndices.push_back(2 * i);
        sideIndices.push_back(2 * ((i + 1) % slices));
        sideIndices.push_back(2 * ((i + 1) % slices) + 1);
    }


    // Merge vertices and indices (shifted)
    int size = vertices.size();
    vertices.insert(vertices.end(), bottomVertices.begin(), bottomVertices.end());
    for (unsigned int bottomIndice : bottomIndices) {
        indices.push_back(bottomIndice + size);
    }

    size = vertices.size();
    vertices.insert(vertices.end(), sideVertices.begin(), sideVertices.end());
    for (unsigned int sideIndice : sideIndices) {
        indices.push_back(sideIndice + size);
    }


}

void MakeCone(float radius, float height, int slices, std::vector<std::array<float,6>> &vertices, std::vector<uint32_t> &indices) {
// Creates a cone, approximated by a pyramid with a base composed by a regular polygon with >slices< sides.
// The radius of the cone is >radius<, and it height is >height<. The cone has its centere
// in the origin, and spans half above and half below the plane that passes thorugh the origin.
// The bottom cap is aligned with xz-plane and perpendicular to the y-axis.
// The procedure should fill the array contained in the >vertices< parameter, with the positions of
// the vertices of the primitive, expressed with their local coordinates in a std::array<float,6> element.
// In particular, the first three elements (index 0,1,2) encode the position of the vertex (x in index 0,
// y in index 1, and z in index 2). The second set of three elements (index 3,4,5) encode the direction
// of the normal vector for the considerd vertex (N.x in index 3, N.y in index 4, and N.z in index 5).
// Indices should be returned in the >indices< array, starting from 0, and up to vertices.size()-1.
// The primitive is encoded as an indexed triangle list, so the size of the >indices< array, should
// be a multiple of 3: each group of three indices, defines a different triangle.
//
// HINT: the procedure below creates a triangle. You have to change it, or you will obtain a wrong result
// You should use a for loop, and you should start from the procedure to create a circle seen during the lesson
    // BOTTOM
    vertices.push_back({0.0f, -height/2.0f, 0.0f, 0.0f, -1.0f, 0.0f});
    for (int i = 0; i < slices; i++) {
        float ang = 2 * M_PI * (float)i / (float)slices;
        vertices.push_back({radius * cos(ang), -height / 2.0f, radius * sin(ang), 0.0f, -1.0f, 0.0f});
    }
    for (int i = 0; i < slices; i++) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back((i + 1) % slices + 1);
    }

    // SIDE
    std::vector<std::array<float,6>> sideVertices{};
    std::vector<uint32_t> sideIndices{};
    sideVertices.push_back({0.0f, height / 2.0f, 0.0f, 0.0f, 1.0f, 0.0f});
    for (int i = 0; i < slices; i++) {
        float ang = 2 * M_PI * (float)i / (float)slices;
        glm::vec3 normal = glm::normalize(glm::vec3(height * cos(ang), radius, height * sin(ang)));
        sideVertices.push_back({radius * cos(ang), -height / 2.0f, radius * sin(ang), normal.x, normal.y, normal.z});
    }
    for (int i = 0; i < slices; i++) {
        sideIndices.push_back(0);
        sideIndices.push_back((i + 1) % slices + 1);
        sideIndices.push_back(i + 1);
    }

    // Merge vertices and indices (shifted)
    int size = vertices.size();
    vertices.insert(vertices.end(), sideVertices.begin(), sideVertices.end());
    for (unsigned int sideIndice : sideIndices) {
        indices.push_back(sideIndice + size);
    }
}

void MakeSphere(float radius, int rings, int slices, std::vector<std::array<float,6>> &vertices, std::vector<uint32_t> &indices)
{
// Creates a sphere, approximated by a poliedron composed by >slices<, and >rings< rings.
// The radius of the sphere is >radius<, and it is centered in the origin.
// The procedure should fill the array contained in the >vertices< parameter, with the positions of
// the vertices of the primitive, expressed with their local coordinates in a std::array<float,6> element.
// In particular, the first three elements (index 0,1,2) encode the position of the vertex (x in index 0,
// y in index 1, and z in index 2). The second set of three elements (index 3,4,5) encode the direction
// of the normal vector for the considerd vertex (N.x in index 3, N.y in index 4, and N.z in index 5).
// Indices should be returned in the >indices< array, starting from 0, and up to vertices.size()-1.
// The primitive is encoded as an indexed triangle list, so the size of the >indices< array, should
// be a multiple of 3: each group of three indices, defines a different triangle.
//
// HINT: the procedure below creates a circle. You have to change it, or you will obtain a wrong result
// You should use two nested for loops, one used to span across the rings, and the other that spans along
// the rings.
    vertices.push_back({0.0f, radius, 0.0f, 0.0f, 1.0f, 0.0f});

    for (int ring = 1; ring <= rings - 1; ring++) {
        float phi = glm::pi<float>() * (float) ring / (float) rings;
        float y = cos(phi);
        float sin_phi = sin(phi);

        for (int slice = 0; slice <= slices; slice++) {
            float theta = 2 * glm::pi<float>() * (float) slice / (float) slices;
            float x = sin_phi * cos(theta);
            float z = sin_phi * sin(theta);

            vertices.push_back({radius * x, radius * y, radius * z , x, y, z});
        }
    }

    vertices.push_back({0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f});

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
