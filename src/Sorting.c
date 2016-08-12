/**
 * Author: David Brotz
 * File: Sorting.c
 */

#include "Sorting.h"

#include "TaskPool.h"

#include <stdlib.h>
#include <string.h>

#define QUICKSORT_INSERTSZ (20)
#define BinaryHeapLeft(_Index) ((_Index) * 2 + 1) 
#define BinaryHeapRight(_Index) ((_Index) * 2 + 2) 
#define BinaryHeapParent(_Index) (((_Index) - 1) / 2) 

void InsertionSort(int* _Table, int _Size) {
	//Start at one as if i = 0 then i == j which is wasted time.
	for(int i = 1; i < _Size; ++i) {
		for(int j = 0; j < i; ++j) {
			//Find the correct index for the value at index j to reside in.
			if(_Table[i] < _Table[j]) {
				int _Temp = _Table[i];

				_Table[i] = _Table[j];
				_Table[j] = _Temp;
			}
		}
	}
}

void ShellSort(int* _Table, int _Size) {
	/**
	 * Find the number of bits equal to 0 that are after the last set bit
	 * this is equal to the next power of 2, so minus one from it to get the 
	 * power of 2 that is equal or less than _Size.
	 */
	int _GapIdx = (sizeof(int) * 8) - __builtin_clz(_Size) - 1;
	/**
	 * Get 2^k - 1, k = k - 1
	 */
	unsigned long int _Gap = (1 << _GapIdx--) - 1;

	while(_Gap > 0) {
		for(int i = _Gap; i < _Size; ++i) {
			int j = i;
			int _Temp = _Table[i];

			for(; j >= _Gap && _Table[j - _Gap] > _Temp && j < _Size; j -= _Gap) {
				_Table[j] = _Table[j -  _Gap];
			}
			//If the above loop does not trigger than i == j will always be true.
			_Table[j] = _Temp;
		}
		//Get the next value for the gap.
		//This will eventually equal 1 ensuring that every element will be compared to each other.
		_Gap = (1 << _GapIdx--) - 1;
	}
}

int QuickSortPartition(int* _Table, int _Size, int _Pivot) {
	int i = 0;
	int j = _Size - 1;
	int _Temp = 0;

	/**
	 * Find all elements that are greater than the pivot and
	 * put them on the right side of the pivot, and find all
	 * elements on the right side of the pivot index that are less than
	 * the pivot and put them on the left side of the pivot.
	 */
	while(1) {
		while(_Table[i] < _Pivot) {
			++i;
		}
		while(_Table[j] > _Pivot) {
			--j;
		}
		if(i >= j)
			return j;
		_Temp = _Table[i];
		_Table[i] = _Table[j];
		_Table[j] = _Temp;
	}
}

void QuickSort_Aux(int* _Table, int _Size, int(*_PivotFunc)(int*, int)) {
	int _Pivot = 0;
	int _PivotIdx = 0;
	
	//A table consisting of one element is always sorted.
	if(_Size <= 1)
		return;
	/**
	 * Find the pivot, then put all elements that are less than the pivot on the left of its
	 * and all elements that are greater on the right of the pivot, then subdivde the table
	 * in to two and repeat.
	 */
	_Pivot = _PivotFunc(_Table, _Size);
	_PivotIdx = QuickSortPartition(_Table, _Size, _Pivot);
	QuickSort_Aux(_Table, _Size - (_Size - _PivotIdx), _PivotFunc);
	QuickSort_Aux(_Table + _PivotIdx, (_Size - _PivotIdx), _PivotFunc);
}

struct QuickSortThreadData {
	int* Table;
	int Size;
	int(*PivotFunc)(int*, int);
};

void QuickSortThread_Aux(int _TaskId, void* _Data) {
	struct QuickSortThreadData* _Table = _Data;
	struct QuickSortThreadData _Left= {0};
	struct QuickSortThreadData _Right= {0};
	int _PivotIdx = 0;

	if(_Table->Size <= 1)
		return;
	_PivotIdx = QuickSortPartition(_Table->Table, _Table->Size, _Table->PivotFunc(_Table->Table, _Table->Size));
	if(_Table->Size >= 1024) {
		_Left.Table = _Table->Table;
		_Left.Size = _Table->Size - (_Table->Size - _PivotIdx);
		_Left.PivotFunc = _Table->PivotFunc;
		_Right.Table = _Table->Table + _PivotIdx;
		_Right.Size = _Table->Size - _PivotIdx;
		_Right.PivotFunc = _Table->PivotFunc;
		TaskPoolExecute(TaskPoolAdd(_TaskId, QuickSortThread_Aux, &_Left, sizeof(&_Left)));
		TaskPoolExecute(TaskPoolAdd(_TaskId, QuickSortThread_Aux, &_Right, sizeof(&_Right))); 
	} else {
		QuickSort_Aux(_Table->Table, _Table->Size - (_Table->Size - _PivotIdx), _Table->PivotFunc);
		QuickSort_Aux(_Table->Table + _PivotIdx, (_Table->Size - _PivotIdx), _Table->PivotFunc);
	}
}

