/*
 * ModeloPL.cpp
 *
 *  Created on: 13 de jun de 2017
 *      Author: alexandre
 */

#include "ModeloPL.h"
#include <cstdio>
#include <limits>

ModeloPL::ModeloPL(Objetivo objetivo, int quantidadeVariaveis, double* coeficienteFuncaoObjetivo, int quantidadeRestricoes,
		double** restricaoCoeficiente, OperadorRelacional* restricaoOperadorRelacional, double* restricaoConstanteLadoDireito,
		OperadorRelacional* limiteVariavelOperadorRelacional, double* limiteVariavelConstanteLadoDireito) {
	this->objetivo = objetivo;

	this->quantidadeVariaveis = quantidadeVariaveis;
	this->quantidadeVariaveisOriginais = quantidadeVariaveis;

	this->tipoVariavel = new TipoVariavel[quantidadeVariaveis];
	for (int i = 0; i < quantidadeVariaveis; i++) {
		this->tipoVariavel[i] = TipoVariavel::Original;
	}

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

	this->indVariavelBasicaRestricao = NULL;
	this->tableauCj = NULL;
	this->tableauCB = NULL;
	this->tableauBase = NULL;
	this->tableauMatriz = NULL;
	this->tableauConstantesLadoDireito = NULL;
	this->tableauContribuicaoLucroLiquido = NULL;
	this->tableauVariavelEliminada = NULL;
	this->tableauZ = 0;

	this->tableauSR_Vetores_P = NULL;
	this->tableauSR_Vetor_b = NULL;
	this->tableauSR_Matriz_B = NULL;
	this->tableauSR_Cj = NULL;
	this->tableauSR_CB = NULL;
	this->tableauSR_Matriz_B_inversa = NULL;
	this->tableauSR_Vetor_b_barra = NULL;
	this->tableauSR_Base = NULL;
	this->tableauSR_C_barra = NULL;
	this->tableauSR_pi = NULL;
	this->tableauSR_VariavelEliminada = NULL;
}

ModeloPL::ModeloPL(ModeloPL* origem) {
	this->objetivo = origem->objetivo;

	this->quantidadeVariaveis = origem->quantidadeVariaveis;
	this->quantidadeVariaveisOriginais = origem->quantidadeVariaveisOriginais;

	this->tipoVariavel = new TipoVariavel[origem->quantidadeVariaveis];
	for (int i = 0; i < origem->quantidadeVariaveis; i++) {
		this->tipoVariavel[i] = origem->tipoVariavel[i];
	}

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

	this->indVariavelBasicaRestricao = NULL;
	this->tableauCj = NULL;
	this->tableauCB = NULL;
	this->tableauBase = NULL;
	this->tableauMatriz = NULL;
	this->tableauConstantesLadoDireito = NULL;
	this->tableauContribuicaoLucroLiquido = NULL;
	this->tableauVariavelEliminada = NULL;
	this->tableauZ = 0;

	this->tableauSR_Vetores_P = NULL;
	this->tableauSR_Vetor_b = NULL;
	this->tableauSR_Matriz_B = NULL;
	this->tableauSR_Cj = NULL;
	this->tableauSR_CB = NULL;
	this->tableauSR_Matriz_B_inversa = NULL;
	this->tableauSR_Vetor_b_barra = NULL;
	this->tableauSR_Base = NULL;
	this->tableauSR_C_barra = NULL;
	this->tableauSR_pi = NULL;
	this->tableauSR_VariavelEliminada = NULL;
}

