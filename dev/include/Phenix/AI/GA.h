/*******************************************************************************
 * @brief	遗传算法
 * @date	2015-4-3
 * @author	phenix
 * @mark	参阅《游戏编程中的人工智能》各例子、《AI游戏编程精粹2》里的GA_Racer源代码
 ******************************************************************************/

#ifndef PHENIX_AI_GA_H
#define PHENIX_AI_GA_H

namespace Phenix
{
namespace AI
{
namespace GA
{
	const double INIT_CROSSOVER_RATE	= 0.7;	// 默认交叉率
	const double INIT_MUTATION_RATE		= 0.05; // 默认变异率
	const double INIT_ELITE_RATE		= 0.03; // 一般0.02~0.05比较理想

	// 交叉算子
	enum CrossoverOperator
	{
		SINGLE = 0, 
		DOUBLE, 
		MULTI, 
		AVERAGE, 
		HEURISTIC		
	};

	// 变异算子
	enum MutationOperator
	{
		GAUSSIAN = 0, 
		UNIFORM, 
		BOUNDARY, 
		REPLACE		
	};

	// 变比算子
	enum ScaleOperator
	{
		NONE	= 0,
		RANK,				// 排名变比
		SIGMA,				// 西格玛变比
		BOLTZMANN			// 波兹曼变比
	};

	// 选择算子
	enum SelectOperator
	{
		ROULETTE = 0,		// 轮盘赌
		TOURNAMENT			// 锦标赛
	};		

	// 染色体数据
	struct ChromosomeData
	{	
		ChromosomeData(const std::vector<double>& genome_list);

		std::vector<double>	genomes;	// 基因组
		double fitness;					// 适应度值

		ChromosomeData(){Reset();}
		void Reset(){genomes.clear(); fitness=0.0;}
		bool operator < (const ChromosomeData& other) const
		{
			return fitness < other.fitness; // sort后为升序
		}
	};

	// 基因组生成器
	interface GenomesGenerator
	{
	public:
		virtual std::vector<double> Make() = 0;
	};

	// 遗传算法
	class GA
	{		
	public:
		GA(Phenix::Int32 population_size, GenomesGenerator& genomes_generator);
		virtual ~GA();

		void			Epoch();	// 统计当代并产生后代
		void			Reset(Phenix::Int32 population_size, GenomesGenerator& genomes_generator);	// 一般在中途改变各个参数或操作子类型后调用

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
		Phenix::Int32	m_cur_epoch_count;	// 当前世代计数
		double			m_fitness_sum;		// 当代适应度总和值
		double			m_fitness_average;	// 当代适应度平均值
		ChromosomeData	m_best_chromosome;	// 历代以来最佳染色体
		double			m_boltzmann_temp;

		double			m_crossove_rate;	// 交叉率
		double			m_mutation_rate;	// 变异率		
		double			m_elite_rate;		// 精英率

		Phenix::UInt16	m_crossover_operator;	// 交叉算子		
		Phenix::UInt16	m_mutation_operator;	// 变异算子
		Phenix::UInt16	m_select_operator;		// 选择算子
		Phenix::UInt16	m_scale_operator;		// 变比算子
		
		std::vector<ChromosomeData>	m_chromosomes;	// 染色体组
	};

} // end namespace GA
} // end namespace AI
} // end namespace Phenix


#endif