#include <iostream>
#include <stdexcept>
#include <iomanip>

template<typename T>
class Range {
private:
    T _first;
    T _last;
    T _step;
    T _offset;
    T _offsetsize;
    uint64_t _count;

public:
    class Iterator {
    private:
        Range& range;
        T current;

    public:
        Iterator(Range& r, T current) : range(r), current(current) {}

        T operator*() const { return current; }

        Iterator& operator++() {
            current += range._step;
			range._offset = current * range._offsetsize;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return (range._step > 0) ? (current < other.current) : (current > other.current);
        }
    };

    Range(T last) : _first(0), _last(last), _step(1), _offsetsize(0), _count(0) {}
    Range(T first, T last) : _first(first), _last(last), _step(1), _offsetsize(0), _count(0) {}
    Range(T first, T last, T step) : _first(first), _last(last), _step(step), _offsetsize(0), _count(0) {
        if (_step == 0) {
            throw std::invalid_argument("step 0 not possible");
        }
    }

    Iterator begin() { return Iterator(*this, _first); }
    Iterator end() { return Iterator(*this, _last); }
    T offset() const { return _offset; }
    void setoffsetsize(T size) { _offsetsize = size; }
    uint64_t count() {return _count++;};
    void reset(T first, T last) {
	    _first=first;
	    _last=last;
    };
};


using namespace std;

int main(int argc, char* argv[])
{
#if 0
    cout << "[Group1--> " << endl;
    for (const auto& i : Range<uint32_t>(1, 1001, 1)) {
        cout << "<pv name=\"ctrlslab:Alarm"<<i<<"\">"<<endl;
        cout << "\t<enabled>true</enabled>"<<endl;
        cout << "</pv>"<<endl;
    }
    cout <<"<--Group1]"<< endl;

    cout << "[Group2--> " << endl;
    for (const auto& i : Range<uint32_t>(1001, 2001, 1)) {
        cout << "<pv name=\"ctrlslab:Alarm"<<i<<"\">"<<endl;
        cout << "\t<enabled>true</enabled>"<<endl;
        cout << "</pv>"<<endl;
    }
    cout <<"<--Group2]"<< endl;

    cout << "[Group3--> " << endl;
    for (const auto& i : Range<uint32_t>(2001, 3001, 1)) {
        cout << "<pv name=\"ctrlslab:Alarm"<<i<<"\">"<<endl;
        cout << "\t<enabled>true</enabled>"<<endl;
        cout << "</pv>"<<endl;
    }
    cout <<"<--Group3]"<< endl;

    cout << "[Group4--> " << endl;
    for (const auto& i : Range<uint32_t>(3001, 4001, 1)) {
        cout << "<pv name=\"ctrlslab:Alarm"<<i<<"\">"<<endl;
        cout << "\t<enabled>true</enabled>"<<endl;
        cout << "</pv>"<<endl;
    }
    cout <<"<--Group4]"<< endl;

    cout << "[Group5--> " << endl;
    for (const auto& i : Range<uint32_t>(4001, 5001, 1)) {
        cout << "<pv name=\"ctrlslab:Alarm"<<i<<"\">"<<endl;
        cout << "\t<enabled>true</enabled>"<<endl;
        cout << "</pv>"<<endl;
    }
    cout <<"<--Group5]"<< endl;
#else

#if 0
    #define PVCOUNT 5000 
#if PVCOUNT == 1000
    const int grpPVCount = pvCount;
    /* 1000PV, 10Group, 100pv/group*/
    Range<uint32_t> group(1, grpCount);
    Range<uint32_t> pvs_per_grp(1, grpPVCount+1);
    
#elif PVCOUNT == 5000
    /* 5000PV, 5Group, 1000pv/group*/
    Range<uint32_t> group(1, 6);
    Range<uint32_t> pvs_per_grp(1, 1001);
    const int grpPVCount = 1000;
#endif
#endif

    if(argc != 3) {
	cout << "./a.out Groups PVs_per_Group" << endl;
	return -1;
    };

    long grpCount = stol(argv[1]);
    long pvCount  = stol(argv[2]);

    const int grpPVCount = pvCount;
    Range<uint32_t> group(1, grpCount);
    Range<uint32_t> pvs_per_grp(1, grpPVCount+1);
    

    cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    cout << "<config name=\"Accelerator\">" << endl;
    for (const auto& grpNo : group) {
	cout << "<component name=\"Alarm:GROUP"<< grpNo << "\">" << endl;
	for (const auto& i : pvs_per_grp) {
	    cout << "\t<pv name=\"ctrlslab:Alarm"<<i<<"\">"<<endl;
	    cout << "\t\t<enabled>true</enabled>"<<endl;
	    cout << "\t</pv>"<<endl;
	}
    	cout << "</component>" << endl;
	uint32_t startNo = (grpNo*grpPVCount)+1;
	uint32_t endNo   = startNo+grpPVCount;
    	pvs_per_grp.reset(startNo, endNo);
    };

#endif
    cout << "</config>" << endl;

}