ModeloPL::~ModeloPL() {
	delete [] this->coeficienteFuncaoObjetivo;

	for (int i = 0; i < quantidadeRestricoes; i++) {
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

	// contar tipos de variáveis
	int originais = 0, folga = 0, excesso = 0, substitutasIrrestritas = 0, artificiais = 0;
	for (int i = 0; i < this->quantidadeVariaveis; i++) {
		switch (this->tipoVariavel[i]) {
		case TipoVariavel::Original:
			originais++;
			break;
		case TipoVariavel::Folga:
			folga++;
			break;
		case TipoVariavel::Excesso:
			excesso++;
			break;
		case TipoVariavel::SubstitutaParaVariavelIrrestrita:
			substitutasIrrestritas++;
			break;
		case TipoVariavel::Artificial:
			artificiais++;
			break;
		}
	}

	// separar as variáveis por tipo
	int* orig = new int[originais];
	int* folg = new int[folga];
	int* exce = new int[excesso];
	int* subs = new int[substitutasIrrestritas];
	int* arti = new int[artificiais];
	int o = 0, f = 0, e = 0, s = 0, a = 0;
	for (int i = 0; i < this->quantidadeVariaveis; i++) {
		switch (this->tipoVariavel[i]) {
		case TipoVariavel::Original:
			orig[o++] = i;
			break;
		case TipoVariavel::Folga:
			folg[f++] = i;
			break;
		case TipoVariavel::Excesso:
			exce[e++] = i;
			break;
		case TipoVariavel::SubstitutaParaVariavelIrrestrita:
			subs[s++] = i;
			break;
		case TipoVariavel::Artificial:
			arti[a++] = i;
			break;
		}
	}

	// variáveis de folga
	if (folga > 0) {
		std::printf("\nvariáve%s de folga:    ", (folga == 1 ? "l" : "is"));
		for (int i = 0; i < folga; i++) {
			if (i > 0)
				std::printf(", ");
			std::printf("x%d", folg[i] + 1);
		}
		std::printf("\n");
	}

	// variáveis de excesso
	if (excesso > 0) {
		std::printf("\nvariáve%s de excesso:  ", (excesso == 1 ? "l" : "is"));
		for (int i = 0; i < excesso; i++) {
			if (i > 0)
				std::printf(", ");
			std::printf("x%d", exce[i] + 1);
		}
		std::printf("\n");
	}

	// variáveis artificiais
	if (artificiais > 0) {
		std::printf("\nvariáve%s artificia%s: ", (artificiais == 1 ? "l" : "is"), (artificiais == 1 ? "l" : "is"));
		for (int i = 0; i < artificiais; i++) {
			if (i > 0)
				std::printf(", ");
			std::printf("x%d", arti[i] + 1);
		}
		std::printf("\n");
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
			int indVariavelExcesso = modeloNaFormaPadrao->adicionarVariavelExcesso();
			modeloNaFormaPadrao->restricaoOperadorRelacional[i] = OperadorRelacional::Igual;
			modeloNaFormaPadrao->restricaoCoeficiente[i][indVariavelExcesso] = -1; // coeficiente -1 na restrição
			modeloNaFormaPadrao->limiteVariavelOperadorRelacional[indVariavelExcesso] = OperadorRelacional::MaiorOuIgual;
			modeloNaFormaPadrao->limiteVariavelConstanteLadoDireito[indVariavelExcesso] = 0;
		} else if (modeloNaFormaPadrao->restricaoOperadorRelacional[i] == OperadorRelacional::MenorOuIgual) {
			// adicionar variável de folga
			int indVariavelFolga = modeloNaFormaPadrao->adicionarVariavelFolga();
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

	// tipos das variáveis
	TipoVariavel* tipoVar = new TipoVariavel[this->quantidadeVariaveis];
	for (int i = 0; i < this->quantidadeVariaveis; i++) {
		tipoVar[i] = this->tipoVariavel[i];
	}
	tipoVar[indVariavel] = TipoVariavel::Artificial; // valor padrão para variável adicionada (arbitrariamente definido)
	delete [] this->tipoVariavel;
	this->tipoVariavel = tipoVar;

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
	this->tipoVariavel[indNovaVariavelPositiva] = TipoVariavel::SubstitutaParaVariavelIrrestrita;
	this->tipoVariavel[indNovaVariavelNegativa] = TipoVariavel::SubstitutaParaVariavelIrrestrita;
}

int ModeloPL::incluirVariavelArtificial(int indRestricaoCoeficienteUm) {
	int indVariavelArtificial = this->adicionarVariavel();
	this->restricaoCoeficiente[indRestricaoCoeficienteUm][indVariavelArtificial] = 1;
	return indVariavelArtificial;
}

double* ModeloPL::definirSolucaoBasicaFactivel() {
	if (indVariavelBasicaRestricao != NULL)
		delete [] indVariavelBasicaRestricao;
	indVariavelBasicaRestricao = new int[this->quantidadeRestricoes]; // uma por restrição

	for (int l = 0; l < this->quantidadeRestricoes; l++) {
		// verificar se na restrição há variável básica
		int indVariavelBasica = -1;
		for (int c = 0; c < this->quantidadeVariaveis; c++) {
			if (this->restricaoCoeficiente[l][c] == 1) {
				// verificar se coeficiente é zero nas outras restrições
				bool coeficientesZero = true;
				for (int indR = 0; indR < this->quantidadeRestricoes; indR++) {
					if (indR != l) {
						if (this->restricaoCoeficiente[indR][c] != 0) {
							coeficientesZero = false;
							break;
						}
					}
				}
				if (coeficientesZero) {
					// variável é básica
					indVariavelBasica = c;
					break;
				}
			}
		}
		if (indVariavelBasica >= 0) { // há variável básica na restrição
			indVariavelBasicaRestricao[l] = indVariavelBasica;
		} else { // é necessário acrescentar variável artificial para ser básica nesta restrição
			indVariavelBasicaRestricao[l] = incluirVariavelArtificial(l);
		}
	}

	// solução básica factível
	double* solucao = new double[this->quantidadeVariaveis];
	for (int i = 0; i < this->quantidadeVariaveis; i++) {
		solucao[i] = 0;
	}
	for (int i = 0; i < this->quantidadeRestricoes; i++) {
		solucao[indVariavelBasicaRestricao[i]] = this->restricaoConstanteLadoDireito[i];
	}
	return solucao;
}

int ModeloPL::adicionarVariavelFolga() {
	int indVariavel = this->adicionarVariavel();
	this->tipoVariavel[indVariavel] = TipoVariavel::Folga;
	return indVariavel;
}

int ModeloPL::adicionarVariavelExcesso() {
	int indVariavel = this->adicionarVariavel();
	this->tipoVariavel[indVariavel] = TipoVariavel::Excesso;
	return indVariavel;
}

void ModeloPL::definirTableauInicialBigM() {
	if (indVariavelBasicaRestricao == NULL) {
		double* solucao = definirSolucaoBasicaFactivel();
		delete [] solucao;
	}

	// tableauCj
	tableauCj = new double[quantidadeVariaveis];
	for (int i = 0; i < quantidadeVariaveis; i++) {
		tableauCj[i] = coeficienteFuncaoObjetivo[i];
		// Big-M
		if (tipoVariavel[i] == TipoVariavel::Artificial)
			tableauCj[i] = objetivo == Objetivo::min ? M : -M;
	}

	// tableauVariavelEliminada
	tableauVariavelEliminada = new bool[quantidadeVariaveis];
	for (int i = 0; i < quantidadeVariaveis; i++) {
		tableauVariavelEliminada[i] = false;
	}

	// tableauCB
	tableauCB = new double[quantidadeRestricoes];
	for (int i = 0; i < quantidadeRestricoes; i++) {
		tableauCB[i] = tableauCj[indVariavelBasicaRestricao[i]];
	}

	// tableauBase
	tableauBase = new int[quantidadeRestricoes];
	for (int i = 0; i < quantidadeRestricoes; i++) {
		tableauBase[i] = indVariavelBasicaRestricao[i];
	}

	// tableauMatriz
	tableauMatriz = new double*[quantidadeRestricoes];
	for (int i = 0; i < quantidadeRestricoes; i++) {
		tableauMatriz[i] = new double[quantidadeVariaveis];
		for (int j = 0; j < quantidadeVariaveis; j++) {
			tableauMatriz[i][j] = restricaoCoeficiente[i][j];
		}
	}

	// tableauConstantesLadoDireito
	tableauConstantesLadoDireito = new double[quantidadeRestricoes];
	for (int i = 0; i < quantidadeRestricoes; i++) {
		tableauConstantesLadoDireito[i] = restricaoConstanteLadoDireito[i];
	}

	// tableauContribuicaoLucroLiquido
	tableauContribuicaoLucroLiquido = new double[quantidadeVariaveis];
	for (int i = 0; i < quantidadeVariaveis; i++) {
		double produtoInterno = 0;
		for (int j = 0; j < quantidadeRestricoes; j++) {
			produtoInterno += tableauCB[j] * tableauMatriz[j][i];
		}
		tableauContribuicaoLucroLiquido[i] = tableauCj[i] - produtoInterno;
	}

	// tableauZ
	tableauZ = 0;
	for (int i = 0; i < quantidadeRestricoes; i++) {
		tableauZ += tableauCB[i] * tableauConstantesLadoDireito[i];
	}
}

bool ModeloPL::executarPassoSimplex() {
	bool haMelhoriaPossivel = false;
	int indMaiorMelhoria = -1;
	double valorContribuicao = 0;
	for (int i = 0; i < quantidadeVariaveis; i++) {
		if (tableauVariavelEliminada[i]) // ignora variável artificial eliminada
			continue;
		if (eVariavelBasica(i))
			continue;

		if (objetivo == Objetivo::max) {
			if (tableauContribuicaoLucroLiquido[i] > 0) {
				haMelhoriaPossivel = true;
				if (tableauContribuicaoLucroLiquido[i] > valorContribuicao) {
					indMaiorMelhoria = i;
					valorContribuicao = tableauContribuicaoLucroLiquido[i];
				}
			}
		} else {
			if (tableauContribuicaoLucroLiquido[i] < 0) {
				haMelhoriaPossivel = true;
				if (tableauContribuicaoLucroLiquido[i] < valorContribuicao) {
					indMaiorMelhoria = i;
					valorContribuicao = tableauContribuicaoLucroLiquido[i];
				}
			}
		}
	}
	if (!haMelhoriaPossivel) {
		std::printf("\nNão há melhoria possível.\n");
		return false;
	}

	// imprime variável que entra na base
	std::printf("\nEntra na base: x%d\n", indMaiorMelhoria + 1);

	// escolher variável que sai da base pela regra da razão mínima:
	std::printf("\nNum. da linha  Var. básica  Razão(lim.sup.sobre x%d)\n", indMaiorMelhoria + 1);
	std::printf("-----------------------------------------------------\n");
	double razaoMinima = std::numeric_limits<double>::max();
	int indLinhaRazaoMinima = -1;
	for (int i = 0; i < quantidadeRestricoes; i++) {
		char str[12];
		sprintf(str, "x%d", tableauBase[i] + 1);

		if (tableauMatriz[i][indMaiorMelhoria] > 0) {
			double razao = tableauConstantesLadoDireito[i] / tableauMatriz[i][indMaiorMelhoria];
			if (razao < razaoMinima) {
				razaoMinima = razao;
				indLinhaRazaoMinima = i;
			}
			std::printf("%13d %12s %23.3f\n", i + 1, str, razao);
		} else {
			std::printf("%13d %12s                INFINITO\n", i + 1, str);
		}
	}
	std::printf("-----------------------------------------------------\n");

	std::printf("Sai da base: x%d\n", tableauBase[indLinhaRazaoMinima] + 1);

	// elimina variável que sai da base, se for artificial
	if (tipoVariavel[tableauBase[indLinhaRazaoMinima]] == TipoVariavel::Artificial)
		tableauVariavelEliminada[tableauBase[indLinhaRazaoMinima]] = true;

	// troca variável básica
	tableauBase[indLinhaRazaoMinima] = indMaiorMelhoria;
	// troca C_j da variável básica
	tableauCB[indLinhaRazaoMinima] = tableauCj[indMaiorMelhoria];
	// divide linha da nova variável básica pelo coeficiente desta, se for diferente de 1
	if (tableauMatriz[indLinhaRazaoMinima][indMaiorMelhoria] != 1) {
		double coef = tableauMatriz[indLinhaRazaoMinima][indMaiorMelhoria];
		for (int i = 0; i < quantidadeVariaveis; i++) {
			tableauMatriz[indLinhaRazaoMinima][i] /= coef;
		}
		tableauConstantesLadoDireito[indLinhaRazaoMinima] /= coef;
	}
	// faz operação de linha para tornar coeficientes da nova variável básica igual a zero nas outras linhas
	for (int i = 0; i < quantidadeRestricoes; i++) {
		if (i != indLinhaRazaoMinima) {
			double coef = tableauMatriz[i][indMaiorMelhoria];
			for (int j = 0; j < quantidadeVariaveis; j++) {
				tableauMatriz[i][j] += tableauMatriz[indLinhaRazaoMinima][j] * (-coef);
			}
			tableauConstantesLadoDireito[i] += tableauConstantesLadoDireito[indLinhaRazaoMinima] * (-coef);
		}
	}
	// atualiza contribuição lucro líquido
	for (int i = 0; i < quantidadeVariaveis; i++) {
		double produtoInterno = 0;
		for (int j = 0; j < quantidadeRestricoes; j++) {
			produtoInterno += tableauCB[j] * tableauMatriz[j][i];
		}
		tableauContribuicaoLucroLiquido[i] = tableauCj[i] - produtoInterno;
	}
	// atualiza Z
	tableauZ = 0;
	for (int i = 0; i < quantidadeRestricoes; i++) {
		tableauZ += tableauCB[i] * tableauConstantesLadoDireito[i];
	}

	return true;
}

bool ModeloPL::eVariavelBasica(int indVar) {
	for (int i = 0; i < quantidadeRestricoes; i++) {
		if (tableauBase[i] == indVar)
			return true;
	}
	return false;
}

void ModeloPL::imprimirTableau() {
	bool haVariavelArtificialNaBase = false;

	std::printf("\n");
	if (objetivo == Objetivo::max)
		std::printf("(MAX)");
	else
		std::printf("(MIN)");
	std::printf("            |     | ");
	for (int i = 0; i < quantidadeVariaveis; i++) {
		if (tableauVariavelEliminada[i])
			std::printf("          ");
		else {
			if (tableauCj[i] <= -M)
				std::printf("      -M  ");
			else if (tableauCj[i] >= M)
				std::printf("       M  ");
			else
				std::printf("%8.3f  ", tableauCj[i]);
		}
	}
	std::printf("|\n");

	std::printf("                 | C_j +-");
	for (int i = 0; i < quantidadeVariaveis; i++) {
		std::printf("----------");
	}
	std::printf("+\n");

	std::printf("      C_B | Base |     | ");
	for (int i = 0; i < quantidadeVariaveis; i++) {
		if (tableauVariavelEliminada[i]) {
			std::printf("          ");
			continue;
		}
		char strNomeVar[8];
		sprintf(strNomeVar, "x%d", i + 1);
		std::printf("%8s  ", strNomeVar);
	}
	std::printf("| Constantes\n");

	std::printf("----------+------+-----+-");
	for (int i = 0; i < quantidadeVariaveis; i++) {
		std::printf("----------");
	}
	std::printf("+-------------\n");

	for (int i = 0; i < quantidadeRestricoes; i++) {
		char strNomeVarBase[8];
		sprintf(strNomeVarBase, "x%d", tableauBase[i] + 1);
		if (tableauCB[i] <= -M) {
			std::printf("       -M | %4s |     | ", strNomeVarBase);
			haVariavelArtificialNaBase = true;
		}
		else if (tableauCB[i] >= M) {
			std::printf("        M | %4s |     | ", strNomeVarBase);
			haVariavelArtificialNaBase = true;
		}
		else
			std::printf(" %8.3f | %4s |     | ", tableauCB[i], strNomeVarBase);

		for (int j = 0; j < quantidadeVariaveis; j++) {
			if (tableauVariavelEliminada[j])
				std::printf("          ");
			else
				std::printf("%8.3f  ", tableauMatriz[i][j]);
		}
		std::printf("| %8.3f\n", tableauConstantesLadoDireito[i]);
	}

	std::printf("----------+------+-----+-");
	for (int i = 0; i < quantidadeVariaveis; i++) {
		std::printf("----------");
	}
	std::printf("+-------------\n");

	std::printf("    Contribuição lucro | ");
	for (int i = 0; i < quantidadeVariaveis; i++) {
		if (tableauVariavelEliminada[i]) {
			std::printf("          ");
			continue;
		}
		if (haVariavelArtificialNaBase) {
			char str[8];
			sprintf(str, "%+.0e", tableauContribuicaoLucroLiquido[i]);
			std::printf("%8s  ", str);
		}
		else
			std::printf("%8.3f  ", tableauContribuicaoLucroLiquido[i]);
	}
	if (haVariavelArtificialNaBase)
		std::printf("| Z = %+.0e\n", tableauZ);
	else
		std::printf("| Z = %8.3f\n", tableauZ);

	std::printf("----------+------+-----+-");
	for (int i = 0; i < quantidadeVariaveis; i++) {
		std::printf("----------");
	}
	std::printf("+-------------\n");
}

double* ModeloPL::obterSolucaoTableau() {
	double* solucao = new double[quantidadeVariaveis];
	for (int i = 0; i < quantidadeVariaveis; i++) {
		solucao[i] = 0;
	}
	for (int i = 0; i < quantidadeRestricoes; i++) {
		solucao[tableauBase[i]] = tableauConstantesLadoDireito[i];
	}
	return solucao;
}

void ModeloPL::imprimirSolucaoTableau() {
	double* solucao = this->obterSolucaoTableau();
	std::printf("\nSolução encontrada:\n");
	for (int i = 0; i < quantidadeVariaveis; i++) {
		std::string tipo = "";
		switch (tipoVariavel[i]) {
		case TipoVariavel::Artificial:
			tipo = "artificial";
			break;
		case TipoVariavel::Excesso:
			tipo = "excesso";
			break;
		case TipoVariavel::Folga:
			tipo = "folga";
			break;
		case TipoVariavel::Original:
			tipo = "original";
			break;
		case TipoVariavel::SubstitutaParaVariavelIrrestrita:
			tipo = "substituta*";
			break;
		}
		std::printf("  x%-2d = %8.3f   ( %11s )\n", i + 1, solucao[i], tipo.c_str());
	}

	if (this->correspondenciaVariaveisIrrestritas != NULL) {
		std::printf("\n(*) substitutas para variáveis originalmente irrestritas em sinal:\n");
		for (int i = 0; i < this->quantidadeVariaveisOriginais; i++) {
			if (this->correspondenciaVariaveisIrrestritas[i] != NULL) {
				std::printf(" x'%-2d = x%-2d - x%-2d\n", i + 1, this->correspondenciaVariaveisIrrestritas[i][0] + 1,
						this->correspondenciaVariaveisIrrestritas[i][1] + 1);
			}
		}
	}
}

void ModeloPL::definirTableauInicialSimplexRevisado() {
	if (indVariavelBasicaRestricao == NULL) {
		double* solucao = definirSolucaoBasicaFactivel();
		delete [] solucao;
	}

	// definir vetores P
	tableauSR_Vetores_P = new double*[quantidadeVariaveis]; // um vetor para cada variável
	for (int i = 0; i < quantidadeVariaveis; i++) {
		tableauSR_Vetores_P[i] = new double[quantidadeRestricoes];
		for (int j = 0; j < quantidadeRestricoes; j++) {
			tableauSR_Vetores_P[i][j] = restricaoCoeficiente[j][i];
		}
	}

	// imprimir Pi
	std::printf("\n Vetores P_i:\n ---\n");
	for (int i = 0; i < quantidadeVariaveis; i++) {
		char str[12];
		sprintf(str, "P_%d", i + 1);
		std::printf(" %8s ", str);
	}
	std::printf("\n");
	for (int i = 0; i < quantidadeVariaveis; i++) {
		std::printf(" -------- ");
	}
	std::printf("\n");
	for (int j = 0; j < quantidadeRestricoes; j++) {
		for (int i = 0; i < quantidadeVariaveis; i++) {
			std::printf(" %8.3f ", tableauSR_Vetores_P[i][j]);
		}
		std::printf("\n");
	}
	std::printf(" ---\n");

	// definir vetor b
	tableauSR_Vetor_b = new double[quantidadeRestricoes];
	for (int i = 0; i < quantidadeRestricoes; i++) {
		tableauSR_Vetor_b[i] = restricaoConstanteLadoDireito[i];
	}

	// definir matriz B
	tableauSR_Matriz_B = new double*[quantidadeRestricoes];
	for (int i = 0; i < quantidadeRestricoes; i++) {
		tableauSR_Matriz_B[i] = new double[quantidadeRestricoes];
		for (int j = 0; j < quantidadeRestricoes; j++) {
			tableauSR_Matriz_B[i][j] = i == j ? 1 : 0; // matriz B inicial é a identidade
		}
	}

	// definir vetor Cj
	tableauSR_Cj = new double[quantidadeVariaveis];
	for (int i = 0; i < quantidadeVariaveis; i++) {
		tableauSR_Cj[i] = coeficienteFuncaoObjetivo[i];
		// Big-M
		if (tipoVariavel[i] == TipoVariavel::Artificial)
			tableauSR_Cj[i] = objetivo == Objetivo::min ? M : -M;
	}

	// definir vetor Base
	tableauSR_Base = new int[quantidadeRestricoes];
	for (int i = 0; i < quantidadeRestricoes; i++) {
		tableauSR_Base[i] = indVariavelBasicaRestricao[i];
	}

	// definir vetor CB
	tableauSR_CB = new double[quantidadeRestricoes];
	atualizarTableauSR_CB();

	// inicializar matriz B inversa
	tableauSR_Matriz_B_inversa = new double*[quantidadeRestricoes];
	for (int i = 0; i < quantidadeRestricoes; i++) {
		tableauSR_Matriz_B_inversa[i] = new double[quantidadeRestricoes];
		for (int j = 0; j < quantidadeRestricoes; j++) {
			tableauSR_Matriz_B_inversa[i][j] = i == j ? 1 : 0; // a inversa da matriz B é a identidade, porque B = I (inicialmente)
		}
	}

	// inicializar pi (multiplicador Simplex)
	tableauSR_pi = new double[quantidadeRestricoes];

	// inicializar C_barra
	tableauSR_C_barra = new double[quantidadeVariaveis];

	// inicializar b_barra
	tableauSR_Vetor_b_barra = new double[quantidadeRestricoes];
	for (int i = 0; i < quantidadeRestricoes; i++) {
		tableauSR_Vetor_b_barra[i] = tableauSR_Vetor_b[i];
	}

	// inicializar vetor de variáveis artificiais eliminadas
	tableauSR_VariavelEliminada = new bool[quantidadeVariaveis];
	for (int i = 0; i < quantidadeVariaveis; i++)
		tableauSR_VariavelEliminada[i] = false;
}

bool ModeloPL::executarPassoSimplexRevisado() {
	std::printf("\n Iteração SIMPLEX Revisado:\n");

	// calcular pi (multiplicador Simplex)
	calcularTableauSR_MultiplicadorSimplex();
	std::printf("\nMultiplicador SIMPLEX: pi = [ ");
	for (int i = 0; i < quantidadeRestricoes; i++)
		if (tableauSR_pi[i] < -1000000 || tableauSR_pi[i] > 1000000)
			std::printf(" %+.0e ", tableauSR_pi[i]);
		else
			std::printf("%8.3f ", tableauSR_pi[i]);
	std::printf("]\n");

	// calcular C_barra
	calcularTableauSR_C_barra();
	bool haVariavelArtificialNaBase = false;
	for (int i = 0; i < quantidadeRestricoes; i++) {
		if (tipoVariavel[tableauSR_Base[i]] == TipoVariavel::Artificial) {
			haVariavelArtificialNaBase = true;
			break;
		}
	}
	std::printf("\n");
	for (int i = 0; i < quantidadeVariaveis; i++)
		if (haVariavelArtificialNaBase) {
			std::printf("C_barra[%2d] = %+.5e\n", i + 1, tableauSR_C_barra[i]);
		}
		else
			std::printf("C_barra[%2d] = %8.3f\n", i + 1, tableauSR_C_barra[i]);

	// verificar se há melhoria
	bool haMelhoria = false;
	int indMaiorMelhoria = -1;
	double valorMaiorMelhoria = 0;
	for (int i = 0; i < quantidadeVariaveis; i++) {
		if (tableauSR_VariavelEliminada[i]) // ignora variável artificial eliminada
			continue;
		if (eVariavelBasicaSR(i))
			continue;
		if (objetivo == Objetivo::max) {
			if (tableauSR_C_barra[i] > 0) {
				haMelhoria = true;
				if (tableauSR_C_barra[i] > valorMaiorMelhoria) {
					indMaiorMelhoria = i;
					valorMaiorMelhoria = tableauSR_C_barra[i];
				}
			}
		} else {
			if (tableauSR_C_barra[i] < 0) {
				haMelhoria = true;
				if (tableauSR_C_barra[i] < valorMaiorMelhoria) {
					indMaiorMelhoria = i;
					valorMaiorMelhoria = tableauSR_C_barra[i];
				}
			}
		}
	}

	if (haMelhoria) {
		// identificar qual variável entra na base
		std::printf("\nEntra na base: x%d\n", indMaiorMelhoria + 1);

		// calcular P_barra para variável que entra na base P_barra[i] = Inv(B) * P[i]
		double* p_barra = new double[quantidadeRestricoes];
		std::printf("\nP_barra[%2d]T = [ ", indMaiorMelhoria + 1);
		for (int i = 0; i < quantidadeRestricoes; i++) {
			double item = 0;
			for (int j = 0; j < quantidadeRestricoes; j++) {
				item += tableauSR_Matriz_B_inversa[i][j] * tableauSR_Vetores_P[indMaiorMelhoria][j];
			}
			p_barra[i] = item;
			std::printf("%8.3f ", item);
		}
		std::printf("]\n");

		// escolher variável que sai da base pela regra da razão mínima:
		std::printf("\nNum. da linha  Var. básica  Razão(lim.sup.sobre x%d)\n", indMaiorMelhoria + 1);
		std::printf("-----------------------------------------------------\n");
		double razaoMinima = std::numeric_limits<double>::max();
		int indLinhaRazaoMinima = -1;
		for (int i = 0; i < quantidadeRestricoes; i++) {
			char str[12];
			sprintf(str, "x%d", tableauSR_Base[i] + 1);

			if (p_barra[i] > 0) {
				double razao = tableauSR_Vetor_b_barra[i] / p_barra[i];
				if (razao < razaoMinima) {
					razaoMinima = razao;
					indLinhaRazaoMinima = i;
				}
				std::printf("%13d %12s %23.3f\n", i + 1, str, razao);
			} else {
				std::printf("%13d %12s                INFINITO\n", i + 1, str);
			}
		}
		std::printf("-----------------------------------------------------\n");

		std::printf("Sai da base: x%d\n", tableauSR_Base[indLinhaRazaoMinima] + 1);

		// fazer pivoteamento, aplicando-o ao tableau, para coluna P_barra da variável entrante ser coluna da identidade
		// para isso, a linha "indLinhaRazaoMinima" deve ser transformada em 1 e as outras, em 0
		double coef = p_barra[indLinhaRazaoMinima];
		//p_barra[indLinhaRazaoMinima] = 1;
		for (int i = 0; i < quantidadeRestricoes; i++) {
			tableauSR_Matriz_B_inversa[indLinhaRazaoMinima][i] /= coef;
		}
		tableauSR_Vetor_b_barra[indLinhaRazaoMinima] /= coef;
		for (int i = 0; i < quantidadeRestricoes; i++) {
			if (i == indLinhaRazaoMinima)
				continue;
			coef = p_barra[i];
			//p_barra[i] = 0;
			for (int j = 0; j < quantidadeRestricoes; j++) {
				tableauSR_Matriz_B_inversa[i][j] -= coef * tableauSR_Matriz_B_inversa[indLinhaRazaoMinima][j];
			}
			tableauSR_Vetor_b_barra[i] -= coef * tableauSR_Vetor_b_barra[indLinhaRazaoMinima];
		}

		// elimina variável que sai da base, se for artificial
		if (tipoVariavel[tableauSR_Base[indLinhaRazaoMinima]] == TipoVariavel::Artificial)
			tableauSR_VariavelEliminada[tableauSR_Base[indLinhaRazaoMinima]] = true;

		// troca variáveis na base
		tableauSR_Base[indLinhaRazaoMinima] = indMaiorMelhoria;
		atualizarTableauSR_CB();
	} else {
		std::printf("\nNão há melhoria possível.\n");
	}

	return haMelhoria;
}

void ModeloPL::imprimirTableauSimplexRevisado() {
	// imprimir cabeçalho tableau
	std::printf("\n");
	std::printf(" Tableau SIMPLEX Revisado (%s)\n", objetivo == Objetivo::max ? "MAX" : "MIN");
	std::printf(" ---\n");
	std::printf(" Base ; Inv(B)   ;");
	for (int i = 1; i < quantidadeRestricoes; i++)
		std::printf("          ;");
	std::printf("  b_barra \n");

	// imprimir linhas tableau
	for (int j = 0; j < quantidadeRestricoes; j++) {
		char str[4];
		sprintf(str, "x%d", tableauSR_Base[j] + 1);
		std::printf(" %4s ;", str);
		for (int i = 0; i < quantidadeRestricoes; i++) {
			std::printf(" %8.3f ;", tableauSR_Matriz_B_inversa[j][i]);
		}
		std::printf(" %8.3f \n", tableauSR_Vetor_b_barra[j]);
	}
	std::printf(" ---\n");
//	for (int j = 0; j < quantidadeRestricoes; j++) {
//		std::printf(" P_barra[%2d]T = [ ", tableauSR_Base[j] + 1);
//		for (int i = 0; i < quantidadeRestricoes; i++) {
//			std::printf("%8.3f ", tableauSR_Matriz_B_inversa[i][j]);
//		}
//		std::printf("]\n");
//	}
//	std::printf(" ---\n");
//	std::printf("\n");
}

double* ModeloPL::obterSolucaoTableauSimplexRevisado() {
	double* solucao = new double[quantidadeVariaveis];
	for (int i = 0; i < quantidadeVariaveis; i++) {
		solucao[i] = 0;
	}
	for (int i = 0; i < quantidadeRestricoes; i++) {
		solucao[tableauSR_Base[i]] = tableauSR_Vetor_b_barra[i];
	}
	return solucao;
}

void ModeloPL::atualizarTableauSR_CB() {
	for (int i = 0; i < quantidadeRestricoes; i++) {
		tableauSR_CB[i] = tableauSR_Cj[tableauSR_Base[i]];
	}
}

void ModeloPL::calcularTableauSR_MultiplicadorSimplex() {
	// C_B * B_inversa
	for (int i = 0; i < quantidadeRestricoes; i++) {
		double item = 0;
		for (int j = 0; j < quantidadeRestricoes; j++) {
			item += tableauSR_CB[j] * tableauSR_Matriz_B_inversa[j][i];
		}
		tableauSR_pi[i] = item;
	}
}

void ModeloPL::calcularTableauSR_C_barra() {
	for (int i = 0; i < quantidadeVariaveis; i++) {
		if (eVariavelBasicaSR(i)) {
			tableauSR_C_barra[i] = 0;
			continue; // não é necessário calcular C_barra de variável básica
		} else if (tableauSR_VariavelEliminada[i]) {
			tableauSR_C_barra[i] = 0;
			continue;
		}
		double prod = 0;
		for (int j = 0; j < quantidadeRestricoes; j++) {
			prod += tableauSR_pi[j] * tableauSR_Vetores_P[i][j];
		}
		tableauSR_C_barra[i] = tableauSR_Cj[i] - prod;
	}
}

bool ModeloPL::eVariavelBasicaSR(int indVar) {
	for (int i = 0; i < quantidadeRestricoes; i++) {
		if (tableauSR_Base[i] == indVar)
			return true;
	}
	return false;
}

void ModeloPL::imprimirSolucaoTableauSimplexRevisado() {
	double* solucao = this->obterSolucaoTableauSimplexRevisado();
	std::printf("\nSolução encontrada:\n");
	for (int i = 0; i < quantidadeVariaveis; i++) {
		std::string tipo = "";
		switch (tipoVariavel[i]) {
		case TipoVariavel::Artificial:
			tipo = "artificial";
			break;
		case TipoVariavel::Excesso:
			tipo = "excesso";
			break;
		case TipoVariavel::Folga:
			tipo = "folga";
			break;
		case TipoVariavel::Original:
			tipo = "original";
			break;
		case TipoVariavel::SubstitutaParaVariavelIrrestrita:
			tipo = "substituta*";
			break;
		}
		std::printf("  x%-2d = %8.3f   ( %11s )\n", i + 1, solucao[i], tipo.c_str());
	}

	if (this->correspondenciaVariaveisIrrestritas != NULL) {
		std::printf("\n(*) substitutas para variáveis originalmente irrestritas em sinal:\n");
		for (int i = 0; i < this->quantidadeVariaveisOriginais; i++) {
			if (this->correspondenciaVariaveisIrrestritas[i] != NULL) {
				std::printf(" x'%-2d = x%-2d - x%-2d\n", i + 1, this->correspondenciaVariaveisIrrestritas[i][0] + 1,
						this->correspondenciaVariaveisIrrestritas[i][1] + 1);
			}
		}
	}
}
