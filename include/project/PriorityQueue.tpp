#pragma once
#include <algorithm>
#include <stdexcept>
#include <utility>

template <typename K, typename V, typename Compare>
AdaptableHeapPriorityQueue<K, V, Compare>::AdaptableHeapPriorityQueue() : heap_{}, comp_{}, nextId_{1} {}
template <typename K, typename V, typename Compare>
AdaptableHeapPriorityQueue<K, V, Compare>::AdaptableHeapPriorityQueue(Compare comp) : heap_{}, comp_{std::move(comp)}, nextId_{1} {}
template <typename K, typename V, typename Compare>
AdaptableHeapPriorityQueue<K, V, Compare>::~AdaptableHeapPriorityQueue()
{
    for (auto e : heap_)
        delete e;
}
template <typename K, typename V, typename Compare>
AdaptableHeapPriorityQueue<K, V, Compare>::AdaptableHeapPriorityQueue(AdaptableHeapPriorityQueue &&other) noexcept : heap_(std::move(other.heap_)), comp_(std::move(other.comp_)), nextId_(other.nextId_)
{
    for (std::size_t i = 0; i < heap_.size(); ++i)
        heap_[i]->index = i;
    other.heap_.clear();
    other.nextId_ = 1;
}
template <typename K, typename V, typename Compare>
AdaptableHeapPriorityQueue<K, V, Compare> &AdaptableHeapPriorityQueue<K, V, Compare>::operator=(AdaptableHeapPriorityQueue &&other) noexcept
{
    if (this == &other)
        return *this;
    for (auto e : heap_)
        delete e;
    heap_ = std::move(other.heap_);
    comp_ = std::move(other.comp_);
    nextId_ = other.nextId_;
    for (std::size_t i = 0; i < heap_.size(); ++i)
        heap_[i]->index = i;
    other.heap_.clear();
    other.nextId_ = 1;
    return *this;
}
template <typename K, typename V, typename Compare>
std::size_t AdaptableHeapPriorityQueue<K, V, Compare>::size() const noexcept { return heap_.size(); }
template <typename K, typename V, typename Compare>
bool AdaptableHeapPriorityQueue<K, V, Compare>::empty() const noexcept { return heap_.empty(); }
template <typename K, typename V, typename Compare>
bool AdaptableHeapPriorityQueue<K, V, Compare>::less(EntryPtr a, EntryPtr b) const { return comp_(a->key, b->key); }
template <typename K, typename V, typename Compare>
void AdaptableHeapPriorityQueue<K, V, Compare>::swapEntries(std::size_t i, std::size_t j) noexcept
{
    std::swap(heap_[i], heap_[j]);
    heap_[i]->index = i;
    heap_[j]->index = j;
}
template <typename K, typename V, typename Compare>
void AdaptableHeapPriorityQueue<K, V, Compare>::upHeap(std::size_t i)
{
    while (i > 0)
    {
        std::size_t p = (i - 1) / 2;
        if (!less(heap_[i], heap_[p]))
            break;
        swapEntries(i, p);
        i = p;
    }
}
template <typename K, typename V, typename Compare>
void AdaptableHeapPriorityQueue<K, V, Compare>::downHeap(std::size_t i)
{
    while (true)
    {
        std::size_t l = 2 * i + 1;
        if (l >= heap_.size())
            break;
        std::size_t r = l + 1, c = l;
        if (r < heap_.size() && less(heap_[r], heap_[l]))
            c = r;
        if (!less(heap_[c], heap_[i]))
            break;
        swapEntries(i, c);
        i = c;
    }
}
template <typename K, typename V, typename Compare>
void AdaptableHeapPriorityQueue<K, V, Compare>::bubble(std::size_t i)
{
    if (i > 0 && less(heap_[i], heap_[(i - 1) / 2]))
        upHeap(i);
    else
        downHeap(i);
}
template <typename K, typename V, typename Compare>
void AdaptableHeapPriorityQueue<K, V, Compare>::validateHandle(EntryPtr e) const
{
    if (!e || e->index >= heap_.size() || heap_[e->index] != e)
        throw std::invalid_argument("Invalid or stale priority-queue entry handle");
}
template <typename K, typename V, typename Compare>
typename AdaptableHeapPriorityQueue<K, V, Compare>::EntryPtr AdaptableHeapPriorityQueue<K, V, Compare>::insert(const K &key, const V &value)
{
    // create a new entry
    EntryPtr entry = new Entry{key, value, heap_.size(), nextId_++};
    // add the entry to the heap
    heap_.push_back(entry);
    // restore the heap property
    upHeap(entry->index);
    // return the inserted entry
    return entry;
}
template <typename K, typename V, typename Compare>
const typename AdaptableHeapPriorityQueue<K, V, Compare>::Entry &AdaptableHeapPriorityQueue<K, V, Compare>::min() const
{
    if (empty())
        throw std::out_of_range("min() on empty priority queue");
    return *heap_.front();
}
template <typename K, typename V, typename Compare>
typename AdaptableHeapPriorityQueue<K, V, Compare>::Entry AdaptableHeapPriorityQueue<K, V, Compare>::removeMin()
{
    // check if the heap is empty
    if (empty())
        throw std::logic_error("removeMin() on empty priority queue");
    EntryPtr removed = heap_[0];
    Entry result = *removed;
    if (heap_.size() == 1)
    {
        heap_.pop_back();
    }
    else
    {
        heap_[0] = heap_.back();
        heap_.pop_back();
        heap_[0]->index = 0;
        downHeap(0);
    }
    delete removed;
    return result;
}
template <typename K, typename V, typename Compare>
void AdaptableHeapPriorityQueue<K, V, Compare>::remove(EntryPtr e)
{
    // check if the entry is valid
    validateHandle(e);
    // get the index of the entry to remove
    std::size_t index = e->index;

    // replace the entry with the last entry if it is not the last entry
    if (index == heap_.size() - 1)
    {
        heap_.pop_back();
        delete e;
        return;
    }

    heap_[index] = heap_.back();
    heap_[index]->index = index;
    heap_.pop_back();
    bubble(index);
    delete e;
}
template <typename K, typename V, typename Compare>
void AdaptableHeapPriorityQueue<K, V, Compare>::replaceKey(EntryPtr e, const K &k)
{
    // check if the entry is valid
    validateHandle(e);
    // update the key of the entry
    e->key = k;
    // restore the heap property
    bubble(e->index);
}
template <typename K, typename V, typename Compare>
void AdaptableHeapPriorityQueue<K, V, Compare>::replaceValue(EntryPtr e, const V &v)
{
    // check if the entry is valid
    validateHandle(e);
    // update the value of the entry
    e->value = v;
}
template <typename K, typename V, typename Compare>
void AdaptableHeapPriorityQueue<K, V, Compare>::buildHeap(std::vector<std::pair<K, V>> entries)
{
    // create a temporary vector of entries
    std::vector<EntryPtr> newHeap;
    newHeap.reserve(entries.size());

    for (const auto &entry : entries)
    {
        EntryPtr e = new Entry{entry.first, entry.second, newHeap.size(), nextId_++};
        newHeap.push_back(e);
    }

    // delete the entries in the current heap
    for (EntryPtr e : heap_)
        delete e;

    // replace the current heap with the temporary heap
    heap_ = std::move(newHeap);

    // build the heap bottom-up
    for (std::size_t i = heap_.size() / 2; i > 0; --i)
    {
        downHeap(i - 1);
    }
}

