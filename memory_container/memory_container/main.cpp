#include <cstdlib>
#include <stdio.h>
#include <iostream>
	using namespace std;

template<typename T>
class myAllocate
{
public:
	myAllocate(int memSize,void* pMem,size_t objectSize)
		:_memSize(memSize),_objectSize(objectSize)
	{		
		//_freeBlock = (void**)((void*)pMem);		//pointer to first free block
		_freeBlock = (void**)(pMem);		//pointer to first free block

		int numObjects = memSize / objectSize;	//numbero of object possible

		void** p = _freeBlock;

		for(int i=0; i < numObjects-1; i++)
		{
			*p = (void*)((unsigned int)p + _objectSize);	//put address to next block in this block first byte
			//cout << *p << endl;
			p = (void**)*p;
		}

		*p = nullptr;	//set last block to point to nothing
	}

	~myAllocate(){};
	
	T* allocate()
	{		

		if(_freeBlock == nullptr)
			return nullptr;

		void* tmp = _freeBlock;
		_freeBlock = (void**)(*_freeBlock);
		return (T*)(tmp);
	}

	void deallocate(void* objPtr)
	{
		*((void**)objPtr) = _freeBlock;
		_freeBlock = (void**)objPtr;
		
	}
	//T* getA(){return new T(a);}

	void** getCurrentPointer(){	return (void**)_freeBlock;}
private:
	int _memSize;
	void** _freeBlock;		//a pointer store the address that can be dereferenced to get a pointer that points to next free block
	size_t _objectSize;
};


struct myStruct
{
	int myInt;
	char myChar;
};

#define TEST_INT 0
#define TEST_STRUCT 1
#define MAX_NUM_ALLOCS 130


void main()
{
#if TEST_INT
	void* allocs[1024];

	myAllocate<int> myAlloc(sizeof(allocs),allocs,sizeof(int));
	
	int* i;

	for(int k=0; k < 1030;k++)
	{
		cout << myAlloc.getCurrentPointer() << endl;
		if( (i = myAlloc.allocate()) != nullptr)
			*i = 5;		
	} 
		
#endif
#if TEST_STRUCT
	void* allocs[1024];
	void* objList[MAX_NUM_ALLOCS];

	myAllocate<myStruct> myAlloc(sizeof(allocs),allocs,sizeof(myStruct));		

	cout << myAlloc.getCurrentPointer() << endl;
	for(int k=0; k < MAX_NUM_ALLOCS;k++)
	{		
		if( (objList[k] = myAlloc.allocate()) != nullptr)
		{
			((myStruct*)objList[k])->myInt = k;
			((myStruct*)objList[k])->myChar = (char)k;
			cout << k << "-" << ((myStruct*)objList[k])->myInt << endl;
		}
	}
	cout << myAlloc.getCurrentPointer() << endl;
	for(int k=MAX_NUM_ALLOCS-1; k >=  0;k--)
	{
		myAlloc.deallocate(objList[k]);
	}
	cout << myAlloc.getCurrentPointer() << endl;
		for(int k=0; k < MAX_NUM_ALLOCS;k++)
	{		
		if( (objList[k] = myAlloc.allocate()) != nullptr)
		{
			((myStruct*)objList[k])->myInt = k;
			((myStruct*)objList[k])->myChar = (char)k;
			cout << k << "-" << ((myStruct*)objList[k])->myInt << endl;
		}
	}
#endif
	/*
	cout << myAlloc.getCurrentPointer() << endl;
	cout << *i  << " " << i << endl;
	cout << myAlloc.getCurrentPointer() << endl;
	*/
	int j=0;
	scanf_s("type something",j);
	
	return;
}