%   Paciente Sexo Idade Hipertensão *Cardíaco Fumante *IMC *Hemoglobina *Glicose Diabetes
paciente(['Maria',       feminino,  37.0, nao, nao, passado,    30.5,   5.7, 100], nao).
paciente(['Gorete',      feminino,  53.0, nao, nao, passado,    26.37,  4.5, 112], nao).
paciente(['Pedro',       masculino, 54.0, nao, nao, nunca,      31.86,  6.6, 145], nao).
paciente(['Juliana',     feminino,  37.0, nao, nao, passado,    21.25,  6.1, 200], nao).
paciente(['Catisvalda',  feminino,  56.0, nao, nao, nunca,      30.95,  6.5, 145], nao).
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
% Regra para montar uma lista com os dados de todos os pacientes
listaPacientes(Lista) :-
    findall([[Nome, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose], Diabete],
        paciente([Nome, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose], Diabete),
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

contaMaiorMenor([], _, 0, 0) :- !.
contaMaiorMenor([X | T], Elemento, Maior, Menor) :-
    contaMaiorMenor(T, Elemento, Maior2, Menor2),
    ((X > Elemento, !, Maior is Maior2 + 1, Menor is Menor2)
    ;
    (!, Maior is Maior2, Menor is Menor2 + 1)).

% Regra para contar quantas vezes um "Elemento" aparece nos pacientes diagnosticados
contaIgualDiabetico([], [], _, 0) :- !.
contaIgualDiabetico([X | T], [X2 | T2], Elemento, Resultado) :-
    contaIgualDiabetico(T, T2, Elemento, Resultado2),
    ((X2 = sim, X = Elemento, !, Resultado is Resultado2 + 1)
    ;
    (!, Resultado is Resultado2)).

% Regra para contar quantos números maiores que o "Elemento" aparecem nos pacientes diagnosticados
contaMaiorDiabetico([], [], _, 0) :- !.
contaMaiorDiabetico([X | T], [X2 | T2], Elemento, Resultado) :-
    contaMaiorDiabetico(T, T2, Elemento, Resultado2),
    ((X2 = sim, X > Elemento, !, Resultado is Resultado2 + 1)
    ;
    (!, Resultado is Resultado2)).

% Regra para contar quantos números menores que o "Elemento" aparecem nos pacientes diagnosticados
contaMenorDiabetico([], [], _, 0) :- !.
contaMenorDiabetico([X | T], [X2 | T2], Elemento, Resultado) :-
    contaMenorDiabetico(T, T2, Elemento, Resultado2),
    ((X2 = sim, X < Elemento, !, Resultado is Resultado2 + 1)
    ;
    (!, Resultado is Resultado2)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

% Regra para "Montar" as listas com todos os dados de todos os pacientes (exceto nome)
listaDados([], [], [], [], [], [], [], [], [], []) :- !.
listaDados([[[_, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose], Diabete] | T],
    [Sexo | TSexo], [Idade | TIdade], [Hipertensao | THipertensao], [Cardiaco | TCardiaco], [Fumante | TFumante], [IMC | TIMC], [Hemoglobina | THemoglobina], [Glicose | TGlicose], [Diabete | TDiabete]) :- 
    listaDados(T, TSexo, TIdade, THipertensao, TCardiaco, TFumante, TIMC, THemoglobina, TGlicose, TDiabete).

% Regra para "Montar" uma lista com o sexo de todos os pacientes
% listaSexo([], []) :- !.
% listaSexo([[[_, Sexo, _, _, _, _, _, _, _], _] | T], [Sexo | T2]) :- 
%     listaSexo(T, T2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Serão calculados os "sintomas" ou fatores com mais chances de indicar diabetes.
% Será levado em consideração o fator com maior porcentagem de ocorrência.
% Por exemplo: [Feminino - 8/15 - 53,3% | Masculino - 4/14 - 28,5%], então aparentemente pessoas do gênero feminino têm mais chances de diabetes

    % sintomas([Sexo, [MediaIdade, SitIdade], Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose]) :-
    sintomas([Sexo, [MediaIdade, SitIdade], Hipertensao, Cardiaco, Fumante, [MediaIMC, SitIMC], [MediaHemoglobina, SitHemoglobina], [MediaGlicose, SitGlicose]]) :-
    listaPacientes(Lista),
    % listaDados(Lista, ListaSexo, ListaIdade, ListaHipertensao, ListaCardiaco, ListaFumante, ListaIMC, ListaHemoglobina, ListaGlicose, ListaDiabete),
    listaDados(Lista, ListaSexo, ListaIdade, ListaHipertensao, ListaCardiaco, ListaFumante, ListaIMC, ListaHemoglobina, ListaGlicose, ListaDiabete),

    % Será calculado a quantidade de gente do sexo feminino e masculino cadastrado no sistema, bem como a porcentagem de diabéticos por gênero
    % sintomas([Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose])
    contaIgual(ListaSexo, feminino, QuantFeminino),
    contaIgualDiabetico(ListaSexo, ListaDiabete, feminino, QuantFeminino2),
    contaIgual(ListaSexo, masculino, QuantMasculino),
    contaIgualDiabetico(ListaSexo, ListaDiabete, masculino, QuantMasculino2),
    write('Sexo: '), nl,
    write('Homens: '), write(QuantMasculino2), write('/'), write(QuantMasculino), nl,
    write('Mulheres: '), write(QuantFeminino2), write('/'), write(QuantFeminino), nl, nl,
    ((QuantFeminino2/QuantFeminino >= QuantMasculino2/QuantMasculino, !, Sexo = feminino);(Sexo = masculino)),

    calculaMedia(ListaIdade, MediaIdade),
    contaMaiorMenor(ListaIdade, MediaIdade, MaiorIdade, MenorIdade),
    contaMaiorDiabetico(ListaIdade, ListaDiabete, MediaIdade, MaiorIdade2),
    contaMenorDiabetico(ListaIdade, ListaDiabete, MediaIdade, MenorIdade2),
    write('Idade: '), nl,
    write('Menor: '), write(MenorIdade2), write('/'), write(MenorIdade), nl,
    write('Maior: '), write(MaiorIdade2), write('/'), write(MaiorIdade), nl, nl,
    ((MaiorIdade2/MaiorIdade >= MenorIdade2/MenorIdade, !, SitIdade = maior);(SitIdade = menor)),

    % Será calculado a quantidade de gente com hipertensão (ou não) cadastrado no sistema, bem como a porcentagem de diabéticos
    contaIgual(ListaHipertensao, sim, SimHipertensao),
    contaIgualDiabetico(ListaHipertensao, ListaDiabete, sim, SimHipertensao2),
    contaIgual(ListaHipertensao, nao, NaoHipertensao),
    contaIgualDiabetico(ListaHipertensao, ListaDiabete, nao, NaoHipertensao2),
    write('Hipertensão: '), nl,
    write('Não: '), write(NaoHipertensao2), write('/'), write(NaoHipertensao), nl,
    write('Sim: '), write(SimHipertensao2), write('/'), write(SimHipertensao), nl, nl,
    ((SimHipertensao2/SimHipertensao >= NaoHipertensao2/NaoHipertensao, !, Hipertensao = sim);(Hipertensao = nao)),

    contaIgual(ListaCardiaco, sim, SimCardiaco),
    contaIgualDiabetico(ListaCardiaco, ListaDiabete, sim, SimCardiaco2),
    contaIgual(ListaCardiaco, nao, NaoCardiaco),
    contaIgualDiabetico(ListaCardiaco, ListaDiabete, nao, NaoCardiaco2),
    write('Cardíaco: '), nl,
    write('Não: '), write(NaoCardiaco2), write('/'), write(NaoCardiaco), nl,
    write('Sim: '), write(SimCardiaco2), write('/'), write(SimCardiaco), nl, nl,
    ((SimCardiaco2/SimCardiaco >= NaoCardiaco2/NaoCardiaco, !, Cardiaco = sim);(Cardiaco = nao)),
    % sintomas([Fumante, IMC, Hemoglobina, Glicose])

    contaIgual(ListaFumante, sim, SimFumante),
    contaIgualDiabetico(ListaFumante, ListaDiabete, sim, SimFumante2),
    contaIgual(ListaFumante, passado, PassadoFumante),
    contaIgualDiabetico(ListaFumante, ListaDiabete, passado, PassadoFumante2),
    contaIgual(ListaFumante, nunca, NuncaFumante),
    contaIgualDiabetico(ListaFumante, ListaDiabete, nunca, NuncaFumante2),
    write('Fumante: '), nl,
    write('Nunca: '), write(NuncaFumante2), write('/'), write(NuncaFumante), nl,
    write('Passado: '), write(PassadoFumante2), write('/'), write(PassadoFumante), nl,
    write('Sim: '), write(SimFumante2), write('/'), write(SimFumante), nl, nl,
    ((SimFumante2/SimFumante >= NuncaFumante2/NuncaFumante, SimFumante2/SimFumante >= PassadoFumante2/PassadoFumante, !, Fumante = sim);
     ((PassadoFumante2/PassadoFumante >= NuncaFumante2/NuncaFumante, !, Fumante = passado); (Fumante = nunca))
    ),

    calculaMedia(ListaIMC, MediaIMC),
    contaMaiorMenor(ListaIMC, MediaIMC, MaiorIMC, MenorIMC),
    contaMaiorDiabetico(ListaIMC, ListaDiabete, MediaIMC, MaiorIMC2),
    contaMenorDiabetico(ListaIMC, ListaDiabete, MediaIMC, MenorIMC2),
    write('IMC: '), nl,
    write('Menor: '), write(MenorIMC2), write('/'), write(MenorIMC), nl,
    write('Maior: '), write(MaiorIMC2), write('/'), write(MaiorIMC), nl, nl,
    ((MaiorIMC2/MaiorIMC >= MenorIMC2/MenorIMC, !, SitIMC = maior);(SitIMC = menor)),

    calculaMedia(ListaHemoglobina, MediaHemoglobina),
    contaMaiorMenor(ListaHemoglobina, MediaHemoglobina, MaiorHemoglobina, MenorHemoglobina),
    contaMaiorDiabetico(ListaHemoglobina, ListaDiabete, MediaHemoglobina, MaiorHemoglobina2),
    contaMenorDiabetico(ListaHemoglobina, ListaDiabete, MediaHemoglobina, MenorHemoglobina2),
    write('Hemoglobina: '), nl,
    write('Menor: '), write(MenorHemoglobina2), write('/'), write(MenorHemoglobina), nl,
    write('Maior: '), write(MaiorHemoglobina2), write('/'), write(MaiorHemoglobina), nl, nl,
    ((MaiorHemoglobina2/MaiorHemoglobina >= MenorHemoglobina2/MenorHemoglobina, !, SitHemoglobina = maior);(SitHemoglobina = menor)),

    calculaMedia(ListaGlicose, MediaGlicose),
    contaMaiorMenor(ListaGlicose, MediaGlicose, MaiorGlicose, MenorGlicose),
    contaMaiorDiabetico(ListaGlicose, ListaDiabete, MediaGlicose, MaiorGlicose2),
    contaMenorDiabetico(ListaGlicose, ListaDiabete, MediaGlicose, MenorGlicose2),
    write('Glicose: '), nl,
    write('Menor: '), write(MenorGlicose2), write('/'), write(MenorGlicose), nl,
    write('Maior: '), write(MaiorGlicose2), write('/'), write(MaiorGlicose), nl, nl,
    ((MaiorGlicose2/MaiorGlicose >= MenorGlicose2/MenorGlicose, !, SitGlicose = maior);(SitGlicose = menor)).


% main() :-
%     listaPacientes(Lista),
    
%     listaDiabete(Lista, Diabete),
%     write(Diabete), nl.
    % listaIdade(Lista, Idade),
    % calculaMedia(Idade, MediaIdade),
    
    % listaIMC(Lista, IMC),
    % calculaMedia(IMC, MediaIMC),

    % listaHemoglobina(Lista, Hemoglobina),
    % calculaMedia(Hemoglobina, MediaHemo),

    % listaGlicose(Lista, Glicose),
    % calculaMedia(Glicose, MediaGlico).

% adicionar_paciente([ConjuntoCaracteristicas], Diabetes).

% editar_paciente([ConjuntoCaracteristicas], Diabetes).

% remover_paciente([ConjuntoCaracteristicas], Diabetes).

% Regra para calcular o IMC de um paciente
calcular_IMC(Peso, Altura, IMC) :- 
    Peso > 0, Altura > 0, IMC is Peso / (Altura * Altura).

% diagnosticar_diabetes([ConjuntoCaracteristicas], Diabetes).
