#include "stdafx.h"
#include "../include/MLP.h"
#include <fstream>
#include <iostream>
#include <xiosbase>
#include <float.h>
#include <math.h>

namespace Phenix
{
namespace AI
{
namespace ANN
{

	/************多层神经元感应器***********
	*1.采用随机搜索的学习方式，而非BP方式
	*2.详见《游戏编程精粹2》的神经网络文章
	***************************************/
	MLP::MLP( const Phenix::String& exemplar_file_name )
		:m_status(UNTRAINED),m_in_neuron_num(0),m_hidden_neuron_num(0),m_out_neuron_num(0),
		m_expected_error(0.000025), m_exemplar_num(0)
	{
		Reset();

		// 加载样本文件
		std::ifstream exemplar_file;
		try
		{
			exemplar_file.open(exemplar_file_name.c_str());
		}		
		catch (...)
		{
			exemplar_file.close();
			throw;
		}
			
		// ------------------加载各个数量---------------------------
		exemplar_file>>m_exemplar_num;		
		exemplar_file>>m_in_neuron_num;		
		exemplar_file>>m_out_neuron_num;
		
		// ------------------加载输入输出数据------------------------
		// 为了归一化输入值时使用
		std::vector<double>	min_each_in;
		std::vector<double>	max_each_in;
		min_each_in.reserve(m_in_neuron_num);
		max_each_in.reserve(m_in_neuron_num);
		for (Phenix::Int32 i=0; i<m_in_neuron_num; ++i)
		{
			min_each_in[i] = DBL_MAX;
			max_each_in[i] = DBL_MIN;
		}

		m_exemplars.reserve(m_exemplar_num);
		Phenix::Int32 exemplar_idx = 0;
		while (exemplar_idx < m_exemplar_num)
		{			
			m_exemplars[exemplar_idx].in.reserve(m_in_neuron_num);
			for (Phenix::Int32 i=0; i<m_in_neuron_num; ++i)
			{
				exemplar_file>>m_exemplars[exemplar_idx].in[i];
				if (m_exemplars[exemplar_idx].in[i] > max_each_in[i])
				{
					max_each_in[i] = m_exemplars[exemplar_idx].in[i];
				}
				if (m_exemplars[exemplar_idx].in[i] < min_each_in[i])
				{
					min_each_in[i] = m_exemplars[exemplar_idx].in[i];
				}
			}
			m_exemplars[exemplar_idx].out.reserve(m_out_neuron_num);
			for (Phenix::Int32 i=0; i<m_out_neuron_num; ++i)
			{
				exemplar_file>>m_exemplars[exemplar_idx].out[i];
			}
			++exemplar_idx;
		}
		for (Phenix::Int32 i=0; i<m_exemplar_num; ++i)
		{			
			for (Phenix::Int32 j=0; j<m_in_neuron_num; ++j)
			{			
				NormalizeInput(m_exemplars[i].in[j], min_each_in[j], max_each_in[j]);
			}
		}
		
		// --------------------处理权值--------------------------
		std::fstream weight_file;
		bool is_success_weight_file = true;
		// 加载权值文件
		try
		{
			weight_file.open(WEIGHT_FILE_NAME.c_str(), std::ios_base::_Nocreate);
		}		
		catch (...)
		{
			is_success_weight_file = false;			
		}

		if (is_success_weight_file)
		{
			weight_file>>m_hidden_neuron_num;
			m_in_hidden_weights.reserve(m_hidden_neuron_num);			
			for (Phenix::Int32 i=0; i<m_hidden_neuron_num; ++i)
			{
				m_in_hidden_weights[i].weights.reserve(m_in_neuron_num+1);// +1意思是包括1个偏移输入值
				for (Phenix::Int32 j=0; j<m_in_neuron_num+1; ++j)
				{
					weight_file>>m_in_hidden_weights[i].weights[j];
				}
			}

			m_hidden_out_weights.reserve(m_out_neuron_num);
			for (Phenix::Int32 i=0; i<m_out_neuron_num; ++i)
			{
				m_hidden_out_weights[i].weights.reserve(m_hidden_neuron_num+1);// +1意思是包括1个偏移输入值
				for (Phenix::Int32 j=0; j<m_hidden_neuron_num+1; ++j)
				{
					weight_file>>m_hidden_out_weights[i].weights[j];
				}
			}
		} 
		else
		{
			// 如果没有权值文件，则取隐含层默认节点数，各权值初始化为0.1
			m_hidden_neuron_num = GetDefaultHiddenNeuronNum();
			m_in_hidden_weights.reserve(m_hidden_neuron_num);			
			for (Phenix::Int32 i=0; i<m_hidden_neuron_num; ++i)
			{
				m_in_hidden_weights[i].weights.reserve(m_in_neuron_num+1);// +1意思是包括1个偏移输入值
				for (Phenix::Int32 j=0; j<m_in_neuron_num+1; ++j)
				{
					m_in_hidden_weights[i].weights[j] = 0.1;
				}
			}

			m_hidden_out_weights.reserve(m_out_neuron_num);
			for (Phenix::Int32 i=0; i<m_out_neuron_num; ++i)
			{
				m_hidden_out_weights[i].weights.reserve(m_hidden_neuron_num+1);// +1意思是包括1个偏移输入值
				for (Phenix::Int32 j=0; j<m_hidden_neuron_num+1; ++j)
				{
					m_hidden_out_weights[i].weights[j] = 0.1;
				}
			}
		}

		std::cout<<__TIME__<<" start training......"<<std::endl;
		double better_error = GetError();
		std::vector<NeuronWeightData>	in_hidden_better_weights = m_in_hidden_weights;
		std::vector<NeuronWeightData>	hidden_out_better_weights = m_hidden_out_weights;
		do 
		{			
			Train(better_error, in_hidden_better_weights, hidden_out_better_weights);
		} while (better_error > m_expected_error);		
		std::cout<<__TIME__<<" training finished. "<<std::endl;

		// 权值写入WEIGHT_FILE_NAME
		SaveWeights(weight_file);

		exemplar_file.close();
		weight_file.close();
	}

