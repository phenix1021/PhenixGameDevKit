#include "stdafx.h"
#include "../include/timer.h"


namespace Phenix
{
namespace Timer
{
	TimingWheel::Wheel::Wheel(Wheel* lower_wheel, Wheel* higher_wheel, Phenix::UInt16 size)
		:m_lower_wheel(lower_wheel),m_higher_wheel(higher_wheel),m_size(size),m_cur_idx(0)
	{
		m_nodes.reserve(size);
	}

	TimingWheel::TimingWheel()		
	{
		m_work_wheel = new Wheel(NULL, NULL, WORK_WHEEL_SIZE);
		Wheel* lower_wheel = m_work_wheel;
		for (Phenix::UInt16 i=1; i<=ASSIST_WHEEL_COUNT; ++i)
		{
			lower_wheel->m_higher_wheel = new Wheel(lower_wheel, NULL, ASSIST_WHEEL_SIZE);
			lower_wheel = lower_wheel->m_higher_wheel;
			lower_wheel->m_nodes[0].lower_wheel = lower_wheel->m_lower_wheel;
		}
	}

	TimingWheel::~TimingWheel()
	{

	}

	TimingWheel::WheelNode* TimingWheel::Hash(Phenix::UInt32 ticks)
	{
		Wheel* wheel = m_work_wheel;
		if (ticks < WORK_WHEEL_SIZE)
		{
			return &(wheel->m_nodes[ticks]);
		}
		wheel = wheel->m_higher_wheel;
		for (Phenix::UInt16 i=1; i<=ASSIST_WHEEL_COUNT; ++i)
		{			
			Phenix::UInt32 range = (1 << (WORK_WHEEL_BITS+i*ASSIST_WHEEL_BITS));
			if (ticks < range)
			{
				Phenix::UInt32 val = (1 << (WORK_WHEEL_BITS+(i-1)*ASSIST_WHEEL_BITS));
				while (ticks >= WORK_WHEEL_SIZE)
				{
					Phenix::Int16 idx = ticks / val;
					ticks %= val;
					if (!wheel->m_nodes[idx].lower_wheel)
					{
						wheel->m_nodes[idx].lower_wheel = new Wheel(NULL, wheel, 
							(ticks >= WORK_WHEEL_SIZE ? ASSIST_WHEEL_SIZE : WORK_WHEEL_SIZE));		
					}					
					wheel = wheel->m_nodes[idx].lower_wheel;
				}
				return &(wheel->m_nodes[ticks]);
			}
			wheel = wheel->m_higher_wheel;
		}
		return NULL;
	}

	bool TimingWheel::Wheel::NextNode()
	{
		if (m_cur_idx < m_size-1)
		{
			m_nodes[m_cur_idx++].Reset();
			return true;
		}
		m_cur_idx = 0;
		if (!m_higher_wheel || !m_higher_wheel->NextNode())
		{
			return false;
		}
		m_nodes.clear();
		m_nodes.insert(m_nodes.end(), 
			m_higher_wheel->m_nodes[m_higher_wheel->m_cur_idx].lower_wheel->m_nodes.begin(),
			m_higher_wheel->m_nodes[m_higher_wheel->m_cur_idx].lower_wheel->m_nodes.end());
		delete m_higher_wheel->m_nodes[m_higher_wheel->m_cur_idx].lower_wheel;
		m_higher_wheel->m_nodes[m_higher_wheel->m_cur_idx].lower_wheel = this;
		return true;
	}

	void TimingWheel::NextTick()
	{

	}





} // end namespace Timer
} // end namespace Phenix