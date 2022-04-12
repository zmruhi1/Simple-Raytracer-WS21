#include <rt/groups/bvh.h>
#include <rt/bbox.h>
#include <queue>

namespace rt {

BVH::BVH()
{
    mBBox = BBox::full();
}

void BVH::insert(TreeNode* node, const ObjBBox& ob, Point volumeMin, Point volumeMax) {
    if (node->isLeaf) {
        if (node->objs.size() < 3) {
            node->objs.push_back(ob);
        }
        else {
            node->isLeaf = false;
            // insert remained bboxes
            while (node->objs.size() != 0) {
                insert(node, node->objs.back(), volumeMin, volumeMax);
                node->objs.pop_back();
            }
            // insert input bbox
            insert(node, ob, volumeMin, volumeMax);
        }
    }
    else {
        float xDim = volumeMax.x - volumeMin.x;
        float yDim = volumeMax.y - volumeMin.y;
        float zDim = volumeMax.z - volumeMin.z;

        std::vector<int> indices;
        Vector diffVector;

        if (xDim >= yDim && xDim >= zDim) {
            float xCenter = (volumeMin.x + volumeMax.x) * 0.5f;
            
            float diff = (volumeMax.x - volumeMin.x) * 0.5f;
            diffVector = Vector(diff, 0, 0);

            float objBoxCentroid = (ob.objBox.min.x + ob.objBox.max.x) * 0.5f;
            if (objBoxCentroid <= xCenter) {
                indices.push_back(0);
            }
            else {
                indices.push_back(1);
            }
        }
        else if (yDim >= xDim && yDim >= zDim) {
            float yCenter = (volumeMin.y + volumeMax.y) * 0.5f;

            float diff = (volumeMax.y - volumeMin.y) * 0.5f;
            diffVector = Vector(0, diff, 0);

            float objBoxCentroid = (ob.objBox.min.y + ob.objBox.max.y) * 0.5f;
            if (objBoxCentroid <= yCenter) {
                indices.push_back(0);
            }
            else {
                indices.push_back(1);
            }
        }
        else if (zDim >= xDim && zDim >= yDim) {
            float zCenter = (volumeMin.z + volumeMax.z) * 0.5f;

            float diff = (volumeMax.z - volumeMin.z) * 0.5f;
            diffVector = Vector(0, 0, diff);

            float objBoxCentroid = (ob.objBox.min.z + ob.objBox.max.z) * 0.5f;
            if (objBoxCentroid <= zCenter) {
                indices.push_back(0);
            }
            else {
                indices.push_back(1);
            }
        }

        while (indices.size() != 0) {
            int curIdx = indices.back();
           
            Point childVolumeMin(0, 0, 0);
            Point childVolumeMax(0, 0, 0);

            if (node->child[curIdx] == NULL) {
                node->child[curIdx] = new TreeNode;
            }

            if (curIdx == 0) {
                childVolumeMin = volumeMin;
                childVolumeMax = volumeMax - diffVector;
            }
            else {
                childVolumeMin = volumeMin + diffVector;
                childVolumeMax = volumeMax;
            }
            
            insert(node->child[curIdx], ob, childVolumeMin, childVolumeMax);

            indices.pop_back();
        }
    }
}

void BVH::build(TreeNode *node) {
    
    if (node->isLeaf) {
        for (int i = 0; i < node->objs.size(); i++) {
            node->nodeBBbox.extend(node->objs[i].objBox);
        }
    }
    else {
        for (int i = 0; i < 2; i++) {
            if (node->child[i]) {
                build(node->child[i]);
                node->nodeBBbox.extend(node->child[i]->nodeBBbox);
            }
        }
    }
}

void BVH::rebuildIndex() {
    root->nodeBBbox = getBounds();
    std::cout << "Insert started" << std::endl;
    for (int i = 0; i < primitives.size(); i++) {
        insert(root, ObjBBox(primitives[i]->getBounds(), primitives[i]), root->nodeBBbox.min, root->nodeBBbox.max);
    }
    std::cout << "Insert finished" << std::endl << std::endl;

    std::cout << "Build started" << std::endl;
    build(root);
    std::cout << "Build finished" << std::endl << std::endl;

//    printTree(root, 0);
}

void BVH::printTree(TreeNode *node, int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "*";
    }
    std::cout << node->nodeBBbox.min.x << ", " << node->nodeBBbox.min.y << ", " << node->nodeBBbox.min.z << " | "
        << node->nodeBBbox.max.x << ", " << node->nodeBBbox.max.y << ", " << node->nodeBBbox.max.z << std::endl;
    
