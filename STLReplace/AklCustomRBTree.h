/// Copyright © 2019-2024 Lincoln Global Inc. All Rights Reserved.
///  \file AklCustomRBTree.h
///  Declaration of the AklCustomRBTree class
///  \author Ruell Magpayo
#pragma once

#include "AklCustomRBTreeCommon.h"
#include "AklCustomRBNodeCreator.h"

#include <iostream>
#include <set>

template <typename Value>
class AklCustomRBTree 
{
private:
    AklCustomRBTreeNode<Value>* m_root;
    AklCustomRBNodeCreator<AklCustomRBTreeNode<Value>>* m_creator;

    /// Performs a left rotation around the given node.
    /// This operation maintains the binary search tree property.
    /// \param x The node around which the left rotation is performed.
    void LeftRotate(AklCustomRBTreeNode<Value>* x) 
    {
        AklCustomRBTreeNode<Value>* y = x->right;
        x->right = y->left;

        if (y->left != nullptr)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            m_root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    /// Performs a right rotation around the given node.
    /// This operation maintains the binary search tree property.
    /// \param y The node around which the right rotation is performed.
    void RightRotate(AklCustomRBTreeNode<Value>* y) 
    {
        AklCustomRBTreeNode<Value>* x = y->left;
        y->left = x->right;

        if (x->right != nullptr)
            x->right->parent = y;

        x->parent = y->parent;

        if (y->parent == nullptr)
            m_root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        x->right = y;
        y->parent = x;
    }

    /// Restores the Red-Black Tree properties after an insertion.
    /// Fixes any violations caused by the insertion.
    /// \param z The node that was inserted and may have caused violations.
    void InsertFixup(AklCustomRBTreeNode<Value>* z) 
    {
        while (z->parent != nullptr && z->parent->color == RED) 
        {
            if (z->parent == z->parent->parent->left) {
                AklCustomRBTreeNode<Value>* y = z->parent->parent->right;
                if (y != nullptr && y->color == RED) 
                {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else 
                {
                    if (z == z->parent->right) 
                    {
                        z = z->parent;
                        LeftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    RightRotate(z->parent->parent);
                }
            }
            else 
            {
                AklCustomRBTreeNode<Value>* y = z->parent->parent->left;
                if (y != nullptr && y->color == RED) 
                {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else 
                {
                    if (z == z->parent->left) 
                    {
                        z = z->parent;
                        RightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    LeftRotate(z->parent->parent);
                }
            }
        }
        m_root->color = BLACK;
    }

    /// \brief Replaces one subtree as a child of its parent with another subtree.
    /// Used in transplanting subtrees during the deletion operation.
    /// \param u The node whose subtree is to be replaced.
    /// \param v The node whose subtree replaces the subtree rooted at u.
    void Transplant(AklCustomRBTreeNode<Value>* u, AklCustomRBTreeNode<Value>* v) 
    {
        if (u->parent == nullptr)
            m_root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        if (v != nullptr)
            v->parent = u->parent;
    }

    /// \brief Finds the node with the minimum key in the subtree rooted at the given node.
    /// Used in the deletion operation to find the successor of a node.
    /// \param x The root of the subtree for which the minimum key is to be found.
    /// \return The node with the minimum key in the subtree rooted at x.
    AklCustomRBTreeNode<Value>* Minimum(AklCustomRBTreeNode<Value>* x)
    {
        while (x->left != nullptr)
            x = x->left;
        return x;
    }


    /// \brief Restores the Red-Black Tree properties after a deletion.
    /// Fixes any violations caused by the deletion.
    /// \param x The node that replaces the deleted node in the tree.
    void EraseFixup(AklCustomRBTreeNode<Value>* x) 
    {
        while (x != m_root && (x == nullptr || x->color == BLACK)) 
        {
            if (x == x->parent->left) {
                AklCustomRBTreeNode<Value>* w = x->parent->right;
                if (w->color == RED) 
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    LeftRotate(x->parent);
                    w = x->parent->right;
                }
                if ((w->left == nullptr || w->left->color == BLACK) &&
                    (w->right == nullptr || w->right->color == BLACK)) 
                {
                    w->color = RED;
                    x = x->parent;
                }
                else 
                {
                    if (w->right == nullptr || w->right->color == BLACK) 
                    {
                        if (w->left != nullptr)
                            w->left->color = BLACK;
                        w->color = RED;
                        RightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right != nullptr)
                        w->right->color = BLACK;
                    LeftRotate(x->parent);
                    x = m_root;
                }
            }
            else {
                AklCustomRBTreeNode<Value>* w = x->parent->left;
                if (w->color == RED) 
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    RightRotate(x->parent);
                    w = x->parent->left;
                }
                if ((w->right == nullptr || w->right->color == BLACK) &&
                    (w->left == nullptr || w->left->color == BLACK)) 
                {
                    w->color = RED;
                    x = x->parent;
                }
                else 
                {
                    if (w->left == nullptr || w->left->color == BLACK) 
                    {
                        if (w->right != nullptr)
                            w->right->color = BLACK;
                        w->color = RED;
                        LeftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left != nullptr)
                        w->left->color = BLACK;
                    RightRotate(x->parent);
                    x = m_root;
                }
            }
        }
        if (x != nullptr)
            x->color = BLACK;
    }

    /// \brief Performs an in-order traversal of the subtree rooted at the given node.
    /// Appends the values in sorted order to the provided set.
    /// \param x The root of the subtree to be traversed in-order.
    /// \param result The set to which the values are appended in sorted order.
    void InorderTraversalHelper(AklCustomRBTreeNode<Value>* x, std::set<Value>& result) 
    {
        if (x != nullptr) 
        {
            InorderTraversalHelper(x->left, result);
            result.insert(x->value);
            InorderTraversalHelper(x->right, result);
        }
    }

public:
    AklCustomRBTree() : m_root(nullptr), m_creator(nullptr)
    {}

    /// Sets the node creator
    void SetNodeCreator(AklCustomRBNodeCreator<AklCustomRBTreeNode<Value>>* creator)
    {
        m_creator = creator;
    }

    /// Insert element to the tree
    void Insert(Value value, AklCustomRBNodeCreator<AklCustomRBTreeNode<Value>>* creator = nullptr)
    {
        AklCustomRBTreeNode<Value>* z = nullptr;
        AklCustomRBTreeNode<Value>* y = nullptr;
        AklCustomRBTreeNode<Value>* x = m_root;

        AklCustomRBNodeCreator<AklCustomRBTreeNode<Value>>* nodeCreator = 
            creator != nullptr ? creator : m_creator;

        if (nodeCreator)
        {
            z = m_creator->Obtain();
            z->value = value;
            z->color = RED;
            z->parent = nullptr;
            z->left = nullptr;
            z->right = nullptr;
        }
        else
        {
            z = new AklCustomRBTreeNode<Value>(value, RED, nullptr, nullptr, nullptr);
        }

        while (x != nullptr) 
        {
            y = x;
            if (z->value < x->value)
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;
        if (y == nullptr)
            m_root = z;
        else if (z->value < y->value)
            y->left = z;
        else
            y->right = z;

        InsertFixup(z);
    }

    /// Check if element exist in the tree
    /// \param value The value to find
    /// \return the Node in the tree or null
    AklCustomRBTreeNode<Value>* Find(Value value) 
    {
        AklCustomRBTreeNode<Value>* current = m_root;
        while (current != nullptr) {
            if (value == current->value)
                return current;
            else if (value < current->value)
                current = current->left;
            else
                current = current->right;
        }
        return nullptr;
    }

    /// Erase the value in the tree
    /// \param value The value to erase
    void Erase(Value value) 
    {
        AklCustomRBTreeNode<Value>* z = Find(value);
        if (z == nullptr)
            return;

        AklCustomRBTreeNode<Value>* y = z;
        AklCustomRBTreeNode<Value>* x;
        AklCustomRBTreeColor yOriginalColor = y->color;

        if (z->left == nullptr) 
        {
            x = z->right;
            Transplant(z, z->right);
        }
        else if (z->right == nullptr) 
        {
            x = z->left;
            Transplant(z, z->left);
        }
        else 
        {
            y = Minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z)
                x->parent = y;
            else 
            {
                Transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            Transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (yOriginalColor == BLACK)
            EraseFixup(x);
    }

    /// Return the set version of the list
    /// \return values as std::set
    std::set<Value> GetAsSet() 
    {
        std::set<Value> result;
        InorderTraversalHelper(m_root, result);
        return result;
    }
};
