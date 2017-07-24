/*
 * ModeloPL.cpp
 *
 *  Created on: 13 de jun de 2017
 *      Author: alexandre
 */

#include "ModeloPL.h"
#include <cstdio>

ModeloPL::ModeloPL(Objetivo objetivo, int quantidadeVariaveis, double* coeficienteFuncaoObjetivo, int quantidadeRestricoes,
		double** restricaoCoeficiente, OperadorRelacional* restricaoOperadorRelacional, double* restricaoConstanteLadoDireito,
		OperadorRelacional* limiteVariavelOperadorRelacional, double* limiteVariavelConstanteLadoDireito) {
	this->objetivo = objetivo;

	this->quantidadeVariaveis = quantidadeVariaveis;
	this->quantidadeVariaveisOriginais = quantidadeVariaveis;
	this->coeficienteFuncaoObjetivo = new double[quantidadeVariaveis];
	for (int i = 0; i < quantidadeVariaveis; i++) {
		this->coeficienteFuncaoObjetivo[i] = coeficienteFuncaoObjetivo[i];
	}

	this->quantidadeRestricoes = quantidadeRestricoes;
	this->restricaoCoeficiente = new double*[quantidadeRestricoes];
	this->restricaoOperadorRelacional = new OperadorRelacional[quantidadeRestricoes];
	this->restricaoConstanteLadoDireito = new double[quantidadeRestricoes];

	for (int i = 0; i < quantidadeRestricoes; i++) {
		this->restricaoCoeficiente[i] = new double[quantidadeVariaveis];
		for (int j = 0; j < quantidadeVariaveis; j++) {
			this->restricaoCoeficiente[i][j] = restricaoCoeficiente[i][j];
		}
		this->restricaoOperadorRelacional[i] = restricaoOperadorRelacional[i];
		this->restricaoConstanteLadoDireito[i] = restricaoConstanteLadoDireito[i];
	}

	this->limiteVariavelOperadorRelacional = new OperadorRelacional[quantidadeVariaveis];
	this->limiteVariavelConstanteLadoDireito = new double[quantidadeVariaveis];

	for (int i = 0; i < quantidadeVariaveis; i++) {
		this->limiteVariavelOperadorRelacional[i] = limiteVariavelOperadorRelacional[i];
		this->limiteVariavelConstanteLadoDireito[i] = limiteVariavelConstanteLadoDireito[i];
	}

	// a princípio, não há necessidade de fazer correspondência com variáveis originais,
	// pois todas as variáveis são originais
	this->correspondenciaVariaveisIrrestritas = NULL;
}

ModeloPL::ModeloPL(ModeloPL* origem) {
	this->objetivo = origem->objetivo;

	this->quantidadeVariaveis = origem->quantidadeVariaveis;
	this->quantidadeVariaveisOriginais = origem->quantidadeVariaveisOriginais;
	this->coeficienteFuncaoObjetivo = new double[origem->quantidadeVariaveis];
	for (int i = 0; i < origem->quantidadeVariaveis; i++) {
		this->coeficienteFuncaoObjetivo[i] = origem->coeficienteFuncaoObjetivo[i];
	}

	this->quantidadeRestricoes = origem->quantidadeRestricoes;
	this->restricaoCoeficiente = new double*[origem->quantidadeRestricoes];
	this->restricaoOperadorRelacional = new OperadorRelacional[origem->quantidadeRestricoes];
	this->restricaoConstanteLadoDireito = new double[origem->quantidadeRestricoes];

	for (int i = 0; i < origem->quantidadeRestricoes; i++) {
		this->restricaoCoeficiente[i] = new double[origem->quantidadeVariaveis];
		for (int j = 0; j < origem->quantidadeVariaveis; j++) {
			this->restricaoCoeficiente[i][j] = origem->restricaoCoeficiente[i][j];
		}
		this->restricaoOperadorRelacional[i] = origem->restricaoOperadorRelacional[i];
		this->restricaoConstanteLadoDireito[i] = origem->restricaoConstanteLadoDireito[i];
	}

	this->limiteVariavelOperadorRelacional = new OperadorRelacional[origem->quantidadeVariaveis];
	this->limiteVariavelConstanteLadoDireito = new double[origem->quantidadeVariaveis];

	for (int i = 0; i < origem->quantidadeVariaveis; i++) {
		this->limiteVariavelOperadorRelacional[i] = origem->limiteVariavelOperadorRelacional[i];
		this->limiteVariavelConstanteLadoDireito[i] = origem->limiteVariavelConstanteLadoDireito[i];
	}

	if (origem->correspondenciaVariaveisIrrestritas != NULL) {
		this->correspondenciaVariaveisIrrestritas = new int*[this->quantidadeVariaveisOriginais];
		for (int i = 0; i < this->quantidadeVariaveisOriginais; i++) {
			if (origem->correspondenciaVariaveisIrrestritas[i] != NULL) {
				this->correspondenciaVariaveisIrrestritas[i] = new int[2];
				this->correspondenciaVariaveisIrrestritas[i][0] = origem->correspondenciaVariaveisIrrestritas[i][0];
				this->correspondenciaVariaveisIrrestritas[i][1] = origem->correspondenciaVariaveisIrrestritas[i][1];
			}
		}
	} else {
		this->correspondenciaVariaveisIrrestritas = NULL;
	}
}

