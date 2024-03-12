#ifndef HASHMAP
#define HASHMAP

#include <string>
#include <vector>
#include <utility>
#include <list>
#include <iostream>

template <typename T>
class HashMap
{
public:
	HashMap(double max_load = 0.75); // constructor
	~HashMap(); // destructor; deletes all of the items in the hashmap
	int size() const; // return the number of associations in the hashmap
	// The insert method associates one item (key) with another (value).
	// If no association currently exists with that key, this method inserts
	// a new association into the hashmap with that key/value pair. If there is
	// already an association with that key in the hashmap, then the item
	// associated with that key is replaced by the second parameter (value).
	// Thus, the hashmap must contain no duplicate keys.
	void insert(const std::string& key, const T& value);
	// Defines the bracket operator for HashMap, so you can use your map like this:
	// your_map["david"] = 2.99;
	// If the key does not exist in the hashmap, this will create a new entry in
	// the hashmap and map it to the default value of type T (0 for builtin types).
	// It returns a reference to the newly created value in the map.
	T& operator[](const std::string& key);
	// If no association exists with the given key, return nullptr; otherwise,
	// return a pointer to the value associated with that key. This pointer can be
	// used to examine that value within the map.
	const T* find(const std::string& key) const;
	HashMap(const HashMap&) = delete;
	HashMap& operator=(const HashMap&) = delete;
	// If no association exists with the given key, return nullptr; otherwise,
	// return a pointer to the value associated with that key. This pointer can be
	// used to examine that value or modify it directly within the map.

	T* find(const std::string& key) //this is to edit the value 
	{
		const auto& hm = *this;
		return const_cast<T*>(hm.find(key));
	}

private:
	int m_buckets; //# of buckets
	int m_size; //# of items 
	double m_load;
	std::vector<std::list<std::pair<std::string, T>>> m_map;

	// make a struct for pairs

	int stringHash(const std::string& key) const
	{
		size_t k = std::hash<std::string>()(key);
		return k % m_buckets; //get mod of the string in order to hash
	}

	void rehash()
	{
		m_buckets *= 2; //multply the number of buckets by 2
		std::vector<std::list<std::pair<std::string, T>>> newMap;
		newMap.resize(m_buckets); //double the size

		for (auto li : m_map) //how to do this without auto
		{
			for (auto pr : li)
			{
				int index = stringHash(pr.first);

				newMap[index].push_back(pr);
			}
		}

		m_map = newMap;
	}
};

//PUBLIC

template <typename T>
HashMap<T>::HashMap(double max_load)
{
	m_buckets = 10;
	m_load = max_load;
	m_map.resize(10);
	m_size = 0;
}

template <typename T>
HashMap<T>::~HashMap()
{
	//empty destructor for now
}

template <typename T>
int HashMap<T>::size() const
{
	return m_size;
}

template <typename T> //TO DO insert doesn't change value of existing key
void HashMap<T>::insert(const std::string& key, const T& value)
{
	//first check to see if the key is already in the map
	T* current = find(key); //uses the other find function so not const return 
	if (current != nullptr)
	{
		*current = value; //change the current value with new value if in map already
		return;
	}

	m_size++;
	if ((double) m_size / m_buckets > m_load) //should evaluate to a double  //check if the number of items / buckets exceeds the load amount
	{
		rehash(); //rehash the map
	}

	int hashNum = stringHash(key);
	m_map[hashNum].push_back(std::make_pair(key, value));
}

template <typename T> // TO DO operator [] doesn't insert a new key?? need to do this??
T& HashMap<T>::operator[](const std::string& key)
{
	//return a pointer to the value associated with a key if in map or nullptr if not in map
	T* inMap = find(key);
	
	if (inMap != nullptr)
	{
		return *inMap;
	}
	else //key is not in map
	{
		insert(key, T()); //T value will be overriden once we return T*
		return *(find(key));
	}

	//SHOULD NEVER GET TO THIS POINT
}

template <typename T>
const T* HashMap<T>::find(const std::string& key) const
{
	int hashNum = stringHash(key); //get the index of the key

	for (const auto &pr : m_map[hashNum]) //how to do this without auto?
	{
		if (pr.first == key)
		{
			return &(pr.second);
		}
	}

	return nullptr;
}

//PRIVATE

#endif