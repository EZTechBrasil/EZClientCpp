- Introdução ( Msg de boas vindas )
- Arquitetura dos Sistemas de Hardware ( Fazer Diagramas )
	- Kit de Integração
	- EZForecourt USB **
	- EZForecourt Plus **
	- EZForecourt Plus 2GS *
	- Vision *
	- EZRemote
	- EZRemote 2G
- QuickStart
	- Pacote de Instalação ( Passo a Passo ) **
	- EZMonitor *
	- EZSim *
- www.integracao.com



# Introdução
========
<a href="http://www.freeimages.com"> <img src="http://www.freeimages.com/assets/183410/1834094233/business-graphics-1428645-m.jpg" align="left" hspace="10" vspace="0"></a>

Seja bem vindo a família **EZTech**, sua presença é muito importante para nós e estamos felizes por ter vocês conosco, juntos estaremos construindo um novo futuro, agradecemos pela confiança em nós depositada. Muito obrigado, estamos a sua disposição para ajudá-lo em suas necessidades, e temos certeza de que corresponderemos a parceria e desenvolveremos um ótimo trabalho conjunto. O objetivo é organizar e acelerar este processo de integração e envolver o seu desenvolvimento em conjunto.

Buscaremos oferecer o que há de melhor em nossos conteúdos, estamos certos que todos os nossos objetivos serão alcançados, muitas novidades estarão sendo oferecidas. Não deixe de explorar e conhecer a fundo nossos produtos, contamos com a sua parceria.

# Arquitetura dos Sistemas de Hardware
========

[ FLUXOGRAMA ]

## Kit de Integração
========
<a href="https://www.tutorialspoint.com"> <img src="https://www.tutorialspoint.com/data_communication_computer_network/images/client_server.jpg" align="left" hspace="10" vspace="0"></a>

Seguindo o conceito de **Cliente - Servidor(em inglês client/server model)**, o **EZServer** disponibiliza suas **API's** para acesso via biblioteca **EZClient.dll** onde vários clientes simultaneamente podem manipular dados contidos dentro do **EZServer**. Toda essa manipulação de dados é monitorada e registrada no BD do **EZServer**, esteja ele dentro de um **EZForecourt** ou dentro de um microcomputador.

Algumas características básica do **EZServer** são:

* Sempre espera por um pedido de um cliente;
* Atende os pedidos e, em seguida, responde aos clientes com os dados solicitados;
* Pode se comunicar com outros servidores para atender uma solicitação específica do cliente;
* Garante a integridade dos dados entregue;

===
<a href="http://icons.iconarchive.com"> <img src="http://icons.iconarchive.com/icons/enhancedlabs/longhorn-pinstripe/128/dll-icon.png" align="right" hspace="10" vspace="0"></a>

**Dynamic-link library (biblioteca de vínculo dinâmico)** ou **DLL**, é a implementação feita pela **Microsoft** para o conceito de bibliotecas compartilhadas nos sistemas operacionais **Microsoft Windows e OS/2**. Essas bibliotecas geralmente tem as extensões **DLL** ou **OCX** (para bibliotecas que contêm controles ActiveX).

A **EZClient.dll** fornecerá as funções reconhecidas pelo **EZServer** e fará a ligação entre o servidor e o cliente, garantindo a requisição dos dados da forma correta e a segurança do serviço.

===
<a href="https://d1avok0lzls2w.cloudfront.net"> <img src="https://d1avok0lzls2w.cloudfront.net/img_uploads/apis-for-marketers.png" align="left" hspace="10" vspace="0"></a>

**Interface de Programação de Aplicação** (português brasileiro), cujo acrónimo API provém do *Inglês Application Programming Interface*, é um conjunto de rotinas e padrões estabelecidos por um software para a utilização das suas funcionalidades por aplicativos que não pretendem envolver-se em detalhes da implementação do software, mas apenas usar seus serviços.

De modo geral, a **API** é composta por uma série de funções acessíveis somente por programação, e que permitem utilizar características do software menos evidentes ao utilizador tradicional.

Com essa camada podemos garantir o acesso do cliente ao **EZServer** de forma segura, o cliente irá consultar a **Client.dll** ou **ocx** e essa se encarregará levar a requisição do cliente para o **EZServer**.

# EZClientCpp
========
<a href="http://ap.imagensbrasil.org"> <img src="http://ap.imagensbrasil.org/images/2017/01/26/EZClientCpp.md.png" align="left" hspace="10" vspace="0"></a>
Programa de exemplo cliente que se conecta ao **EZServer**, usado como referência para novas integrações. É um tipo de cliente Bronze e possui exemplos de:

* Tipo de Cliente Ativo ( Comum ) ou Passivo ( Por Evento ) ;
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

------------ Máscara ------------
## Synopsis

This project aims to assist partners in integrating with the EZTech service, using the APIs in a simple and practical way to serve as an example for a new integration.

## Code Example

This project will consume a library called EZClient and will use its APIs to connect and consume EZTech information, such as: Deliveries, Tank Reading, Pump States...

## Motivation

EZTech's priority is to make it easier for you to understand the consumables APIs of our service, in order to ensure that you partner, feel secure when integrating with our equipment. Uncomplication is our goal.

## Installation

[ Em construção ]

## API Reference

Depending on the size of the project, if it is small and simple enough the reference docs can be added to the README. For medium size to larger projects it is important to at least provide a link to where the API reference docs live.

## Tests

Describe and show how to run the tests with code examples.

## Contributors

Let people know how they can dive into the project, include important links to things like issue trackers, irc, twitter accounts if applicable.

## License

A short snippet describing the license (MIT, Apache, etc.)
