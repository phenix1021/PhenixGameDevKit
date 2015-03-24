#ifndef PHENIX_AI_MLP_H
#define PHENIX_AI_MLP_H

class fstream;

namespace Phenix
{
namespace AI
{
namespace ANN
{
	const Phenix::String WEIGHT_FILE_NAME = "weights.dat";

	/*Multiple Layer Perception*/
	class MLP
	{
		// 样本数据
		struct ExemplarData
		{
			std::vector<double> in;
			std::vector<double> out;
			ExemplarData(){memset(this, 0, sizeof(*this));}
		};

		// 神经元权值数据
		struct NeuronWeightData
		{
			std::vector<double> weights;
		};

		enum
		{
			UNTRAINED = 0,
			TRAINING,
			TRAINED
		};

	public:
		MLP(const Phenix::String& exemplar_file_path);
		virtual ~MLP();
		
		void GetOutputs(const std::vector<double>& in, std::vector<double>& out);

	private:
		void	Reset();
		void	Train(double& better_error, std::vector<NeuronWeightData>& in_hidden_better_weights,
			std::vector<NeuronWeightData>& hidden_out_better_weights);
		double	GetError();				
		
		inline void	NormalizeInput(double& val, double min, double max);
		Phenix::Int32 GetDefaultHiddenNeuronNum(){return 10;}
		
		void SaveWeights(std::fstream& file);

	private:
		std::vector<ExemplarData>		m_exemplars;
		std::vector<NeuronWeightData>	m_in_hidden_weights;
		std::vector<NeuronWeightData>	m_hidden_out_weights;

		Phenix::Int32	m_exemplar_num;			// 样本数量
		Phenix::Int32	m_in_neuron_num;		// 输入神经元数量
		Phenix::Int32	m_hidden_neuron_num;	// 隐含层神经元数量
		Phenix::Int32	m_out_neuron_num;		// 输出神经元数量	

		Phenix::UInt16	m_status;
		double			m_expected_error;
		double			m_step_size;		
	};

} // end namespace ANN
} // end namespace AI
} // end namespace Phenix


#endif