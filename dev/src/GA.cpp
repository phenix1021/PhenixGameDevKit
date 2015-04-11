#include "stdafx.h"
#include <AI/GA.h>
#include <Math/Rand.h>
#include <Math/Utils.h>

namespace Phenix
{
namespace AI
{
namespace GA
{

	ChromosomeData::ChromosomeData( const std::vector<double>& genome_list )
		:fitness(0.0)
	{
		genomes.assign(genome_list.begin(), genome_list.end());
	}

	GA::GA( Phenix::Int32 population_size, GenomesGenerator& genomes_generator )
		:m_crossove_rate(INIT_CROSSOVER_RATE), m_mutation_rate(INIT_MUTATION_RATE), m_elite_rate(INIT_ELITE_RATE), 
		m_crossover_operator(DOUBLE), m_mutation_operator(0), m_select_operator(ROULETTE),m_scale_operator(NONE)		
	{
		Reset(population_size, genomes_generator);		
	}

	GA::~GA()
	{

	}

	void GA::Epoch()
	{
		MakeStat();
		Scale();

		// 过滤出精英直接放入子代
		std::vector<ChromosomeData>	children;
		SelectElite(children);
		
		while (children.size() != m_chromosomes.size())
		{
			// 选择
			ChromosomeData& father = Select();
			ChromosomeData& mother = Select();
			// 交叉
			ChromosomeData child;
			Crossover(father, mother, child);
			// 变异
			Mutate(child);			
			
			children.push_back(child);
		}		

		m_chromosomes = children;		
		++m_cur_epoch_count;
	}

	Phenix::AI::GA::ChromosomeData& GA::Select()
	{
		switch (m_select_operator)
		{
		case ROULETTE:
			return RouletteWheelSelection();
			break;
		case TOURNAMENT:
			return TournamentSelection();
			break;
		default:
			throw;
		}
	}

	void GA::SelectElite( std::vector<ChromosomeData>& children )
	{		
		Phenix::Int32 elite_num = m_chromosomes.size()*m_elite_rate;
		for (Phenix::Int32 i=0; i<elite_num; ++i)
		{				
			children.push_back(m_chromosomes[m_chromosomes.size()-1-i]);
			children.back().fitness = 0.0; // 下一代时适应度值要清0，和之后那些选择、交叉、变异产生的子代染色体统一处理
		}
	}

	Phenix::AI::GA::ChromosomeData& GA::RouletteWheelSelection()
	{
		double slice = Phenix::Math::Random::RandFloatEx() * m_fitness_sum;
		double total  = 0.0;
		Phenix::Int32 idx = 0;
		for (int i=0; i<m_chromosomes.size(); ++i)
		{
			total += m_chromosomes[i].fitness;
			if (total > slice) 
			{
				idx = i;
				break;
			}
		}
		return m_chromosomes[idx];
	}

	Phenix::AI::GA::ChromosomeData& GA::TournamentSelection()
	{
		double better_fitness = 0;
		Phenix::Int32 idx = 0;
		//Select N members from the population at random testing against 
		//the best found so far
		Phenix::Int32 N = m_chromosomes.size() * 0.1;
		for (Phenix::Int32 i=0; i<N; ++i)
		{
			Phenix::Int32 j = Phenix::Math::Random::RandInt(0, m_chromosomes.size()-1);

			if (m_chromosomes[j].fitness > better_fitness)
			{
				idx = j;
				better_fitness = m_chromosomes[j].fitness;
			}
		}

		//return the champion
		return m_chromosomes[idx];
	}

	void GA::Reset(Phenix::Int32 population_size, GenomesGenerator& genomes_generator)
	{
		m_cur_epoch_count = 0;
		m_fitness_average = 0.0;
		m_fitness_sum = 0.0;
		m_boltzmann_temp = 3*population_size;
		m_best_chromosome.Reset();
		m_chromosomes.clear();
		for (Phenix::Int32 i=0; i<population_size-1; ++i)
		{
			m_chromosomes.push_back(genomes_generator.Make());
		}
		if (m_chromosomes.empty())
		{
			throw;
		}
	}

	void GA::MakeStat()
	{
		std::sort(m_chromosomes.begin(), m_chromosomes.end());
		for (Phenix::Int32 i=0; i<m_chromosomes.size(); ++i)
		{
			m_fitness_sum += m_chromosomes[i].fitness;
		}
		m_fitness_average = m_fitness_sum / m_chromosomes.size();
		m_best_chromosome = m_chromosomes.back();
	}

