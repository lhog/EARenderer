//
//  RenderableEntity.cpp
//  EARenderer
//
//  Created by Pavlo Muratov on 26.10.2017.
//  Copyright © 2017 MPO. All rights reserved.
//

#include "MeshInstance.hpp"
#include "ResourcePool.hpp"
#include "Scene.hpp"
#include "StringUtils.hpp"

namespace EARenderer {
    
#pragma mark - Lifecycle
    
    MeshInstance::MeshInstance(ID meshID)
    :
    mMeshID(meshID)
    {
        // Preallocate material map
        auto& subMeshes = ResourcePool::shared().meshes[meshID].subMeshes();
        for (ID subMeshID : subMeshes) {
            mSubMeshMaterialMap[subMeshID] = IDNotFound;
            mSubMeshSHTextureIndexMap[subMeshID] = InvalidIndex;
        }
        
        Mesh& mesh = ResourcePool::shared().meshes[mMeshID];
        mTransformation = mesh.baseTransform();
        mModelMatrix = mTransformation.modelMatrix();
    }
    
#pragma mark - Getters
    
    ID MeshInstance::meshID() const {
        return mMeshID;
    }
    
    bool MeshInstance::isSelected() const {
        return mIsSelected;
    }
    
    bool MeshInstance::isHighlighted() const {
        return mIsHighlighted;
    }

    const glm::mat4& MeshInstance::modelMatrix() const {
        return mModelMatrix;
    }
    
    Transformation MeshInstance::transformation() const {
        return mTransformation;
    }
    
    AxisAlignedBox3D MeshInstance::boundingBox() const {
        Mesh& mesh = ResourcePool::shared().meshes[mMeshID];
        return mesh.boundingBox().transformedBy(mTransformation);
    }
    
    ID MeshInstance::materialIDForSubMeshID(ID subMeshID) const {
        if (mSubMeshMaterialMap.find(subMeshID) == mSubMeshMaterialMap.end()) {
            return IDNotFound;
        }
        return mSubMeshMaterialMap.at(subMeshID);
    }

    MeshInstance::Index MeshInstance::SHTextureIndexForSubMesh(ID subMeshID) const {
        if (mSubMeshSHTextureIndexMap.find(subMeshID) == mSubMeshSHTextureIndexMap.end()) {
            return InvalidIndex;
        }
        return mSubMeshSHTextureIndexMap.at(subMeshID);
    }

#pragma mark - Setters
    
    void MeshInstance::setIsSelected(bool selected) {
        mIsSelected = selected;
    }
    
    void MeshInstance::setIsHighlighted(bool highlighted) {
        mIsHighlighted = highlighted;
    }
    
    void MeshInstance::setTransformation(const Transformation& transform) {
        mTransformation = transform;
        mModelMatrix = transform.modelMatrix();
    }
    
    void MeshInstance::setMaterialIDForSubMeshID(ID materialID, ID subMeshID) {
        if (mSubMeshMaterialMap.find(subMeshID) == mSubMeshMaterialMap.end()) {
            throw std::invalid_argument(string_format("Mesh (ID: %d) doesn't contain submesh with ID %d. Therefore, cannot set a material for it.", mMeshID, subMeshID));
        }
        mSubMeshMaterialMap[subMeshID] = materialID;
    }
    
    void MeshInstance::setMaterialIDForAllSubmeshes(ID materialID) {
        for (auto& keyValuePair : mSubMeshMaterialMap) {
            ID subMeshID = keyValuePair.first;
            mSubMeshMaterialMap[subMeshID] = materialID;
        }
    }

    void MeshInstance::setDedicatedSHTextureIndexForSubMeshID(Index textureIndex, ID subMeshID) {
        if (mSubMeshSHTextureIndexMap.find(subMeshID) == mSubMeshSHTextureIndexMap.end()) {
            throw std::invalid_argument(string_format("Mesh (ID: %d) doesn't contain submesh with ID %d.", mMeshID, subMeshID));
        }
        mSubMeshSHTextureIndexMap[subMeshID] = textureIndex;
    }
    
}
