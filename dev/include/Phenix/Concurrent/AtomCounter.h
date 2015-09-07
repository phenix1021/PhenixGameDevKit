/*******************************************************************************
 * @brief	原子计数器            
 * @date	2015-9-7
 * @author	phenix
 * @mark 
 ******************************************************************************/
#ifndef PHENIX_CONCURRENT_ATOM_COUNTER_H
#define PHENIX_CONCURRENT_ATOM_COUNTER_H

namespace Phenix
{
namespace Concurrent
{	

	class AtomCounter		
	{
	public:	
		AtomCounter();
		AtomCounter(Phenix::Int32 val);
		virtual ~AtomCounter();		

		inline AtomCounter& operator = (Phenix::Int32 val);

		inline Phenix::Int32 value();
		inline Phenix::Int32 inc();
		inline Phenix::Int32 dec();
		inline Phenix::Int32 add(Phenix::Int32 delta);

	private:
		volatile long _val; 
	};	


} // end namespace Concurrent
} // end namespace Phenix


#endif