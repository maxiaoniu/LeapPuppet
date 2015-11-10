//
//  Mesh.hpp
//  LeapPupeet
//
//  Created by mayue on 10/29/15.
//
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "cinder/gl/gl.h"
using namespace ci;

class Mesh {
public:
    Mesh();
    ~Mesh();
    bool LoadMesh(const std::string& Filename);
    gl::VboMeshRef m_Model;
private:
    const aiScene* m_pScene;
    Assimp::Importer m_Importer;
    
    mat4 m_GlobalInverseTransform;
    
    bool initFromScene(const aiScene* pScene, const std::string& Filename);
    
    #define INVALID_MATERIAL 0xFFFFFFFF
    struct MeshEntry {
        MeshEntry()
        {
            NumIndices    = 0;
            BaseVertex    = 0;
            BaseIndex     = 0;
            MaterialIndex = INVALID_MATERIAL;
        }
        
        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };
    
    std::vector<MeshEntry> m_Entries;
    
    void initMesh(uint MeshIndex,
                  const aiMesh* paiMesh,
                  std::vector<glm::vec3>& Positions,
                  std::vector<glm::vec3>& Normals,
                  std::vector<glm::vec2>& TexCoords,
                  std::vector<unsigned int>& Indices);
    

};

#endif /* Mesh_hpp */