	void MLP::Reset()
	{
		m_status = UNTRAINED;
		m_step_size = 0.001;
	}

	void MLP::NormalizeInput( double& val, double min, double max )
	{
		// 把输入值归一化为(-1,1)
		//Inputs range between 0 and max-min
		val -= min;
		//Inputs range between 0 and 1
		val /= (max - min);
		//Inputs range between -0.5 and +0.5
		val -= 0.5;
		//Inputs range between -1.0 and +1.0
		val *= 2.0;	
	}

	void MLP::Train(double& better_error, std::vector<NeuronWeightData>& in_hidden_better_weights,
		std::vector<NeuronWeightData>& hidden_out_better_weights)
	{
		// 随机扰动生成新的权值
		for(Phenix::Int32 i=0; i<m_hidden_neuron_num; ++i)
		{
			for(Phenix::Int32 j=0; j<m_in_neuron_num+1; ++j)
			{
				m_in_hidden_weights[i].weights[j] += 
					m_step_size*(double(rand())/double(RAND_MAX)-0.5)*2.0;
			}
		}		
		for(Phenix::Int32 i=0; i<m_out_neuron_num; ++i)
		{
			for(Phenix::Int32 j=0; j<m_hidden_neuron_num+1; ++j)
			{				
				m_hidden_out_weights[i].weights[j] += 
					m_step_size*(double(rand())/double(RAND_MAX)-0.5)*2.0;
			}
		}

		double new_error = GetError();

		//If the performance is worse (the new error is larger)
		if(new_error > better_error)
		{
			//Reduce the size of the perturbation a bit - we need to be more conservative!
			m_step_size *= 0.9;

			//Restore the weights back to their old values
			m_in_hidden_weights = in_hidden_better_weights;
			m_in_hidden_weights = hidden_out_better_weights;
		}
		else
		{
			//Otherwise the new weights performed at least as well as the old ones, so record the
			//performance of the network with the new weights,
			better_error = new_error;

			//Increase the step size a little - we're doing well and can afford to be more 
			//adventurous.
			m_step_size *= 1.2;

			//Record the new weights as the best so far discovered
			in_hidden_better_weights = m_in_hidden_weights;
			hidden_out_better_weights = m_in_hidden_weights;
		}
	}

	double MLP::GetError()
	{
		double error = 0.0;
		std::vector<double> out;
		for(Phenix::Int32 i=0; i<m_exemplar_num; ++i)
		{
			GetOutputs(m_exemplars[i].in, out);
			for(Phenix::Int32 j=0; j<m_out_neuron_num; ++j)
			{
				error += 0.5*pow(fabs(m_exemplars[i].out[j] - out[j]), 2.0);
			}			
		}
		error /= double(m_exemplar_num);
		return error;
	}

	void MLP::GetOutputs( const std::vector<double>& in, std::vector<double>& out )
	{
		double stimulus = 0;

		std::vector<double> hidden_rlt;
		hidden_rlt.reserve(m_hidden_neuron_num);		
		for(Phenix::Int32 i=0; i<m_hidden_neuron_num; ++i)
		{			
			stimulus = m_in_hidden_weights[i].weights[0]; // 偏移量权值			
			for(Phenix::Int32 j=1; j<m_in_neuron_num+1; ++j)
			{
				stimulus += m_in_hidden_weights[i].weights[j]*in[j-1];
			}

			hidden_rlt[i] = 1.0/(1.0+exp(-stimulus));
		}
		
		out.reserve(m_out_neuron_num);
		for(Phenix::Int32 i=0; i<m_out_neuron_num; ++i)
		{			
			stimulus = m_hidden_out_weights[i].weights[0]; // 偏移量权值						
			for(Phenix::Int32 j=1; j<m_hidden_neuron_num+1; ++j)
			{
				stimulus += m_hidden_out_weights[i].weights[j]*hidden_rlt[j-1];
			}
			
			out[i] = stimulus;			
		}
	}

	void MLP::SaveWeights(std::fstream& file)
	{
		file.seekg(std::ios_base::beg);
		file<<m_hidden_neuron_num;		
		file<<"\n";		

		//Its weights
		for(Phenix::Int32 i=0; i<m_hidden_neuron_num; ++i)
		{
			for(Phenix::Int32 j=0; j<m_in_neuron_num+1; ++j)
			{
				file<<m_in_hidden_weights[i].weights[j];
				file<<"\t";
			}
			file<<"\n";
		}
		for(Phenix::Int32 i=0; i<m_out_neuron_num; ++i)
		{
			for(Phenix::Int32 j=0; j<m_hidden_neuron_num+1; ++j)
			{
				file<<m_hidden_out_weights[i].weights[j];
				file<<"\t";
			}
			file<<"\n";
		}
	}

} // end namespace ANN
} // end namespace AI
} // end namespace Phenix