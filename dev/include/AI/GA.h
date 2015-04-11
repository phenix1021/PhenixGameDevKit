/*******************************************************************************
 * @brief	�Ŵ��㷨
 * @date	2015-4-3
 * @author	phenix
 * @mark	���ġ���Ϸ����е��˹����ܡ������ӡ���AI��Ϸ��̾���2�����GA_RacerԴ����
 ******************************************************************************/

#ifndef PHENIX_AI_GA_H
#define PHENIX_AI_GA_H

namespace Phenix
{
namespace AI
{
namespace GA
{
	const double INIT_CROSSOVER_RATE	= 0.7;	// Ĭ�Ͻ�����
	const double INIT_MUTATION_RATE		= 0.05; // Ĭ�ϱ�����
	const double INIT_ELITE_RATE		= 0.03; // һ��0.02~0.05�Ƚ�����

	// ��������
	enum CrossoverOperator
	{
		SINGLE = 0, 
		DOUBLE, 
		MULTI, 
		AVERAGE, 
		HEURISTIC		
	};

	// ��������
	enum MutationOperator
	{
		GAUSSIAN = 0, 
		UNIFORM, 
		BOUNDARY, 
		REPLACE		
	};

	// �������
	enum ScaleOperator
	{
		NONE	= 0,
		RANK,				// �������
		SIGMA,				// ��������
		BOLTZMANN			// ���������
	};

	// ѡ������
	enum SelectOperator
	{
		ROULETTE = 0,		// ���̶�
		TOURNAMENT			// ������
	};		

	// Ⱦɫ������
	struct ChromosomeData
	{	
		ChromosomeData(const std::vector<double>& genome_list);

		std::vector<double>	genomes;	// ������
		double fitness;					// ��Ӧ��ֵ

		ChromosomeData(){Reset();}
		void Reset(){genomes.clear(); fitness=0.0;}
		bool operator < (const ChromosomeData& other) const
		{
			return fitness < other.fitness; // sort��Ϊ����
		}
	};

	// ������������
	interface GenomesGenerator
	{
	public:
		virtual std::vector<double> Make() = 0;
	};

	// �Ŵ��㷨
	class GA
	{		
	public:
		GA(Phenix::Int32 population_size, GenomesGenerator& genomes_generator);
		virtual ~GA();

		void			Epoch();	// ͳ�Ƶ������������
		void			Reset(Phenix::Int32 population_size, GenomesGenerator& genomes_generator);	// һ������;�ı������������������ͺ����

		Phenix::Int32	GetPopulationSize(){return m_chromosomes.size();}
		Phenix::Int32	GetCurEpochCount() const { return m_cur_epoch_count; }		
		
		double GetCrossoveRate() const { return m_crossove_rate; }
		void SetCrossoveRate(double val){ m_crossove_rate= val; }

		double GetMutationRate() const { return m_mutation_rate; }
		void SetMutationRate(double val){ m_mutation_rate = val; }

		double GetEliteRate() const { return m_elite_rate; }
		void SetEliteRate(double val){ m_elite_rate = val; }

		Phenix::UInt16 GetCrossoverOperator() const { return m_crossover_operator; }
		void SetCrossoverOperator(Phenix::UInt16 val) { m_crossover_operator = val; }

		Phenix::UInt16 GetMutationOperator() const { return m_mutation_operator; }
		void SetMutationOperator(Phenix::UInt16 val) { m_mutation_operator = val; }

		Phenix::UInt16 GetSelectOperator() const { return m_select_operator; }
		void SetSelectOperator(Phenix::UInt16 val) { m_select_operator = val; }

		Phenix::UInt16 GetScaleOperator() const { return m_scale_operator; }
		void SetScaleOperator(Phenix::UInt16 val) { m_scale_operator = val; }

	private:		
		void			MakeStat();
		void			Scale();

		void			SelectElite(std::vector<ChromosomeData>& children);
		ChromosomeData&	Select();

		ChromosomeData&	RouletteWheelSelection();
		ChromosomeData& TournamentSelection();

		void			Crossover(ChromosomeData& father, ChromosomeData& mother, ChromosomeData& child);
		void			CrossoverSingle(ChromosomeData& father, ChromosomeData& mother, ChromosomeData& child);
		void			CrossoverDouble(ChromosomeData& father, ChromosomeData& mother, ChromosomeData& child);
		void			CrossoverMulti(ChromosomeData& father, ChromosomeData& mother, ChromosomeData& child);
		void			CrossoverAverage(ChromosomeData& father, ChromosomeData& mother, ChromosomeData& child);
		void			CrossoverHeuristic(ChromosomeData& father, ChromosomeData& mother, ChromosomeData& child);
		
		void			Mutate(ChromosomeData& chrom);
// 		void            MutateUniform(ChromosomeData& chrom);
// 		void            MutateReplace(ChromosomeData& chrom);
// 		void            MutateBoundary(ChromosomeData& chrom);
// 		void            MutateGaussian(ChromosomeData& chrom);

		void			ScaleRank();
		void			ScaleSigma();
		void			ScaleBoltzmann();

	private:		
		Phenix::Int32	m_cur_epoch_count;	// ��ǰ��������
		double			m_fitness_sum;		// ������Ӧ���ܺ�ֵ
		double			m_fitness_average;	// ������Ӧ��ƽ��ֵ
		ChromosomeData	m_best_chromosome;	// �����������Ⱦɫ��
		double			m_boltzmann_temp;

		double			m_crossove_rate;	// ������
		double			m_mutation_rate;	// ������		
		double			m_elite_rate;		// ��Ӣ��

		Phenix::UInt16	m_crossover_operator;	// ��������		
		Phenix::UInt16	m_mutation_operator;	// ��������
		Phenix::UInt16	m_select_operator;		// ѡ������
		Phenix::UInt16	m_scale_operator;		// �������
		
		std::vector<ChromosomeData>	m_chromosomes;	// Ⱦɫ����
	};

} // end namespace GA
} // end namespace AI
} // end namespace Phenix


#endif