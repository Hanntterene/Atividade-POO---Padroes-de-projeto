#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <algorithm>

//using namespace std para evitar std:: em todo o código
using namespace std;
class Atendente; 

// ============================ PADRÃO ESTRATÉGIA DE DESCONTO =========================
// Interface base para estratégias de desconto
class PoliticasdeDesconto
{
public:
    virtual void calcular(float *valor) = 0;
    virtual ~PoliticasdeDesconto() {}
};

// Estratégia: Sem desconto
class SemDesconto : public PoliticasdeDesconto
{
public:
    void calcular(float *valor) override
    {
        cout << "Esse cliente não tem desconto..." << endl;
    }
};

// Estratégia: Cliente VIP (10% de desconto)
class ClientesVIP : public PoliticasdeDesconto
{
public:
    void calcular(float *valor) override
    {
        cout << "Esse cliente tem 10% de desconto." << endl;
        *valor = *valor * 0.90;
    }
};

// Estratégia: Baixa Temporada (20% de desconto)
class BaixaTemporada : public PoliticasdeDesconto
{
public:
    void calcular(float *valor) override
    {
        cout << "Esse cliente tem 20% de desconto" << endl;
        *valor = *valor * 0.80;
    }
};

// Estratégia: Promoção de Feriado (15% de desconto)
class PromFeriado : public PoliticasdeDesconto{
public:
    void calcular(float *valor) override
    {
        cout << "Esse cliente tem 15% de desconto" << endl;
        *valor = *valor * 0.85;
    }
};

// Contexto para aplicar a estratégia de desconto
class Desconto
{
private:
    PoliticasdeDesconto *estrategia;

public:
    Desconto() { this->estrategia = nullptr; }
    void calcularDesconto(float *valor)
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
    void setDesconto(PoliticasdeDesconto *novaEstrategia)
    {
        this->estrategia = novaEstrategia;
    }
};

// ============================ CLASSE RESERVA =========================
// Representa uma reserva de hotel
class Reserva
{
private:
    string atendente;
    string cliente;
    string cpf;
    string localidade;
    string tipoQuarto;
    string dataCheckin;
    int numeroDiarias;
    float valorTotal;
    float valorEntrada;
    bool confirmada;

public:
    Reserva(string atendente, string cliente, string cpf, string localidade,
            string tipoQuarto, string dataCheckin, int numeroDiarias,
            float valorTotal, float valorEntrada);
    string getResumo() const;
    static void fazerReserva(Atendente &autenticado);
    string getDataCheckin() const;
    void setConfirmada(bool status);
    string getLocalidade() const;
    string getCliente() const { return cliente; }
    string getTipoQuarto() const;
    bool isConfirmada() const;
};

// ============================ SINGLETON: CONTROLADOR DE RESERVAS =========================
// Gerencia todas as reservas do sistema (Singleton)
class ControladorDeReservas
{
private:
    static ControladorDeReservas *instancia;
    vector<Reserva> reservas;

    ControladorDeReservas() {}

public:
    // Retorna a instância única do controlador
    static ControladorDeReservas *getInstancia()
    {
        if (!instancia)
        {
            instancia = new ControladorDeReservas();
        }
        return instancia;
    }

    // Verifica se já existe reserva com mesmos: localidade, tipo e data
    bool verificarDisponibilidade(string localidade, string dataCheckin, string tipoQuarto)
    {
        for (const Reserva &r : reservas)
        {
            if (r.getDataCheckin() == dataCheckin &&
                r.getLocalidade() == localidade &&
                r.getTipoQuarto() == tipoQuarto &&
                r.isConfirmada())
            {
                return false; // já está reservado
            }
        }
        return true; // está disponível
    }

    // Cria uma nova reserva e adiciona ao vetor
    Reserva criarReserva(string atendente, string cliente, string cpf, string localidade,
                         string tipoQuarto, string dataCheckin, int numeroDiarias,
                         float valorTotal, float valorEntrada)
    {
        if (!verificarDisponibilidade(localidade, dataCheckin, tipoQuarto))
        {
            throw runtime_error("Quarto indisponível para essa data/localidade.");
        }

        Reserva r(atendente, cliente, cpf, localidade, tipoQuarto, dataCheckin, numeroDiarias, valorTotal, valorEntrada);
        r.setConfirmada(false); // pode deixar como false se quiser controle de pagamento
        reservas.push_back(r);
        cout << "Reserva realizada, falta realizar pagamento para a confirmação...!" << endl;
        return r;
    }

