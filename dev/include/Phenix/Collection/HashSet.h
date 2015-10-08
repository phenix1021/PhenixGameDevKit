/*******************************************************************************
 * @brief	¹þÏ£±í£¨ÏßÐÔ¹þÏ££©
 * @date	2015-10-5
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_COLLECTION_HASHTABLE_H
#define PHENIX_COLLECTION_HASHTABLE_H

#include <Phenix/Base/Hash.h>

using Phenix::HashCodeTraits;

namespace Phenix
{
namespace Collection
{

	template<typename T>
	class HashTable
	{
		typedef HashCodeTraits<T> Value;
		typedef std::vector<Value> Bucket;
		typedef Bucket::iterator BucketIterator;
		typedef Bucket::const_iterator ConstBucketIterator;
		typedef std::vector<Bucket> BucketVec;
		typedef BucketVec::iterator BucketVecIterator;
		typedef BucketVec::const_iterator ConstBucketVecIterator;

	public:
		T* find(T& );
	private:
		BucketVec _buckets;
	};

	typedef HashTable HashSet;

}
}


#endif