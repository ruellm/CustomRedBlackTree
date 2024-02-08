// STLReplace.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "AklCustomRBTree.h"
#include "AklCustomRBNodeCreator.h"
#include "AklCustomRBTreeMap.h"

#define TOTAL_BLOCKS 1024x1024

int main()
{
    /// RB Tree as Set demo
    AklCustomRBNodeCreator<AklCustomRBTreeNode<int>> creator;
    creator.Initialize(2);

    AklCustomRBTree<int> rbTree;
    rbTree.SetNodeCreator(&creator);

    rbTree.Insert(10);
    rbTree.Insert(11);
    rbTree.Insert(1);
    rbTree.Insert(2);

    std::set<int> elements = rbTree.GetAsSet();
    for (const auto& element : elements) {
        std::cout << element << " ";
    }

    auto r = rbTree.Find(3);
    if (r == nullptr)
        std::cout << "Not found\n";

    /// Test for RegionConnections
    typedef AklCustomRBTreeMapNode<int, AklCustomRBTree<int>> RegionConnectionMapNode;
    AklCustomRBNodeCreator<RegionConnectionMapNode> regionNodeCreator;
    AklCustomRBNodeCreator<AklCustomRBTreeNode<int>> regionSetCreator;

    regionNodeCreator.Initialize(10);
    regionSetCreator.Initialize(10);

    // create the map
    typedef AklCustomRBTreeMap<int, AklCustomRBTree<int>> RegionConnections;
    RegionConnections frontConnections;
    frontConnections.SetNodeCreator(&regionNodeCreator);

    frontConnections[0].Insert(0, &regionSetCreator);

    creator.Release();
    regionNodeCreator.Release();
    regionSetCreator.Release();

    std::cout << std::endl;
}