	void GA::Scale()
	{
		switch (m_scale_operator)
		{
		case NONE:
			return;
		case RANK:
			ScaleRank();
			break;
		case SIGMA:
			ScaleSigma();
			break;
		case BOLTZMANN:
			ScaleBoltzmann();
			break;
		default:
			throw;
		}

		for (Phenix::Int32 i=0; i<m_chromosomes.size(); ++i)
		{
			m_fitness_sum += m_chromosomes[i].fitness;
		}
		m_fitness_average = m_fitness_sum / m_chromosomes.size();
	}

	void GA::ScaleRank()
	{
		for (Phenix::Int32 i=0; i<m_chromosomes.size(); ++i)
		{
			m_chromosomes[i].fitness = i;
		}
	}

	void GA::ScaleSigma()
	{
		double RunningTotal = 0.0;
		//first iterate through the population to calculate the standard
		//deviation
		for (Phenix::Int32 i=0; i<m_chromosomes.size(); ++i)
		{
			RunningTotal += (m_chromosomes[i].fitness - m_fitness_average) *
				(m_chromosomes[i].fitness - m_fitness_average);
		}
		double sd = RunningTotal/(double)m_chromosomes.size();

		//standard deviation is the square root of the variance
		double sigma = sqrt(sd);
		//now iterate through the population to reassign the fitness scores
		for (Phenix::Int32 i=0; i<m_chromosomes.size(); ++i)
		{
			double old_fitness = m_chromosomes[i].fitness;

			m_chromosomes[i].fitness = 1.0 + ((old_fitness - m_fitness_average) /
				(2.0 * sigma));
			//sometimes the individual may be assigned a score of <= 0. 
			//this is undesirable so it is simply reset to a very small
			//value.
			if (m_chromosomes[i].fitness <= 0.0)
			{
				m_chromosomes[i].fitness = 0.1;
			}
		}
	}

	void GA::ScaleBoltzmann()
	{
		//reduce the temp a little each generation
		const double boltzmann_dt = 0.05;
		m_boltzmann_temp -= boltzmann_dt;
		//make sure it doesn't fall below MinTemp.
		const double min_temp = 1.0;
		if (m_boltzmann_temp< min_temp)
			m_boltzmann_temp = min_temp;

		//iterate through the population to find the average e^(fitness/temp)
		//keep a record of e^(fitness/temp) for each individual
		std::vector<double> expboltz; 
		double average = 0.0;
		for (Phenix::Int32 i=0; i<m_chromosomes.size(); ++i)
		{
			expboltz.push_back(exp(m_chromosomes[i].fitness / m_boltzmann_temp));

			average += expboltz[i];
		}
		average /= (double)m_chromosomes.size();

		//now iterate once more to calculate the new expected values
		for (Phenix::Int32 i=0; i<m_chromosomes.size(); ++i) 
		{ 
			m_chromosomes[i].fitness = expboltz[i]/average;
		}
	}

	void GA::Crossover( ChromosomeData& father, ChromosomeData& mother, ChromosomeData& child )
	{
		if ((Phenix::Math::Random::RandFloat() > m_crossove_rate) || (mother.genomes == father.genomes)) 
		{
			child = mother;
			return;
		}

		switch(m_crossover_operator)
		{
		case SINGLE:
			CrossoverSingle(father, mother, child);
			break;
		case DOUBLE:
			CrossoverDouble(father, mother, child);
			break;
		case MULTI:
			CrossoverMulti(father, mother, child);
			break;
		case AVERAGE:
			CrossoverAverage(father, mother, child);
			break;
		case HEURISTIC:
			CrossoverHeuristic(father, mother, child);
			break;
		default:
			throw;
		}//end switch
	}

	void GA::CrossoverSingle( ChromosomeData& father, ChromosomeData& mother, ChromosomeData& child )
	{
		//determine a crossover point
		Phenix::Int32 cp = Phenix::Math::Random::RandInt(0, mother.genomes.size() - 1);

		//create the offspring
		for (Phenix::Int32 i=0; i<cp; ++i)                                               
		{
			child.genomes.push_back(mother.genomes[i]);
		}

		for (Phenix::Int32 i=cp; i<mother.genomes.size(); ++i)
		{
			child.genomes.push_back(father.genomes[i]);
		}	
	}