ModeloPL::~ModeloPL() {
	delete [] this->coeficienteFuncaoObjetivo;

	for (int i = 0; i < quantidadeVariaveis; i++) {
		delete [] this->restricaoCoeficiente[i];
	}
	delete [] this->restricaoCoeficiente;
	delete [] this->limiteVariavelOperadorRelacional;
	delete [] this->limiteVariavelConstanteLadoDireito;

	delete [] this->restricaoCoeficiente;
	delete [] this->restricaoOperadorRelacional;
	delete [] this->restricaoConstanteLadoDireito;

	if (this->correspondenciaVariaveisIrrestritas != NULL) {
		for (int i = 0; i < quantidadeVariaveisOriginais; i++)
			if (this->correspondenciaVariaveisIrrestritas[i] != NULL)
				delete [] this->correspondenciaVariaveisIrrestritas[i];
		delete [] this->correspondenciaVariaveisIrrestritas;
	}
}

void ModeloPL::imprimirModelo(std::string* legenda) {
	// legenda
	std::printf("\n-------- Modelo: \"%s\" --------\n\n", legenda->c_str());
	// função objetivo
	switch (this->objetivo) {
	case Objetivo::max:
		std::printf("MAX Z = ");
		break;
	case Objetivo::min:
		std::printf("MIN Z = ");
		break;
	}
	for (int i = 0; i < this->quantidadeVariaveis; i++) {
		if (this->coeficienteFuncaoObjetivo[i] == 0)
			std::printf("             ");
		else {
			if (i == 0)
				std::printf("%8.3f x%-2d ", this->coeficienteFuncaoObjetivo[i], i + 1);
			else
				std::printf("%+8.3f x%-2d ", this->coeficienteFuncaoObjetivo[i], i + 1);
		}
	}

	// restrições
	std::printf("\n\nsujeito a:\n");
	for (int i = 0; i < this->quantidadeRestricoes; i++) {
		std::printf("        ");
		for (int j = 0; j < this->quantidadeVariaveis; j++) {
			if (this->restricaoCoeficiente[i][j] == 0)
				std::printf("             ");
			else {
				if (j == 0)
					std::printf("%8.3f x%-2d ", this->restricaoCoeficiente[i][j], j + 1);
				else
					std::printf("%+8.3f x%-2d ", this->restricaoCoeficiente[i][j], j + 1);
			}
		}
		switch (this->restricaoOperadorRelacional[i]) {
		case OperadorRelacional::Menor:
			std::printf("  <   ");
			break;
		case OperadorRelacional::MenorOuIgual:
			std::printf("  <=  ");
			break;
		case OperadorRelacional::Igual:
			std::printf("  =   ");
			break;
		case OperadorRelacional::MaiorOuIgual:
			std::printf("  >=  ");
			break;
		case OperadorRelacional::Maior:
			std::printf("  >   ");
			break;
		case OperadorRelacional::Irrestrito:
			break;
		}
		std::printf("%8.3f\n", this->restricaoConstanteLadoDireito[i]);
	}
	std::printf("\n");

	// limites das variáveis
	std::printf("\nlimites:\n");
	for (int i = 0; i < this->quantidadeVariaveis; i++) {
		std::string operadorLimite = "";
		char limiteCoeficienteLadoDireito[8];
		std::sprintf(limiteCoeficienteLadoDireito, "%8.3f", this->limiteVariavelConstanteLadoDireito[i]);
		switch (this->limiteVariavelOperadorRelacional[i]) {
		case OperadorRelacional::Menor:
			operadorLimite = " <  ";
			break;
		case OperadorRelacional::MenorOuIgual:
			operadorLimite = " <= ";
			break;
		case OperadorRelacional::Igual:
			operadorLimite = " =  ";
			break;
		case OperadorRelacional::MaiorOuIgual:
			operadorLimite = " >= ";
			break;
		case OperadorRelacional::Maior:
			operadorLimite = " >  ";
			break;
		case OperadorRelacional::Irrestrito:
			operadorLimite = " irr";
			std::sprintf(limiteCoeficienteLadoDireito, " ");
			break;
		}
		std::printf(" x%-2d %s %s\n", i + 1, operadorLimite.c_str(), limiteCoeficienteLadoDireito);
	}

	// correspondência de variáveis originais irrestritas com variáveis atuais
	if (this->correspondenciaVariaveisIrrestritas != NULL) {
		std::printf("\nvariáveis originalmente irrestritas em sinal:\n");
		for (int i = 0; i < this->quantidadeVariaveisOriginais; i++) {
			if (this->correspondenciaVariaveisIrrestritas[i] != NULL) {
				std::printf(" x'%-2d = x%-2d - x%-2d\n", i + 1, this->correspondenciaVariaveisIrrestritas[i][0] + 1,
						this->correspondenciaVariaveisIrrestritas[i][1] + 1);
			}
		}
	}

	std::printf("\n-------- Fim-Modelo --------\n\n");
}