/**
 * Same as above but if there are few enough elements in the table the 
 * algorithm will instead use an insertion sort.
 */
void QuickSortInsert_Aux(int* _Table, int _Size, int(*_PivotFunc)(int*, int)) {
	int _Pivot = 0;
	int _PivotIdx = 0;

	if(_Size <= 1)
		return;
	if(_Size <= QUICKSORT_INSERTSZ) {
		InsertionSort(_Table, _Size);
		return;
	}
	_Pivot = _PivotFunc(_Table, _Size);
	_PivotIdx = QuickSortPartition(_Table, _Size, _Pivot);
	QuickSort_Aux(_Table, _Size - (_Size - _PivotIdx), _PivotFunc);
	QuickSort_Aux(_Table + _PivotIdx, (_Size - _PivotIdx), _PivotFunc);
	
}

int RightPivot(int* _Table, int _Size) {
	return _Table[_Size - 1];
}

int MedianPivot(int* _Table, int _Size) {
	int _Mid = _Size / 2;
	int _Temp = 0;

	//Sort the three points.
	if(_Table[_Size - 1] < _Table[0]) {
		_Temp = _Table[_Size - 1];
		_Table[_Size - 1] = _Table[0];
		_Table[0] = _Temp;	
	}
	if(_Table[_Mid] < _Table[0]) {
		_Temp = _Table[_Mid];
		_Table[_Mid] = _Table[0];
		_Table[0] = _Temp;
	}
	if(_Table[_Size - 1] < _Table[_Mid]) {
		_Temp = _Table[_Size - 1];
		_Table[_Size - 1] = _Table[_Mid];
		_Table[_Mid] = _Temp;	
	}
	return _Table[_Mid];
}

/**
 * Sort the two tables and place the result in _OutTable.
 */
void MergeSort_Merge(int* _Table, int* _OutTable, int _Size) {
	/**
	 * We don't need to use 2 pointers for _Table since we know how 
	 * MergeSort_Aux split the table into 2.
	 */
	int _Mid = _Size / 2;
	int i = 0;
	int j = _Mid;

	for(int k = 0; k < _Size; ++k) {
		if(i < _Mid && (j >= _Size || (_Table[i] < _Table[j]))) {
			//Pick this if its less or no more elements exist on the right table.
			_OutTable[k] = _Table[i++];
		} else {
			//Pick this if its less than i or no more elemenets exist in the left table.
			_OutTable[k] = _Table[j++];
		}
	}
}

void MergeSort_Aux(int* _Table, int* _OutTable, int _Size) {
	int _Left = _Size / 2;
	/**
	 * Use this instead of _Size / 2 to account for off by 1 errors
	 * when dividing odd integers.
	 */
	int _Right = _Size - _Left;
	if(_Size <= 1)
		return;
	MergeSort_Aux(_Table, _OutTable, _Left);
	MergeSort_Aux(_Table + _Left, _OutTable + _Left, _Right);
	MergeSort_Merge(_Table, _OutTable, _Size);
	/**
	 * Copy _OutTable back to _Table to keep the constraint of elements of the sorted
	 * section of _Table being in order.
	 */
	memcpy(_Table, _OutTable, sizeof(int) * _Size);
}

void HeapShiftDown(int* _Table, int _Size, int _Elem) {
	int _Left = 0;
	int _Right = 0;

	/**
	 * Get the bigger element of _Elem's children and swap it with _Elem
	 * then do the same with the selected child.
	 */
	while((_Left = BinaryHeapLeft(_Elem)) < _Size) {
		int _Temp = _Elem;

		if(_Table[_Temp] < _Table[_Left])
			_Temp = _Left;
		if((_Right = BinaryHeapRight(_Elem)) < _Size) {
			if(_Table[_Temp] < _Table[_Right])
				_Temp = _Right; 
		}
		if(_Temp == _Elem) {
			break;
		}
		else {
			int _TempVal = _Table[_Temp];

			_Table[_Temp] = _Table[_Elem];
			_Table[_Elem] = _TempVal;
			_Elem = _Temp;
		}
	}
}

void Heapify(int* _Table, int _Size) {
	int _Parent = 0;
 
 	_Parent = BinaryHeapParent(_Size - 1);
	while(_Parent >= 0) {
		HeapShiftDown(_Table, _Size, _Parent);
		--_Parent;
	}
}

void HeapSort(int* _Table, int _Size) {
	int _Temp = 0;

	for(int i = _Size - 1; i >= 0; --i) {
		_Temp = _Table[i];
		_Table[i] = _Table[0];
		_Table[0] = _Temp;
		HeapShiftDown(_Table, i, 0);	
	}
}