	void GA::CrossoverDouble( ChromosomeData& father, ChromosomeData& mother, ChromosomeData& child )
	{
		//make sure the genome is of the minimum length required to perform 
		//this type of crossover operator
		if (mother.genomes.size() < 2)
			throw;
		child.genomes = mother.genomes;

		//determine two crossover points
		Phenix::Int32 cp1 = Phenix::Math::Random::RandInt(0, mother.genomes.size() - 2);
		Phenix::Int32 cp2 = Phenix::Math::Random::RandInt(cp1, mother.genomes.size()-1);

		//create the offspring
		for (Phenix::Int32 i=cp1; i<cp2; ++i)
		{
			child.genomes[i] = father.genomes[i];
		}
	}

	void GA::CrossoverMulti( ChromosomeData& father, ChromosomeData& mother, ChromosomeData& child )
	{
		const float max_genes_that_can_get_swapped = (float)mother.genomes.size() * 0.5f;

		//first determine a swapping rate for this chromosome
		float gene_crossover_rate = (Phenix::Math::Random::RandFloat() * max_genes_that_can_get_swapped) / 
			(float)mother.genomes.size();

		for (Phenix::Int32 i=0; i<mother.genomes.size(); ++i)
		{
			if (Phenix::Math::Random::RandFloat() < gene_crossover_rate)
			{
				//switch the genes at this point
				child.genomes.push_back(father.genomes[i]);    
			}
			else
			{
				//just copy into offspring 
				child.genomes.push_back(mother.genomes[i]);
			}
		}//next gene
	}

	void GA::CrossoverAverage( ChromosomeData& father, ChromosomeData& mother, ChromosomeData& child )
	{
		//iterate down the length of the genome averaging the values
		for (Phenix::Int32 i=0; i<mother.genomes.size(); ++i)
		{
			child.genomes.push_back((mother.genomes[i] + father.genomes[i]) * 0.5);
		}
	}

	void GA::CrossoverHeuristic( ChromosomeData& father, ChromosomeData& mother, ChromosomeData& child )
	{
		double rate = 0.1;//RandFloat();

		//iterate down the length of the genome using the heuristic
		for (Phenix::Int32 i=0; i<father.genomes.size(); ++i)
		{
			double new_gene_value = father.genomes[i] + rate * 
				(father.genomes[i] - mother.genomes[i]);
			child.genomes.push_back(new_gene_value);
		}
	}

	void GA::Mutate( ChromosomeData& chrom )
	{
		if (Phenix::Math::Random::RandFloat() > m_mutation_rate) 
		{			
			return;
		}
/*		这几种变异都是假设基因值都在0~1之间，所以不通用
		switch(m_crossover_operator)
		{
		case UNIFORM:
			MutateUniform(chrom);
			break;
		case REPLACE:
			MutateReplace(chrom);
			break;
		case GAUSSIAN:
			MutateGaussian(chrom);
			break;
		case BOUNDARY:
			MutateBoundary(chrom);
			break;		
		default:
			throw;
		}//end switch*/
	}
/*
	void GA::MutateUniform( ChromosomeData& chrom )
	{
		//this is the max amount a gene can be mutated by using UniformMutation
		const double mutation_delta     = 0.3; 

		for (Phenix::Int32 i=0; i<chrom.genomes.size(); ++i)
		{
			chrom.genomes[i] += Phenix::Math::Random::RandClamped() * mutation_delta;
			//make sure the value stays within the desired lims
			Phenix::Math::Utils::Clamp(chrom.genomes[i], 0.0, 1.0);
		}//next gene
	}

	void GA::MutateReplace( ChromosomeData& chrom )
	{
		for (Phenix::Int32 i=0; i<chrom.genomes.size(); ++i)
		{
			chrom.genomes[i] = Phenix::Math::Random::RandFloat();
		}//next gene 
	}

	void GA::MutateBoundary( ChromosomeData& chrom )
	{
		for (Phenix::Int32 i=0; i<chrom.genomes.size(); ++i)
		{			
			if (Phenix::Math::Random::RandFloat() < 0.5)
			{
				chrom.genomes[i] = 0.0;
			}
			else
			{
				chrom.genomes[i] = 1.0;
			}
		}//next gene
	}

	void GA::MutateGaussian( ChromosomeData& chrom )
	{
		for (Phenix::Int32 i=0; i<chrom.genomes.size(); ++i)
		{
			chrom.genomes[i] += Phenix::Math::Random::RandGaussian(0.0, 0.1);
			//make sure the value stays within the desired lims
			Phenix::Math::Utils::Clamp(chrom.genomes[i], 0.0, 1.0);
		}//next gene 
	}
*/

} // end namespace GA
} // end namespace AI
} // end namespace Phenix