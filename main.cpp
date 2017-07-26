/*
 * File:   main.cpp
 * Author: alexandre
 *
 * Created on 12 de Junho de 2017, 17:49
 */

#include <iterator>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ProgramacaoLinear/ModeloPL.h"
#include "ProgramacaoLinear/Util/NumeroRacional.h"

std::vector<std::string>* splitBySpaces(std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string>* elementos = new std::vector<std::string>((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    return elementos;
}

/**
 * Formato (exemplo):
 * min x1 + x2
 * s.a:
 *     x1 + 2x2 <= 4
 *           x2 <= 1
 *     x1 >= 0
 *     x2 >= 0
 * +--------------------------------------+
 * |objetivo: min                         |
 * |variaveis: 2                          |
 * |coeficientes-funcao-objetivo: 1 1     |
 * |restricoes: 2                         |
 * |1 2 <= 4                              |
 * |0 1 <= 1                              |
 * |limites-variaveis:                    |
 * |1 >= 0                                |
 * |2 irr                                 |
 * +--------------------------------------+
 */
ModeloPL* lerModeloProgramacaoLinear() {
    std::string linha;

    std::getline(std::cin, linha, '\n');
    std::vector<std::string>* elem = splitBySpaces(linha);
    int quantidadeVariaveis = atoi((*elem)[1].c_str());
    delete elem;

    std::getline(std::cin, linha, '\n');
    elem = splitBySpaces(linha);
    Objetivo objetivo;
    if ((*elem)[1].compare("min") == 0) {
    	objetivo = Objetivo::min;
    } else if ((*elem)[1].compare("max") == 0) {
    	objetivo = Objetivo::max;
    }
    delete elem;

    double* coeficientesFuncaoObjetivo = new double[2];
    std::getline(std::cin, linha, '\n');
    elem = splitBySpaces(linha);
    for (int i = 0; i < quantidadeVariaveis; i++) {
    	coeficientesFuncaoObjetivo[i] = atof((*elem)[i + 1].c_str());
    }
    delete elem;

    std::getline(std::cin, linha, '\n');
    elem = splitBySpaces(linha);
    int quantidadeRestricoes = atoi((*elem)[1].c_str());
    delete elem;

    // declaração de variáveis para os dados das restrições
    double** matrizCoeficientes = new double*[quantidadeRestricoes];
    for (int i = 0; i < quantidadeRestricoes; i++)
    	matrizCoeficientes[i] = new double[quantidadeVariaveis];
    double* vetorConstantes = new double[quantidadeRestricoes];
    OperadorRelacional* operadorRelacional = new OperadorRelacional[quantidadeRestricoes];

    // obter cada restrição
    for (int i = 0; i < quantidadeRestricoes; i++) {
        std::getline(std::cin, linha, '\n');
        elem = splitBySpaces(linha);

        // obter coeficientes das variáveis
        for (int j = 0; j < quantidadeVariaveis; j++) {
        	matrizCoeficientes[i][j] = atof((*elem)[j].c_str());
        }

        // obter operador relacional
        if ((*elem)[quantidadeVariaveis].compare("<") == 0) {
            operadorRelacional[i] = OperadorRelacional::Menor;
        } else if ((*elem)[quantidadeVariaveis].compare("<=") == 0) {
            operadorRelacional[i] = OperadorRelacional::MenorOuIgual;
        } else if ((*elem)[quantidadeVariaveis].compare("=") == 0) {
            operadorRelacional[i] = OperadorRelacional::Igual;
        } else if ((*elem)[quantidadeVariaveis].compare(">=") == 0) {
            operadorRelacional[i] = OperadorRelacional::MaiorOuIgual;
        } else if ((*elem)[quantidadeVariaveis].compare(">") == 0) {
            operadorRelacional[i] = OperadorRelacional::Maior;
        }

        // obter constante do lado direito
        vetorConstantes[i] = atof((*elem)[quantidadeVariaveis + 1].c_str());

        delete elem;
    }

    // salta linha com texto "limites-variaveis:"
    std::getline(std::cin, linha);

    // declaração de estruturas para os limites das variáveis
    OperadorRelacional* limiteVariavelRelacao = new OperadorRelacional[quantidadeVariaveis];
    double* limiteVariavelConstante = new double[quantidadeVariaveis];

    // obter limites de variáveis
    for (int i = 0; i < quantidadeVariaveis; i++) {
    	std::getline(std::cin, linha, '\n');
    	elem = splitBySpaces(linha);

    	// obter índice da variável
    	int indVariavel = atoi((*elem)[0].c_str()) - 1;

        // obter operador relacional
    	OperadorRelacional op;
        if ((*elem)[1].compare("<") == 0) {
        	op = OperadorRelacional::Menor;
        } else if ((*elem)[1].compare("<=") == 0) {
        	op = OperadorRelacional::MenorOuIgual;
        } else if ((*elem)[1].compare("=") == 0) {
        	op = OperadorRelacional::Igual;
        } else if ((*elem)[1].compare(">=") == 0) {
        	op = OperadorRelacional::MaiorOuIgual;
        } else if ((*elem)[1].compare(">") == 0) {
        	op = OperadorRelacional::Maior;
        } else if ((*elem)[1].compare("irr") == 0) {
        	op = OperadorRelacional::Irrestrito;
        }

        // obter constante do lado direito
        double cons = 0;
        if (op != OperadorRelacional::Irrestrito)
        	cons = atof((*elem)[2].c_str());

        limiteVariavelRelacao[indVariavel] = op;
        limiteVariavelConstante[indVariavel] = cons;
    }

    ModeloPL* modelo = new ModeloPL(objetivo, quantidadeVariaveis, coeficientesFuncaoObjetivo, quantidadeRestricoes,
    		matrizCoeficientes, operadorRelacional, vetorConstantes, limiteVariavelRelacao, limiteVariavelConstante);

    return modelo;
}

int main() {
    // entrada de dados:
	ModeloPL* modeloOriginal = lerModeloProgramacaoLinear();
	std::string legenda = "Modelo original";
	modeloOriginal->imprimirModelo(&legenda);
	ModeloPL* modeloNaFormaPadrao = modeloOriginal->obterModeloNaFormaPadrao();
	legenda = "Modelo na forma padrão";
	modeloNaFormaPadrao->imprimirModelo(&legenda);

	// determina solução básica factível
	double* solucaoBasicaFactivel = modeloNaFormaPadrao->definirSolucaoBasicaFactivel();
	legenda = "Modelo na forma padrão após definir solução básica factível";
	modeloNaFormaPadrao->imprimirModelo(&legenda);

	std::printf("\n Solução básica factível:\n");
	for (int i = 0; i < modeloNaFormaPadrao->getQuantidadeVariaveis(); i++) {
		std::printf("  x%-2d = %8.3f\n", i + 1, solucaoBasicaFactivel[i]);
	}

	modeloNaFormaPadrao->definirTableauInicialBigM();
	modeloNaFormaPadrao->imprimirTableau();

	if (modeloNaFormaPadrao->executarPassoSimplex())
		modeloNaFormaPadrao->imprimirTableau();
	if (modeloNaFormaPadrao->executarPassoSimplex())
		modeloNaFormaPadrao->imprimirTableau();

    return 0;
}

