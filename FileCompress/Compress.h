#pragma once

#include "Heap.h"
#include "Huffman.h"
#include <string>
#include <cassert>
#include <algorithm>
#include <queue>
#include <string>

typedef long long LongType;

struct CharInfo
{
	char _ch; //字符
	LongType _count; //字符出现的编码
	string _code; //编码

	CharInfo(LongType count = 0)
		:_ch(0)
		,_count(count)
	{}
	bool operator!=(const CharInfo& c)
	{
		return _count != c._count;
	}
	
	CharInfo operator+(const CharInfo& c)
	{
		return (_count+c._count);
	}

	bool operator<(const CharInfo& c)
	{
		return (_count < c._count);
	}

	CharInfo operator+=(const CharInfo& c)
	{
		_count += c._count;
		return (_count);
	}
	bool operator==(const CharInfo& c)
	{
		return _count == c._count;
	}
	CharInfo operator++()
	{
		return _count++;
	}
	CharInfo operator--()
	{
		return _count--;
	}
};


class Filecompress
{
public:
	Filecompress()
	{
		for(int i = 0; i<256; ++i)
		{
			_infos[i]._ch = i;
		}
	}

	string CompressFile(const char* filename,CharInfo invalid = CharInfo())
	{
		FILE* fOut = fopen(filename,"rb");
		//assert(fOut);

		unsigned char ch = fgetc(fOut);

		while(!feof(fOut))
		{
			++(_infos[ch]._count);
			ch = fgetc(fOut);
		}

		HuffmanTree<CharInfo> hp(_infos,256,invalid);

		HuffmanTreeNode<CharInfo>* root = hp.GetRoot();
		string code = "";
		_GenerateHuffman(root,code);

		string name = filename;

		string compressfilename = name + ".compress";

		FILE* fIn = fopen(compressfilename.c_str(),"wb");
		assert(fIn);

		fseek(fOut,0,SEEK_SET);

		ch = fgetc(fOut);
		char pos = 0;
		int num = 0;
	
		while(!feof(fOut))
		{
			int size = _infos[ch]._code.size();

			for(int i = 0; i < size; ++i)
			{
				string& code = _infos[ch]._code;
				pos <<= 1;
				pos |= code[i] - '0';

				++num;

				if(num == 8) 
				{
					fputc(pos,fIn);
					pos = 0;
					num = 0;
				}

			}

			ch = fgetc(fOut);
		}
		
		if(num)
		{
			pos <<= (8-num);
			fputc(pos,fIn);
		}

		string configurename = name + ".configure";
		FILE* fConfigure = fopen(configurename.c_str(),"wb");
		string line;
		char buffer[128];

		for(int i = 0; i<256; ++i)
		{
			if(_infos[i] != invalid)
			{
				line += _infos[i]._ch;
				line += ',';
				line += itoa(_infos[i]._count,buffer,10);
				line +='\n';

				fwrite(line.c_str(),1, line.size(), fConfigure);
				line.clear();
			}
		}

		fclose(fConfigure);
		fclose(fOut);
		fclose(fIn);

		return compressfilename;
	}

	string UnCompressFile(const char* filename)
	{
		string name = filename;
		size_t index = name.rfind(".");
		name = name.substr(0,index);

		string configurename = name + ".configure";
		FILE* fConfigure = fopen(configurename.c_str(),"rb");
		assert(fConfigure);

		string uncompressfile = name+".uncompress";
		FILE* fIn = fopen(uncompressfile.c_str(),"wb");
		assert(fIn);

		string line;
		
		while(ReadLine(fConfigure,line))
		{
			{
				string sub = line.substr(2,line.size()-2);
				_infos[(unsigned char)line[0]]._count = atoi(sub.c_str());

				line.clear();
			}
			
		}
		HuffmanTree<CharInfo> hp(_infos,256);

		HuffmanTreeNode<CharInfo>* root = hp.GetRoot();
		HuffmanTreeNode<CharInfo>* cur = root;

		FILE* fOut = fopen(filename,"rb");
		assert(fOut);
	
		unsigned char ch = fgetc(fOut);
	
		CharInfo chcount = hp.GetRoot()->_weight;


		int pos = 7;
		while(!feof(fOut))
		{
			if(chcount == 0)
				break;
			if(cur == NULL)
				break;

			if(ch &(1<<pos))
				cur = cur->_right;
			else
				cur = cur->_left;

			--pos;

			if(cur->_left == NULL && cur->_right == NULL)
			{
				fputc(cur->_weight._ch,fIn);
				--_infos[(unsigned char)cur->_weight._ch]._count;
				--chcount;
				cur = root;
			}
			
			if(pos < 0)
			{
				ch = fgetc(fOut);
				pos = 7;
			}
		}

		fclose(fIn);
		fclose(fOut);
	
		return uncompressfile;
	}
protected:
	void _GenerateHuffman(HuffmanTreeNode<CharInfo>* root, string& code)
	{
		if(root == NULL)
				return;

		if(root->_left == NULL && root->_right == NULL)
		{
			_infos[(unsigned char)root->_weight._ch]._code = code;
			return;
		}
		if(root->_left)
			_GenerateHuffman(root->_left, code+'0');
		if(root->_right)
			_GenerateHuffman(root->_right, code+'1');

	}

	bool ReadLine(FILE*& fConfigure,string& str)
	{
		assert(fConfigure);

		int ch = fgetc(fConfigure);
		if (ch == EOF)
			return false;

		str.push_back(ch);
		ch = fgetc(fConfigure);
		while (ch != '\n' && ch != EOF)
		{
			str += ch;
			ch = fgetc(fConfigure);
		}

		return true;
	}
protected:
	CharInfo _infos[256];
};

void TestCompress()
{
	Filecompress fn;
	fn.CompressFile("证书.jpg");
	cout<<"压缩"<<endl;
}

void TestUnCompress()
{
	Filecompress fn;
	fn.UnCompressFile("证书.jpg.compress");
	cout<<"解压缩"<<endl;
}