template <typename K, typename V, typename Compare>
bool AdaptableHeapPriorityQueue<K, V, Compare>::validHeap() const noexcept
{
    for (std::size_t i = 1; i < heap_.size(); ++i)
    {
        std::size_t p = (i - 1) / 2;
        if (less(heap_[i], heap_[p]) || heap_[i]->index != i)
            return false;
    }
    return true;
}
template <typename T, typename Compare>
void heapSort(std::vector<T> &values, Compare comp)
{
    // Get the number of elements.
    std::size_t n = values.size();

    // Helper function to restore the max-heap property.
    auto downHeap = [&](std::size_t root, std::size_t heapSize)
    {
        while (true)
        {
            std::size_t left = 2 * root + 1;
            std::size_t right = left + 1;
            std::size_t largest = root;

            // Check if the left child is larger.
            if (left < heapSize && comp(values[largest], values[left]))
                largest = left;

            // Check if the right child is larger.
            if (right < heapSize && comp(values[largest], values[right]))
                largest = right;

            // Stop if the parent is already in the correct position.
            if (largest == root)
                break;

            // Swap the parent with the largest child.
            std::swap(values[root], values[largest]);

            root = largest;
        }
    };

    // Build the max-heap from the bottom up.
    for (std::size_t i = n / 2; i > 0; --i)
    {
        downHeap(i - 1, n);
    }

    // Move the largest elements to the end of the vector.
    for (std::size_t i = n; i > 1; --i)
    {
        std::swap(values[0], values[i - 1]);

        // Restore the heap property.
        downHeap(0, i - 1);
    }
}
