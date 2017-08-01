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
//#include "ProgramacaoLinear/ModeloPL.h"
#include "ProgramacaoLinear/ModeloPLRacional.h"
#include "ProgramacaoLinear/Util/NumeroRacional.h"

//std::vector<std::string>* splitBySpaces(std::string& str) {
//    std::istringstream iss(str);
//    std::vector<std::string>* elementos = new std::vector<std::string>((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
//    return elementos;
//}

//void splitBySpaces(std::string linha, std::vector<std::string>& elem) {
//    std::istringstream iss(linha);
//    elem.clear();
//    std::copy(std::istream_iterator<std::string>(iss),
//         std::istream_iterator<std::string>(),
//         back_inserter(elem));
//}

void splitBySpaces(std::string* linha, std::vector<std::string>* elem) {
	elem->clear();
	std::string parte = "";
	for (unsigned int i = 0; i < linha->length(); i++) {
		if (linha->at(i) == ' ') {
			std::string item = parte.c_str();
			elem->push_back(item);
			parte.clear();
		} else {
			parte.push_back(linha->at(i));
		}
	}
	if (parte.length() > 0) {
		std::string item = parte.c_str();
		elem->push_back(item);
	}
}

/**
 * Formato (exemplo):
 * min x1 + x2
 * s.a:
 *     x1 + 2x2 <= 4
 *           x2 <= 1
 *     x1 >= 0
 *     x2 irrestrita em sinal
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

//ModeloPL* lerModeloProgramacaoLinear() {
//	std::string* linha = new std::string("");
//	std::vector<std::string>* elem = new std::vector<std::string>();
//
//	std::getline(std::cin, *linha, '\n');
//    splitBySpaces(linha, elem);
//    int quantidadeVariaveis = atoi((*elem)[1].c_str());
//
//    std::getline(std::cin, *linha, '\n');
//    splitBySpaces(linha, elem);
//    Objetivo objetivo = Objetivo::max; // valor default arbitrário
//    if ((*elem)[1].compare("min") == 0) {
//    	objetivo = Objetivo::min;
//    } else if ((*elem)[1].compare("max") == 0) {
//    	objetivo = Objetivo::max;
//    }
//
//    double* coeficientesFuncaoObjetivo = new double[quantidadeVariaveis];
//    std::getline(std::cin, *linha, '\n');
//    splitBySpaces(linha, elem);
//    for (int i = 0; i < quantidadeVariaveis; i++) {
//    	coeficientesFuncaoObjetivo[i] = atof((*elem)[i + 1].c_str());
//    }
//
//    std::getline(std::cin, *linha, '\n');
//    splitBySpaces(linha, elem);
//    int quantidadeRestricoes = atoi((*elem)[1].c_str());
//
//    // declaração de variáveis para os dados das restrições
//    double** matrizCoeficientes = new double*[quantidadeRestricoes];
//    for (int i = 0; i < quantidadeRestricoes; i++)
//    	matrizCoeficientes[i] = new double[quantidadeVariaveis];
//    double* vetorConstantes = new double[quantidadeRestricoes];
//    OperadorRelacional* operadorRelacional = new OperadorRelacional[quantidadeRestricoes];
//
//    // obter cada restrição
//    for (int i = 0; i < quantidadeRestricoes; i++) {
//        std::getline(std::cin, *linha, '\n');
//        splitBySpaces(linha, elem);
//
//        // obter coeficientes das variáveis
//        for (int j = 0; j < quantidadeVariaveis; j++) {
//        	matrizCoeficientes[i][j] = atof((*elem)[j].c_str());
//        }
//
//        // obter operador relacional
//        if ((*elem)[quantidadeVariaveis].compare("<") == 0) {
//            operadorRelacional[i] = OperadorRelacional::Menor;
//        } else if ((*elem)[quantidadeVariaveis].compare("<=") == 0) {
//            operadorRelacional[i] = OperadorRelacional::MenorOuIgual;
//        } else if ((*elem)[quantidadeVariaveis].compare("=") == 0) {
//            operadorRelacional[i] = OperadorRelacional::Igual;
//        } else if ((*elem)[quantidadeVariaveis].compare(">=") == 0) {
//            operadorRelacional[i] = OperadorRelacional::MaiorOuIgual;
//        } else if ((*elem)[quantidadeVariaveis].compare(">") == 0) {
//            operadorRelacional[i] = OperadorRelacional::Maior;
//        }
//
//        // obter constante do lado direito
//        vetorConstantes[i] = atof((*elem)[quantidadeVariaveis + 1].c_str());
//    }
//
//    // salta linha com texto "limites-variaveis:"
//    std::getline(std::cin, *linha);
//
//    // declaração de estruturas para os limites das variáveis
//    OperadorRelacional* limiteVariavelRelacao = new OperadorRelacional[quantidadeVariaveis];
//    double* limiteVariavelConstante = new double[quantidadeVariaveis];
//
//    // obter limites de variáveis
//    for (int i = 0; i < quantidadeVariaveis; i++) {
//    	std::getline(std::cin, *linha, '\n');
//    	splitBySpaces(linha, elem);
//
//    	// obter índice da variável
//    	int indVariavel = atoi((*elem)[0].c_str()) - 1;
//
//        // obter operador relacional
//    	OperadorRelacional op = OperadorRelacional::Irrestrito; // valor default arbitrário
//        if ((*elem)[1].compare("<") == 0) {
//        	op = OperadorRelacional::Menor;
//        } else if ((*elem)[1].compare("<=") == 0) {
//        	op = OperadorRelacional::MenorOuIgual;
//        } else if ((*elem)[1].compare("=") == 0) {
//        	op = OperadorRelacional::Igual;
//        } else if ((*elem)[1].compare(">=") == 0) {
//        	op = OperadorRelacional::MaiorOuIgual;
//        } else if ((*elem)[1].compare(">") == 0) {
//        	op = OperadorRelacional::Maior;
//        } else if ((*elem)[1].compare("irr") == 0) {
//        	op = OperadorRelacional::Irrestrito;
//        }
//
//        // obter constante do lado direito
//        double cons = 0;
//        if (op != OperadorRelacional::Irrestrito)
//        	cons = atof((*elem)[2].c_str());
//
//        limiteVariavelRelacao[indVariavel] = op;
//        limiteVariavelConstante[indVariavel] = cons;
//    }
//
//    ModeloPL* modelo = new ModeloPL(objetivo, quantidadeVariaveis, coeficientesFuncaoObjetivo, quantidadeRestricoes,
//    		matrizCoeficientes, operadorRelacional, vetorConstantes, limiteVariavelRelacao, limiteVariavelConstante);
//
//    delete [] coeficientesFuncaoObjetivo;
//    for (int i = 0; i < quantidadeRestricoes; i++)
//    	delete [] matrizCoeficientes[i];
//    delete [] matrizCoeficientes;
//    delete [] vetorConstantes;
//    delete [] operadorRelacional;
//    delete [] limiteVariavelRelacao;
//    delete [] limiteVariavelConstante;
//
//    delete linha;
//    delete elem;
//
//    return modelo;
//}

ModeloPL_Racional* lerModeloProgramacaoLinear_Racional() {
	std::string* linha = new std::string("");
	std::vector<std::string>* elem = new std::vector<std::string>();

	std::getline(std::cin, *linha, '\n');
    splitBySpaces(linha, elem);
    int quantidadeVariaveis = atoi((*elem)[1].c_str());

    std::getline(std::cin, *linha, '\n');
    splitBySpaces(linha, elem);
    Objetivo objetivo = Objetivo::max; // valor default arbitrário
    if ((*elem)[1].compare("min") == 0) {
    	objetivo = Objetivo::min;
    } else if ((*elem)[1].compare("max") == 0) {
    	objetivo = Objetivo::max;
    }

    NumeroRacional* coeficientesFuncaoObjetivo = new NumeroRacional[quantidadeVariaveis];
    std::getline(std::cin, *linha, '\n');
    splitBySpaces(linha, elem);
    for (int i = 0; i < quantidadeVariaveis; i++) {
    	coeficientesFuncaoObjetivo[i] = atoi((*elem)[i + 1].c_str());
    }

    std::getline(std::cin, *linha, '\n');
    splitBySpaces(linha, elem);
    int quantidadeRestricoes = atoi((*elem)[1].c_str());

    // declaração de variáveis para os dados das restrições
    NumeroRacional** matrizCoeficientes = new NumeroRacional*[quantidadeRestricoes];
    for (int i = 0; i < quantidadeRestricoes; i++)
    	matrizCoeficientes[i] = new NumeroRacional[quantidadeVariaveis];
    NumeroRacional* vetorConstantes = new NumeroRacional[quantidadeRestricoes];
    OperadorRelacional* operadorRelacional = new OperadorRelacional[quantidadeRestricoes];

    // obter cada restrição
    for (int i = 0; i < quantidadeRestricoes; i++) {
        std::getline(std::cin, *linha, '\n');
        splitBySpaces(linha, elem);

        // obter coeficientes das variáveis
        for (int j = 0; j < quantidadeVariaveis; j++) {
        	matrizCoeficientes[i][j] = atoi((*elem)[j].c_str());
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
        vetorConstantes[i] = atoi((*elem)[quantidadeVariaveis + 1].c_str());
    }

    // salta linha com texto "limites-variaveis:"
    std::getline(std::cin, *linha);

    // declaração de estruturas para os limites das variáveis
    OperadorRelacional* limiteVariavelRelacao = new OperadorRelacional[quantidadeVariaveis];
    NumeroRacional* limiteVariavelConstante = new NumeroRacional[quantidadeVariaveis];

    // obter limites de variáveis
    for (int i = 0; i < quantidadeVariaveis; i++) {
    	std::getline(std::cin, *linha, '\n');
    	splitBySpaces(linha, elem);

    	// obter índice da variável
    	int indVariavel = atoi((*elem)[0].c_str()) - 1;

        // obter operador relacional
    	OperadorRelacional op = OperadorRelacional::Irrestrito; // valor default arbitrário
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
        int cons = 0;
        if (op != OperadorRelacional::Irrestrito)
        	cons = atoi((*elem)[2].c_str());

        limiteVariavelRelacao[indVariavel] = op;
        limiteVariavelConstante[indVariavel] = cons;
    }

    ModeloPL_Racional* modelo = new ModeloPL_Racional(objetivo, quantidadeVariaveis, coeficientesFuncaoObjetivo, quantidadeRestricoes,
    		matrizCoeficientes, operadorRelacional, vetorConstantes, limiteVariavelRelacao, limiteVariavelConstante);

    delete [] coeficientesFuncaoObjetivo;
    for (int i = 0; i < quantidadeRestricoes; i++)
    	delete [] matrizCoeficientes[i];
    delete [] matrizCoeficientes;
    delete [] vetorConstantes;
    delete [] operadorRelacional;
    delete [] limiteVariavelRelacao;
    delete [] limiteVariavelConstante;

    delete linha;
    delete elem;

    return modelo;
}

//int main() {
//    // entrada de dados:
//	ModeloPL* modeloOriginal = lerModeloProgramacaoLinear();
//	std::string* legenda = new std::string("Modelo original");
//	modeloOriginal->imprimirModelo(legenda);
//	ModeloPL* modeloNaFormaPadrao = modeloOriginal->obterModeloNaFormaPadrao();
//	*legenda = "Modelo na forma padrão";
//	modeloNaFormaPadrao->imprimirModelo(legenda);
//
//	// determina solução básica factível
//	double* solucaoBasicaFactivel = modeloNaFormaPadrao->definirSolucaoBasicaFactivel();
//	*legenda = "Modelo na forma padrão após definir solução básica factível";
//	modeloNaFormaPadrao->imprimirModelo(legenda);
//
//	std::printf("\n Solução básica factível:\n");
//	for (int i = 0; i < modeloNaFormaPadrao->getQuantidadeVariaveis(); i++) {
//		std::printf("  x%-2d = %8.3f\n", i + 1, solucaoBasicaFactivel[i]);
//	}
//
//	modeloNaFormaPadrao->definirTableauInicialBigM();
//	modeloNaFormaPadrao->imprimirTableau();
//
//	int iteracoes = 0;
//	bool solOtima = false;
//	while (true) {
//		bool deuPasso = modeloNaFormaPadrao->executarPassoSimplex();
//		if (deuPasso) {
//			modeloNaFormaPadrao->imprimirTableau();
//			iteracoes++;
//			if (iteracoes > 10)
//				break;
//		} else {
//			solOtima = true;
//			break;
//		}
//	}
//	if (solOtima) {
//		modeloNaFormaPadrao->imprimirSolucaoTableau();
//	} else {
//		std::printf("\nProblema para encontrar solução ótima. Loop.");
//	}
//	std::printf("\n\nFim da execução do SIMPLEX \"normal\".\n");
//	//delete modeloNaFormaPadrao;
//
//	std::printf("\n\nExecução do SIMPLEX Revisado:\n");
//	modeloNaFormaPadrao = modeloOriginal->obterModeloNaFormaPadrao();
//	modeloNaFormaPadrao->definirTableauInicialSimplexRevisado();
//	modeloNaFormaPadrao->imprimirTableauSimplexRevisado();
//
//	iteracoes = 0;
//	solOtima = false;
//	while (true) {
//		bool deuPasso = modeloNaFormaPadrao->executarPassoSimplexRevisado();
//		if (deuPasso) {
//			modeloNaFormaPadrao->imprimirTableauSimplexRevisado();
//			iteracoes++;
//			if (iteracoes > 10)
//				break;
//		} else {
//			solOtima = true;
//			break;
//		}
//	}
//	if (solOtima) {
//		modeloNaFormaPadrao->imprimirSolucaoTableauSimplexRevisado();
//	} else {
//		std::printf("\nProblema para encontrar solução ótima. Loop.");
//	}
//
//	std::printf("\n\nFim da execução do SIMPLEX Revisado.\n\n");
//	//delete modeloNaFormaPadrao;
//
//    return 0;
//}

void imprimirComparativoSimplexNormalRevisado(int totalAdicoesSimplexNormal, int totalMultiplicacoesSimplexNormal,
		int totalAdicoesSimplexRevisado, int totalMultiplicacoesSimplexRevisado) {
	std::printf("\nComparativo Simplex-Normal vs Simplex-Revisado:\n\n");
	std::printf(" Operação       |    Normal | Revisado \n");
	std::printf("----------------+-----------+----------\n");
	std::printf(" Adições        | %9d | %9d \n", totalAdicoesSimplexNormal, totalAdicoesSimplexRevisado);
	std::printf(" Multiplicações | %9d | %9d \n", totalMultiplicacoesSimplexNormal, totalMultiplicacoesSimplexRevisado);
	std::printf("----------------+-----------+----------\n");
}

int main() {
    // entrada de dados:
	ModeloPL_Racional* modeloOriginal = lerModeloProgramacaoLinear_Racional();
	std::string* legenda = new std::string("Modelo original");
	modeloOriginal->imprimirModelo(legenda);
	ModeloPL_Racional* modeloNaFormaPadrao = modeloOriginal->obterModeloNaFormaPadrao();
	*legenda = "Modelo na forma padrão";
	modeloNaFormaPadrao->imprimirModelo(legenda);

	// determina solução básica factível
	NumeroRacional* solucaoBasicaFactivel = modeloNaFormaPadrao->definirSolucaoBasicaFactivel();
	*legenda = "Modelo na forma padrão após definir solução básica factível";
	modeloNaFormaPadrao->imprimirModelo(legenda);

	std::printf("\n Solução básica factível:\n");
	for (int i = 0; i < modeloNaFormaPadrao->getQuantidadeVariaveis(); i++) {
		std::printf("  x%-2d = %8s\n", i + 1, solucaoBasicaFactivel[i].toString().c_str());
	}

	modeloNaFormaPadrao->definirTableauInicialBigM();
	modeloNaFormaPadrao->imprimirTableau();

	int iteracoes = 0;
	bool solOtima = false;
	while (true) {
		bool deuPasso = modeloNaFormaPadrao->executarPassoSimplex();
		if (deuPasso) {
			modeloNaFormaPadrao->imprimirTableau();
			iteracoes++;
			if (iteracoes > 10)
				break;
		} else {
			solOtima = true;
			break;
		}
	}
	if (solOtima) {
		modeloNaFormaPadrao->imprimirSolucaoTableau();
	} else {
		std::printf("\nProblema para encontrar solução ótima. Loop.");
	}
	std::printf("\n\nFim da execução do SIMPLEX \"normal\".\n");

	int totalAdicoesSimplexNormal = modeloNaFormaPadrao->obterTotalAdicoesSimplexNormal();
	int totalMultiplicacoesSimplexNormal = modeloNaFormaPadrao->obterTotalMultiplicacoesSimplexNormal();
	//delete modeloNaFormaPadrao;

	std::printf("\n\nExecução do SIMPLEX Revisado:\n");
	modeloNaFormaPadrao = modeloOriginal->obterModeloNaFormaPadrao();
	modeloNaFormaPadrao->definirTableauInicialSimplexRevisado();
	modeloNaFormaPadrao->imprimirTableauSimplexRevisado();

	iteracoes = 0;
	solOtima = false;
	while (true) {
		bool deuPasso = modeloNaFormaPadrao->executarPassoSimplexRevisado();
		if (deuPasso) {
			modeloNaFormaPadrao->imprimirTableauSimplexRevisado();
			iteracoes++;
			if (iteracoes > 10)
				break;
		} else {
			solOtima = true;
			break;
		}
	}
	if (solOtima) {
		modeloNaFormaPadrao->imprimirSolucaoTableauSimplexRevisado();
	} else {
		std::printf("\nProblema para encontrar solução ótima. Loop.");
	}

	std::printf("\n\nFim da execução do SIMPLEX Revisado.\n\n");
	int totalAdicoesSimplexRevisado = modeloNaFormaPadrao->obterTotalAdicoesSimplexRevisado();
	int totalMultiplicacoesSimplexRevisado = modeloNaFormaPadrao->obterTotalMultiplicacoesSimplexRevisado();
	//delete modeloNaFormaPadrao;

	imprimirComparativoSimplexNormalRevisado(totalAdicoesSimplexNormal, totalMultiplicacoesSimplexNormal,
			totalAdicoesSimplexRevisado, totalMultiplicacoesSimplexRevisado);

    return 0;
}
