#include <bitset>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
 
struct Key
{
    std::string first;
    std::string second;
};
 
struct KeyHash
{
    std::size_t operator()(const Key& k) const
    {
        return std::hash<std::string>()(k.first) ^
            (std::hash<std::string>()(k.second) << 1);
    }
};
 
struct KeyEqual
{
    bool operator()(const Key& lhs, const Key& rhs) const
    {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }
};
 
struct Foo
{
    Foo(int val_) : val(val_) {}
    int val;
    bool operator==(const Foo &rhs) const { return val == rhs.val; }
};
 
template<> struct std::hash<Foo>
{
    std::size_t operator()(const Foo &f) const
    {
        return std::hash<int>{}(f.val);
    }
};
 
int main()
{
    // default constructor: empty map
    std::unordered_map<std::string, std::string> m1;
 
    // list constructor
    std::unordered_map<int, std::string> m2 =
    {
        {1, "foo"},
        {3, "bar"},
        {2, "baz"}
    };
 
    // copy constructor
    std::unordered_map<int, std::string> m3 = m2;
 
    // move constructor
    std::unordered_map<int, std::string> m4 = std::move(m2);
 
    // range constructor
    std::vector<std::pair<std::bitset<8>, int>> v = {{0x12, 1}, {0x01,-1}};
    std::unordered_map<std::bitset<8>, double> m5(v.begin(), v.end());
 
    // Option 1 for a constructor with a custom Key type
    // Define the KeyHash and KeyEqual structs and use them in the template
    std::unordered_map<Key, std::string, KeyHash, KeyEqual> m6 =
    {
        {{"John", "Doe"}, "example"},
        {{"Mary", "Sue"}, "another"}
    };
 
    // Option 2 for a constructor with a custom Key type.
    // Define a const == operator for the class/struct and specialize std::hash
    // structure in the std namespace
    std::unordered_map<Foo, std::string> m7 =
    {
        {Foo(1), "One"}, {2, "Two"}, {3, "Three"}
    };
 
    // Option 3: Use lambdas
    // Note that the initial bucket count has to be passed to the constructor
    struct Goo { int val; };
    auto hash = [](const Goo &g){ return std::hash<int>{}(g.val); };
    auto comp = [](const Goo &l, const Goo &r){ return l.val == r.val; };
    std::unordered_map<Goo, double, decltype(hash), decltype(comp)> m8(10, hash, comp);
}
