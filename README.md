# EZServer
========
<a href="http://www.gta.ufrj.br" style="float:left; margin-right:10px; max-width:300px; width:100%; display:block; border:1px solid #CCC;"><img src="http://www.gta.ufrj.br/ensino/eel879/trabalhos_vf_2010_2/andre/images/Server-based-network.png"
></a>

Seguindo o conceito de Cliente - Servidor, o EZServer disponibiliza suas API's para acesso via biblioteca EZClient.dll onde vários clientes simultaneamente podem manipular dados contidos dentro do EZServer. Toda essa manipulação de dados é monitorada e registrada no BD do EZServer.

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