    // Retorna todas as reservas
    const vector<Reserva> &getReservas() const
    {
        return reservas;
    }

    // Confirma uma reserva pelo nome do cliente
    bool confirmarReservaPorNome(const string &nomeCliente);

    // Salva todas as reservas em arquivo, ordenadas por data de check-in
    void salvarReservasEmArquivo(const string &nomeArquivo)
    {
        // Copia as reservas para ordenar
        vector<Reserva> ordenadas = reservas;
        // Ordena por dataCheckin (formato DD/MM/AAAA)
        sort(ordenadas.begin(), ordenadas.end(), [](const Reserva &a, const Reserva &b)
             { return a.getDataCheckin() < b.getDataCheckin(); });

        ofstream arquivo(nomeArquivo);
        if (!arquivo)
        {
            cout << "Erro ao abrir arquivo para salvar reservas.\n";
            return;
        }

        for (const Reserva &r : ordenadas)
        {
            arquivo << r.getResumo() << "--------------------\n";
        }
        arquivo.close();
        cout << "Reservas salvas em arquivo: " << nomeArquivo << endl;
    }

    // Carrega reservas do arquivo para o sistema
    void carregarReservasDeArquivo(const string &nomeArquivo)
    {
        ifstream arquivo(nomeArquivo);
        if (!arquivo)
            return; // Arquivo não existe, nada a carregar

        string linha, atendente, cliente, cpf, localidade, tipoQuarto, dataCheckin, status;
        int numeroDiarias;
        float valorTotal, valorEntrada;

        while (getline(arquivo, linha))
        {
            if (linha.find("Atendente: ") == 0)
                atendente = linha.substr(11);
            else if (linha.find("Cliente: ") == 0)
            {
                size_t pos1 = linha.find(": ") + 2;
                size_t pos2 = linha.find(" (");
                cliente = linha.substr(pos1, pos2 - pos1);
                size_t pos3 = linha.find("(") + 1;
                size_t pos4 = linha.find(")");
                cpf = linha.substr(pos3, pos4 - pos3);
            }

            else if (linha.find("Localidade: ") == 0)
                localidade = linha.substr(12);
            else if (linha.find("Quarto: ") == 0)
                tipoQuarto = linha.substr(8);
            else if (linha.find("Check-in: ") == 0)
                dataCheckin = linha.substr(10);
            else if (linha.find("Diárias: ") == 0)
                numeroDiarias = stoi(linha.substr(9));
            else if (linha.find("Total: R$") == 0)
                valorTotal = stof(linha.substr(9));
            else if (linha.find("Entrada: R$") == 0)
                valorEntrada = stof(linha.substr(11));
            else if (linha.find("Status: ") == 0)
                status = linha.substr(8);
            else if (linha.find("--------------------") == 0)
            {
                Reserva r(atendente, cliente, cpf, localidade, tipoQuarto, dataCheckin, numeroDiarias, valorTotal, valorEntrada);
                r.setConfirmada(status == "Confirmada");
                reservas.push_back(r);
            }
        }
        arquivo.close();
    }
};

// Implementação do método para confirmar reserva pelo nome
bool ControladorDeReservas::confirmarReservaPorNome(const string &nomeCliente)
{
    for (Reserva &r : reservas)
    {
        if (r.getCliente() == nomeCliente)
        {
            r.setConfirmada(true);
            cout << "Reserva de \"" << nomeCliente << "\" confirmada com sucesso.\n";
            return true;
        }
    }

    return false; // Nenhuma reserva com esse nome encontrada
}

// Inicialização do ponteiro estático do singleton
ControladorDeReservas *ControladorDeReservas::instancia = nullptr;

// ============================ CLASSE ATENDENTE =========================
// Representa um atendente do hotel
class Atendente
{
private:
    string login;
    string senha;

