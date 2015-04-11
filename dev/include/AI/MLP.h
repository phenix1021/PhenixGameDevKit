/*******************************************************************************
 * @brief	���������
 * @date	2015-4-11
 * @author	phenix
 * @mark	
			1.���������̽�޸�Ȩֵ��ѧϰ��ʽ������BP��ʽ
			2.�������Ϸ��̾���2����tank��ս����������
			3.����ѵ��ʱʹ�������һ���������ܼӿ�ѵ���ٶȣ���������ѵ����֮��
			���������Ҳ�������������[min,max]֮��
			4.samples.datΪ�����ļ���weights.dataΪȨֵ�ļ�������Ҳ������ļ���
			���������Ȩֵ��ֵ��ѵ����ɺ��ٴ�������Ȩֵд����ļ�
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
		// ��������
		struct ExemplarData
		{
			double* in;
			double* out;
			ExemplarData(){memset(this, 0, sizeof(*this));}
		};

		// ��ԪȨֵ����
		typedef	double*	NeuronWeightData;

	public:
		MLP(const Phenix::String& exemplar_file_name, const Phenix::String& weight_file_name);
		virtual ~MLP(){}
		
		// ������ֵ
		void			GetOutputs(const std::vector<double>& in, std::vector<double>& out);
		void			GetOutputs(double* in, double* out);

	private:		
		void			Train(double& better_error, NeuronWeightData* in_hidden_better_weights,
						NeuronWeightData* hidden_out_better_weights);
		// �������
		double			GetError();						
		// �����һ��
		inline void		NormalizeInput(double& val, double min, double max);
		Phenix::Int32	GetDefaultHiddenNeuronNum(){return 10;}		
		void			SaveWeights(std::ofstream& file);

	private:
		ExemplarData*		m_exemplars;
		NeuronWeightData*	m_in_hidden_weights;
		NeuronWeightData*	m_hidden_out_weights;

		Phenix::Int32	m_exemplar_num;			// ��������
		Phenix::Int32	m_in_neuron_num;		// ������Ԫ����
		Phenix::Int32	m_hidden_neuron_num;	// ��������Ԫ����
		Phenix::Int32	m_out_neuron_num;		// �����Ԫ����	

		double*			min_each_in;	// ��������ֵ����Сֵ�б�for ���������һ����,ѵ����Ϻ�����ʵ������ҲӦ����÷�Χ
		double*			max_each_in;	// ��������ֵ�����ֵ�б�for ���������һ����,ѵ����Ϻ�����ʵ������ҲӦ����÷�Χ

		unsigned short	m_status;
		double			m_expected_error;
		double			m_step_size;		
	};

} // end namespace ANN
} // end namespace AI
} // end namespace Phenix


#endif