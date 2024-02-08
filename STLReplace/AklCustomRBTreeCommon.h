/// Copyright © 2019-2024 Lincoln Global Inc. All Rights Reserved.
///  \file AklCustomRBTreeCommon.h
///  Declaration of the classes used for custom Red black tree node
///  \author Ruell Magpayo
#pragma once

enum AklCustomRBTreeColor { RED, BLACK };

/// Definition for the custom Red Black Tree Node
/// Single information
template <typename Value>
struct AklCustomRBTreeNode
{
    Value value;
    AklCustomRBTreeColor color;
    AklCustomRBTreeNode* parent;
    AklCustomRBTreeNode* left;
    AklCustomRBTreeNode* right;

    AklCustomRBTreeNode() : color(BLACK), parent(nullptr), left(nullptr), right(nullptr) {}
    AklCustomRBTreeNode(Value v, AklCustomRBTreeColor c, AklCustomRBTreeNode* p, AklCustomRBTreeNode* l, AklCustomRBTreeNode* r)
        : value(v), color(c), parent(p), left(l), right(r)
    {}
};

/// Definition for the custom Red Black Tree Node
/// Definition with key and value
template <typename Key, typename Value>
struct AklCustomRBTreeMapNode {
    Key key;
    Value value;
    AklCustomRBTreeColor color;
    AklCustomRBTreeMapNode* parent;
    AklCustomRBTreeMapNode* left;
    AklCustomRBTreeMapNode* right;

    AklCustomRBTreeMapNode() : color(BLACK), parent(nullptr), left(nullptr), right(nullptr) {}
    AklCustomRBTreeMapNode(Key k, Value v, AklCustomRBTreeColor c, AklCustomRBTreeMapNode* p, AklCustomRBTreeMapNode* l, AklCustomRBTreeMapNode* r)
        : key(k), value(v), color(c), parent(p), left(l), right(r) {}
};