    for (int i = 0; i < 2; i++) {
        if (node->child[i] != NULL) {
            printTree(node->child[i], depth + 1);
            if (node->child[i]->isLeaf) {
                for (int j = 0; j < node->child[i]->objs.size(); j++) {
                    std::cout << "-->  " << node->child[i]->objs[j].objBox.min.x << ", " << node->child[i]->objs[j].objBox.min.y << ", " << node->child[i]->objs[j].objBox.min.z << " | "
                        << node->child[i]->objs[j].objBox.max.x << ", " << node->child[i]->objs[j].objBox.max.y << ", " << node->child[i]->objs[j].objBox.max.z << std::endl;
                }
            }
        }
    }
}

BBox BVH::getBounds() const {
    return mBBox;
}

Intersection BVH::intersect(const Ray& ray, float previousBestDistance) const {
    Intersection isec;
    
    std::pair<float, float> t = root->nodeBBbox.intersect(ray);
    float tNear = t.first;
    float tFar = t.second;
    if (tNear > tFar || tFar < 0) return Intersection::failure();
    float tMin = tFar;
    std::priority_queue<BVH::QueueElement> queue;
    queue.push(BVH::QueueElement(root, 0));

    while (!queue.empty() && queue.top().t < tMin) {
        TreeNode* node = queue.top().node;
        queue.pop();
        if (node->isLeaf) {
            for (int i = 0; i < node->objs.size(); i++) {
                Intersection objIsec = node->objs[i].obj->intersect(ray, previousBestDistance);
                if (objIsec.isintersec) {
                    isec = objIsec;
                    previousBestDistance = objIsec.distance;
                }
            }
        }
        else {
            for (int i = 0; i < 2; i++) {
                if (node->child[i] != NULL) {
                    std::pair<float, float> childT = node->child[i]->nodeBBbox.intersect(ray);
                    float childTNear = childT.first, childTFar = childT.second;
                    bool isChildIntersect = ((childTNear <= childTFar) && (childTFar >= 0));
                    if (isChildIntersect) {
                        float tChild = (childTNear < 0 && childTFar >= 0) ? childTFar : childTNear;
                        queue.push(BVH::QueueElement(node->child[i], tChild));
                    }
                }
            }
        }
    }

    return isec;
}

void BVH::add(Primitive* p) {
    primitives.push_back(p);
    mBBox.extend(p->getBounds());
}

void BVH::setMaterial(Material* m) {
    /* TODO */ NOT_IMPLEMENTED;
}

void BVH::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

void BVH::serialize(BVH::Output& output) {
    // To implement this function:
    // - Call output.setNodeCount() with the number of nodes in the BVH
    /* TODO */
    // - Set the root node index using output.setRootId()
    /* TODO */
    // - Write each and every one of the BVH nodes to the output using output.writeNode()
    /* TODO */ NOT_IMPLEMENTED;
}
void BVH::deserialize(BVH::Input& input) {
    // To implement this function:
    // - Allocate and initialize input.getNodeCount() nodes
    /* TODO */
    // - Fill your nodes with input.readNode(index)
    /* TODO */
    // - Use the node at index input.getRootId() as the root node
    /* TODO */ NOT_IMPLEMENTED;
}

}