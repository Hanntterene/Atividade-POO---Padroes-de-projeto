#ifndef ESTRATEGIADESCONTO_H
#define ESTRATEGIADESCONTO_H

class PoliticasdeDesconto
{
public:
    virtual void calcular(float *valor) = 0;
};

class SemDesconto : public PoliticasdeDesconto
{
public:
    void calcular(float *valor) override;
};

class ClientesVIP : public PoliticasdeDesconto
{
public:
    void calcular(float *valor) override;
};

class BaixaTemporada : public PoliticasdeDesconto
{
public:
    void calcular(float *valor) override;
};

class PromFeriado : public PoliticasdeDesconto
{
public:
    void calcular(float *valor) override;
};

class Desconto
{
private:
    PoliticasdeDesconto *estrategia;

public:
    Desconto();
    void calcularDesconto(float *valor);
    void setDesconto(PoliticasdeDesconto *novaEstrategia);
};

#endif
