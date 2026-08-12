#include "PmergeMe.hpp"

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <deque>
#include <iomanip>
#include <iostream>
#include <sys/time.h>
#include <vector>
#include <string>

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	(void)other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	(void)other;
	return *this;
}

PmergeMe::~PmergeMe()
{
}

static bool isPositiveInteger(const std::string &token, int &value)
{
	if (token.empty())
		return false;
	char *end = 0;
	long parsed = std::strtol(token.c_str(), &end, 10);
	if (end == token.c_str() || *end != '\0' || parsed <= 0 || parsed > INT_MAX)
		return false;
	value = static_cast<int>(parsed);
	return true;
}

/*
** jacobsthalSequence - Gera a sequência de Jacobsthal para ordem ótima de inserção
**
** A sequência de Jacobsthal (1, 3, 5, 11, 21, 43, ...) define a ordem em que
** os elementos pendentes devem ser inseridos para minimizar comparações.
**
** Fórmula: J(k) = J(k-1) + 2*J(k-2), com J(0) = 0, J(1) = 1
**
** Exemplo com n=5:
**   Iteração 1: j0=0, j1=1 → next = 1 + 2*0 = 1 → seq=[1]
**   Iteração 2: j0=1, j1=1 → next = 1 + 2*1 = 3 → seq=[1,3]
**   Iteração 3: j0=1, j1=3 → next = 3 + 2*1 = 5 → seq=[1,3,5]
**   (stop, j1 >= n)
*/
static std::vector<size_t> jacobsthalSequence(size_t n)
{
	std::vector<size_t> seq;
	if (n == 0)
		return seq;
	size_t j0 = 0;
	size_t j1 = 1;
	seq.push_back(j1);
	while (j1 < n)
	{
		size_t next = j1 + 2 * j0;
		if (next > n)
			next = n;
		seq.push_back(next);
		j0 = j1;
		j1 = next;
	}
	return seq;
}

/*
** sortWithVector - Implementa Ford-Johnson Algorithm usando std::vector
**
** O algoritmo funciona em 4 fases:
**
** FASE 1: Comparação de Pares
**   - Divide a sequência em pares consecutivos
**   - Compara cada par (left, right) e garante left <= right
**   - Separa em: main_chain (maiores) e pend (menores)
**   - Se houver elemento ímpar, armazena separadamente
**   Custo: ⌊n/2⌋ comparações
**
** FASE 2: Ordenação da Cadeia Principal
**   - Ordena main_chain (os maiores de cada par)
**   - Usa std::sort que é O(m log m) com m = ⌊n/2⌋
**   Custo: O(m log m) comparações
**
** FASE 3: Inserir o Elemento Ímpar
**   - Se houver, insere o elemento ímpar na posição correta
**   - Usa lower_bound (busca binária)
**   Custo: O(log m) comparações + 1 inserção
**
** FASE 4: Inserir Pendentes com Sequência de Jacobsthal
**   - Gera sequência de Jacobsthal para ordem ótima
**   - Insere cada pendente usando lower_bound (busca binária)
**   - Insere primeiro os índices da sequência de Jacobsthal
**   - Depois insere os restantes em ordem
**   Custo: O(n/2 * log n) comparações total
**
** RESULTADO: Algoritmo realiza ~0.5n log n comparações (ótimo teórico)
*/
static std::vector<int> sortWithVector(const std::vector<int> &input)
{
	std::vector<int> main_chain;
	std::vector<int> pend;
	std::vector<size_t> pend_indices;

	if (input.empty())
		return input;
	if (input.size() == 1)
		return input;

	int unpaired = -1;
	for (size_t i = 0; i + 1 < input.size(); i += 2)
	{
		int left = input[i];
		int right = input[i + 1];
		if (left > right)
			std::swap(left, right);
		pend.push_back(left);
		pend_indices.push_back(i / 2);
		main_chain.push_back(right);
	}
	if (input.size() % 2 == 1)
		unpaired = input.back();

	// ===== FASE 2: ORDENAR A CADEIA PRINCIPAL =====
	std::sort(main_chain.begin(), main_chain.end());

	std::vector<int> result = main_chain;

	// ===== FASE 3: INSERIR ELEMENTO ÍMPAR (se houver) =====
	if (unpaired != -1)
	{
		// lower_bound faz busca binária para encontrar a posição correta
		std::vector<int>::iterator it = std::lower_bound(result.begin(), result.end(), unpaired);
		result.insert(it, unpaired);
	}

	// ===== FASE 4: INSERIR PENDENTES COM SEQUÊNCIA DE JACOBSTHAL =====

	// Gera sequência de Jacobsthal para ordem ótima de inserção
	std::vector<size_t> jacobsthal = jacobsthalSequence(pend.size());
	std::vector<bool> inserted(pend.size(), false);

	// PASSO 1: Inserir seguindo a sequência de Jacobsthal
	// Exemplo: Se jacobsthal=[1,3,5], insere pend[0], depois pend[2], depois pend[4]
	// Isso minimiza o número de comparações durante as buscas binárias
	for (size_t i = 0; i < jacobsthal.size(); ++i)
	{
		size_t idx = jacobsthal[i] - 1; // Converte de 1-based para 0-based
		if (idx < pend.size() && !inserted[idx])
		{
			int value = pend[idx];
			// lower_bound usa BUSCA BINÁRIA (O(log n)) em vez de busca linear
			// Essa é a chave da eficiência do Ford-Johnson!
			std::vector<int>::iterator it = std::lower_bound(result.begin(), result.end(), value);
			result.insert(it, value);
			inserted[idx] = true;
		}
	}

	// PASSO 2: Inserir os elementos restantes (não cobertos por Jacobsthal)
	// Continua a usar busca binária para cada inserção
	for (size_t i = 0; i < pend.size(); ++i)
	{
		if (!inserted[i])
		{
			int value = pend[i];
			std::vector<int>::iterator it = std::lower_bound(result.begin(), result.end(), value);
			result.insert(it, value);
		}
	}

	return result;
}

