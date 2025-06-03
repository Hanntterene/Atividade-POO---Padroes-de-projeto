#include "estrategiadesconto.h"
#include <iostream>

using namespace std;

// Implementações das classes já declaradas no .h

void SemDesconto::calcular(float *valor)
{
    cout << "Esse cliente não tem desconto." << endl;
}

void ClientesVIP::calcular(float *valor)
{
    cout << "Esse cliente tem 10% de desconto." << endl;
    *valor = *valor * 0.90;
}

void BaixaTemporada::calcular(float *valor)
{
    cout << "Esse cliente tem 20% de desconto" << endl;
    *valor = *valor * 0.80;
}

void PromFeriado::calcular(float *valor)
{
    cout << "Esse cliente tem 15% de desconto" << endl;
    *valor = *valor * 0.85;
}

// Desconto

Desconto::Desconto()
{
    this->estrategia = nullptr;
}

void Desconto::calcularDesconto(float *valor)
{
    if (this->estrategia != nullptr)
    {
        estrategia->calcular(valor);
    }
    else
    {
        cout << "nenhuma estrategia foi definida" << endl;
    }
}

void Desconto::setDesconto(PoliticasdeDesconto *novaEstrategia)
{
    this->estrategia = novaEstrategia;
}
