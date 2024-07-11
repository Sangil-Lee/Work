#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

const int TEST_TABLE_SIZE = 10000000;



template<class K, class V>
class SimpleHash {
 public:
  int usedslots = 0;

  std::vector<K> keys;
  std::vector<V> vals;
  std::vector<uint8_t> used;

  //size0 should be a prime and about 30% larger than the maximum number needed
  SimpleHash(int size0){
    vals.resize(size0);
    keys.resize(size0);
    used.resize(size0/8+1,0);
  }

  //If the key values are already uniformly distributed, using a hash gains us
  //nothing
  uint64_t hash(const K key){
    return key;
  }

  bool isUsed(const uint64_t loc){
    const auto used_loc = loc/8;
    const auto used_bit = 1<<(loc%8);
    return used[used_loc]&used_bit;    
  }

  void setUsed(const uint64_t loc){
    const auto used_loc = loc/8;
    const auto used_bit = 1<<(loc%8);
    used[used_loc] |= used_bit;
  }

  void insert(const K key, const V val){
    uint64_t loc = hash(key)%keys.size();

    //Use linear probing. Can create infinite loops if table too full.
    while(isUsed(loc)){ loc = (loc+1)%keys.size(); }

    setUsed(loc);
    keys[loc] = key;
    vals[loc] = val;
  }

  V& get(const K key) {
    uint64_t loc = hash(key)%keys.size();

    while(true){
      if(!isUsed(loc))
        throw std::runtime_error("Item not present!");
      if(keys[loc]==key)
        return vals[loc];

      loc = (loc+1)%keys.size();
    }
  }

  uint64_t usedSize() const {
    return usedslots;
  }

  uint64_t size() const {
    return keys.size();
  }
};

typedef SimpleHash<uint64_t, char> table_t;

void SaveSimpleHash(const table_t &map){
  std::cout<<"Save. ";
  const auto start = std::chrono::steady_clock::now();
  FILE *f = fopen("./map", "wb");
  uint64_t size = map.size();
  fwrite(&size, 8, 1, f);
  fwrite(map.keys.data(), 8, size, f);
  fwrite(map.vals.data(), 1, size, f);
  fwrite(map.used.data(), 1, size/8+1, f);
  fclose(f);
  const auto end = std::chrono::steady_clock::now();
  std::cout<<"Save time = "<< std::chrono::duration<double, std::milli> (end-start).count() << " ms" << std::endl;
}

table_t LoadSimpleHash(){
  std::cout<<"Load. ";
  const auto start = std::chrono::steady_clock::now();
  FILE *f = fopen("./map", "rb");

  uint64_t size;
  fread(&size, 8, 1, f);

  table_t map(size);
  fread(map.keys.data(), 8, size, f);
  fread(map.vals.data(), 1, size, f);
  fread(map.used.data(), 1, size/8+1, f);
  fclose(f);
  const auto end = std::chrono::steady_clock::now();
  std::cout<<"Load time = "<< std::chrono::duration<double, std::milli> (end-start).count() << " ms" << std::endl;

  return map;
}

int main(){
  //Perfectly horrendous way of seeding a PRNG, but we'll do it here for brevity
  auto generator = std::mt19937(12345); //Combination of my luggage
  //Generate values within the specified closed intervals
  auto key_rand  = std::bind(std::uniform_int_distribution<uint64_t>(0,std::numeric_limits<uint64_t>::max()), generator);
  auto val_rand  = std::bind(std::uniform_int_distribution<int>(std::numeric_limits<char>::lowest(),std::numeric_limits<char>::max()), generator);

  table_t map(1.3*TEST_TABLE_SIZE);
  std::cout<<"Created table of size "<<map.size()<<std::endl;

  std::cout<<"Generating test data..."<<std::endl;
  for(int i=0;i<TEST_TABLE_SIZE;i++)
    map.insert(key_rand(),(char)val_rand()); //Low chance of collisions, so we get quite close to the desired size

  map.insert(23,42);
  assert(map.get(23)==42);

  SaveSimpleHash(map);
  auto newmap = LoadSimpleHash();

  //Ensure that the load worked
  for(int i=0;i<map.keys.size();i++)
    assert(map.keys.at(i)==newmap.keys.at(i));
  for(int i=0;i<map.vals.size();i++)
    assert(map.vals.at(i)==newmap.vals.at(i));  
  for(int i=0;i<map.used.size();i++)
    assert(map.used.at(i)==newmap.used.at(i));    
}
