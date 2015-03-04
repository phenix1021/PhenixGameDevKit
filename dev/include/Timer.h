#ifndef PHENIX_BASE_TIMER_H
#define PHENIX_BASE_TIMER_H

namespace Phenix
{
namespace Timer
{

struct TimerNode 
{
	// callback
	TimerNode* next_timer;
	TimerNode():next_timer(NULL){}
};

class TimingWheel
{
	enum
	{
		WORK_WHEEL_BITS		= 8,
		ASSIST_WHEEL_BITS	= 6,
		ASSIST_WHEEL_COUNT	= 4,
		WORK_WHEEL_SIZE		= 256,
		ASSIST_WHEEL_SIZE	= 64
	};

	class Wheel;
	struct WheelNode
	{
		union
		{
			Wheel* lower_wheel;
			TimerNode* timer_head;
		};
		void Reset(){memset(this, 0, sizeof(WheelNode));}
		WheelNode(){Reset();}
	};

	class Wheel
	{
		friend class TimingWheel;
	public:
		Wheel(Wheel* lower_wheel, Wheel* higher_wheel, Phenix::UInt16 size);		

		bool NextNode();

	private:
		Wheel* m_lower_wheel;
		Wheel* m_higher_wheel;	

		Phenix::UInt16 m_cur_idx;
		Phenix::UInt16 m_size;
		std::vector<WheelNode> m_nodes;
	};

public:
	TimingWheel();
	virtual ~TimingWheel();

	TimerNode* AddTimer(Phenix::UInt32 delay, Phenix::UInt32 interval, Phenix::UInt32 count, 
		bool act_first/*, call_back*/);
	void NextTick();
	void Act();

private:
	WheelNode* Hash(Phenix::UInt32 ticks);

private:
	Wheel* m_work_wheel;
};



} // end namespace Timer
} // end namespace Phenix

#endif