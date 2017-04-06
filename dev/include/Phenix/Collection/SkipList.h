/*******************************************************************************
 * @brief	����
 * @date	2017-4-3
 * @author	phenix
 * @mark    
 ******************************************************************************/
#ifndef PHENIX_COLLECTION_SKIP_LIST_H
#define PHENIX_COLLECTION_SKIP_LIST_H

namespace Phenix
{
namespace Collection
{

template<typename T>
class SkipList	
{
	struct Node
	{
		T	value;
		std::vector<*Node> next;
	};
	
public:
	SkipList(){}
	virtual ~SkipList(){}	



private:
	std::vector<*Node> _head;	// ͷ���
};


}
}


#endif // PHENIX_COLLECTION_SKIP_LIST_H