ModeloPL* ModeloPL::obterModeloNaFormaPadrao() {
	ModeloPL* modeloNaFormaPadrao = new ModeloPL(this);

	// verificar restrições e adicionar, se necessário, variáveis de folga ou excesso, e, se
	// o lado direito for negativo, multiplicar os dois lados por -1
	for (int i = 0; i < modeloNaFormaPadrao->quantidadeRestricoes; i++) {
		if (modeloNaFormaPadrao->restricaoOperadorRelacional[i] == OperadorRelacional::MaiorOuIgual) {
			// adicionar variável de excesso
			int indVariavelExcesso = modeloNaFormaPadrao->adicionarVariavel();
			modeloNaFormaPadrao->restricaoOperadorRelacional[i] = OperadorRelacional::Igual;
			modeloNaFormaPadrao->restricaoCoeficiente[i][indVariavelExcesso] = -1; // coeficiente -1 na restrição
			modeloNaFormaPadrao->limiteVariavelOperadorRelacional[indVariavelExcesso] = OperadorRelacional::MaiorOuIgual;
			modeloNaFormaPadrao->limiteVariavelConstanteLadoDireito[indVariavelExcesso] = 0;
		} else if (modeloNaFormaPadrao->restricaoOperadorRelacional[i] == OperadorRelacional::MenorOuIgual) {
			// adicionar variável de folga
			int indVariavelFolga = modeloNaFormaPadrao->adicionarVariavel();
			modeloNaFormaPadrao->restricaoOperadorRelacional[i] = OperadorRelacional::Igual;
			modeloNaFormaPadrao->restricaoCoeficiente[i][indVariavelFolga] = 1; // coeficiente 1 na restrição
			modeloNaFormaPadrao->limiteVariavelOperadorRelacional[indVariavelFolga] = OperadorRelacional::MaiorOuIgual;
			modeloNaFormaPadrao->limiteVariavelConstanteLadoDireito[indVariavelFolga] = 0;
		}

		// verificar se lado direito é negativo
		if (modeloNaFormaPadrao->restricaoConstanteLadoDireito[i] < 0) {
			for (int j = 0; j < modeloNaFormaPadrao->quantidadeVariaveis; j++) {
				modeloNaFormaPadrao->restricaoCoeficiente[i][j] *= -1;
			}
			modeloNaFormaPadrao->restricaoConstanteLadoDireito[i] *= -1;
		}
	}

	// verificar limites e, se encontrar variáveis irrestritas em sinal, fazer, para cada variável irrestrita i:
	// 1) adicionar uma nova variável, x[k]', com coeficientes da função objetivo e das restrições iguais aos da variável i,
	//    mas com sinais invertidos;
	// 2) fazer limites das variáveis x[i]' e x[k]' serem >= 0
	// 3) ajustar correspondência x[i] = x[i]' - x[k]'  --> x[i]' é a variável i atual, x[i] é a variável irrestrita original
	for (int i = 0; i < modeloNaFormaPadrao->quantidadeVariaveis; i++) {
		if (modeloNaFormaPadrao->limiteVariavelOperadorRelacional[i] == OperadorRelacional::Irrestrito) {
			// (1)
			int k = modeloNaFormaPadrao->adicionarVariavel();
			modeloNaFormaPadrao->coeficienteFuncaoObjetivo[k] = -modeloNaFormaPadrao->coeficienteFuncaoObjetivo[i];
			for (int j = 0; j < modeloNaFormaPadrao->quantidadeRestricoes; j++) {
				modeloNaFormaPadrao->restricaoCoeficiente[j][k] = -modeloNaFormaPadrao->restricaoCoeficiente[j][i];
			}
			// (2)
			modeloNaFormaPadrao->limiteVariavelOperadorRelacional[i] = OperadorRelacional::MaiorOuIgual;
			modeloNaFormaPadrao->limiteVariavelConstanteLadoDireito[i] = 0;
			modeloNaFormaPadrao->limiteVariavelOperadorRelacional[k] = OperadorRelacional::MaiorOuIgual;
			modeloNaFormaPadrao->limiteVariavelConstanteLadoDireito[k] = 0;
			// (3)
			modeloNaFormaPadrao->ajustarCorrespondenciaVariavelIrrestrita(i, i, k);
		}
	}

	return modeloNaFormaPadrao;
}

