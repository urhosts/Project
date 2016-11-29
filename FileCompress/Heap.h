#pragma once
#include <cassert>

#include <vector>

template<class T>
struct Less
{
	bool operator()(const T& l,const T& r)
	{
		return l < r;
	}
};

template<class T>
struct Greater
{
	bool operator()(const T& l,const T& r)
	{
		return l > r;
	}
};

template<class T,class Compear = Less<T>>
class Heap
{
public:
	Heap()
	{}

	Heap(const T* a,size_t size)
	{
		for(int i = 0; i<size; ++i)
		{
			_root.push_back(a[i]);
		}

		for(int i =( _root.size()-2)/2; i>=0; --i)
		{
			_AdjustDown(i);
		}
		
	}

	void Push(const T& x)
	{
		_root.push_back(x);

		_AdjustUp(_root.size()-1);//向上调整
	}

	void Pop()
	{
		assert(_root.size() > 0);
		swap(_root[0],_root[_root.size()-1]);
		_root.pop_back();

		if(Size() > 0);
			_AdjustDown(0);
	}

	const T& Top()
	{
		assert(_root.size() > 0);
		return _root[0];
	}

	bool Empty()
	{
		return _root.empty();

	}

	size_t Size()
	{
		return _root.size();
	}

	void Print()
	{
		for(int i = 0; i<Size(); ++i)
		{
			cout<<_root[i]<<" ";
		}

		cout<<endl;
	}

protected:
	void _AdjustDown(int root)//从根向下调整
	{
		int child = root*2 + 1;

		Compear com;
		while(child < _root.size())
		{
			if((child+1<_root.size())&&
				com(_root[child+1],_root[child]))
			{
				++child;
			}

			
			if(com(_root[child],_root[root]))			
			{
				swap(_root[child],_root[root]);
				root = child;
				child = root*2 + 1;
			}
			else
				break;
		}

	}

	void _AdjustUp(size_t child)
	{
		assert(!Empty());

		while(child > 0)
		{
			size_t parent = (child-1)/2;
			Compear com;

			if(com(_root[child],_root[parent]))			
			{
				swap(_root[child],_root[parent]);
				child = parent;
			}
			else
				break;
		}

	}
protected:
	vector<T> _root;
};

