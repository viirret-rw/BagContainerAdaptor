#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <cstddef>
#include <vector>
#include <list>

template <typename Key, typename Value, size_t BucketSize = 100>
class HashMap
{
public:
	// I think this is somewhat ugly syntax for inserting stuff.
	// But I cannot think of a better one, at least yet.
	// This is somewhat similar syntax to std::map so it's most likely okay.
	using value_type = std::pair<Key, Value>;

	struct Node
	{
		/// Constructor.
		Node(const value_type& hashPair) :
			m_key(hashPair.first), m_value(hashPair.second)
		{
		}

		Key m_key;
		Value m_value;
		Node* m_next = nullptr;
	};

	class iterator : public std::iterator<
						std::forward_iterator_tag, 
						Node, 
						std::ptrdiff_t, 
						Node*, 
						Node&
						>
	{
	public:
		explicit iterator(Node* node) : m_currentNode(node)
		{
		}

		Value& operator*() const
		{
			return m_currentNode->m_value;
		}

		Value* operator->() const
		{
			return &(m_currentNode->m_value);
		}

		iterator& operator++()
		{

		}

	private:
		Node* m_currentNode;
	};

    // Constructor
    HashMap(size_t numBuckets = BucketSize) : numBuckets(numBuckets)
    {
        buckets.resize(numBuckets);
    }
	

	void insert(const value_type& hashPair)
	{
		size_t index = hash(hashPair.first);
		auto& bucket = buckets[index];

		bucket.push_back(Node(hashPair));
	}

	// I don't actually know if this function HAVE to be implemented,
	// but it would make sense for the HashMap only to be able to delete
	// based on iterator.

	// Also deleting by Key could be nice but it's really hard to implement
	// from arch perspective.
	void remove(const value_type& hashPair) = delete;

private:

	// TODO This should be implemented with my custom types.
	std::vector<std::list<Node>> buckets;

	size_t numBuckets;

	// Simple hash function.
	size_t hash(const Key& key) const
	{
		return std::hash<Key>{}(key) % numBuckets;
	}

};

#endif
