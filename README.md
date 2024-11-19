# Sistema de Controle de Acesso com Arduino Uno e Leitor Biométrico

Este projeto utiliza um Arduino Uno em conjunto com um leitor biométrico para criar um sistema de controle de acesso. O sistema é capaz de cadastrar impressões digitais, armazená-las na nuvem utilizando a plataforma Ubidots e verificar a compatibilidade das impressões digitais quando um usuário tenta acessar.

## Funcionalidades

- **Cadastro de Impressões Digitais**: O sistema permite que novos usuários sejam cadastrados, armazenando suas impressões digitais na nuvem do Ubidots.
- **Verificação de Acesso**: Ao colocar o dedo no leitor biométrico, o sistema verifica se a impressão digital está cadastrada na nuvem, permitindo ou negando o acesso.
- **Integração com Ubidots**: Os dados biométricos são enviados e armazenados na nuvem do Ubidots, facilitando o gerenciamento e a visualização dos dados.
- **Interface de Monitoramento**: Através da plataforma Ubidots, é possível monitorar e gerenciar os dados de acesso em tempo real.

## Tecnologias Utilizadas

- **Hardware**: Arduino Uno, Leitor Biométrico (ex: R305 ou similar).
- **Plataforma de Nuvem**: Ubidots para armazenamento e gerenciamento de dados.
- **Linguagem de Programação**: C/C++ para programação do Arduino.

## Como Funciona

1. **Cadastro**: O usuário coloca o dedo no leitor biométrico, que captura a impressão digital e a envia para a nuvem do Ubidots para armazenamento.
2. **Verificação**: Quando um usuário tenta acessar, o leitor biométrico captura a impressão digital e a compara com os dados armazenados na nuvem.
3. **Acesso**: Se a impressão digital estiver cadastrada, o sistema permite o acesso; caso contrário, o acesso é negado.

## Instruções de Uso

1. Conecte o leitor biométrico ao Arduino Uno.
2. Configure a conexão com a API do Ubidots.
3. Carregue o código no Arduino.
4. Utilize a interface do Ubidots para gerenciar os dados.

## Contribuições

Sinta-se à vontade para contribuir com melhorias ou sugestões para este projeto. Pull requests são bem-vindos!

## Licença

Este projeto está licenciado sob a [Licença MIT](LICENSE).
