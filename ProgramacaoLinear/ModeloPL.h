/*
 * ModeloPL.h
 *
 *  Created on: 13 de jun de 2017
 *      Author: alexandre
 */

#ifndef MODELOPL_H_
#define MODELOPL_H_

#include <string>

const double M = 1e+300;

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

class ModeloPL {
private:
	Objetivo objetivo;
	int quantidadeVariaveis;
	int quantidadeVariaveisOriginais;
	TipoVariavel* tipoVariavel;
	double* coeficienteFuncaoObjetivo;
	int quantidadeRestricoes;
	double** restricaoCoeficiente;
	OperadorRelacional* restricaoOperadorRelacional;
	double* restricaoConstanteLadoDireito;
	OperadorRelacional* limiteVariavelOperadorRelacional;
	double* limiteVariavelConstanteLadoDireito;

	int* indVariavelBasicaRestricao;

	double* tableauCj;
	double* tableauCB;
	int* tableauBase;
	double** tableauMatriz;
	double* tableauConstantesLadoDireito;
	double* tableauContribuicaoLucroLiquido; // C_Barra
	double tableauZ;
	bool* tableauVariavelEliminada; // para o caso de eliminar variáveis artificiais

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

	/**
	 * Acrescenta uma nova linha na matriz de correspondência entre variáveis originais irrestritas
	 * e variáveis atuais do modelo.
	 */
	void ajustarCorrespondenciaVariavelIrrestrita(int indVariavelIrrestrita, int indNovaVariavelPositiva,
			int indNovaVariavelNegativa);
public:
	ModeloPL(Objetivo objetivo, int quantidadeVariaveis, double* coeficienteFuncaoObjetivo, int quantidadeRestricoes,
			double** restricaoCoeficiente, OperadorRelacional* restricaoOperadorRelacional, double* restricaoConstanteLadoDireito,
			OperadorRelacional* limiteVariavelOperadorRelacional, double* limiteVariavelConstanteLadoDireito);

	ModeloPL(ModeloPL* origem);
	virtual ~ModeloPL();

	ModeloPL* obterModeloNaFormaPadrao();
	void imprimirModelo(std::string* legenda);
	int incluirVariavelArtificial(int indRestricaoCoeficienteUm);
	double* definirSolucaoBasicaFactivel();

	int getQuantidadeVariaveis() { return this->quantidadeVariaveis; }

	void definirTableauInicialBigM();
	bool executarPassoSimplex();
	void imprimirTableau();
	double* obterSolucaoTableau();
	void imprimirSolucaoTableau();

};

#endif /* MODELOPL_H_ */