int ModeloPL::adicionarVariavel() {
	int indVariavel = this->quantidadeVariaveis;
	this->quantidadeVariaveis++;

	// coeficientes função objetivo
	double* coefFO = new double[this->quantidadeVariaveis];
	for (int i = 0; i < this->quantidadeVariaveis - 1; i++) {
		coefFO[i] = this->coeficienteFuncaoObjetivo[i];
	}
	coefFO[indVariavel] = 0;
	delete [] this->coeficienteFuncaoObjetivo;
	this->coeficienteFuncaoObjetivo = coefFO;

	// coeficientes restrições
	for (int i = 0; i < this->quantidadeRestricoes; i++) {
		double* coefRestr = new double[this->quantidadeVariaveis];
		for (int j = 0; j < this->quantidadeVariaveis - 1; j++) {
			coefRestr[j] = this->restricaoCoeficiente[i][j];
		}
		coefRestr[indVariavel] = 0;

		delete [] this->restricaoCoeficiente[i];
		this->restricaoCoeficiente[i] = coefRestr;
	}

	// limite variável
	OperadorRelacional* limVarOR = new OperadorRelacional[this->quantidadeVariaveis];
	double* limVarCLD = new double[this->quantidadeVariaveis];
	for (int i = 0; i < this->quantidadeVariaveis - 1; i++) {
		limVarOR[i] = this->limiteVariavelOperadorRelacional[i];
		limVarCLD[i] = this->limiteVariavelConstanteLadoDireito[i];
	}
	limVarOR[indVariavel] = OperadorRelacional::MaiorOuIgual;
	limVarCLD[indVariavel] = 0;
	delete [] this->limiteVariavelOperadorRelacional;
	delete [] this->limiteVariavelConstanteLadoDireito;
	this->limiteVariavelOperadorRelacional = limVarOR;
	this->limiteVariavelConstanteLadoDireito = limVarCLD;

	return indVariavel;
}

void ModeloPL::ajustarCorrespondenciaVariavelIrrestrita(int indVariavelIrrestrita,
		int indNovaVariavelPositiva, int indNovaVariavelNegativa) {
	if (this->correspondenciaVariaveisIrrestritas == NULL) {
		this->correspondenciaVariaveisIrrestritas = new int*[this->quantidadeVariaveisOriginais];
		for (int i = 0; i < this->quantidadeVariaveisOriginais; i++) {
			this->correspondenciaVariaveisIrrestritas[i] = NULL;
		}
	}
	this->correspondenciaVariaveisIrrestritas[indVariavelIrrestrita] = new int[2];
	this->correspondenciaVariaveisIrrestritas[indVariavelIrrestrita][0] = indNovaVariavelPositiva;
	this->correspondenciaVariaveisIrrestritas[indVariavelIrrestrita][1] = indNovaVariavelNegativa;
}
