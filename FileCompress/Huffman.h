#pragma once
#include "Heap.h"

template<class T>
struct HuffmanTreeNode
{
	T _weight;
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;

	HuffmanTreeNode(const T& w)
		:_weight(w)
		,_left(NULL)
		,_right(NULL)
		,_parent(NULL)
	{}
};

template<class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}

	HuffmanTree(T* a,size_t size,const T& invalid = T())
	{
		struct NodeLess
		{
			bool operator()(Node* l,Node* r)
			{
				return l->_weight < r->_weight;
			}
		};

		Heap<Node*,NodeLess> minHeap;
	
		for(int i = 0; i < size; ++i)
		{	
			if(a[i] != invalid)
			{
				Node* node = new Node(a[i]);
				minHeap.Push(node);
			}
		}

		while(minHeap.Size() > 1)
		{
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();

			Node* parent = new Node(left->_weight+right->_weight);

			parent->_left = left;
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;

			minHeap.Push(parent);

			_root = minHeap.Top();

		}
	}

	Node* GetRoot()
	{
		return _root;
	}
protected:
	Node* _root;
};
