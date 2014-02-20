## BloomFilter_x64实现

仿照Cassandra中的BloomFilter实现，Hash选用[MurmurHash2]，通过双重散列公式生成散列函数
~~~~~ c
  Hash(key, i) = (H1(key) + i * H2(key)) % m
~~~~~

版本历史
---------------------------------
 * 2013.4.10  增加k/m的动态计算功能，参考：http://hur.st/bloomfilter
 * 2012.12    完成初始版本

BloomFilter使用例子
---------------------------------
~~~~~ c
  static BaseBloomFilter stBloomFilter = {0};

  // 初始化BloomFilter(容纳100,000元素，假阳概率不超过万分之一)：
  InitBloomFilter(&stBloomFilter, 0, 100000, 0.00001);
  // 重置BloomFilter：
  ResetBloomFilter(&stBloomFilter);
  // 释放BloomFilter:
  FreeBloomFilter(&stBloomFilter);

  // 向BloomFilter中新增一个数值（0-正常，1-加入数值过多）：
  uint32_t dwValue;
  iRet = BloomFilter_Add(&stBloomFilter, &dwValue, sizeof(uint32_t));

  // 检查数值是否在BloomFilter内（0-存在，1-不存在）：
  iRet = BloomFilter_Check(&stBloomFilter, &dwValue, sizeof(uint32_t));

  // (1.1新增) 将生成好的BloomFilter写入文件:
  iRet = SaveBloomFilterToFile(&stBloomFilter, "dump.bin")
  // (1.1新增) 从文件读取生成好的BloomFilter:
  iRet = LoadBloomFilterFromFile(&stBloomFilter, "dump.bin")
~~~~~

[MurmurHash2]: https://sites.google.com/site/murmurhash/