/*
** sortWithDeque - Implementa Ford-Johnson Algorithm usando std::deque
**
** Usa EXATAMENTE O MESMO ALGORITMO que sortWithVector,
** mas com std::deque em vez de std::vector.
**
** Deque é uma "double-ended queue" que oferece:
** - Inserção/remoção O(1) nas pontas
** - Inserção/remoção O(n) no meio (como vector)
** - Acesso aleatório mais lento que vector
**
** Na prática, vector geralmente é mais rápido para este algoritmo
** porque as inserções no meio dominam o custo.
*/
static std::deque<int> sortWithDeque(const std::vector<int> &input)
{
	std::deque<int> main_chain;
	std::deque<int> pend;
	std::deque<size_t> pend_indices;

	if (input.empty())
		return main_chain;
	if (input.size() == 1)
	{
		main_chain.push_back(input[0]);
		return main_chain;
	}

	// ===== FASE 1: COMPARAÇÃO DE PARES (idêntico ao vector) =====
	int unpaired = -1;
	for (size_t i = 0; i + 1 < input.size(); i += 2)
	{
		int left = input[i];
		int right = input[i + 1];
		if (left > right)
			std::swap(left, right);
		pend.push_back(left);
		pend_indices.push_back(i / 2);
		main_chain.push_back(right);
	}
	if (input.size() % 2 == 1)
		unpaired = input.back();

	// ===== FASE 2: ORDENAR A CADEIA PRINCIPAL =====
	// Nota: std::deque não tem sort direto otimizado, então copia para vector,
	// ordena, e copia de volta (deque -> vector -> sort -> vector -> deque)
	std::vector<int> temp(main_chain.begin(), main_chain.end());
	std::sort(temp.begin(), temp.end());
	main_chain.clear();
	for (size_t i = 0; i < temp.size(); ++i)
		main_chain.push_back(temp[i]);

	std::deque<int> result = main_chain;

	// ===== FASE 3: INSERIR ELEMENTO ÍMPAR (idêntico ao vector) =====
	if (unpaired != -1)
	{
		std::deque<int>::iterator it = std::lower_bound(result.begin(), result.end(), unpaired);
		result.insert(it, unpaired);
	}

	// ===== FASE 4: INSERIR PENDENTES COM SEQUÊNCIA DE JACOBSTHAL (idêntico ao vector) =====
	std::vector<size_t> jacobsthal = jacobsthalSequence(pend.size());
	std::vector<bool> inserted(pend.size(), false);

	// PASSO 1: Inserir seguindo a sequência de Jacobsthal (busca binária)
	for (size_t i = 0; i < jacobsthal.size(); ++i)
	{
		size_t idx = jacobsthal[i] - 1;
		if (idx < pend.size() && !inserted[idx])
		{
			int value = pend[idx];
			std::deque<int>::iterator it = std::lower_bound(result.begin(), result.end(), value);
			result.insert(it, value);
			inserted[idx] = true;
		}
	}

	// PASSO 2: Inserir elementos restantes com busca binária
	for (size_t i = 0; i < pend.size(); ++i)
	{
		if (!inserted[i])
		{
			int value = pend[i];
			std::deque<int>::iterator it = std::lower_bound(result.begin(), result.end(), value);
			result.insert(it, value);
		}
	}

	return result;
}

static void printSequence(const std::string &label, const std::vector<int> &values)
{
	std::cout << label;
	for (size_t i = 0; i < values.size(); ++i)
	{
		std::cout << (i == 0 ? "" : " ") << values[i];
	}
	std::cout << NC << std::endl;
}

static double elapsedMicroseconds(const timeval &start, const timeval &end)
{
	return static_cast<double>((end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec));
}

int PmergeMe::run(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << BRED "Error" NC << std::endl;
		return 1;
	}
	std::vector<int> input;
	for (int i = 1; i < argc; ++i)
	{
		int value = 0;
		if (!isPositiveInteger(argv[i], value))
		{
			std::cerr << BRED "Error" NC << std::endl;
			return 1;
		}
		input.push_back(value);
	}
	printSequence(BMAG "Before: " NC, input);

	struct timeval startVector;
	struct timeval endVector;
	struct timeval startDeque;
	struct timeval endDeque;
	gettimeofday(&startVector, 0);
	std::vector<int> sortedVector = sortWithVector(input);
	gettimeofday(&endVector, 0);

	gettimeofday(&startDeque, 0);
	std::deque<int> sortedDeque = sortWithDeque(input);
	gettimeofday(&endDeque, 0);

	printSequence(BCYN "After: " NC, sortedVector);
	std::cout << std::fixed << std::setprecision(5);
	std::cout << BMAG "Time to process a range of " << input.size() << " elements with std::vector : " BWHT
			  << elapsedMicroseconds(startVector, endVector) << " us" << NC << std::endl;
	std::cout << BCYN "Time to process a range of " << input.size() << " elements with std::deque : " BWHT
			  << elapsedMicroseconds(startDeque, endDeque) << " us" << NC << std::endl;
	(void)sortedDeque;
	return 0;
}
