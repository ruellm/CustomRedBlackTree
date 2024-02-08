/// Copyright © 2019-2024 Lincoln Global Inc. All Rights Reserved.
///  \file AklCustomRBTree.h
///  Declaration of the AklCustomRBTree class
///  \author Ruell Magpayo
#pragma once

#include "AklCustomRBTreeCommon.h"
#include "AklCustomRBNodeCreator.h"

template <typename Key, typename Value>
class AklCustomRBTreeMap {
public:
    AklCustomRBTreeMap() : m_root(nullptr), m_creator(nullptr)
    {}

    /// Sets the node creator
    void SetNodeCreator(AklCustomRBNodeCreator<AklCustomRBTreeMapNode<Key, Value>>* creator)
    {
        m_creator = creator;
    }

    /// Insert Key value pair
    /// \param key The Key to insert
    /// \param value The value to insert
    void Insert(const Key& key, const Value& value) 
    {
        AklCustomRBTreeMapNode<Key, Value>* node = nullptr;// = 
        
        if (m_creator)
        {
            node = m_creator->Obtain();
            node->key = key;
            node->value = value;
            node->color = RED;
            node->parent = nullptr;
            node->left = nullptr;
            node->right = nullptr;
        }
        else
        {
            node = new AklCustomRBTreeMapNode<Key, Value>(key, value, RED, nullptr, nullptr, nullptr);
        }
        
        InsertNode(node);
        FixInsert(node);
    }

    /// Clears the tree
    /// \param node The node to start
    void Clear()
    {
        // if the nodes are created from a creator, we dont need to do the cleanup
        if(m_creator == nullptr)
            ClearInternal(m_root);

        m_root = nullptr;
    }

    /// Access value thru its key
    Value& operator[](const Key& key) 
    {
        AklCustomRBTreeMapNode<Key, Value>* node = Find(key);
        if (node == nullptr) {
            Value default_value = Value();
            Insert(key, default_value);
            node = Find(key);
        }
        return node->value;
    }

    /// Assignment Operator
    AklCustomRBTreeMap& operator=(const AklCustomRBTreeMap& other) 
    {
        if (this != &other) 
        {
            Clear();
            CopyTree(m_root, other.m_root);
        }
        return *this;
    }

#if _DEBUG
    void Print() 
    {
        PrintContents(m_root);
    }
#endif

private:
    AklCustomRBTreeMapNode<Key, Value>* m_root;
    AklCustomRBNodeCreator<AklCustomRBTreeMapNode<Key, Value>>* m_creator;

    /// Performs a left rotation on the Red-Black Tree rooted at the given node.
    /// \param node The node around which the left rotation is performed.
    void LeftRotate(AklCustomRBTreeMapNode<Key, Value>* node) 
    {
        AklCustomRBTreeMapNode<Key, Value>* rightChild = node->right;
        node->right = rightChild->left;

        if (rightChild->left != nullptr) 
        {
            rightChild->left->parent = node;
        }

        rightChild->parent = node->parent;

        if (node->parent == nullptr) 
        {
            m_root = rightChild;
        }
        else if (node == node->parent->left) 
        {
            node->parent->left = rightChild;
        }
        else 
        {
            node->parent->right = rightChild;
        }

        rightChild->left = node;
        node->parent = rightChild;
    }

    /// Performs a right rotation on the Red-Black Tree rooted at the given node.
    /// \param node The node around which the right rotation is performed.
    void RightRotate(AklCustomRBTreeMapNode<Key, Value>* node) 
    {
        AklCustomRBTreeMapNode<Key, Value>* leftChild = node->left;
        node->left = leftChild->right;

        if (leftChild->right != nullptr) 
        {
            leftChild->right->parent = node;
        }

        leftChild->parent = node->parent;

        if (node->parent == nullptr) 
        {
            m_root = leftChild;
        }
        else if (node == node->parent->left) 
        {
            node->parent->left = leftChild;
        }
        else 
        {
            node->parent->right = leftChild;
        }

        leftChild->right = node;
        node->parent = leftChild;
    }

