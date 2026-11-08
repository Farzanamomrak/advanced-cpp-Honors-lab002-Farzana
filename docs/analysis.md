# Lab 22 — Complexity Analysis

## 1. Heap Height
A complete binary heap is a binary tree where every level is full except possibly the last level, which is filled from left to right. Because each level can contain twice as many nodes as the level before it, the number of nodes grows exponentially as the height increases. For a heap with n entries, its height is approximately log_2 n. Therefore, the height of a complete binary heap is O(log n). This is important because operations that move an entry up or down the tree only need to travel through its height.

## 2. Priority Queue Operations
The insert operation takes O(log n) time. A new entry is added to the end of the heap, and then the up-heap operation moves it toward the root if its key is smaller than its parent's key. In the worst case, it can move through the entire height of the heap.
The min operation takes O(1) time because the smallest entry is always stored at the root of the min-heap. The program only needs to access the first entry without searching through the other elements.
The removeMin operation takes O(log n) time. It removes the root, moves the last entry into its place, and uses down-heap to restore the heap property. The entry moves from the root to the bottom level, so its running time depends on the height of the heap.

## 3. Adaptable Operations
In my implementation, each entry stores its current array index. This index works as a handle that allows the program to locate an entry without searching through the entire heap. For remove(entry), the program uses the handle to find the entry, replaces it with the last entry, and restores the heap property using the bubble operation. This takes O(log n) time. For replaceKey(entry, key), the program updates the key and uses the bubble operation to restore the correct heap order, which also takes O(log n) time.
Whenever two entries are swapped, their indexes are updated so that their handles continue to point to the correct positions. If the program scanned the entire heap to find an entry, the operation would take O(n) time instead of meeting the required O(log n) time.

## 4. Bottom-Up Construction
This is a repeated insertion and can take O(n log n) time in the worst case because each of the n entries may need to move up the heap. Each insertion can take up to O(log n) time.
My buildHeap implementation uses bottom-up construction instead. It first places all entries into a vector and then starts restoring the heap property from the bottom toward the root. This method takes o(n) time because most nodes are near the bottom of the tree and only need to move a short distance. Only a small number of nodes near the root can move through many levels. When the work for all nodes is added together, the total is linear rather than O(n log n).

## 5. Heap Sort
My heapSort implementation uses a max-heap to sort the values. First, it builds a max-heap from the vector. Then, it repeatedly swaps the largest element at the root with the last element in the heap and restores the heap property for the remaining elements. Building the heap takes O(n) time. Removing the largest element and restoring the heap takes O(log n) time, and this process is repeated for the elements in the vector. Therefore, the overall running time is O(n log n).
The algorithm is in place because it sorts the original vector without creating another vector to store the sorted values. It uses O(1) auxiliary space because it only uses a few variables and swaps elements within the original vector.

## 6. Design Reflection
A heap-based priority queue is useful when both insertion and minimum removal need to be efficient. In a heap, inserting an entry takes O(log n), and removing the minimum also takes O(log n). Finding the minimum takes only O(1) time.
An unsorted list makes insertion easy because a new entry can be added in O(1) time but finding and removing the minimum requires searching through the list, which takes O(n) time.
A sorted list makes finding and removing the minimum efficient because the smallest entry is kept at the beginning. However, inserting a new entry in the correct position can take O(n) time because other entries may need to be shifted.

A heap is a good choice when both insertion and minimum removal are important. It provides a balance between the two operations and avoids the slower O(n) operations that can happen with unsorted and sorted lists.