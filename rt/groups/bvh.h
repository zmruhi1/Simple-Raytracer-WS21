#ifndef CG1RAYTRACER_GROUPS_BVH_HEADER
#define CG1RAYTRACER_GROUPS_BVH_HEADER

#include <rt/groups/group.h>
#include <rt/bbox.h>
#include <cfloat>
#include <cstring>

namespace rt {

class BVH : public Group {
public:
    BVH();

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
    virtual void rebuildIndex();
    virtual void add(Primitive* p);
    virtual void setMaterial(Material* m);
    virtual void setCoordMapper(CoordMapper* cm);

    BBox mBBox;
    Primitives primitives;

    struct ObjBBox {
        BBox objBox;
        Primitive* obj;
        ObjBBox(BBox b, Primitive* o): objBox(b), obj(o) {}
    };

    struct TreeNode {
        TreeNode* child[2];
        std::vector<ObjBBox> objs;
        BBox nodeBBbox;
        bool isLeaf;
        TreeNode() : isLeaf(true) { memset(child, 0x0, sizeof(TreeNode*) * 2); nodeBBbox = BBox::empty(); }
        ~TreeNode() { 
            if (child[0] != NULL) delete child[0]; 
            if (child[1] != NULL) delete child[1]; 
        }
    };

    struct QueueElement {
        TreeNode* node;
        float t;
        QueueElement(TreeNode* n, float t): node(n), t(t) {}
        friend bool operator < (const QueueElement& a, const QueueElement& b) {
            return a.t > b.t;
        }
    };

    TreeNode* root = new TreeNode;

    void insert(TreeNode* node, const ObjBBox&ob, Point volumeMin, Point volumeMax);
    void build(TreeNode* node);
    void printTree(TreeNode* node, int depth);

    // Do not use this structure as your node layout:
    // It is inefficient and has a large memory footprint.
    struct SerializedNode {
        bool isLeaf;
        BBox bbox;

        // For leaves only
        std::vector<Primitive*> primitives;

        // For internal nodes only
        size_t leftChildId;
        size_t rightChildId;
    };

    // Implement this function if you want to take part in the BVH speed competition
    struct Output {
        virtual void setNodeCount(size_t nodeCount) = 0;
        virtual void setRootId(size_t rootId) = 0;
        virtual void writeNode(size_t nodeId, const SerializedNode& node) = 0;
    };
    void serialize(Output& output);

    // Implementing this is optional and not needed for the BVH speed competition,
    // but it will allow you to run the benchmark utility and test your BVH exporter.
    struct Input {
        virtual size_t getNodeCount() = 0;
        virtual size_t getRootId() = 0;
        virtual const SerializedNode& readNode(size_t nodeId) = 0;
    };
    void deserialize(Input& input);
};

}

#endif