    /// Inserts a new node with the specified key and value into the Red-Black Tree.
    /// \param newNode The new node to be inserted.
    void InsertNode(AklCustomRBTreeMapNode<Key, Value>* newNode) 
    {
        AklCustomRBTreeMapNode<Key, Value>* parent = nullptr;
        AklCustomRBTreeMapNode<Key, Value>* current = m_root;

        while (current != nullptr) 
        {
            parent = current;
            if (newNode->key < current->key) 
            {
                current = current->left;
            }
            else 
            {
                current = current->right;
            }
        }

        newNode->parent = parent;

        if (parent == nullptr) 
        {
            m_root = newNode;
        }
        else if (newNode->key < parent->key) 
        {
            parent->left = newNode;
        }
        else 
        {
            parent->right = newNode;
        }
    }

    /// Fixes the Red-Black Tree properties after the insertion of a new node.
    /// \param node The newly inserted node that may violate the Red-Black Tree properties.
    void FixInsert(AklCustomRBTreeMapNode<Key, Value>* node) 
    {
        while (node != m_root && node->parent->color == RED) 
        {
            if (node->parent == node->parent->parent->left) 
            {
                AklCustomRBTreeMapNode<Key, Value>* uncle = node->parent->parent->right;

                if (uncle != nullptr && uncle->color == RED) 
                {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else {
                    if (node == node->parent->right) 
                    {
                        node = node->parent;
                        LeftRotate(node);
                    }

                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    RightRotate(node->parent->parent);
                }
            }
            else 
            {
                AklCustomRBTreeMapNode<Key, Value>* uncle = node->parent->parent->left;

                if (uncle != nullptr && uncle->color == RED) 
                {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else 
                {
                    if (node == node->parent->left) 
                    {
                        node = node->parent;
                        RightRotate(node);
                    }

                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    LeftRotate(node->parent->parent);
                }
            }
        }

        m_root->color = BLACK;
    }

    /// Searches for a node with the given key in the Red-Black Tree.
    /// \param key The key to search for.
    /// \return A pointer to the node with the specified key if found, otherwise nullptr.
    AklCustomRBTreeMapNode<Key, Value>* Find(const Key& key) 
    {
        AklCustomRBTreeMapNode<Key, Value>* current = m_root;

        while (current != nullptr) 
        {
            if (key == current->key) 
            {
                return current;
            }
            else if (key < current->key) 
            {
                current = current->left;
            }
            else 
            {
                current = current->right;
            }
        }

        return nullptr;
    }

    /// Clears and delete the node
    void ClearInternal(AklCustomRBTreeMapNode<Key, Value>* node) {
        if (node != nullptr) 
        {
            ClearInternal(node->left);
            ClearInternal(node->right);
            delete node;
        }
    }

    /// Copy the tree node content
    void CopyTree(AklCustomRBTreeMapNode<Key, Value>*& destination, const AklCustomRBTreeMapNode<Key, Value>* source) 
    {
        if (source != nullptr) 
        {
            if (m_creator)
            {
                destination = m_creator->Obtain();
                destination->key = source->key;
                destination->value = source->value;
                destination->color = source->color;
                destination->parent = nullptr;
                destination->left = nullptr;
                destination->right = nullptr;
            }
            else
            {
               // node = new AklCustomRBTreeMapNode<Key, Value>(key, value, RED, nullptr, nullptr, nullptr);
                destination = new AklCustomRBTreeMapNode<Key, Value>(source->key, source->value, source->color, nullptr, nullptr, nullptr);
            }

            CopyTree(destination->left, source->left);
            CopyTree(destination->right, source->right);
        }
    }

#if _DEBUG
    void PrintContents(AklCustomRBTreeMapNode<Key, Value>* node)
    {
        if (node != nullptr) 
        {
            PrintContents(node->left);
            std::cout << "(" << node->key << ": " << node->value << ") " << std::endl;
            PrintContents(node->right);
        }
    }
#endif
};