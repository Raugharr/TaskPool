/**
 * Author: David Brotz
 * File: Sorting.h
 */

#ifndef __SORTING_H
#define __SORTING_H

void InsertionSort(int* _Table, int _Size);
void ShellSort(int* _Table, int _Size);
int QuickSortPartition(int* _Table, int _Size, int _Pivot);
void QuickSort_Aux(int* _Table, int _Size, int(*_PivotFunc)(int*, int));
int RightPivot(int* _Table, int _Size);
int MedianPivot(int* _Table, int _Size);
void QuickSortInsert_Aux(int* _Table, int _Size, int(*_PivotFunc)(int*, int));
void MergeSort_Aux(int* _Table, int* _OutTable, int _Size);
void Heapify(int* _Table, int _Size);
void HeapSort(int* _Table, int _Size);
#endif
