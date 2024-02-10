:- dynamic paciente/2.

%   Paciente Sexo Idade Hipertensão *Cardíaco Fumante *IMC *Hemoglobina *Glicose Diabetes
paciente(['Maria',       feminino,  37.0, nao, nao, passado,    30.5,   5.7, 100], nao).
paciente(['Gorete',      feminino,  53.0, nao, nao, passado,    26.37,  4.5, 112], nao).
paciente(['Pedro',       masculino, 54.0, nao, nao, nunca,      31.86,  6.6, 145], nao). %%
paciente(['Juliana',     feminino,  37.0, nao, nao, passado,    21.25,  6.1, 200], nao).
paciente(['Catisvalda',  feminino,  56.0, nao, nao, nunca,      30.95,  6.5, 145], nao). %%
paciente(['Ursulino',    masculino, 79.0, nao, nao, passado,    31.19,  5.8, 155], nao).
paciente(['José',        masculino, 67.0, nao, nao, sim,        27.32,  6.8, 220], sim).
paciente(['Marta',       feminino,  74.0, nao, nao, sim,        29.51,  8.2, 240], sim).
paciente(['Jussara',     feminino,  37.0, nao, nao, passado,    36.87,  8.8, 160], sim).
paciente(['Katia',       feminino,  65.0, sim, sim, nunca,      33.55,  8.9, 202], sim).
paciente(['Marcelo',     masculino, 55.0, nao, sim, passado,    30.42,  6.2, 300], sim).
paciente(['Ayla',        feminino,  61.0, nao, nao, passado,    34.45,  6.5, 280], sim).
paciente(['Rafaela',     feminino,  58.0, sim, nao, nunca,      38.31,  7.0, 200], sim).
paciente(['Cássia',      feminino,  61.0, nao, nao, sim,        30.11,  6.2, 240], sim).
paciente(['Elisa',       feminino,  26.0, nao, nao, passado,    27.32,  4.0, 200], nao).
paciente(['Alice',       feminino,  74.0, nao, nao, passado,    28.12,  5.0, 100], nao).
paciente(['Jade',        feminino,  45.0, sim, nao, nunca,      23.05,  4.8, 130], nao).
paciente(['Heloise',     masculino, 56.0, nao, nao, nunca,      26.78,  4.8, 200], nao).
paciente(['Benício',     masculino, 20.0, nao, nao, passado,    23.04,  5.7, 160], nao).
paciente(['Paulo',       masculino, 70.0, nao, nao, passado,    15.94,  5.8, 158], nao).
paciente(['Otávio',      masculino, 30.0, nao, nao, passado,    15.8,   6.2, 90],  nao).
paciente(['Isis',        feminino,  80.0, nao, nao, nunca,      22.04,  9.0, 209], sim).
paciente(['Francivaldo', masculino, 63.0, nao, sim, passado,    27.32,  6.6, 300], sim).
paciente(['Juvelino',    masculino, 58.0, nao, nao, passado,    32.38,  6.6, 159], sim).
paciente(['Maya',        feminino,  43.0, sim, nao, nunca,      34.21,  9.0, 160], sim).
paciente(['Genetíldes',  masculino, 56.0, nao, nao, nunca,      37.59,  4.0, 159], nao).
paciente(['Waldisney',   masculino, 60.0, nao, nao, passado,    25.4,   4.0, 200], nao).
paciente(['Reinaldo',    masculino, 27.0, nao, nao, passado,    27.32,  3.5, 100], nao).
paciente(['Frederico',   masculino, 54.0, nao, nao, passado,    30.41,  5.0, 158], nao).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Regra para exibir todos os pacientes da base de dados
exibirTodos :- listing(paciente/2).

% Regra para montar uma lista com todos os dados de todos os pacientes
listaPacientes(Lista) :-
    findall([ConjuntoCaracteristicas, Diabete],
        paciente(ConjuntoCaracteristicas, Diabete),
        Lista
        ).

% Regra para calcular a média de uma lista
calculaMedia(Lista, Media) :-
    sumlist(Lista, Soma),
    length(Lista, Tam),
    Tam > 0,
    Media is Soma / Tam.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Regra para contar quantas vezes um "Elemento" aparece na lista
