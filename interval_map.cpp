#include <iostream>
#include <map>

template<typename K>
class Key
{
public:
    Key(K val) : _val(val) {}

    // copyable
    Key(K& k) : _val(k._val) {}

    // assignable
    void operator=(const Key<K>& k) { _val = k._val; }

    // less than
    bool operator<(const Key<K>& k) const { return _val < k._val; }

private:
    K _val;
    Key() = delete;
};

template<typename V>
class Value
{
public:
    Value(V val) : _val(val) {}

    // copyable
    Value(V& v) : _val(v._val) {}

    // assignable
    void operator=(const Value<V>& v) { _val = v._val; }

    // comparable via operator==
    bool operator==(const Value<V>& v) const { return _val == v._val; }

private:
    V _val;
    Value() = delete;
};


template<typename K, typename V>
class interval_map {
    V m_valBegin;
    std::map<K, V> m_map;
public:
    // constructor associates whole range of K with val
    interval_map(V const& val)
        : m_valBegin(val)
    {}

    // Assign value val to interval [keyBegin, keyEnd).
    // Overwrite previous values in this interval.
    // Conforming to the C++ Standard Library conventions, the interval
    // includes keyBegin, but excludes keyEnd.
    // If !( keyBegin < keyEnd ), this designates an empty interval,
    // and assign must do nothing.
    void assign(K const& keyBegin, K const& keyEnd, V const& val) {
        // INSERT YOUR SOLUTION HERE
        if (!(keyBegin < keyEnd)) return;

        if (m_map.empty()) {
            m_map.insert(std::make_pair(keyBegin, val));
            m_map.insert(std::make_pair(keyEnd, m_valBegin));
            return;
        }

        //canonical?
      
        bool isCanonical = true; 


        //cases for keyEnd
        if (!(m_map.rbegin()->first < keyEnd) && !(keyEnd < m_map.rbegin()->first)) isCanonical &= !(val == m_valBegin);

        else if (m_map.rbegin()->first < keyEnd) isCanonical &= !(val == m_valBegin);
      
        else if (!(keyEnd < m_map.begin()->first) && !(m_map.begin()->first < keyEnd)) { 
            isCanonical &= !(val == m_map.begin()->second);
        }
        else if (keyEnd < m_map.begin()->first) {
            isCanonical &= !(val == m_valBegin);
        }
        else {
            //to be
            auto b = m_map.find(keyEnd); 
            if (b != m_map.end()) {
                isCanonical &= !(val == b->second);

            }
            else {
                isCanonical &= !(val == m_valBegin);
            }
        }

      
        // cases for keyBegin
        if (!(keyBegin < m_map.begin()->first) && !(m_map.begin()->first < keyBegin)) isCanonical &= !(val == m_valBegin);
        else if (keyBegin < m_map.begin()->first) isCanonical &= !(val == m_valBegin);
        else if (!(m_map.rbegin()->first < keyBegin) && !(keyBegin < m_map.rbegin()->first)) {
            auto b = m_map.rbegin();
            isCanonical &= !((++b)->second == val);

        }
        else if (m_map.rbegin()->first < keyBegin) {
            isCanonical &= !(val == m_valBegin);
        }

        else {
            if(m_map.find(keyBegin) == m_map.end()) {
                //key is not there but still inside

                isCanonical &= !(val == m_valBegin);
            }
            else {
                auto b = m_map.begin();
                b = m_map.lower_bound(keyBegin);
                if (--b != m_map.end())   isCanonical &= !(val == (--b)->second);

            }

        }
        
            
        //if canonical
        if (isCanonical) {
            m_map.erase(m_map.lower_bound(keyBegin), m_map.lower_bound(keyEnd));
            V keyEndValue = operator[](keyEnd);   //
            m_map.insert_or_assign(keyBegin, val);
            m_map.insert_or_assign(keyEnd, keyEndValue);

        }



    }

    // look-up of the value associated with key
    V const& operator[](K const& key) const {
        auto it = m_map.upper_bound(key);
        if (it == m_map.begin()) {
            return m_valBegin;
        }
        else {
            return (--it)->second;
        }
    }

    void IntervalMapTest() {
       //m_map.assign(8, 10, 'k');
       //imap.assign(8, 12, 'k');
    }

    void show() {
        std::cout << "\nshow" << std::endl;
        for (auto entry : m_map) {
            std::cout << entry.first << "," << entry.second << std::endl;
        }
    }


};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.

int main(void) {

    interval_map<int, char> imap('A');
    

    imap.assign(8, 10, 'k');
    imap.show();
    imap.assign(8, 10, 'b');
    imap.show();
    imap.assign(10, 15, 'b');
    imap.show();
    imap.assign(0, 4, 'b');

    imap.show();





    return 0;
}