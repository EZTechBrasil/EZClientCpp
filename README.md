# EZForecourt
========
<a href="http://eztech.ind.br"> <img src="http://eztech.ind.br/img/produtos/3a10fda23695a1c68906f3a592e55faf1c20e529.png" align="center" hspace="10" vspace="0"></a>

XXX

# EZServer
========
<a href="https://www.tutorialspoint.com"> <img src="https://www.tutorialspoint.com/data_communication_computer_network/images/client_server.jpg" align="left" hspace="10" vspace="0"></a>

Seguindo o conceito de Cliente - Servidor(em inglês client/server model), o EZServer disponibiliza suas API's para acesso via biblioteca EZClient.dll onde vários clientes simultaneamente podem manipular dados contidos dentro do EZServer. Toda essa manipulação de dados é monitorada e registrada no BD do EZServer, esteja ele dentro de um EZForecourt ou dentro de um microcomputador.

===
Algumas características básica do EZServer são:

* Sempre espera por um pedido de um cliente;
* Atende os pedidos e, em seguida, responde aos clientes com os dados solicitados;
* Pode se comunicar com outros servidores para atender uma solicitação específica do cliente;

===
      
<a href="http://icons.iconarchive.com"> <img src="http://icons.iconarchive.com/icons/arrioch/senary/256/Misc-file-dll-icon.png" align="right" hspace="10" vspace="0"></a>

Dynamic-link library (biblioteca de vínculo dinâmico) ou DLL, é a implementação feita pela Microsoft para o conceito de bibliotecas compartilhadas nos sistemas operacionais Microsoft Windows e OS/2. Essas bibliotecas geralmente tem as extensões DLL, OCX (para bibliotecas que contêm controles ActiveX), ou DRV (para drivers de sistema legados).


# EZClientCpp
========
Programa de exemplo cliente que se conecta ao EZServer, usado como referência para novas integrações. 
Possui exemplos de:

* Processo Ativo ( Comum ) ou Passivo ( Por Evento ) ;
* Leitura de Abastecimentos ;
* Leitura de Configuração da Bomba ;
* Troca de Preço da Bomba ;
* Autorização e Bloqueio da Bomba ;
* Leitura de Encerrantes ;
* Leitura de Cartões ;
* Leitura de Tanques ;
* Setando valor fixo de Abastecimento ;

Requirements
------------
* EZClient.dll compatível com a versão do programa de exemplo. Atualmente 2.3.0.1 ou maior ;
* 
