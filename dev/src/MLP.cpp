#include "stdafx.h"
#include <Phenix/AI/MLP.h>
#include <fstream>
#include <iostream>
#include <float.h>
#include <math.h>
#include <time.h>

namespace Phenix
{
namespace AI
{
namespace ANN
{	
	MLP::MLP( const Phenix::String& exemplar_file_name, const Phenix::String& weight_file_name )
		:m_in_neuron_num(0), m_hidden_neuron_num(0), m_out_neuron_num(0), m_step_size(0.001),
		m_expected_error(0.000025), m_exemplar_num(0)
	{
		srand(Phenix::UInt32(time(NULL)));

		// 加载样本文件
		std::ifstream exemplar_file(exemplar_file_name.c_str());
		//exemplar_file.open(exemplar_file_name.c_str());
		if (!exemplar_file.is_open())
		{
			exemplar_file.close();
			throw;
		}		
			
		// ------------------加载各个数量---------------------------
		exemplar_file>>m_exemplar_num;		
		exemplar_file>>m_in_neuron_num;		
		exemplar_file>>m_hidden_neuron_num;
		exemplar_file>>m_out_neuron_num;
		
		// ------------------加载输入输出数据------------------------
		// 为了归一化输入值时使用
		min_each_in = new double[m_in_neuron_num];
		max_each_in = new double[m_in_neuron_num];		
		for (Phenix::Int32 i=0; i<m_in_neuron_num; ++i)
		{
			min_each_in[i] = DBL_MAX;
			max_each_in[i] = DBL_MIN;
		}

		m_exemplars = new ExemplarData[m_exemplar_num];
		Phenix::Int32 exemplar_idx = 0;
		while (exemplar_idx < m_exemplar_num)
		{			
			m_exemplars[exemplar_idx].in = new double[m_in_neuron_num];
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
			m_exemplars[exemplar_idx].out = new double[m_out_neuron_num];
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
		std::ifstream in_weight_file(weight_file_name.c_str(), std::ios_base::in);
		if (in_weight_file.is_open())
		{			
			// 加载权值文件
			m_in_hidden_weights = new NeuronWeightData[m_hidden_neuron_num];
			for (Phenix::Int32 i=0; i<m_hidden_neuron_num; ++i)
			{
				m_in_hidden_weights[i] = new double[m_in_neuron_num+1];// +1意思是包括1个偏移输入值
				for (Phenix::Int32 j=0; j<m_in_neuron_num+1; ++j)
				{
					in_weight_file>>m_in_hidden_weights[i][j];
				}
			}

			m_hidden_out_weights = new NeuronWeightData[m_out_neuron_num];
			for (Phenix::Int32 i=0; i<m_out_neuron_num; ++i)
			{
				m_hidden_out_weights[i] = new double[m_hidden_neuron_num+1];// +1意思是包括1个偏移输入值
				for (Phenix::Int32 j=0; j<m_hidden_neuron_num+1; ++j)
				{
					in_weight_file>>m_hidden_out_weights[i][j];
				}
			}			
		} 
		else
		{
			// 如果没有权值文件，各权值初始化为0.05			
			m_in_hidden_weights = new NeuronWeightData[m_hidden_neuron_num];
			for (Phenix::Int32 i=0; i<m_hidden_neuron_num; ++i)
			{
				m_in_hidden_weights[i] = new double[m_in_neuron_num+1];// +1意思是包括1个偏移输入值
				for (Phenix::Int32 j=0; j<m_in_neuron_num+1; ++j)
				{
					m_in_hidden_weights[i][j] = 0.05;
				}
			}

			m_hidden_out_weights = new NeuronWeightData[m_out_neuron_num];
			for (Phenix::Int32 i=0; i<m_out_neuron_num; ++i)
			{
				m_hidden_out_weights[i] = new double[m_hidden_neuron_num+1];// +1意思是包括1个偏移输入值
				for (Phenix::Int32 j=0; j<m_hidden_neuron_num+1; ++j)
				{
					m_hidden_out_weights[i][j] = 0.05;
				}
			}
		}

		char cur_date[256];
		char cur_time[256];
		std::cout<<_strdate(cur_date)<<"\t"<<_strtime(cur_time)<<std::endl;
		std::cout<<"start training......"<<std::endl;

		NeuronWeightData* in_hidden_better_weights = new NeuronWeightData[m_hidden_neuron_num];		
		for (Phenix::Int32 i=0; i<m_hidden_neuron_num; ++i)
		{
			for (Phenix::Int32 j=0; j<m_in_neuron_num+1; ++j)
			{
				in_hidden_better_weights[i] = new double[m_in_neuron_num+1];
				in_hidden_better_weights[i][j] = m_in_hidden_weights[i][j];
			}
		}
		NeuronWeightData* hidden_out_better_weights = new NeuronWeightData[m_out_neuron_num];		
		for (Phenix::Int32 i=0; i<m_out_neuron_num; ++i)
		{
			for (Phenix::Int32 j=0; j<m_hidden_neuron_num+1; ++j)
			{
				hidden_out_better_weights[i] = new double[m_hidden_neuron_num+1];
				hidden_out_better_weights[i][j] = m_hidden_out_weights[i][j];
			}
		}

		double better_error = GetError();	
		while (better_error > m_expected_error) 
		{			
			Train(better_error, in_hidden_better_weights, hidden_out_better_weights);			
		}	
		
		std::cout<<_strdate(cur_date)<<"\t"<<_strtime(cur_time)<<std::endl;
		std::cout<<"train finished. "<<std::endl;
		

		// 权值写入WEIGHT_FILE_NAME
		std::ofstream out_weight_file(weight_file_name.c_str(), std::ios_base::out);
		SaveWeights(out_weight_file);

		exemplar_file.close();
		in_weight_file.close();
		out_weight_file.close();
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

	void MLP::Train(double& better_error, NeuronWeightData* in_hidden_better_weights,
		NeuronWeightData* hidden_out_better_weights)
	{
		Phenix::Int32 i, j;
		// 随机扰动生成新的权值
		for(i=0; i<m_hidden_neuron_num; ++i)
		{
			for(j=0; j<m_in_neuron_num+1; ++j)
			{				
				m_in_hidden_weights[i][j] += m_step_size*(double(rand())/double(RAND_MAX)-0.5)*2.0;
			}
		}		
		for(i=0; i<m_out_neuron_num; ++i)
		{
			for(j=0; j<m_hidden_neuron_num+1; ++j)
			{				
				m_hidden_out_weights[i][j] += m_step_size*(double(rand())/double(RAND_MAX)-0.5)*2.0;
			}
		}

		double new_error = GetError();

		//If the performance is worse (the new error is larger)
		if(new_error > better_error)
		{
			//Reduce the size of the perturbation a bit - we need to be more conservative!
			m_step_size *= 0.9;

			//Restore the weights back to their old values			
			//Copy(m_in_hidden_weights, in_hidden_better_weights, m_hidden_neuron_num, m_in_neuron_num+1);
			//Copy(m_hidden_out_weights, hidden_out_better_weights, m_out_neuron_num, m_hidden_neuron_num+1);
			
			for (i=0; i<m_hidden_neuron_num; ++i)
			{
				for (j=0; j<m_in_neuron_num+1; ++j)
				{					
					m_in_hidden_weights[i][j] = in_hidden_better_weights[i][j];
				}
			}

			for (i=0; i<m_out_neuron_num; ++i)
			{
				for (j=0; j<m_hidden_neuron_num+1; ++j)
				{					
					m_hidden_out_weights[i][j] = hidden_out_better_weights[i][j];
				}
			}
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
			for (i=0; i<m_hidden_neuron_num; ++i)
			{
				for (j=0; j<m_in_neuron_num+1; ++j)
				{
					in_hidden_better_weights[i][j] = m_in_hidden_weights[i][j];
				}
			}

			for (i=0; i<m_out_neuron_num; ++i)
			{
				for (j=0; j<m_hidden_neuron_num+1; ++j)
				{					
					hidden_out_better_weights[i][j] = m_hidden_out_weights[i][j];
				}
			}
		}
	}

	double MLP::GetError()
	{
		Phenix::Int32 i, j;
		double error = 0.0;
		double* out = new double[m_out_neuron_num];;
		for(i=0; i<m_exemplar_num; ++i)
		{
			GetOutputs(m_exemplars[i].in, out);
			for(j=0; j<m_out_neuron_num; ++j)
			{
				error += 0.5*pow(fabs(m_exemplars[i].out[j] - out[j]), 2.0);
			}			
		}
		error /= double(m_exemplar_num);
		delete [] out;
		return error;
	}

	void MLP::GetOutputs( const std::vector<double>& in, std::vector<double>& out )
	{
		double* p_in = new double[m_in_neuron_num];
		double* p_out = new double[m_out_neuron_num];
		for (Phenix::Int32 i=0; i<m_in_neuron_num; ++i)
		{
			p_in[i] = in[i];
			NormalizeInput(p_in[i], min_each_in[i], max_each_in[i]);
		}
		GetOutputs(p_in, p_out);
		out.clear();
		for (Phenix::Int32 i=0; i<m_out_neuron_num; ++i)
		{
			out[i] = p_out[i];
		}
		delete [] p_in;
		delete [] p_out;
	}

	void MLP::GetOutputs( double* in, double* out )
	{
		Phenix::Int32 i, j;
		double stimulus = 0;

		double* hidden_rlt = new double[m_hidden_neuron_num];
		for(i=0; i<m_hidden_neuron_num; ++i)
		{			
			stimulus = m_in_hidden_weights[i][0]; // 偏移量权值			
			for(j=1; j<m_in_neuron_num+1; ++j)
			{
				stimulus += m_in_hidden_weights[i][j]*in[j-1];
			}

			hidden_rlt[i] = 1.0/(1.0+exp(-stimulus));
		}		
		
		for(i=0; i<m_out_neuron_num; ++i)
		{			
			stimulus = m_hidden_out_weights[i][0]; // 偏移量权值						
			for(j=1; j<m_hidden_neuron_num+1; ++j)
			{
				stimulus += m_hidden_out_weights[i][j]*hidden_rlt[j-1];
			}
			
			out[i] = stimulus;			
		}

		delete [] hidden_rlt;
	}

	void MLP::SaveWeights(std::ofstream& file)
	{		
		for(Phenix::Int32 i=0; i<m_hidden_neuron_num; ++i)
		{
			for(Phenix::Int32 j=0; j<m_in_neuron_num+1; ++j)
			{
				file<<m_in_hidden_weights[i][j];
				file<<"\t";
			}
			file<<"\n";
		}
		for(Phenix::Int32 i=0; i<m_out_neuron_num; ++i)
		{
			for(Phenix::Int32 j=0; j<m_hidden_neuron_num+1; ++j)
			{
				file<<m_hidden_out_weights[i][j];
				file<<"\t";
			}
			file<<"\n";
		}
	}

// 	void MLP::Copy( NeuronWeightData* dst, NeuronWeightData* src, Phenix::Int32 length, Phenix::Int32 weights_length, bool is_init/*=false*/ )
// 	{
// 		for (Phenix::Int32 i=0; i<length; ++i)
// 		{
// 			for (Phenix::Int32 j=0; j<weights_length; ++j)
// 			{
// 				if (is_init)
// 				{
// 					dst[i] = new double[weights_length];
// 				}
// 				dst[i][j] = src[i][j];
// 			}
// 		}
// 	}

} // end namespace ANN
} // end namespace AI
} // end namespace Phenix