    // contrutor privado
    Atendente(string login, string senha)
    {
        this->login = login;
        this->senha = senha;
    }

    static vector<Atendente> atendentes;

public:
    Atendente() {} // construtor público padrão
    static const vector<Atendente> &getAtendentes()
    {
        return atendentes;
    }

    bool autenticar(string login, string senha) const
    {
        return (this->login == login && this->senha == senha);
    }

    static bool autenticarAtendente(string login, string senha, Atendente &autenticado)
    {
        for (const Atendente &a : atendentes)
        {
            if (a.autenticar(login, senha))
            {
                autenticado = a;
                return true;
            }
        }
        return false;
    }

    string getLogin()
    {
        return login;
    }
};

// Lista de atendentes cadastrados
vector<Atendente> Atendente::atendentes = {
    Atendente("atendente1", "senha1"),
    Atendente("atendente2", "senha2"),
    Atendente("atendente3", "senha3"),
    Atendente("atendente4", "senha4")};

// ============================ IMPLEMENTAÇÃO MÉTODOS RESERVA =========================
// Construtor da classe Reserva
Reserva::Reserva(string atendente, string cliente, string cpf, string localidade,
                 string tipoQuarto, string dataCheckin, int numeroDiarias,
                 float valorTotal, float valorEntrada)
{
    this->atendente = atendente;
    this->cliente = cliente;
    this->cpf = cpf;
    this->localidade = localidade;
    this->tipoQuarto = tipoQuarto;
    this->dataCheckin = dataCheckin;
    this->numeroDiarias = numeroDiarias;
    this->valorTotal = valorTotal;
    this->valorEntrada = valorEntrada;
    this->confirmada = false;
}

// Retorna um resumo da reserva (para exibição e arquivo)
string Reserva::getResumo() const
{
    string status = confirmada ? "Confirmada" : "Pendente";
    return "Atendente: " + atendente + "\nCliente: " + cliente + " (" + cpf + ")\nLocalidade: " +
           localidade + "\nQuarto: " + tipoQuarto + "\nCheck-in: " + dataCheckin +
           "\nDiárias: " + to_string(numeroDiarias) + "\nTotal: R$" + to_string(valorTotal) +
           "\nEntrada: R$" + to_string(valorEntrada) + "\nStatus: " + status + "\n";
}

string Reserva::getDataCheckin() const { return dataCheckin; }
void Reserva::setConfirmada(bool status) { confirmada = status; }
string Reserva::getLocalidade() const { return localidade; }
string Reserva::getTipoQuarto() const { return tipoQuarto; }
bool Reserva::isConfirmada() const { return confirmada; }

