//
//  Mesh.cpp
//  LeapPupeet
//
//  Created by mayue on 10/29/15.
//
//

#include "Mesh.hpp"
#include <stdio.h>
using namespace std;
inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from)
{
    glm::mat4 to;
    
    
    to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
    to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
    to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
    to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;
    
    return to;
}

Mesh::Mesh() {
    
}

Mesh::~Mesh() {
    
}

bool Mesh::LoadMesh(const string& Filename) {
    bool Ret = false;
    
    m_pScene = m_Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate |
                                   aiProcess_GenSmoothNormals |
                                   aiProcess_FlipUVs |
                                   aiProcess_JoinIdenticalVertices);
    
    if(m_pScene) {
        m_GlobalInverseTransform = aiMatrix4x4ToGlm(m_pScene->mRootNode->mTransformation);
        inverse(m_GlobalInverseTransform);
        Ret = initFromScene(m_pScene, Filename);
    }
    else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), m_Importer.GetErrorString());
    }
    return Ret;
    
}

bool Mesh::initFromScene(const aiScene *pScene, const string &Filename) {
    
    m_Entries.resize(pScene->mNumMeshes);
    
    vector<glm::vec3> Positions;
    vector<glm::vec3> Normals;
    vector<glm::vec2> TexCoords;
    vector<uint> Indices;
    
    uint numVertices = 0;
    uint numIndices = 0;
    
    // Count the number of vertices and indices
    for (uint i = 0 ; i < m_Entries.size() ; i++) {
        m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
        m_Entries[i].NumIndices    = pScene->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].BaseVertex    = numVertices;
        m_Entries[i].BaseIndex     = numIndices;
        
        numVertices += pScene->mMeshes[i]->mNumVertices;
        numIndices  += m_Entries[i].NumIndices;
    }
    
    // Reserve space in the vectors for the vertex attributes and indices
    Positions.reserve(numVertices);
    Normals.reserve(numVertices);
    TexCoords.reserve(numVertices);
    Indices.reserve(numIndices);
    
    for (uint i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh, Positions, Normals, TexCoords, Indices);
    }
    
    vector<gl::VboMesh::Layout> bufferLayout = {
        gl::VboMesh::Layout().usage( GL_STATIC_DRAW ).attrib( geom::Attrib::POSITION, 3 ),
    };
    m_Model = gl::VboMesh::create( Positions.size(), GL_LINES, bufferLayout, Indices.size(), GL_UNSIGNED_INT );
    m_Model->bufferAttrib( geom::Attrib::POSITION, Positions );
    m_Model->bufferIndices( Indices.size() * sizeof( uint ), Indices.data() );
    return true;
}
void Mesh::initMesh(uint MeshIndex,
                    const aiMesh* paiMesh,
                    std::vector<glm::vec3>& Positions,
                    std::vector<glm::vec3>& Normals,
                    std::vector<glm::vec2>& TexCoords,
                    std::vector<unsigned int>& Indices) {

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    
    // Populate the vertex attribute vectors
    for (uint i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
        
        Positions.push_back(vec3(pPos->x, pPos->y, pPos->z));
        Normals.push_back(vec3(pNormal->x, pNormal->y, pNormal->z));
        TexCoords.push_back(vec2(pTexCoord->x, pTexCoord->y));
    }

    // Populate the index buffer
    for (uint i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }
    
}


