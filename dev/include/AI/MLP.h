/*******************************************************************************
 * @brief	多层神经网络
 * @date	2015-4-11
 * @author	phenix
 * @mark	
			1.采用随机试探修改权值的学习方式，而非BP方式
			2.详见《游戏编程精粹2》的tank对战神经网络文章
			3.由于训练时使用输入归一化操作，能加快训练速度，所以网络训练好之后，
			各输入参数也必须在样本里的[min,max]之间
			4.samples.dat为样本文件；weights.data为权值文件，如果找不到该文件，
			则随机生成权值初值，训练完成后再创建并把权值写入该文件
 ******************************************************************************/
#ifndef PHENIX_AI_MLP_H
#define PHENIX_AI_MLP_H

//class fstream;

namespace Phenix
{
namespace AI
{
namespace ANN
{

	/*Multiple Layer Perception*/
	class MLP
	{
		// 样本数据
		struct ExemplarData
		{
			double* in;
			double* out;
			ExemplarData(){memset(this, 0, sizeof(*this));}
		};

		// 神经元权值数据
		typedef	double*	NeuronWeightData;

	public:
		MLP(const Phenix::String& exemplar_file_name, const Phenix::String& weight_file_name);
		virtual ~MLP(){}
		
		// 获得输出值
		void			GetOutputs(const std::vector<double>& in, std::vector<double>& out);
		void			GetOutputs(double* in, double* out);

	private:		
		void			Train(double& better_error, NeuronWeightData* in_hidden_better_weights,
						NeuronWeightData* hidden_out_better_weights);
		// 计算误差
		double			GetError();						
		// 输入归一化
		inline void		NormalizeInput(double& val, double min, double max);
		Phenix::Int32	GetDefaultHiddenNeuronNum(){return 10;}		
		void			SaveWeights(std::ofstream& file);

	private:
		ExemplarData*		m_exemplars;
		NeuronWeightData*	m_in_hidden_weights;
		NeuronWeightData*	m_hidden_out_weights;

		Phenix::Int32	m_exemplar_num;			// 样本数量
		Phenix::Int32	m_in_neuron_num;		// 输入神经元数量
		Phenix::Int32	m_hidden_neuron_num;	// 隐含层神经元数量
		Phenix::Int32	m_out_neuron_num;		// 输出神经元数量	

		double*			min_each_in;	// 各个输入值的最小值列表（for 输入参数归一化）,训练完毕后输入实际数据也应满足该范围
		double*			max_each_in;	// 各个输入值的最大值列表（for 输入参数归一化）,训练完毕后输入实际数据也应满足该范围

		unsigned short	m_status;
		double			m_expected_error;
		double			m_step_size;		
	};

} // end namespace ANN
} // end namespace AI
} // end namespace Phenix


#endif