contaIgual([], _, 0) :- !.
contaIgual([X | T], Elemento, Resultado) :-
    contaIgual(T, Elemento, Resultado2),
    ((X = Elemento, !, Resultado is Resultado2 + 1)
    ;
    (!, Resultado is Resultado2)).

% Regra para contar quantos números maiores e menores que o "Elemento" aparecem nos pacientes
contaMaiorMenor([], _, 0, 0) :- !.
contaMaiorMenor([X | T], Elemento, Maior, Menor) :-
    contaMaiorMenor(T, Elemento, Maior2, Menor2),
    ((X > Elemento, !, Maior is Maior2 + 1, Menor is Menor2)
    ;
    (!, Maior is Maior2, Menor is Menor2 + 1)).

% Regra para contar quantas vezes um "Elemento" aparece nos pacientes diagnosticados com diabete
contaIgualDiabetico([], [], _, 0) :- !.
contaIgualDiabetico([X | T], [X2 | T2], Elemento, Resultado) :-
    contaIgualDiabetico(T, T2, Elemento, Resultado2),
    ((X2 = sim, X = Elemento, !, Resultado is Resultado2 + 1)
    ;
    (!, Resultado is Resultado2)).

% Regra para contar quantos números maiores que o "Elemento" aparecem nos pacientes diagnosticados com diabete
contaMaiorDiabetico([], [], _, 0) :- !.
contaMaiorDiabetico([X | T], [X2 | T2], Elemento, Resultado) :-
    contaMaiorDiabetico(T, T2, Elemento, Resultado2),
    ((X2 = sim, X > Elemento, !, Resultado is Resultado2 + 1)
    ;
    (!, Resultado is Resultado2)).