// ============================ MÉTODO ESTÁTICO: FAZER RESERVA =========================
// Método para criar uma nova reserva interativamente
void Reserva::fazerReserva(Atendente &autenticado)
{
    cout << "============ PEGANDO DADOS ===========" << endl;
    cin.ignore(); // Limpar buffer de entrada

    string cliente, cpf, localidade, tipoQuarto, dataCheckin;
    int numeroDiarias, tipoDesconto;
    float valorTotal = 0.0f;
    float valorEntrada = 0.0f;
    float precoBase = 0.0f;

    cout << "Nome do cliente: ";
    getline(cin, cliente);

    cout << "CPF do cliente: ";
    getline(cin, cpf);

    int opcaoLocal;
    cout << "Escolha a localidade:\n";
    cout << "1 - Jericoacoara\n";
    cout << "2 - Canoa Quebrada\n";
    cout << "3 - Cumbuco\n";
    cout << "Opção: ";
    cin >> opcaoLocal;
    cin.ignore();

    switch (opcaoLocal)
    {
    case 1:
        localidade = "Jericoacoara";
        break;
    case 2:
        localidade = "Canoa Quebrada";
        break;
    case 3:
        localidade = "Cumbuco";
        break;
    default:
        cout << "Opção inválida para localidade.\n";
        return;
    }

    int opcaoQuarto;
    cout << "Escolha o tipo de quarto:\n";
    cout << "1 - Solteiro (R$200)\n";
    cout << "2 - Duplo (R$300)\n";
    cout << "3 - Casal (R$350)\n";
    cout << "4 - Triplo (R$450)\n";
    cout << "5 - Quádruplo (R$550)\n";
    cout << "Opção: ";
    cin >> opcaoQuarto;
    cin.ignore();

    switch (opcaoQuarto)
    {
    case 1:
        tipoQuarto = "Solteiro";
        precoBase = 200;
        break;
    case 2:
        tipoQuarto = "Duplo";
        precoBase = 300;
        break;
    case 3:
        tipoQuarto = "Casal";
        precoBase = 350;
        break;
    case 4:
        tipoQuarto = "Triplo";
        precoBase = 450;
        break;
    case 5:
        tipoQuarto = "Quádruplo";
        precoBase = 550;
        break;
    default:
        cout << "Opção inválida para tipo de quarto.\n";
        return;
    }

    // ============================ ENTRADA DE DATA E DIÁRIAS =========================
    cout << "Data de check-in (DD/MM/AAAA): ";
    getline(cin, dataCheckin);

    cout << "Número de diárias: ";
    cin >> numeroDiarias;

    // ============================ CÁLCULO DE DESCONTO =========================
    float valorSemDesconto = precoBase * numeroDiarias;
    Desconto desconto;

    cout << "Selecione o tipo de desconto:\n";
    cout << "1 - Sem desconto\n";
    cout << "2 - Cliente VIP (10%)\n";
    cout << "3 - Baixa Temporada (20%)\n";
    cout << "4 - Promoção de Feriado (15%)\n";
    cout << "Opção: ";
    cin >> tipoDesconto;

    // ============================ ESCOLHA DO TIPO DE DESCONTO =========================
    switch (tipoDesconto)
    {
    case 2:
        desconto.setDesconto(new ClientesVIP());
        break;
    case 3:
        desconto.setDesconto(new BaixaTemporada());
        break;
    case 4:
        desconto.setDesconto(new PromFeriado());
        break;
    default:
        desconto.setDesconto(new SemDesconto());
        break;
    }

    desconto.calcularDesconto(&valorSemDesconto);
    valorTotal = valorSemDesconto;
    valorEntrada = valorTotal / 3.0f;

    try
    {
        ControladorDeReservas *sistema = ControladorDeReservas::getInstancia();
        Reserva nova = sistema->criarReserva(
            autenticado.getLogin(),
            cliente, cpf, localidade,
            tipoQuarto, dataCheckin,
            numeroDiarias, valorTotal, valorEntrada);

        cout << "Resumo da reserva:\n";
        cout << nova.getResumo() << endl;
    }
    catch (const exception &e)
    {
        cout << "Erro: " << e.what() << endl;
    }
}

