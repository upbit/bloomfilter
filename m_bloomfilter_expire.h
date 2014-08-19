#ifndef __MICRO_BLOOMFILTER_EXPIRE_H__
#define __MICRO_BLOOMFILTER_EXPIRE_H__

/**
 * BloomFilter带淘汰版本
 *
 * 利用大小为N的最小堆维持最近N个数据，在BloomFilter占用达到一定
 * 阀值时清空BloomFilter，并导入最小堆中数据，并清空最小堆，完成
 * BloomFilter的切换
 *
 */

#include <iostream>
#include <algorithm>
#include <vector>

#ifdef __cplusplus
extern "C" {
#include "m_bloomfilter.h"
}
#endif

using namespace std;

template<typename T>
class BloomFilter {
	public:
		BloomFilter(bool (*Compare)(T &, T &), 
				uint32_t max_item = 100000, 
				uint32_t max_keep = 1000, 
				double prob_false = 0.00001, 
				uint32_t seed = 0);
		~BloomFilter();

		int Add(T &key);
		int In(T &key);
		int Clear(int clear);
		int Expire();

		/* tool */
		void PrintHeap();
	
	private:
		int max_keep_;
		bool (*Compare_)(T &, T &);
		vector<T> *heap_;
		BaseBloomFilter bfilter_;
};

template<typename T>
BloomFilter<T>::BloomFilter(bool (*Compare)(T &, T &), 
		uint32_t max_item, 
		uint32_t max_keep, 
		double prob_false, 
		uint32_t seed): max_keep_(max_keep), Compare_(Compare)
{
	memset(&bfilter_, 0, sizeof(BaseBloomFilter));
	int ret = InitBloomFilter(&bfilter_, seed, max_item, prob_false);
	if (ret < 0)
		return;
	
	// 初始化堆
	heap_ = new vector<T>;
	heap_->reserve(max_keep + 1);
	make_heap(heap_->begin(), heap_->end(), Compare);
}

template<typename T>
BloomFilter<T>::~BloomFilter()
{
	// 释放bloom filter
	RealResetBloomFilter(&bfilter_);

	// 释放heap
	delete heap_;
}

template<typename T>
int BloomFilter<T>::Add(T &key)
{
	BloomFilter_Add(&bfilter_, (const void *)&key, sizeof(T));

	// 加入到堆中
	heap_->push_back(key);
	push_heap(heap_->begin(), heap_->end(), Compare_);
	
	// 大于max_keep_时弹出最小的一个 
	if (heap_->size() > max_keep_) {
		pop_heap(heap_->begin(), heap_->end(), Compare_);
		heap_->pop_back();
	}
	
	return 0;
}

template<typename T>
int BloomFilter<T>::In(T &key)
{
	return BloomFilter_Check(&bfilter_, (const void *)&key, sizeof(T)) == 0 ? 1 : 0;
}

template<typename T>
int BloomFilter<T>::Clear(int clear)
{
	int ret;
	
	// 重置bloom filter
	if (clear)
		ret = RealResetBloomFilter(&bfilter_);
	else
		ret = ResetBloomFilter(&bfilter_);

	// 清空heap
	heap_->clear();
	heap_->reserve(max_keep_);
	
	return 0;
}

template<typename T>
int BloomFilter<T>::Expire()
{
	int ret;

	// 清空旧的bloom filter
	ret = RealResetBloomFilter(&bfilter_);

	// 将heap中的导入到bloom filter
	for (int i = 0; i < heap_->size(); i++) {
		BloomFilter_Add(&bfilter_, (const void *)&heap_->at(i), sizeof(T));
	}

	// 清空heap
	heap_->clear();
	heap_->reserve(max_keep_ + 1);

	return 0;
}

template<typename T>
void BloomFilter<T>::PrintHeap()
{
	//sort_heap(heap_->begin(), heap_->end());
	cout << "Elements in Heap: " << endl;
	for (int i = 0; i < heap_->size(); i++) {
		cout << "  Element: " << heap_->at(i) << endl;
	}
}

#endif