% Regra para contar quantos números menores que o "Elemento" aparecem nos pacientes diagnosticados com diabete
contaMenorDiabetico([], [], _, 0) :- !.
contaMenorDiabetico([X | T], [X2 | T2], Elemento, Resultado) :-
    contaMenorDiabetico(T, T2, Elemento, Resultado2),
    ((X2 = sim, X < Elemento, !, Resultado is Resultado2 + 1)
    ;
    (!, Resultado is Resultado2)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
% Regra para calcular o peso de algum critério, baseado em sua porcentagem
% Peso 1 caso seja abaixo de 55%
% Peso 2 caso seja abaixo de 75%
% Peso 3 caso seja acima ou igual a 75%
calculaPeso(Item, Peso) :-
    Num is Item,
    ((Num < 0.55, !, Peso = 1);
    (Num < 0.75, !, Peso = 2);
    (Num >= 0.75, !, Peso = 3)).

% Regra para "Montar" as listas com todos os dados de todos os pacientes (uma por dado, exceto nome)
listaDados([], [], [], [], [], [], [], [], [], []) :- !.
listaDados([[[_, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose], Diabete] | T],
    [Sexo | TSexo], [Idade | TIdade], [Hipertensao | THipertensao], [Cardiaco | TCardiaco], [Fumante | TFumante], [IMC | TIMC], [Hemoglobina | THemoglobina], [Glicose | TGlicose], [Diabete | TDiabete]) :- 
    listaDados(T, TSexo, TIdade, THipertensao, TCardiaco, TFumante, TIMC, THemoglobina, TGlicose, TDiabete).


% Regra para calcular os "sintomas" ou fatores com mais chances de indicar diabetes.
% Será levado em consideração o fator com maior porcentagem de ocorrência.
% Caso queira ver as porcentagens/quantidades, o primeiro argumento deve ser sim
% Por exemplo: [Feminino - 8/15 - 53,3% | Masculino - 4/14 - 28,5%], então aparentemente pessoas do gênero feminino têm mais chances de diabetes
sintomas(Exibir, [[Sexo, PesoSexo], [MediaIdade, SitIdade, PesoIdade], [Hipertensao, PesoHipertensao], [Cardiaco, PesoCardiaco], [Fumante, PesoFumante], [MediaIMC, SitIMC, PesoIMC], [MediaHemoglobina, SitHemoglobina, PesoHemoglobina], [MediaGlicose, SitGlicose, PesoGlicose]]) :-
    listaPacientes(Lista),
    listaDados(Lista, ListaSexo, ListaIdade, ListaHipertensao, ListaCardiaco, ListaFumante, ListaIMC, ListaHemoglobina, ListaGlicose, ListaDiabete),

    % Será calculado a quantidade de gente do sexo feminino e masculino cadastrado no sistema, bem como a porcentagem de diabéticos por gênero
    contaIgual(ListaSexo, feminino, QuantFeminino),
    contaIgualDiabetico(ListaSexo, ListaDiabete, feminino, QuantFeminino2),
    contaIgual(ListaSexo, masculino, QuantMasculino),
    contaIgualDiabetico(ListaSexo, ListaDiabete, masculino, QuantMasculino2),
    ((QuantFeminino2/QuantFeminino >= QuantMasculino2/QuantMasculino, !, Sexo = feminino, calculaPeso(QuantFeminino2/QuantFeminino, PesoSexo));(Sexo = masculino, calculaPeso(QuantMasculino2/QuantMasculino, PesoSexo))),

    calculaMedia(ListaIdade, MediaIdade),
    contaMaiorMenor(ListaIdade, MediaIdade, MaiorIdade, MenorIdade),
    contaMaiorDiabetico(ListaIdade, ListaDiabete, MediaIdade, MaiorIdade2),
    contaMenorDiabetico(ListaIdade, ListaDiabete, MediaIdade, MenorIdade2),
    
    ((MaiorIdade2/MaiorIdade >= MenorIdade2/MenorIdade, !, SitIdade = maior, calculaPeso(MaiorIdade2/MaiorIdade, PesoIdade));(SitIdade = menor, calculaPeso(MenorIdade2/MenorIdade, PesoIdade))),

    % Será calculado a quantidade de gente com hipertensão (ou não) cadastrado no sistema, bem como a porcentagem de diabéticos
    contaIgual(ListaHipertensao, sim, SimHipertensao),
    contaIgualDiabetico(ListaHipertensao, ListaDiabete, sim, SimHipertensao2),
    contaIgual(ListaHipertensao, nao, NaoHipertensao),
    contaIgualDiabetico(ListaHipertensao, ListaDiabete, nao, NaoHipertensao2),
    
    ((SimHipertensao2/SimHipertensao >= NaoHipertensao2/NaoHipertensao, !, Hipertensao = sim, calculaPeso(SimHipertensao2/SimHipertensao, PesoHipertensao));(Hipertensao = nao, calculaPeso(NaoHipertensao2/NaoHipertensao, PesoHipertensao))),

    % Será calculado a quantidade de gente cardíaca (ou não) cadastrado no sistema, bem como a porcentagem de diabéticos
    contaIgual(ListaCardiaco, sim, SimCardiaco),
    contaIgualDiabetico(ListaCardiaco, ListaDiabete, sim, SimCardiaco2),
    contaIgual(ListaCardiaco, nao, NaoCardiaco),
    contaIgualDiabetico(ListaCardiaco, ListaDiabete, nao, NaoCardiaco2),
   
    ((SimCardiaco2/SimCardiaco >= NaoCardiaco2/NaoCardiaco, !, Cardiaco = sim, calculaPeso(SimCardiaco2/SimCardiaco, PesoCardiaco));(Cardiaco = nao, calculaPeso(NaoCardiaco2/NaoCardiaco, PesoCardiaco))),

    % Será calculado a quantidade de gente fumante (atualmente, no passado, ou nunca) cadastrado no sistema, bem como a porcentagem de diabéticos
    contaIgual(ListaFumante, sim, SimFumante),
    contaIgualDiabetico(ListaFumante, ListaDiabete, sim, SimFumante2),
    contaIgual(ListaFumante, passado, PassadoFumante),
    contaIgualDiabetico(ListaFumante, ListaDiabete, passado, PassadoFumante2),
    contaIgual(ListaFumante, nunca, NuncaFumante),
    contaIgualDiabetico(ListaFumante, ListaDiabete, nunca, NuncaFumante2),
   
    ((SimFumante2/SimFumante >= NuncaFumante2/NuncaFumante, SimFumante2/SimFumante >= PassadoFumante2/PassadoFumante, !, Fumante = sim, calculaPeso(SimFumante2/SimFumante, PesoFumante));
     ((PassadoFumante2/PassadoFumante >= NuncaFumante2/NuncaFumante, !, Fumante = passado, calculaPeso(PassadoFumante2/PassadoFumante, PesoFumante)); (Fumante = nunca, calculaPeso(NuncaFumante2/NuncaFumante, PesoFumante)))
    ),

    % Será calculado a quantidade de gente com IMC acima e abaixo da média cadastrado no sistema, bem como a porcentagem de diabéticos
    calculaMedia(ListaIMC, MediaIMC),
    contaMaiorMenor(ListaIMC, MediaIMC, MaiorIMC, MenorIMC),
    contaMaiorDiabetico(ListaIMC, ListaDiabete, MediaIMC, MaiorIMC2),
    contaMenorDiabetico(ListaIMC, ListaDiabete, MediaIMC, MenorIMC2),
    
    ((MaiorIMC2/MaiorIMC >= MenorIMC2/MenorIMC, !, SitIMC = maior, calculaPeso(MaiorIMC2/MaiorIMC, PesoIMC));(SitIMC = menor, calculaPeso(MenorIMC2/MenorIMC, PesoIMC))),

    % Será calculado a quantidade de gente com hemoglobina acima e abaixo da média cadastrado no sistema, bem como a porcentagem de diabéticos
    calculaMedia(ListaHemoglobina, MediaHemoglobina),
    contaMaiorMenor(ListaHemoglobina, MediaHemoglobina, MaiorHemoglobina, MenorHemoglobina),
    contaMaiorDiabetico(ListaHemoglobina, ListaDiabete, MediaHemoglobina, MaiorHemoglobina2),
    contaMenorDiabetico(ListaHemoglobina, ListaDiabete, MediaHemoglobina, MenorHemoglobina2),
   
    ((MaiorHemoglobina2/MaiorHemoglobina >= MenorHemoglobina2/MenorHemoglobina, !, SitHemoglobina = maior, calculaPeso(MaiorHemoglobina2/MaiorHemoglobina, PesoHemoglobina));(SitHemoglobina = menor, calculaPeso(MenorHemoglobina2/MenorHemoglobina, PesoHemoglobina))),

    % Será calculado a quantidade de gente com glicose acima e abaixo da média cadastrado no sistema, bem como a porcentagem de diabéticos
    calculaMedia(ListaGlicose, MediaGlicose),
    contaMaiorMenor(ListaGlicose, MediaGlicose, MaiorGlicose, MenorGlicose),
    contaMaiorDiabetico(ListaGlicose, ListaDiabete, MediaGlicose, MaiorGlicose2),
    contaMenorDiabetico(ListaGlicose, ListaDiabete, MediaGlicose, MenorGlicose2),
    
    ((MaiorGlicose2/MaiorGlicose >= MenorGlicose2/MenorGlicose, !, SitGlicose = maior, calculaPeso(MaiorGlicose2/MaiorGlicose, PesoGlicose));(SitGlicose = menor, calculaPeso(MenorGlicose2/MenorGlicose, PesoGlicose))),

    % Caso o primeiro argumento seja sim, irá exibir os dados calculados
    (Exibir = sim, !,
        write('Sexo: '), nl,
        write('Homens: '), write(QuantMasculino2/QuantMasculino), nl,
        write('Mulheres: '), write(QuantFeminino2/QuantFeminino), nl, nl,

        write('Idade: '), nl,
        write('Menor: '), write(MenorIdade2/MenorIdade), nl,
        write('Maior: '), write(MaiorIdade2/MaiorIdade), nl, nl,

        write('Hipertensão: '), nl,
        write('Não: '), write(NaoHipertensao2/NaoHipertensao), nl,
        write('Sim: '), write(SimHipertensao2/SimHipertensao), nl, nl,

        write('Cardíaco: '), nl,
        write('Não: '), write(NaoCardiaco2/NaoCardiaco), nl,
        write('Sim: '), write(SimCardiaco2/SimCardiaco), nl, nl,

        write('Fumante: '), nl,
        write('Nunca: '), write(NuncaFumante2/NuncaFumante), nl,
        write('Passado: '), write(PassadoFumante2/PassadoFumante), nl,
        write('Sim: '), write(SimFumante2/SimFumante), nl, nl,

        write('IMC: '), nl,
        write('Menor: '), write(MenorIMC2/MenorIMC), nl,
        write('Maior: '), write(MaiorIMC2/MaiorIMC), nl, nl,

        write('Hemoglobina: '), nl,
        write('Menor: '), write(MenorHemoglobina2/MenorHemoglobina), nl,
        write('Maior: '), write(MaiorHemoglobina2/MaiorHemoglobina), nl, nl,

        write('Glicose: '), nl,
        write('Menor: '), write(MenorGlicose2/MenorGlicose), nl,
        write('Maior: '), write(MaiorGlicose2/MaiorGlicose), nl, nl
    ; true).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Regra para verificar se todos os dados inseridos na lista são "corretos"
verificaLista([Nome, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose]) :-
    % Nome deve ser "string" ou então um 'átomo' / átomo
    (string(Nome) ; atom(Nome)),
    % Sexo deve ser um átomo feminino ou masculino
    atom(Sexo), (Sexo = feminino ; Sexo = masculino),
    % Idade deve ser um número
    number(Idade),
    % Hipertensão deve ser um átomo sim ou não
    atom(Hipertensao), (Hipertensao = sim ; Hipertensao = nao),
    % Cardíaco deve ser um átomo sim ou não
    atom(Cardiaco), (Cardiaco = sim ; Cardiaco = nao),
    % Fumante deve ser um átomo sim, passado ou nunca,
    atom(Fumante), (Fumante = sim ; Fumante = passado ; Fumante = nunca),
    % IMC deve ser um número
    number(IMC),
    % Hemoglobina deve ser um número
    number(Hemoglobina),
    % Glicose deve ser um número
    number(Glicose).

% Permite apenas átomos como nome
leiaNome(Nome) :- 
    (var(Nome) -> repeat, nl, write('Digite seu Nome: '), nl, read(Nome), (string(Nome) -> true; nl, write('Erro! Digite um nome entre aspas!!'),  nl, fail) ; true).

% Permite apenas átomos feminino ou masculino como sexo
leiaSexo(Sexo) :-
    var(Sexo) -> repeat, nl, write('Digite seu Sexo: '), nl, read(Sexo), ((atom(Sexo), (Sexo = feminino ; Sexo = masculino)) -> true; nl, write('Erro! Digite um masculino ou feminino (letras minúsculas)!!'),  nl, fail) ; true.
    
% Permite apenas números como idade
leiaIdade(Idade) :-
    var(Idade) -> repeat, nl, write('Digite sua Idade: '), nl, read(Idade), (number(Idade) -> true; nl, write('Erro! Digite uma idade válida!!'),  nl, fail) ; true.
    
% Permite apenas átomos sim ou nao como hipertensao
leiaHipertensao(Hipertensao) :-
    var(Hipertensao) -> repeat, nl, write('Tem Hipertensao? '), nl, read(Hipertensao), ((atom(Hipertensao), (Hipertensao = sim ; Hipertensao = nao)) -> true; nl, write('Erro! Digite sim ou nao (letras minúsculas)!!'),  nl, fail) ; true.
    
% Permite apenas átomos sim ou nao como cardiaco
leiaCardiaco(Cardiaco) :-
    var(Cardiaco) -> repeat, nl, write('Tem problemas Cardiacos? '), nl, read(Cardiaco), ((atom(Cardiaco), (Cardiaco = sim ; Cardiaco = nao)) -> true; nl, write('Erro! Digite sim ou nao (letras minúsculas)!!'),  nl, fail) ; true.
    
% Permite apenas átomos sim, passado ou nunca como fumante
leiaFumante(Fumante) :-
    var(Fumante) -> repeat, nl, write('É Fumante? '), nl, read(Fumante), ((atom(Fumante), (Fumante = sim ; Fumante = passado ; Fumante = nunca)) -> true; nl, write('Erro! Digite sim, passado ou nunca (letras minúsculas)!!'),  nl, fail) ; true.
    
% Permite apenas números para IMC
leiaIMC(IMC) :-
    % Permite apenas números como peso
    (var(IMC) -> repeat, nl, write('Digite seu peso (Kg): '), nl, read(Peso), (number(Peso) -> true; nl, write('Erro! Digite um peso válido!!'),  nl, fail) ; true),
    
    % Permite apenas números como altura. Calcula também o IMC
    (var(IMC) -> repeat, nl, write('Digite sua altura (m): '), nl, read(Altura), (number(Altura) -> calcular_IMC(Peso, Altura, IMC); nl, write('Erro! Digite uma altura válida!!'),  nl, fail) ; true).

% Permite apenas números como hemoglobina
leiaHemoglobina(Hemoglobina) :-
    var(Hemoglobina) -> repeat, nl, write('Taxa de Hemoglobina: '), nl, read(Hemoglobina), (number(Hemoglobina) -> true; nl, write('Erro! Digite uma taxa válida!!'),  nl, fail) ; true.

% Permite apenas números como glicose
leiaGlicose(Glicose) :-
    var(Glicose) -> repeat, nl, write('Taxa de Glicose: '), nl, read(Glicose), (number(Glicose) -> true; nl, write('Erro! Digite uma taxa válida!!'),  nl, fail) ; true.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Regra para Adicionar um paciente
adicionar_paciente(ConjuntoCaracteristicas, Diabetes)  :-
    verificaLista(ConjuntoCaracteristicas),
    assert(paciente(ConjuntoCaracteristicas, Diabetes)).

% Regra para Editar um paciente
editar_paciente(ConjuntoCaracteristicas, Diabetes) :-
    paciente(ConjuntoCaracteristicas, Diabetes),
    write('Editar: '), nl,
    write('[1] - Nome'), nl,
    write('[2] - Sexo'), nl,
    write('[3] - Idade'), nl,
    write('[4] - Hipertensão'), nl,
    write('[5] - Cardiaco'), nl,
    write('[6] - Fumante'), nl,
    write('[7] - IMC'), nl,
    write('[8] - Hemoglobina'), nl,
    write('[9] - Glicose'), nl,
    write('[0] - Diabete'), nl,
    read(Opcao),
    [Nome, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose] = ConjuntoCaracteristicas,
    remover_paciente(ConjuntoCaracteristicas, Diabetes),
    ((Opcao = 1, !, leiaNome(Nome2), adicionar_paciente([Nome2, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose], Diabetes));
    (Opcao = 2, !, leiaSexo(Sexo2), adicionar_paciente([Nome, Sexo2, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose], Diabetes));
    (Opcao = 3, !, leiaIdade(Idade2), adicionar_paciente([Nome, Sexo, Idade2, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose], Diabetes));
    (Opcao = 4, !, leiaHipertensao(Hipertensao2), adicionar_paciente([Nome, Sexo, Idade, Hipertensao2, Cardiaco, Fumante, IMC, Hemoglobina, Glicose], Diabetes));
    (Opcao = 5, !, leiaCardiaco(Cardiaco2), adicionar_paciente([Nome, Sexo, Idade, Hipertensao, Cardiaco2, Fumante, IMC, Hemoglobina, Glicose], Diabetes));
    (Opcao = 6, !, leiaFumante(Fumante2), adicionar_paciente([Nome, Sexo, Idade, Hipertensao, Cardiaco, Fumante2, IMC, Hemoglobina, Glicose], Diabetes));
    (Opcao = 7, !, leiaIMC(IMC2), adicionar_paciente([Nome, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC2, Hemoglobina, Glicose], Diabetes));
    (Opcao = 8, !, leiaHemoglobina(Hemoglobina2), adicionar_paciente([Nome, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina2, Glicose], Diabetes));
    (Opcao = 9, !, leiaGlicose(Glicose2), adicionar_paciente([Nome, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose2], Diabetes));
    (Opcao = 0, !, repeat, write('Diabete? '), nl, read(Diabetes2), (atom(Diabetes2), (Diabetes2 = sim ; Diabetes2 = nao) -> adicionar_paciente(ConjuntoCaracteristicas, Diabetes2) ; write('Erro! Digite sim ou nao (letras minúsculas)!!')))).

% Regra para Remover um paciente
remover_paciente(ConjuntoCaracteristicas, Diabetes) :-
    retract(paciente(ConjuntoCaracteristicas, Diabetes)).

% Regra para calcular o IMC
calcular_IMC(Peso, Altura, IMC) :- 
    Peso > 0, Altura > 0, IMC is Peso / (Altura * Altura).

% Regra para diagnosticar diabetes de algum paciente, seja ele cadastrado ou não
% Caso o usuário não informe algum dado, o programa irá perguntar
diagnosticar_diabetes([Nome, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose], Diabetes) :-
    
    (var(Nome) -> leiaNome(Nome) ; 
        ((paciente([Nome | Conjunto2], Diabetes2), Base = antigo, [Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose] = Conjunto2);
        (Base = novo))
    ),

    leiaSexo(Sexo),
    leiaIdade(Idade),
    leiaHipertensao(Hipertensao),
    leiaCardiaco(Cardiaco),
    leiaFumante(Fumante),
    leiaIMC(IMC),
    leiaHemoglobina(Hemoglobina),
    leiaGlicose(Glicose),
    !,
    
    verificaLista([Nome, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose]),

    % Cria uma lista com todos os sintomas mais recorrentes em pacientes diagnosticados com diabete
    sintomas(nao, [[Sexo2, PesoSexo], [MediaIdade, SitIdade, PesoIdade], [Hipertensao2, PesoHipertensao], [Cardiaco2, PesoCardiaco], [Fumante2, PesoFumante], [MediaIMC, SitIMC, PesoIMC], [MediaHemoglobina, SitHemoglobina, PesoHemoglobina], [MediaGlicose, SitGlicose, PesoGlicose]]),

    % Compara os dados inseridos pelo usuário com os obtidos da análise do banco de dados.
    % Conta quantos "critérios de diabete" a pessoa tem.
    (SitGlicose = maior -> (Glicose >= MediaGlicose -> Criterio1 = PesoGlicose; Criterio1 = 0); (Glicose < MediaGlicose -> Criterio1 = PesoGlicose; Criterio1 = 0)),

    (SitHemoglobina = maior -> (Hemoglobina >= MediaHemoglobina -> Criterio2 = PesoHemoglobina; Criterio2 = 0); (Hemoglobina < MediaHemoglobina -> Criterio2 = PesoHemoglobina; Criterio2 = 0)),

    (SitIMC = maior -> (IMC >= MediaIMC -> Criterio3 = PesoIMC; Criterio3 = 0); (IMC < MediaIMC -> Criterio3 = PesoIMC; Criterio3 = 0)),

    (Hipertensao = Hipertensao2 -> Criterio4 = PesoHipertensao; Criterio4 = 0),

    (Cardiaco = Cardiaco2 -> Criterio5 = PesoCardiaco; Criterio5 = 0),

    (Fumante = Fumante2 -> Criterio6 = PesoFumante; Criterio6 = 0),

    (SitIdade = maior -> (Idade >= MediaIdade -> Criterio7 = PesoIdade; Criterio7 = 0); (Idade < MediaIdade -> Criterio7 = PesoIdade; Criterio7 = 0)),

    (Sexo = Sexo2 -> Criterio8 = PesoSexo; Criterio8 = 0),

    % Soma os "critérios de diabete" da pessoa.
    Criterio is Criterio1 + Criterio2 + Criterio3 + Criterio4 + Criterio5 + Criterio6 + Criterio7 + Criterio8,

    ((Criterio = 0, !, nl, write('Não foram encontrados indícios de diabete nos dados desse paciente'), (Diabetes = nao, Diabetes2 = Diabetes, ! ; nl, write('Erro! É sugerível realizar novos exames')))
    ;
    (Criterio < 2, !, nl, write('Foram encontrados pouquíssimos indícios de diabete nos dados desse paciente'), (Diabetes = nao, Diabetes2 = Diabetes, ! ; nl, write('Erro! É sugerível realizar novos exames')))
    ;
    (Criterio < 5 ->
        !, nl, write('Há chances do paciente desenvolver diabete no futuro'), (Diabetes = nao, Diabetes2 = Diabetes, ! ; nl, write('Erro! É sugerível realizar novos exames')) ;
        !, nl, write('As chances do paciente conter diabetes estão altíssimas'), (Diabetes = sim, Diabetes2 = Diabetes, ! ; nl, write('Erro! É sugerível realizar novos exames')))),

    ((Base = novo,
        Pessoa = [Nome, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose],
        (repeat, nl, nl, write('Deseja adicionar esse paciente ao banco de dados? '), nl, read(Resposta), (atom(Resposta),
            (Resposta = sim ; Resposta = nao) -> (Resposta = sim, adicionar_paciente(Pessoa, Diabetes); Resposta = nao, true)
        ; fail))
    );
    true).