// ============================ FUNÇÃO PRINCIPAL =========================
int main()
{
    int user_escolha;
    cout << "========== Hotel Paradise ============" << endl;
    cout << "Seja bem vindo! porfavor digite o numero se deseja..." << endl
         << "1 - entrar" << endl
         << "2 - sair" << endl;

    // Loop para garantir escolha válida do usuário
    while (true)
    {
        try
        {
            cout << "Escolha: ";
            cin >> user_escolha;

            if (user_escolha != 1 && user_escolha != 2)
            {
                throw invalid_argument("Escolha inválida! O número deve ser 1 ou 2.");
            }

            break; // Sai do while se deu tudo certo
        }
        catch (const invalid_argument &e)
        {
            cout << e.what() << endl; // Mostra mensagem de erro
        }
    }

    bool autenticadoFlag = false;
    Atendente autenticado;
    string login, senha;

    // Loop de autenticação
    while (!autenticadoFlag && user_escolha == 1)
    {
        cout << "Login: ";
        cin >> login;
        cout << "Senha: ";
        cin >> senha;

        if (Atendente::autenticarAtendente(login, senha, autenticado))
        {
            cout << "Bem-vindo, " << autenticado.getLogin() << "!" << endl
                 << endl;
            autenticadoFlag = true;
            // Carrega reservas do arquivo ao iniciar
            ControladorDeReservas::getInstancia()->carregarReservasDeArquivo("reservas.csv");
        }
        else
        {
            cout << "Login ou senha incorretos. Tente novamente." << endl;
        }
    }

    cout << "================ MENU ================" << endl;
    cout << "seja bem vindo(a) " << autenticado.getLogin() << "!" << endl;

    // Loop principal do menu do sistema
    while (!(user_escolha == 3) && autenticadoFlag)
    {
        cout << endl
             << "Digite o que deseja fazer:" << endl
             << "1 - Verificar reservas" << endl
             << "2 - Reservar uma data para um cliente" << endl
             << "3 - Sair" << endl
             << "4 - Confirmar uma reserva (pagamento)" << endl
             << "Escolha: ";

        cin >> user_escolha;

        // ============================ VERIFICAR RESERVAS =========================
        if (user_escolha == 1)
        {
            ControladorDeReservas *sistema = ControladorDeReservas::getInstancia();
            const vector<Reserva> &reservas = sistema->getReservas();

            if (reservas.empty())
            {
                cout << "Nenhuma reserva cadastrada no sistema.\n";
            }
            else
            {
                cout << "===== Reservas cadastradas =====\n";
                for (const Reserva &r : reservas)
                {
                    cout << r.getResumo() << "\n--------------------\n";
                }
            }

            cout << "\nPressione ENTER para voltar ao menu...";
            cin.ignore();
            cin.get(); // Espera ENTER

            // Submenu após visualizar reservas
            while (true)
            {
                cout << "\nO que deseja fazer agora?\n";
                cout << "1 - Voltar ao menu principal\n";
                cout << "2 - Sair do sistema\n";
                cout << "Escolha: ";
                cin >> user_escolha;
                if (user_escolha == 1)
                {
                    user_escolha = 1; // Volta para o menu principal
                    break;
                }
                else if (user_escolha == 2)
                {
                    cout << "Saindo do sistema... Até logo!" << endl;
                    user_escolha = 3; // Sair do sistema
                    break;
                }
                else
                {
                    cout << "Opção inválida. Tente novamente.\n";
                }
            }
        }
        // ============================ CONFIRMAR RESERVA =========================
        if (user_escolha == 4)
        {
            cin.ignore();
            string nomeBusca;
            cout << "Digite o nome do cliente para confirmar a reserva: ";
            getline(cin, nomeBusca);

            ControladorDeReservas *sistema = ControladorDeReservas::getInstancia();
            bool sucesso = sistema->confirmarReservaPorNome(nomeBusca);

            if (!sucesso)
            {
                cout << "Nenhuma reserva encontrada para o cliente \"" << nomeBusca << "\".\n";
            }

            // Salva as reservas após confirmação
            ControladorDeReservas::getInstancia()->salvarReservasEmArquivo("reservas.csv");

            cout << "\nPressione ENTER para voltar ao menu...";
            cin.get();
        }

        // ============================ RESERVAR UMA DATA =========================
        if (user_escolha == 2)
        {
            Reserva::fazerReserva(autenticado);
            ControladorDeReservas::getInstancia()->salvarReservasEmArquivo("reservas.csv");

            // Submenu após reservar
            while (true)
            {
                cout << "\nO que deseja fazer agora?\n";
                cout << "1 - Realizar nova reserva\n";
                cout << "2 - Voltar ao menu principal\n";
                cout << "3 - Sair do sistema\n";
                cout << "Escolha: ";
                cin >> user_escolha;

                if (user_escolha == 1)
                {
                    Reserva::fazerReserva(autenticado);
                }
                else if (user_escolha == 2)
                {
                    user_escolha = 1; // Volta para o menu principal
                    break;
                }
                else if (user_escolha == 3)
                {
                    break; // Sair do sistema
                }
                else
                {
                    cout << "Opção inválida. Tente novamente.\n";
                }
            }
        }
    }

    // Salva as reservas antes de sair do sistema
    if (user_escolha == 3)
    {
        ControladorDeReservas::getInstancia()->salvarReservasEmArquivo("reservas.csv");
        cout << "Saindo do sistema... Até logo!" << endl;
        return 0;
    }
}
