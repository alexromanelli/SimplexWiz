/*
 * ModeloPLRacional.h
 *
 *  Created on: 29 de jul de 2017
 *      Author: alexandre
 */

#ifndef MODELOPLRACIONAL_H_
#define MODELOPLRACIONAL_H_

#include <string>
#include "Util/NumeroRacional.h"

const int M = 10000000;

enum Objetivo {
	max,
	min
};

enum OperadorRelacional {
	Menor,
	MenorOuIgual,
	Igual,
	MaiorOuIgual,
	Maior,
	Irrestrito // incluído nesta estrutura para caso de variáveis irrestritas
};

enum TipoVariavel {
	Original,
	Folga,
	Excesso,
	SubstitutaParaVariavelIrrestrita,
	Artificial
};

class ModeloPL_Racional {
private:
	Objetivo objetivo;
	int quantidadeVariaveis;
	int quantidadeVariaveisOriginais;
	TipoVariavel* tipoVariavel;
	NumeroRacional* coeficienteFuncaoObjetivo;
	int quantidadeRestricoes;
	NumeroRacional** restricaoCoeficiente;
	OperadorRelacional* restricaoOperadorRelacional;
	NumeroRacional* restricaoConstanteLadoDireito;
	OperadorRelacional* limiteVariavelOperadorRelacional;
	NumeroRacional* limiteVariavelConstanteLadoDireito;

	int* indVariavelBasicaRestricao;

	// -- declarações para o SIMPLEX (com variáveis artificiais e Big-M)
	NumeroRacional* tableauCj;
	NumeroRacional* tableauCB;
	int* tableauBase;
	NumeroRacional** tableauMatriz;
	NumeroRacional* tableauConstantesLadoDireito;
	NumeroRacional* tableauContribuicaoLucroLiquido; // C_Barra
	NumeroRacional tableauZ;
	bool* tableauVariavelEliminada; // para o caso de eliminar variáveis artificiais

	// -- declarações para o SIMPLEX Revisado (SR)
	int* tableauSR_Base;
	NumeroRacional* tableauSR_Cj;
	NumeroRacional* tableauSR_CB;
	NumeroRacional** tableauSR_Vetores_P;
	NumeroRacional* tableauSR_Vetor_b;
	NumeroRacional** tableauSR_Matriz_B;
	NumeroRacional** tableauSR_Matriz_B_inversa;
	NumeroRacional* tableauSR_pi;
	NumeroRacional* tableauSR_C_barra;
	NumeroRacional* tableauSR_Vetor_b_barra;
	bool* tableauSR_VariavelEliminada;

	/**
	 * Faz um mapeamento entre as variáveis originais no modelo, representadas pelas linhas desta
	 * matriz, e as variáveis atuais, representadas por duas colunas.
	 * A correspondência é indicada por dois valores na linha i, sendo que um deles, k, é o índice
	 * da variável com coeficiente 1 e o outro, l, da variável com coeficiente -1. Eles significam:
	 * x[i] = 1*x[k] -1*x[l], sendo x[k] e x[l] >= 0
	 */
	int** correspondenciaVariaveisIrrestritas;

	int adicionarVariavel(); // retorna o índice da variável adicionada
	int adicionarVariavelFolga();
	int adicionarVariavelExcesso();
	bool eVariavelBasica(int indVar);

	void atualizarTableauSR_CB();
	void calcularTableauSR_MultiplicadorSimplex();
	void calcularTableauSR_C_barra();
	bool eVariavelBasicaSR(int indVar);

	/**
	 * Acrescenta uma nova linha na matriz de correspondência entre variáveis originais irrestritas
	 * e variáveis atuais do modelo.
	 */
	void ajustarCorrespondenciaVariavelIrrestrita(int indVariavelIrrestrita, int indNovaVariavelPositiva,
			int indNovaVariavelNegativa);
public:
	ModeloPL_Racional(Objetivo objetivo, int quantidadeVariaveis, NumeroRacional* coeficienteFuncaoObjetivo, int quantidadeRestricoes,
			NumeroRacional** restricaoCoeficiente, OperadorRelacional* restricaoOperadorRelacional, NumeroRacional* restricaoConstanteLadoDireito,
			OperadorRelacional* limiteVariavelOperadorRelacional, NumeroRacional* limiteVariavelConstanteLadoDireito);

	ModeloPL_Racional(ModeloPL_Racional* origem);
	virtual ~ModeloPL_Racional();

	ModeloPL_Racional* obterModeloNaFormaPadrao();
	void imprimirModelo(std::string* legenda);
	int incluirVariavelArtificial(int indRestricaoCoeficienteUm);
	NumeroRacional* definirSolucaoBasicaFactivel();

	int getQuantidadeVariaveis() { return this->quantidadeVariaveis; }

	void definirTableauInicialBigM();
	bool executarPassoSimplex();
	void imprimirTableau();
	NumeroRacional* obterSolucaoTableau();
	void imprimirSolucaoTableau();

	void definirTableauInicialSimplexRevisado();
	bool executarPassoSimplexRevisado();
	void imprimirTableauSimplexRevisado();
	NumeroRacional* obterSolucaoTableauSimplexRevisado();
	void imprimirSolucaoTableauSimplexRevisado();

};

#endif /* MODELOPLRACIONAL_H_ */
